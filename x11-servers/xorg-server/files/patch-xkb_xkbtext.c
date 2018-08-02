--- xkb/xkbtext.c.orig	2016-07-15 16:18:11 UTC
+++ xkb/xkbtext.c
@@ -47,23 +47,27 @@
 
 /***====================================================================***/
 
-#define	BUFFER_SIZE	512
+#define NUM_BUFFER      8
+static struct textBuffer {
+    int size;
+    char *buffer;
+} textBuffer[NUM_BUFFER];
+static int textBufferIndex;
 
-static char textBuffer[BUFFER_SIZE];
-static int tbNext = 0;
-
 static char *
 tbGetBuffer(unsigned size)
 {
-    char *rtrn;
+    struct textBuffer *tb;
 
-    if (size >= BUFFER_SIZE)
-        return NULL;
-    if ((BUFFER_SIZE - tbNext) <= size)
-        tbNext = 0;
-    rtrn = &textBuffer[tbNext];
-    tbNext += size;
-    return rtrn;
+    tb = &textBuffer[textBufferIndex];
+    textBufferIndex = (textBufferIndex + 1) % NUM_BUFFER;
+
+    if (size > tb->size) {
+        free(tb->buffer);
+        tb->buffer = xnfalloc(size);
+        tb->size = size;
+    }
+    return tb->buffer;
 }
 
 /***====================================================================***/
@@ -79,8 +83,6 @@ XkbAtomText(Atom atm, unsigned format)
         int len;
 
         len = strlen(atmstr) + 1;
-        if (len > BUFFER_SIZE)
-            len = BUFFER_SIZE - 2;
         rtrn = tbGetBuffer(len);
         strlcpy(rtrn, atmstr, len);
     }
@@ -128,8 +130,6 @@ XkbVModIndexText(XkbDescPtr xkb, unsigned ndx, unsigne
     len = strlen(tmp) + 1;
     if (format == XkbCFile)
         len += 4;
-    if (len >= BUFFER_SIZE)
-        len = BUFFER_SIZE - 1;
     rtrn = tbGetBuffer(len);
     if (format == XkbCFile) {
         strcpy(rtrn, "vmod_");
@@ -140,6 +140,8 @@ XkbVModIndexText(XkbDescPtr xkb, unsigned ndx, unsigne
     return rtrn;
 }
 
+#define VMOD_BUFFER_SIZE        512
+
 char *
 XkbVModMaskText(XkbDescPtr xkb,
                 unsigned modMask, unsigned mask, unsigned format)
@@ -147,7 +149,7 @@ XkbVModMaskText(XkbDescPtr xkb,
     register int i, bit;
     int len;
     char *mm, *rtrn;
-    char *str, buf[BUFFER_SIZE];
+    char *str, buf[VMOD_BUFFER_SIZE];
 
     if ((modMask == 0) && (mask == 0)) {
         rtrn = tbGetBuffer(5);
@@ -173,7 +175,7 @@ XkbVModMaskText(XkbDescPtr xkb,
                 len = strlen(tmp) + 1 + (str == buf ? 0 : 1);
                 if (format == XkbCFile)
                     len += 4;
-                if ((str - (buf + len)) <= BUFFER_SIZE) {
+                if ((str - (buf + len)) <= VMOD_BUFFER_SIZE) {
                     if (str != buf) {
                         if (format == XkbCFile)
                             *str++ = '|';
@@ -199,8 +201,6 @@ XkbVModMaskText(XkbDescPtr xkb,
         len = 0;
     if (str)
         len += strlen(str) + (mm == NULL ? 0 : 1);
-    if (len >= BUFFER_SIZE)
-        len = BUFFER_SIZE - 1;
     rtrn = tbGetBuffer(len + 1);
     rtrn[0] = '\0';
 
