/// @file event.cc
/// Implementation of Event Class.
#include "event.h"

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/pointer_cast.hpp>

namespace scram {

const std::set<std::string> Formula::two_or_more_ =
    boost::assign::list_of ("and") ("or") ("nand") ("nor");

const std::set<std::string> Formula::single_ =
    boost::assign::list_of ("not") ("null");

Event::Event(const std::string& name, const std::string& base_path,
             bool is_public)
    : Role::Role(is_public),
      name_(name),
      base_path_(base_path),
      orphan_(true) {
  assert(name != "");
  id_ = is_public ? name : base_path + "." + name;  // Unique combination.
  boost::to_lower(id_);
}

Gate::Gate(const std::string& name, const std::string& base_path,
           bool is_public)
    : Event(name, base_path, is_public),
      mark_("") {}

void Gate::Validate() {
  // Detect inhibit flavor.
  if (formula_->type() == "and" && this->HasAttribute("flavor")) {
    const Attribute* attr = &this->GetAttribute("flavor");
    if (attr->value == "inhibit") {
      if (formula_->num_args() != 2) {
        throw ValidationError(this->name() +
                              "INHIBIT gate must have only 2 children");
      }

      std::stringstream msg;
      msg << "";
      bool conditional_found = false;
      std::map<std::string, boost::shared_ptr<Event> >::const_iterator it;
      for (it = formula_->event_args().begin();
           it != formula_->event_args().end(); ++it) {
        if (!boost::dynamic_pointer_cast<BasicEvent>(it->second)) continue;
        if (!it->second->HasAttribute("flavor")) continue;
        std::string type = it->second->GetAttribute("flavor").value;
        if (type != "conditional") continue;
        if (!conditional_found) {
          conditional_found = true;
        } else {
          msg << this->name() << " : " << "INHIBIT"
              << " gate must have exactly one conditional event.\n";
        }
      }
      if (!conditional_found) {
        msg << this->name() << " : " << "INHIBIT"
            << " gate is missing a conditional event.\n";
      }
      if (!msg.str().empty()) throw ValidationError(msg.str());
    }
  }
}

int Formula::vote_number() {
  if (vote_number_ == -1) {
    std::string msg = "Vote number is not set for this formula.";
    throw LogicError(msg);
  }
  return vote_number_;
}

void Formula::vote_number(int vnumber) {
  if (type_ != "atleast") {
    std::string msg = "Vote number can only be defined for ATLEAST operator. "
                      "The operator of this formula is " + type_ + ".";
    throw LogicError(msg);
  } else if (vnumber < 2) {
    std::string msg = "Vote number cannot be less than 2.";
    throw InvalidArgument(msg);
  } else if (vote_number_ != -1) {
    std::string msg = "Trying to re-assign a vote number";
    throw LogicError(msg);
  }
  vote_number_ = vnumber;
}

void Formula::AddArgument(const boost::shared_ptr<Event>& event) {
  if (event_args_.count(event->id())) {
    std::string msg = "Detected a repeated argument: " + event->name();
    throw ValidationError(msg);
  }
  event_args_.insert(std::make_pair(event->id(), event));
}

void Formula::AddArgument(const boost::shared_ptr<Formula>& formula) {
  if (formula_args_.count(formula)) {
    std::string msg = "Trying to re-insert a formula as an argument";
    throw LogicError(msg);
  }
  formula_args_.insert(formula);
}

void Formula::Validate() {
  assert(two_or_more_.count(type_) || single_.count(type_) ||
         type_ == "atleast" || type_ == "xor");

  std::string form = type_;  // Copying for manipulations.

  int size = formula_args_.size() + event_args_.size();
  std::stringstream msg;
  if (two_or_more_.count(form) && size < 2) {
    boost::to_upper(form);
    msg << form << " formula must have 2 or more arguments.";

  } else if (single_.count(form) && size != 1) {
    boost::to_upper(form);
    msg << form << " formula must have only one argument.";

  } else if (form == "xor" && size != 2) {
    boost::to_upper(form);
    msg << form << " formula must have exactly 2 arguments.";

  } else if (form == "atleast" && size <= vote_number_) {
    boost::to_upper(form);
    msg << form << " formula must have more arguments than its vote number "
        << vote_number_ << ".";
  }
  if (!msg.str().empty()) throw ValidationError(msg.str());
}

const std::map<std::string, boost::shared_ptr<Event> >& Formula::event_args() {
  if (event_args_.empty() && formula_args_.empty()) {
    throw LogicError("Formula does not have arguments.");
  }
  return event_args_;
}

const std::set<boost::shared_ptr<Formula> >& Formula::formula_args() {
  if (event_args_.empty() && formula_args_.empty()) {
    throw LogicError("Formula does not have arguments.");
  }
  return formula_args_;
}

void Formula::GatherNodesAndConnectors() {
  assert(nodes_.empty());
  assert(connectors_.empty());
  std::map<std::string, boost::shared_ptr<Event> >::iterator it;
  for (it = event_args_.begin(); it != event_args_.end(); ++it) {
    Gate* ptr = dynamic_cast<Gate*>(&*it->second);
    if (ptr) {
      nodes_.push_back(ptr);
    }
  }
  std::set<boost::shared_ptr<Formula> >::iterator it_f;
  for (it_f = formula_args_.begin(); it_f != formula_args_.end();
       ++it_f) {
    connectors_.push_back(&**it_f);
  }
  gather_ = false;
}

}  // namespace scram
