variable
   bool  r1, g1, g2, g3, g4, g22, g12, g57, g5, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, g6, g7, g8, g9, g18, g19, g34, g35, g45, g49, g67, e76, g50, e79, g10, g11, e12, e39, e13, e14, e15, e16, g13, g14, g59, g61, g15, g16, g17, e28, e29, e35, e34, e38, e41, g20, g21, e17, e23, e18, e19, e20, g23, g24, g25, e21, e22, g79, g74, g72, g73, g66, g26, e24, g27, g28, e25, e31, g29, e26, e27, g30, e30, g31, e32, g32, e33, g33, e36, e37, e40, g36, g37, g38, g39, e42, g40, g41, e92, e78, g42, g43, g44, e43, e44, e45, e51, e52, e64, e65, e50, e46, e47, e54, e71, e48, e49, g46, g47, g48, e73, e74, e77, e53, e55, e56, e57, e58, e59, e60, e61, e62, g51, g52, e63, e70, g54, g53, e66, e67, e68, e69, g60, g55, g56, e72, e75, g58, e88, e91, g75, g69, g78, e89, e90, g68, e80, e81, e82, e83, g62, g63, e84, g64, g65, e85, e86, e87, e93, g70, g71, e94, e96, e97, e95, e98, e100, e99, g76, g77;

rule
   (r1==(g1 || g2 || g3 || g4));
   (g1==(g22 && g12 && g57));
   (g2==(g22 && g57 && g5));
   (g3==(g12 && g57 && g5));
   (g4==(g22 && g12 && g5));
   (g5==(e1 || e2 || e3 || e4 || e5 || e6 || e7 || e8 || e9 || e10 || e11 || g6 || g7 || g8 || g9));
   (g6==(g18 && g19 && g34 && g35));
   (g7==(g45 && g49));
   (g8==(g67 && e76));
   (g9==(g50 && e79));
   (g10==(g11 && e12));
   (g11==e39);
   (g12==(e13 || e14 || e2 || e3 || e4 || e5 || e6 || e15 || e16 || e8 || e9 || e10 || g6 || g7 || g13 || g14));
   (g13==(g59 && e76));
   (g14==(g61 && e79));
   (g15==(g16 && g17 && g18 && g19));
   (g16==(e28 || e29));
   (g17==(e35 || e34));
   (g18==e38);
   (g19==e41);
   (g20==(g21 && e17));
   (g21==e23);
   (g22==(e4 || e5 || e6 || e7 || e15 || e16 || e18 || e19 || e20 || g23 || g24 || g25 || e21 || e22));
   (g23==(g79 && g74));
   (g24==(g72 && g73));
   (g25==(g66 && e79));
   (g26==e24);
   (g27==(g28 && e25));
   (g28==e31);
   (g29==(e26 || e27));
   (g30==(g26 && e30));
   (g31==(e32 || e27));
   (g32==(e33 || e34));
   (g33==(e36 || e34));
   (g34==(e37 || e29));
   (g35==(e40 || e34));
   (g36==(g18 && g19 && g29 && g32));
   (g37==(g18 && g19 && g31 && g33));
   (g38==(g19 && g39));
   (g39==e42);
   (g40==(g19 && g41));
   (g41==(e92 || e78));
   (g42==(g43 && g44));
   (g43==(e43 || e44 || e45 || e51 || e52 || e64 || e65 || e24 || e50 || g20 || g36));
   (g44==(e43 || e51 || e65 || e23 || e46 || e47 || e50 || e54 || e71 || g30 || g37));
   (g45==(e43 || e44 || e45 || e46 || e47 || e39 || e48 || e49 || e50 || g27 || g15));
   (g46==(g18 && g19 && g47 && g48));
   (g47==(e73 || e74));
   (g48==(e77 || e78));
   (g49==(e44 || e51 || e52 || e46 || e49 || e31 || e53 || e50 || e54 || g10 || g6));
   (g50==(e55 || e56 || e57 || e58 || e59 || e60 || e61 || e62));
   (g51==(g52 && e63));
   (g52==(e70 || g54));
   (g53==(e57 || e66 || e67 || e68 || e69 || e58 || e60 || e61));
   (g54==(g19 && g60));
   (g55==(g56 && e72));
   (g56==(e75 || g58));
   (g57==(e14 || e4 || e6 || e15 || e18 || e88 || e9 || e11 || e91 || g75 || g69 || g78 || e89 || e90));
   (g58==(g18 && g19 && g68 && g41));
   (g59==(e80 || g58));
   (g60==(e81 || e78));
   (g61==(e56 || e82 || e57 || e83 || e68 || e69 || e60 || e62));
   (g62==(g63 && e72));
   (g63==(e84 || g46));
   (g64==(g65 && e63));
   (g65==(e85 || g40));
   (g66==(e57 || e66 || e86 || e83 || e69 || e59 || e61 || e62));
   (g67==(e87 || g46));
   (g68==(e93 || e74));
   (g69==(g70 && g71));
   (g70==(e2 || e94 || e96 || e5 || e8 || g37 || g42 || g62 || e97 || e21 || e89));
   (g71==(e95 || e96 || e3 || e5 || e8 || g38 || g64 || e98 || e22 || e90));
   (g72==(e95 || e96 || e3 || e5 || e8 || g38 || g51 || e100 || e22 || e90));
   (g73==(e2 || e94 || e96 || e5 || e8 || g37 || g42 || g55 || e99 || e21 || e89));
   (g74==(e88 || e89 || e90));
   (g75==(g76 && g77));
   (g76==(e14 || e2 || e94 || e95 || e96 || e3 || e4 || e5 || e6 || e7 || e15 || e18 || e88 || e8 || e9 || e11 || e91 || g37 || g38 || g42 || g64 || g62 || g78 || e97 || e98 || e21 || e89 || e22 || e90));
   (g77==(e20 || e21 || e22));
   (g78==(g53 && e79));
   (g79==(e2 || e94 || e95 || e96 || e3 || e4 || e5 || e6 || e7 || e15 || e16 || e18 || e19 || e20 || e8 || g37 || g38 || g42 || g51 || g55 || g25 || e99 || e100 || e21 || e89 || e22 || e90));
