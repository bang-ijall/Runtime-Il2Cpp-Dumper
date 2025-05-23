DO_API(bool, il2cpp_is_vm_thread, (void *));
DO_API(void *, il2cpp_thread_attach, (void *));
DO_API(void *, il2cpp_domain_get, ());
DO_API(Il2CppAssembly **, il2cpp_domain_get_assemblies, (void *, size_t *));
DO_API(Il2CppImage *, il2cpp_assembly_get_image, (Il2CppAssembly *));
DO_API(size_t, il2cpp_image_get_class_count, (Il2CppImage *));
DO_API(const char *, il2cpp_image_get_name, (Il2CppImage *));
DO_API(Il2CppClass *, il2cpp_image_get_class, (Il2CppImage *, size_t));
DO_API(const char *, il2cpp_class_get_namespace, (Il2CppClass *));
DO_API(const char *, il2cpp_class_get_name, (Il2CppClass *));
DO_API(Il2CppClass *, il2cpp_class_get_declaring_type, (Il2CppClass *));
DO_API(int, il2cpp_class_get_flags, (Il2CppClass *));
DO_API(bool, il2cpp_class_is_valuetype, (void *));
DO_API(bool, il2cpp_class_is_enum, (Il2CppClass *));
DO_API(Il2CppClass *, il2cpp_class_get_parent, (Il2CppClass *));
DO_API(Il2CppType *, il2cpp_class_get_type, (Il2CppClass *));
DO_API(Il2CppTypeEnum, il2cpp_type_get_type, (Il2CppType *));
DO_API(Il2CppClass *, il2cpp_class_get_interfaces, (Il2CppClass *, void **));
DO_API(Il2CppField *, il2cpp_class_get_fields, (Il2CppClass *, void **));
DO_API(int, il2cpp_field_get_flags, (Il2CppField *));
DO_API(Il2CppType *, il2cpp_field_get_type, (Il2CppField *));
DO_API(const char *, il2cpp_type_get_name, (Il2CppType *));
DO_API(const char *, il2cpp_field_get_name, (Il2CppField *));
DO_API(void, il2cpp_field_static_get_value, (Il2CppField *, void *));
DO_API(int, il2cpp_field_get_offset, (Il2CppField *));
DO_API(uint32_t, il2cpp_method_get_flags, (Il2CppMethod *, uint32_t *));
DO_API(Il2CppType *, il2cpp_method_get_return_type, (Il2CppMethod *));
DO_API(Il2CppType *, il2cpp_method_get_param, (Il2CppMethod *, uint32_t));
DO_API(Il2CppClass *, il2cpp_class_from_type, (Il2CppType *));
DO_API(Il2CppMethod *, il2cpp_class_get_methods, (Il2CppClass *, void **));
DO_API(bool, il2cpp_type_is_byref, (Il2CppType *));
DO_API(const char *, il2cpp_method_get_name, (Il2CppMethod *));
DO_API(uint32_t, il2cpp_method_get_param_count, (Il2CppMethod *));
DO_API(const char *, il2cpp_method_get_param_name, (Il2CppMethod *, uint32_t));
