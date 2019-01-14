--- src/hunspell/hunspell.hxx.orig	2018-11-12 20:38:56 UTC
+++ src/hunspell/hunspell.hxx
@@ -155,7 +155,7 @@ class LIBHUNSPELL_DLL_EXPORTED Hunspell {
   H_DEPRECATED void free_list(char*** slst, int n);
 
   const std::string& get_dict_encoding() const;
-  char* get_dic_encoding();
+  H_DEPRECATED const char* get_dic_encoding() const;
 
   /* morphological functions */
 
