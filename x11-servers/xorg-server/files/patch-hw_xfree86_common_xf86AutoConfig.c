--- hw/xfree86/common/xf86AutoConfig.c.orig	2016-07-19 17:07:29 UTC
+++ hw/xfree86/common/xf86AutoConfig.c
@@ -267,7 +267,7 @@ listPossibleVideoDrivers(char *matches[], int nmatches
         i += xf86PciMatchDriver(&matches[i], nmatches - i);
 #endif
 
-#if defined(__linux__)
+#if defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__)
     matches[i++] = xnfstrdup("modesetting");
 #endif
 
@@ -276,8 +276,10 @@ listPossibleVideoDrivers(char *matches[], int nmatches
     if (i < (nmatches - 1)) {
 #if !defined(__linux__) && defined(__sparc__)
         matches[i++] = xnfstrdup("wsfb");
-#else
+#elif defined(__linux__)
         matches[i++] = xnfstrdup("fbdev");
+#elif defined(__FreeBSD__)
+        matches[i++] = xnfstrdup("scfb");
 #endif
     }
 #endif                          /* !sun */
