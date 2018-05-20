--- src/stub-cache.cc.orig	2018-05-20 22:22:01 UTC
+++ src/stub-cache.cc
@@ -1474,7 +1474,7 @@ Handle<Code> StubCompiler::GetCodeWithFlags(Code::Flag
                                             Handle<Name> name) {
   return (FLAG_print_code_stubs && !name.is_null() && name->IsString())
       ? GetCodeWithFlags(flags, *Handle<String>::cast(name)->ToCString())
-      : GetCodeWithFlags(flags, reinterpret_cast<char*>(NULL));
+      : GetCodeWithFlags(flags, static_cast<char*>(NULL));
 }
 
 
