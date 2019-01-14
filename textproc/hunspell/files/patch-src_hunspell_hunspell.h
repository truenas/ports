--- src/hunspell/hunspell.h.orig	2018-11-12 20:38:56 UTC
+++ src/hunspell/hunspell.h
@@ -68,7 +68,7 @@ LIBHUNSPELL_DLL_EXPORTED int Hunspell_add_dic(Hunhandl
  */
 LIBHUNSPELL_DLL_EXPORTED int Hunspell_spell(Hunhandle* pHunspell, const char*);
 
-LIBHUNSPELL_DLL_EXPORTED char* Hunspell_get_dic_encoding(Hunhandle* pHunspell);
+LIBHUNSPELL_DLL_EXPORTED const char* Hunspell_get_dic_encoding(Hunhandle* pHunspell);
 
 /* suggest(suggestions, word) - search suggestions
  * input: pointer to an array of strings pointer and the (bad) word
