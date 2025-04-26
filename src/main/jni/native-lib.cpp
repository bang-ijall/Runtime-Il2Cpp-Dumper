#include <string>
#include <thread>
#include <unistd.h>
#include <xdl/xdl.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <jni.h>

#include "include/zygisk.h"
#include "il2cpp-struct.h"
#include "include/macros.h"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

uintptr_t il2cpp_base;

#define DO_API(r, n, p) r (*n) p
#include "il2cpp-api.h"
#undef DO_API

#include "il2cpp.h"

std::string get_genericname(std::string value, bool is = false) {
    size_t generic_start = value.find('<');
    std::string base = value;
    std::string generic;
    std::string suffix;

    if (generic_start != std::string::npos) {
        base = value.substr(0, generic_start);
        size_t generic_end = value.rfind('>');

        if (generic_end != std::string::npos && generic_end > generic_start) {
            generic = value.substr(generic_start + 1, generic_end - generic_start - 1);

            if (generic_end + 1 < value.length()) {
                suffix = value.substr(generic_end + 1);
            }
        }
    }

    std::string simple_base = base;

    if (is) {
        size_t last_dot = base.rfind('.');
        simple_base = (last_dot != std::string::npos) ? base.substr(last_dot + 1) : base;
    }

    if (!generic.empty()) {
        std::vector<std::string> simplified_parts;
        std::vector<std::string> parts;
        int depth = 0;
        std::string current;

        for (char ch : generic) {
            if (ch == '<') depth++;
            else if (ch == '>') depth--;

            if (ch == ',' && depth == 0) {
                parts.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }

        if (!current.empty()) {
            parts.push_back(current);
        }

        for (auto part : parts) {
            simplified_parts.push_back(get_genericname(part, true));
        }

        std::string join;

        for (size_t i = 0; i < simplified_parts.size(); ++i) {
            join += simplified_parts[i];

            if (i < simplified_parts.size() - 1) {
                join += ", ";
            }
        }

        return simple_base + "<" + join + ">" + suffix;
    }

    return simple_base + suffix;
}

std::string get_simplename(Il2CppType *type) {
    std::string name = il2cpp_type_get_name(type);
    Il2CppClass *klass = il2cpp_class_from_type(type);
    std::string namespaze = il2cpp_class_get_namespace ? il2cpp_class_get_namespace(klass) : klass->namespaze;

    if (name.find(namespaze) != std::string::npos) {
        name = name.substr(namespaze.empty() ? 0 : namespaze.length() + 1);

        if (namespaze.empty()) {
            size_t check_generic = name.find("<");

            if (check_generic != std::string::npos) {
                std::string base = name.substr(0, check_generic);
                size_t last_dot = base.rfind(".");

                if (last_dot != std::string::npos) {
                    name = name.substr(last_dot + 1, name.length());
                }
            } else {
                size_t last_dot = name.rfind(".");

                if (last_dot != std::string::npos) {
                    name = name.substr(last_dot + 1, name.length());
                }
            }
        }
    }

    name = get_genericname(name);
    Il2CppTypeEnum type_type = il2cpp_type_get_type ? il2cpp_type_get_type(type) : type->type;

    if (type_type == IL2CPP_TYPE_SZARRAY) {
        return get_simplename(type->data.type) + "[]";
    }

    while (klass = il2cpp_class_get_declaring_type(klass)) {
        std::string declare = il2cpp_class_get_name ? il2cpp_class_get_name(klass) : klass->name;
        size_t backtick = declare.find('`');

        if (backtick != std::string::npos) {
            declare = declare.substr(0, backtick);
        }

        name = declare + "." + name;
    }

    return name;
}

std::string dump_fields(Il2CppClass *klass, bool is_valuetype) {
    std::stringstream result;
    std::stringstream value;
    void *iter = 0;

    while (Il2CppField *field = il2cpp_class_get_fields(klass, &iter)) {
        result << "\t";
        int flags = il2cpp_field_get_flags(field);
        int access = flags & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;

        switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE:
                result << "private ";
                break;
            case FIELD_ATTRIBUTE_PUBLIC:
                result << "public ";
                break;
            case FIELD_ATTRIBUTE_FAMILY:
                result << "protected ";
                break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                result << "internal ";
                break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                result << "protected internal ";
                break;
        }

        if (flags & FIELD_ATTRIBUTE_LITERAL) {
            result << "const ";
        } else {
            if (flags & FIELD_ATTRIBUTE_STATIC) {
                result << "static ";
            }

            if (flags & FIELD_ATTRIBUTE_INIT_ONLY) {
                result << "readonly ";
            }
        }

        Il2CppType *type = il2cpp_field_get_type ? il2cpp_field_get_type(field) : field->type;
        const char *name = il2cpp_field_get_name ? il2cpp_field_get_name(field) : field->name;
        result << get_simplename(type) << " " << name;

        if (flags & FIELD_ATTRIBUTE_LITERAL) {
            result << " = ";
            Il2CppTypeEnum type_type = il2cpp_type_get_type ? il2cpp_type_get_type(type) : type->type;

            switch (type_type) {
                case IL2CPP_TYPE_BOOLEAN: {
                    bool val = false;
                    il2cpp_field_static_get_value(field, &val);
                    result << (val ? "true" : "false");
                    break;
                }
                case IL2CPP_TYPE_I1: {
                    int8_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << (int) val;
                    break;
                }
                case IL2CPP_TYPE_U1: {
                    uint8_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << (uint32_t) val;
                    break;
                }
                case IL2CPP_TYPE_CHAR: {
                    uint16_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << "'\\x" << val;
                    break;
                }
                case IL2CPP_TYPE_I2: {
                    int16_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_U2: {
                    uint16_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_I4: {
                    int val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_U4: {
                    uint32_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_I8: {
                    int64_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_U8: {
                    uint64_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_R4: {
                    float val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_R8: {
                    double val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
                case IL2CPP_TYPE_STRING: {
                    Il2CppString *str = nullptr;
                    il2cpp_field_static_get_value(field, &str);

                    if (str) {
                        std::string valu = str->ToChar();
                        result << "\"" << valu << "\"";
                    }

                    break;
                }
                default: {
                    uintptr_t val = 0;
                    il2cpp_field_static_get_value(field, &val);
                    result << std::dec << val;
                    break;
                }
            }

            result << ";";
        } else {
            int offset = il2cpp_field_get_offset ? il2cpp_field_get_offset(field) : field->offset;

            if (!(flags & FIELD_ATTRIBUTE_STATIC)) {
                offset = is_valuetype && offset >= 16 ? offset - 16 : offset;
            }

            result << "; // 0x" << std::hex << std::uppercase << offset;
        }

        result << "\n";
    }

    if (iter != 0) {
        value << "\n\t// Fields\n" << result.str();
    }

    return value.str();
}

std::string dump_methods(Il2CppClass *klass) {
    std::stringstream result;
    std::stringstream value;
    void *iter = 0;

    while (auto method = il2cpp_class_get_methods(klass, &iter)) {
        void *pointer = method->method_pointer;

        if (pointer != 0) {
            uintptr_t hex = (uint64_t) pointer - il2cpp_base;
            result << "\n\t// 0x" << std::hex << std::uppercase << hex;
        }

        result << "\n\t";
        uint32_t iflags = 0;
        uint32_t flags = il2cpp_method_get_flags(method, &iflags);
        uint32_t access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;

        switch (access) {
            case METHOD_ATTRIBUTE_PRIVATE:
                result << "private ";
                break;
            case METHOD_ATTRIBUTE_PUBLIC:
                result << "public ";
                break;
            case METHOD_ATTRIBUTE_FAMILY:
                result << "protected ";
                break;
            case METHOD_ATTRIBUTE_ASSEM:
            case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
                result << "internal ";
                break;
            case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
                result << "protected internal ";
                break;
        }

        if (flags & METHOD_ATTRIBUTE_STATIC) {
            result << "static ";
        }

        if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
            result << "abstract ";

            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
                result << "override ";
            }
        } else if (flags & METHOD_ATTRIBUTE_FINAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
                result << "sealed override ";
            }
        } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
                result << "virtual ";
            } else {
                result << "override ";
            }
        }

        if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
            result << "extern ";
        }

        Il2CppType *type = il2cpp_method_get_return_type(method);

        if (il2cpp_type_is_byref(type)) {
            result << "ref ";
        }

        result << get_simplename(type) << " " << il2cpp_method_get_name(method) << "(";
        uint32_t param_count = il2cpp_method_get_param_count(method);

        for (int i = 0; i < param_count; ++i) {
            auto param = il2cpp_method_get_param(method, i);
            uint32_t attrs = param->attrs;

            if (il2cpp_type_is_byref(param)) {
                if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                    result << "out ";
                } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                    result << "in ";
                } else {
                    result << "ref ";
                }
            } else {
                if (attrs & PARAM_ATTRIBUTE_IN) {
                    result << "[In] ";
                }
                if (attrs & PARAM_ATTRIBUTE_OUT) {
                    result << "[Out] ";
                }
            }

            result << get_simplename(param) << " " << il2cpp_method_get_param_name(method, i);
            result << ", ";
        }

        if (param_count > 0) {
            result.seekp(-2, result.cur);
        }

        result << ") {}\n";
    }

    if (iter != 0) {
        value << "\n\t// Methods\n" << result.str();
    }

    return value.str();
}

std::string dump_class(Il2CppClass *klass) {
    std::stringstream result;
    const char *namespaze = il2cpp_class_get_namespace ? il2cpp_class_get_namespace(klass) : klass->namespaze;
    result << "// Namespace: " << namespaze << "\n";
    uint32_t flags = il2cpp_class_get_flags(klass);

    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        result << "[Serializable]\n";
    }

    std::vector<std::string> extends;
    Il2CppClass *parent = il2cpp_class_get_parent(klass);
    Il2CppType *type = il2cpp_class_get_type ? il2cpp_class_get_type(klass) : (Il2CppType *) &klass->byval_arg;
    bool is_valuetype = il2cpp_class_is_valuetype(klass);
    bool is_enum = il2cpp_class_is_enum(klass);

    if (parent != 0 && !is_valuetype && !is_enum) {
        Il2CppType *parent_type = il2cpp_class_get_type ? il2cpp_class_get_type(parent) : (Il2CppType *) &parent->byval_arg;
        Il2CppTypeEnum type_type = il2cpp_type_get_type ? il2cpp_type_get_type(parent_type) : parent_type->type;

        if (type_type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(get_simplename(parent_type));
        }
    }

    int visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;

    switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            result << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            result << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            result << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            result << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            result << "protected internal ";
            break;
    }

    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        result << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        result << "abstract ";
    } else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
        result << "sealed ";
    }

    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        result << "interface ";
    } else if (is_enum) {
        result << "enum ";
    } else if (is_valuetype) {
        result << "struct ";
    } else {
        result << "class ";
    }

    result << get_simplename(type);
    void *iter = 0;

    while (Il2CppClass *interface = il2cpp_class_get_interfaces(klass, &iter)) {
        Il2CppType *interface_type = il2cpp_class_get_type ? il2cpp_class_get_type(interface) : (Il2CppType *) &interface->byval_arg;
        extends.emplace_back(get_simplename(interface_type));
    }

    if (!extends.empty()) {
        result << " : " << extends[0];

        for (int k = 1; k < extends.size(); k++) {
            result << ", " << extends[k];
        }
    }

    result << " {" << dump_fields(klass, is_valuetype) << dump_methods(klass) << "}\n";
    return result.str();
}

