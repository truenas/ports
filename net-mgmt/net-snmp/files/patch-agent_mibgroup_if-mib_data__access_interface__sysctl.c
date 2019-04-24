--- agent/mibgroup/if-mib/data_access/interface_sysctl.c	2017-06-07 18:05:47.060166000 +0300
+++ agent/mibgroup/if-mib/data_access/interface_sysctl.c	2017-06-07 18:04:32.194621000 +0300
@@ -105,6 +105,9 @@ netsnmp_sysctl_ifmedia_to_speed(int media, u_int *spee
                 case IFM_100_T4:
                 case IFM_100_VG:
                 case IFM_100_T2:
+#ifdef IFM_100_T
+		case IFM_100_T:
+#endif
                     *speed = 100000000;
                     *speed_high = 100; break;
                 case IFM_1000_LX:
@@ -112,6 +115,18 @@ netsnmp_sysctl_ifmedia_to_speed(int media, u_int *spee
 #ifdef IFM_1000_T
                 case IFM_1000_T:
 #endif
+#ifdef IFM_1000_SX
+		case IFM_1000_SX:
+#endif
+#ifdef IFM_1000_KX
+		case IFM_1000_KX:
+#endif
+#ifdef IFM_1000_SGMII
+		case IFM_1000_SGMII:
+#endif
+#ifdef IFM_1000_CX_SGMII
+		case IFM_1000_CX_SGMII:
+#endif
                     *speed = 1000000000;
                     *speed_high = 1000; break;
 #ifdef IFM_10GBASE_SR
@@ -120,6 +135,119 @@ netsnmp_sysctl_ifmedia_to_speed(int media, u_int *spee
                     *speed = (u_int) -1; /* 4294967295; */
                     *speed_high = 10000; break;
 #endif
+#if defined(__FreeBSD__)
+#ifdef IFM_10G_LR
+		case IFM_10G_LR:
+#ifdef IFM_10G_SR
+		case IFM_10G_SR:
+#endif
+#ifdef IFM_10G_CX4
+		case IFM_10G_CX4:
+#endif
+#ifdef IFM_10G_TWINAX
+		case IFM_10G_TWINAX:
+#endif
+#ifdef IFM_10G_TWINAX_LONG
+		case IFM_10G_TWINAX_LONG:
+#endif
+#ifdef IFM_10G_LRM
+		case IFM_10G_LRM:
+#endif
+#ifdef IFM_10G_T
+		case IFM_10G_T:
+#endif
+#ifdef IFM_10G_KX4
+		case IFM_10G_KX4:
+#endif
+#ifdef IFM_10G_KR
+		case IFM_10G_KR:
+#endif
+#ifdef IFM_10G_CR1
+		case IFM_10G_CR1:
+#endif
+#ifdef IFM_10G_SFI
+		case IFM_10G_SFI:
+#endif
+#ifdef IFM_10G_ER
+		case IFM_10G_ER:
+#endif
+#ifdef IFM_10G_AOC
+		case IFM_10G_AOC:
+#endif
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 10000; break;
+#endif /* IFM_10G_LR */
+#ifdef IFM_20G_KR2
+		case IFM_20G_KR2:
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 20000; break;
+#endif /* IFM_20G_KR2 */
+#ifdef IFM_25G_PCIE
+		case IFM_25G_PCIE:
+#ifdef IFM_25G_CR
+		case IFM_25G_CR:
+#endif
+#ifdef IFM_25G_KR
+		case IFM_25G_KR:
+#endif
+#ifdef IFM_25G_SR
+		case IFM_25G_SR:
+#endif
+#ifdef IFM_25G_LR
+		case IFM_25G_LR:
+#endif
+#ifdef IFM_25G_ACC
+		case IFM_25G_ACC:
+#endif
+#ifdef IFM_25G_AOC
+		case IFM_25G_AOC:
+#endif
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 25000; break;
+#endif /* IFM_25G_PCIE */
+#ifdef IFM_40G_CR4
+		case IFM_40G_CR4:
+#ifdef IFM_40G_SR4
+		case IFM_40G_SR4:
+#endif
+#ifdef IFM_40G_LR4
+		case IFM_40G_LR4:
+#endif
+#ifdef IFM_40G_XLPPI
+		case IFM_40G_XLPPI:
+#endif
+#ifdef IFM_40G_KR4
+		case IFM_40G_KR4:
+#endif
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 40000; break;
+#endif /* IFM_40G_CR4 */
+#ifdef IFM_50G_PCIE
+		case IFM_50G_PCIE:
+#ifdef IFM_50G_CR2
+		case IFM_50G_CR2:
+#endif
+#ifdef IFM_50G_KR2
+		case IFM_50G_KR2:
+#endif
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 50000; break;
+#endif /* IFM_50G_PCIE */
+#ifdef IFM_100G_CR4
+		case IFM_100G_CR4:
+#ifdef IFM_100G_SR4
+		case IFM_100G_SR4:
+#endif
+#ifdef IFM_100G_KR4
+		case IFM_100G_KR4:
+#endif
+#ifdef IFM_100G_LR4
+		case IFM_100G_LR4:
+#endif
+                    *speed = (u_int) -1; /* 4294967295; */
+                    *speed_high = 100000; break;
+#endif /* IFM_100G_CR4 */
+#endif /* __FreeBSD__ */
             }
             break;
         case IFM_IEEE80211:
