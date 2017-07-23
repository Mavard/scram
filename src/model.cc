/*
 * Copyright (C) 2014-2017 Olzhas Rakhimov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/// @file model.cc
/// Implementation of functions in Model class.

#include "model.h"

#include "error.h"
#include "ext/find_iterator.h"

namespace scram {
namespace mef {

const char Model::kDefaultName[] = "__unnamed-model__";

Model::Model(std::string name)
    : Element(name.empty() ? kDefaultName : std::move(name)),
      mission_time_(std::make_shared<MissionTime>()) {}

void Model::Add(InitiatingEventPtr initiating_event) {
  mef::AddElement<RedefinitionError>(std::move(initiating_event),
                                     &initiating_events_,
                                     "Redefinition of initiating event: ");
}

void Model::Add(EventTreePtr event_tree) {
  mef::AddElement<RedefinitionError>(std::move(event_tree), &event_trees_,
                                     "Redefinition of event tree: ");
}

void Model::Add(const SequencePtr& sequence) {
  mef::AddElement<RedefinitionError>(sequence, &sequences_,
                                     "Redefinition of sequence: ");
}

void Model::Add(RulePtr rule) {
  mef::AddElement<RedefinitionError>(std::move(rule), &rules_,
                                     "Redefinition of rule: ");
}

void Model::Add(FaultTreePtr fault_tree) {
  mef::AddElement<RedefinitionError>(std::move(fault_tree), &fault_trees_,
                                     "Redefinition of fault tree: ");
}

void Model::Add(const ParameterPtr& parameter) {
  mef::AddElement<RedefinitionError>(parameter, &parameters_,
                                     "Redefinition of parameter: ");
}

void Model::Add(const HouseEventPtr& house_event) {
  mef::AddElement<RedefinitionError>(house_event.get(), &events_,
                                     "Redefinition of event: ");
  house_events_.insert(house_event);
}

void Model::Add(const BasicEventPtr& basic_event) {
  mef::AddElement<RedefinitionError>(basic_event.get(), &events_,
                                     "Redefinition of event: ");
  basic_events_.insert(basic_event);
}

void Model::Add(const GatePtr& gate) {
  mef::AddElement<RedefinitionError>(gate.get(), &events_,
                                     "Redefinition of event: ");
  gates_.insert(gate);
}

void Model::Add(const CcfGroupPtr& ccf_group) {
  mef::AddElement<RedefinitionError>(ccf_group, &ccf_groups_,
                                     "Redefinition of CCF group: ");
}

void Model::Remove(HouseEvent* house_event) {
  auto it = events_.find(house_event->id());
  if (it == events_.end())
    throw std::out_of_range("House event " + house_event->id() +
                            " is not in the model.");
  if (*it != house_event)
    throw std::out_of_range("Duplicate event " + house_event->id() +
                            " does not belong to the model.");

  events_.erase(it);
  house_events_.erase(house_event);
}

void Model::Remove(BasicEvent* basic_event) {
  auto it = events_.find(basic_event->id());
  if (it == events_.end())
    throw std::out_of_range("Basic event " + basic_event->id() +
                            " is not in the model.");
  if (*it != basic_event)
    throw std::out_of_range("Duplicate event " + basic_event->id() +
                            " does not belong to the model.");

  events_.erase(it);
  basic_events_.erase(basic_event);
}

void Model::Remove(Gate* gate) {
  auto it = events_.find(gate->id());
  if (it == events_.end())
    throw std::out_of_range("Gate " + gate->id() +
                            " is not in the model.");
  if (*it != gate)
    throw std::out_of_range("Duplicate event " + gate->id() +
                            " does not belong to the model.");

  events_.erase(it);
  gates_.erase(gate);
}

void Model::Remove(FaultTree* fault_tree) {
  auto it = fault_trees_.find(fault_tree->name());
  if (it == fault_trees_.end())
    throw std::out_of_range("Fault tree " + fault_tree->name() +
                            " is not in the model.");
  if (it->get() != fault_tree)
    throw std::out_of_range("Duplicate fault tree " + fault_tree->name() +
                            " does not belong to the model.");
  fault_trees_.erase(it);
}

Parameter* Model::GetParameter(const std::string& entity_reference,
                               const std::string& base_path) {
  return GetEntity(entity_reference, base_path, parameters_);
}

HouseEvent* Model::GetHouseEvent(const std::string& entity_reference,
                                 const std::string& base_path) {
  return GetEntity(entity_reference, base_path, house_events_);
}

BasicEvent* Model::GetBasicEvent(const std::string& entity_reference,
                                 const std::string& base_path) {
  return GetEntity(entity_reference, base_path, basic_events_);
}

Gate* Model::GetGate(const std::string& entity_reference,
                       const std::string& base_path) {
  return GetEntity(entity_reference, base_path, gates_);
}

template <class T>
T* Model::GetEntity(const std::string& entity_reference,
                    const std::string& base_path,
                    const LookupTable<T>& container) {
  assert(!entity_reference.empty());
  if (!base_path.empty()) {  // Check the local scope.
    if (auto it = ext::find(container.entities_by_path,
                            base_path + "." + entity_reference))
      return it->get();
  }

  auto at = [&entity_reference](const auto& reference_container) {
    if (auto it = ext::find(reference_container, entity_reference))
      return it->get();
    throw std::out_of_range("The event cannot be found.");
  };

  if (entity_reference.find('.') == std::string::npos)  // Public entity.
    return at(container.entities_by_id);

  return at(container.entities_by_path);  // Direct access.
}

/// Helper macro for Model::GetEvent event discovery.
#define GET_EVENT(access, path_reference)                          \
  do {                                                             \
    if (auto it = ext::find(gates_.access, path_reference))        \
      return it->get();                                            \
    if (auto it = ext::find(basic_events_.access, path_reference)) \
      return it->get();                                            \
    if (auto it = ext::find(house_events_.access, path_reference)) \
      return it->get();                                            \
  } while (false)

Formula::EventArg Model::GetEvent(const std::string& entity_reference,
                                  const std::string& base_path) {
  assert(!entity_reference.empty());
  if (!base_path.empty()) {  // Check the local scope.
    std::string full_path = base_path + "." + entity_reference;
    GET_EVENT(entities_by_path, full_path);
  }

  if (entity_reference.find('.') == std::string::npos) {  // Public entity.
    GET_EVENT(entities_by_id, entity_reference);
  } else {  // Direct access.
    GET_EVENT(entities_by_path, entity_reference);
  }
  throw std::out_of_range("The event cannot be bound.");
}

#undef GET_EVENT

}  // namespace mef
}  // namespace scram