void il2cpp_dump(std::string path) {
    LOGI("dumping...");
    std::vector<std::string> out;
    void *domain = il2cpp_domain_get();
    size_t size;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);
    int id;

    for (int i = 0; i < size; i++) {
        std::stringstream result;
        Il2CppImage *image = il2cpp_assembly_get_image ? il2cpp_assembly_get_image(assemblies[i]) : assemblies[i]->image;
        size_t class_count = il2cpp_image_get_class_count ? il2cpp_image_get_class_count(image) : image->typeCount;

        for (int j = 0; j < class_count; j++) {
            const char *name = il2cpp_image_get_name ? il2cpp_image_get_name(image) : image->name;
            result << "\n// Image: " << name << "\n";

            if (il2cpp_image_get_class) {
                Il2CppClass *klass = il2cpp_image_get_class(image, j);
                result << dump_class(klass);
            }
        }

        out.push_back(result.str());
    }

    LOGI("write dump file");
    std::string out_path = path.append("/files/dump.cs");
    std::ofstream content(out_path);
    content << "//\n//\tCredit: BangJALL\n//\n";

    for (int i = 0; i < out.size(); i++) {
        content << out[i];
    }

    content.close();
    LOGI("dumped");
}

void init(std::string path) {
    LOGD("thread: %d", gettid());
    init_setup();
    il2cpp_dump(path);
}

