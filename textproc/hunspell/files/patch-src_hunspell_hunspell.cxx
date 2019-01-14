Add back `const` keyword to keep consumers forward compatible by
reverting https://github.com/hunspell/hunspell/commit/dd4b14899bfb

--- src/hunspell/hunspell.cxx.orig	2018-11-12 20:38:56 UTC
+++ src/hunspell/hunspell.cxx
@@ -2122,8 +2122,8 @@ void Hunspell::free_list(char*** slst, int n) {
   m_Impl->free_list(slst, n);
 }
 
-char* Hunspell::get_dic_encoding() {
-  return m_Impl->get_dic_encoding();
+const char* Hunspell::get_dic_encoding() const {
+  return Hunspell_get_dic_encoding((Hunhandle*)(this));
 }
 
 int Hunspell::analyze(char*** slst, const char* word) {
@@ -2180,7 +2180,7 @@ int Hunspell_spell(Hunhandle* pHunspell, const char* w
   return reinterpret_cast<HunspellImpl*>(pHunspell)->spell(word);
 }
 
-char* Hunspell_get_dic_encoding(Hunhandle* pHunspell) {
+const char* Hunspell_get_dic_encoding(Hunhandle* pHunspell) {
   return reinterpret_cast<HunspellImpl*>(pHunspell)->get_dic_encoding();
 }
 