const char* getExternalFilesDirPath(JavaVM *jvm) {
    if (jvm != nullptr) {
        JNIEnv* env = nullptr;
        jvm->AttachCurrentThread(&env, nullptr);

        if (env != nullptr) {
            jclass activityThread = env->FindClass("android/app/ActivityThread");
            jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
            jobject activityThreadInstance = env->CallStaticObjectMethod(activityThread, currentActivityThread);
            jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
            jobject appContext = env->CallObjectMethod(activityThreadInstance, getApplication);
            jclass contextClass = env->FindClass("android/content/Context");
            jmethodID getExternalFilesDir = env->GetMethodID(contextClass, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");
            jobject fileObj = env->CallObjectMethod(appContext, getExternalFilesDir, nullptr);
            jclass fileClass = env->FindClass("java/io/File");
            jmethodID getPath = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");
            jstring pathString = (jstring) env->CallObjectMethod(fileObj, getPath);
            return env->GetStringUTFChars(pathString, 0);
        }
    }

    return 0;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *) {
    std::string dir = getExternalFilesDirPath(vm);
    size_t pos = dir.rfind("/");

    if (pos != std::string::npos) {
        dir = dir.substr(0, pos);
    }

    std::thread(init, dir).detach();
    return JNI_VERSION_1_6;
}

class Module : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
        int fd = api->connectCompanion();

        if (fd >= 0) {
            uint32_t len;
            read(fd, &len, sizeof(len));

            if (len > 0) {
                char *buf = new char[len + 1];
                read(fd, buf, len);
                buf[len] = '\0';
                file_content = std::string(buf);
                delete[] buf;
            }

            close(fd);
        }
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        const char *pkg = env->GetStringUTFChars(args->nice_name, nullptr);
        const char *data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);

        if (file_content.find(pkg) != std::string::npos) {
            is_init = true;
            path = new char[strlen(data_dir) + 1];
            strcpy(path, data_dir);
        } else {
            api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
        }

        env->ReleaseStringUTFChars(args->nice_name, pkg);
        env->ReleaseStringUTFChars(args->app_data_dir, data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (is_init) {
            std::thread(init, path).detach();
        }
    }

private:
    Api *api;
    JNIEnv *env;
    bool is_init = false;
    char *path = nullptr;
    std::string file_content;
};

static void companion_handler(int fd) {
    std::ifstream file("/data/local/tmp/il2cpp-dumper.txt");
    std::string content;

    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        file.close();
    }

    uint32_t len = content.size();
    write(fd, &len, sizeof(len));

    if (len > 0) {
        write(fd, content.c_str(), len);
    }
}

REGISTER_ZYGISK_MODULE(Module)
REGISTER_ZYGISK_COMPANION(companion_handler)
