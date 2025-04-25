#ifndef IL2CPP_H
#define IL2CPP_H

#define TYPE_ATTRIBUTE_VISIBILITY_MASK       0x00000007
#define TYPE_ATTRIBUTE_NOT_PUBLIC            0x00000000
#define TYPE_ATTRIBUTE_PUBLIC                0x00000001
#define TYPE_ATTRIBUTE_NESTED_PUBLIC         0x00000002
#define TYPE_ATTRIBUTE_NESTED_PRIVATE        0x00000003
#define TYPE_ATTRIBUTE_NESTED_FAMILY         0x00000004
#define TYPE_ATTRIBUTE_NESTED_ASSEMBLY       0x00000005
#define TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM  0x00000006
#define TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM   0x00000007

#define TYPE_ATTRIBUTE_LAYOUT_MASK           0x00000018
#define TYPE_ATTRIBUTE_AUTO_LAYOUT           0x00000000
#define TYPE_ATTRIBUTE_SEQUENTIAL_LAYOUT     0x00000008
#define TYPE_ATTRIBUTE_EXPLICIT_LAYOUT       0x00000010

#define TYPE_ATTRIBUTE_CLASS_SEMANTIC_MASK   0x00000020
#define TYPE_ATTRIBUTE_CLASS                 0x00000000
#define TYPE_ATTRIBUTE_INTERFACE             0x00000020

#define TYPE_ATTRIBUTE_ABSTRACT              0x00000080
#define TYPE_ATTRIBUTE_SEALED                0x00000100
#define TYPE_ATTRIBUTE_SPECIAL_NAME          0x00000400

#define TYPE_ATTRIBUTE_IMPORT                0x00001000
#define TYPE_ATTRIBUTE_SERIALIZABLE          0x00002000

#define TYPE_ATTRIBUTE_STRING_FORMAT_MASK    0x00030000
#define TYPE_ATTRIBUTE_ANSI_CLASS            0x00000000
#define TYPE_ATTRIBUTE_UNICODE_CLASS         0x00010000
#define TYPE_ATTRIBUTE_AUTO_CLASS            0x00020000

#define TYPE_ATTRIBUTE_BEFORE_FIELD_INIT     0x00100000
#define TYPE_ATTRIBUTE_FORWARDER             0x00200000

#define TYPE_ATTRIBUTE_RESERVED_MASK         0x00040800
#define TYPE_ATTRIBUTE_RT_SPECIAL_NAME       0x00000800
#define TYPE_ATTRIBUTE_HAS_SECURITY          0x00040000

#define FIELD_ATTRIBUTE_FIELD_ACCESS_MASK     0x0007
#define FIELD_ATTRIBUTE_COMPILER_CONTROLLED   0x0000
#define FIELD_ATTRIBUTE_PRIVATE               0x0001
#define FIELD_ATTRIBUTE_FAM_AND_ASSEM         0x0002
#define FIELD_ATTRIBUTE_ASSEMBLY              0x0003
#define FIELD_ATTRIBUTE_FAMILY                0x0004
#define FIELD_ATTRIBUTE_FAM_OR_ASSEM          0x0005
#define FIELD_ATTRIBUTE_PUBLIC                0x0006

#define FIELD_ATTRIBUTE_STATIC                0x0010
#define FIELD_ATTRIBUTE_INIT_ONLY             0x0020
#define FIELD_ATTRIBUTE_LITERAL               0x0040
#define FIELD_ATTRIBUTE_NOT_SERIALIZED        0x0080
#define FIELD_ATTRIBUTE_SPECIAL_NAME          0x0200
#define FIELD_ATTRIBUTE_PINVOKE_IMPL          0x2000

#define FIELD_ATTRIBUTE_RESERVED_MASK         0x9500
#define FIELD_ATTRIBUTE_RT_SPECIAL_NAME       0x0400
#define FIELD_ATTRIBUTE_HAS_FIELD_MARSHAL     0x1000
#define FIELD_ATTRIBUTE_HAS_DEFAULT           0x8000
#define FIELD_ATTRIBUTE_HAS_FIELD_RVA         0x0100

#define METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK        0x0007
#define METHOD_ATTRIBUTE_COMPILER_CONTROLLED       0x0000
#define METHOD_ATTRIBUTE_PRIVATE                   0x0001
#define METHOD_ATTRIBUTE_FAM_AND_ASSEM             0x0002
#define METHOD_ATTRIBUTE_ASSEM                     0x0003
#define METHOD_ATTRIBUTE_FAMILY                    0x0004
#define METHOD_ATTRIBUTE_FAM_OR_ASSEM              0x0005
#define METHOD_ATTRIBUTE_PUBLIC                    0x0006

#define METHOD_ATTRIBUTE_STATIC                    0x0010
#define METHOD_ATTRIBUTE_FINAL                     0x0020
#define METHOD_ATTRIBUTE_VIRTUAL                   0x0040
#define METHOD_ATTRIBUTE_HIDE_BY_SIG               0x0080

#define METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK        0x0100
#define METHOD_ATTRIBUTE_REUSE_SLOT                0x0000
#define METHOD_ATTRIBUTE_NEW_SLOT                  0x0100

#define METHOD_ATTRIBUTE_STRICT                    0x0200
#define METHOD_ATTRIBUTE_ABSTRACT                  0x0400
#define METHOD_ATTRIBUTE_SPECIAL_NAME              0x0800

#define METHOD_ATTRIBUTE_PINVOKE_IMPL              0x2000
#define METHOD_ATTRIBUTE_UNMANAGED_EXPORT          0x0008

#define METHOD_ATTRIBUTE_RESERVED_MASK             0xd000
#define METHOD_ATTRIBUTE_RT_SPECIAL_NAME           0x1000
#define METHOD_ATTRIBUTE_HAS_SECURITY              0x4000
#define METHOD_ATTRIBUTE_REQUIRE_SEC_OBJECT        0x8000

#define PARAM_ATTRIBUTE_IN                 0x0001
#define PARAM_ATTRIBUTE_OUT                0x0002
#define PARAM_ATTRIBUTE_OPTIONAL           0x0010
#define PARAM_ATTRIBUTE_RESERVED_MASK      0xf000
#define PARAM_ATTRIBUTE_HAS_DEFAULT        0x1000
#define PARAM_ATTRIBUTE_HAS_FIELD_MARSHAL  0x2000
#define PARAM_ATTRIBUTE_UNUSED             0xcfe0

#include <codecvt>
#include <locale>

enum Il2CppTypeEnum {
    IL2CPP_TYPE_END = 0x00,
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0a,
    IL2CPP_TYPE_U8 = 0x0b,
    IL2CPP_TYPE_R4 = 0x0c,
    IL2CPP_TYPE_R8 = 0x0d,
    IL2CPP_TYPE_STRING = 0x0e,
    IL2CPP_TYPE_PTR = 0x0f,
    IL2CPP_TYPE_BYREF = 0x10,
    IL2CPP_TYPE_VALUETYPE = 0x11,
    IL2CPP_TYPE_CLASS = 0x12,
    IL2CPP_TYPE_VAR = 0x13,
    IL2CPP_TYPE_ARRAY = 0x14,
    IL2CPP_TYPE_GENERICINST = 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1b,
    IL2CPP_TYPE_OBJECT = 0x1c,
    IL2CPP_TYPE_SZARRAY = 0x1d,
    IL2CPP_TYPE_MVAR = 0x1e,
    IL2CPP_TYPE_CMOD_REQD = 0x1f,
    IL2CPP_TYPE_CMOD_OPT = 0x20,
    IL2CPP_TYPE_INTERNAL = 0x21,
    IL2CPP_TYPE_MODIFIER = 0x40,
    IL2CPP_TYPE_SENTINEL = 0x41,
    IL2CPP_TYPE_PINNED = 0x45,
    IL2CPP_TYPE_ENUM = 0x55,
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xff
};

struct Il2CppType {
    union {
        void* dummy;
        int klassIndex;
        Il2CppType* type;
    } data;
    unsigned int attrs: 16;
    Il2CppTypeEnum type: 8;
};

struct Il2CppImage;

struct Il2CppClass {
    Il2CppImage *image;
    void *gc_desc;
    const char *name;
    const char *namespaze;
    Il2CppType *byval_arg;
};

struct Il2CppField {
    const char *name;
    Il2CppType *type;
    Il2CppClass *klass;
    int offset;
    uint32_t token;
    uint32_t flags;
};

struct Il2CppMethod {
    void *method_pointer;
};

struct Il2CppAssembly;

struct Il2CppImage {
    const char *name;
    const char *name_no_ext;
    Il2CppAssembly *assembly;
    int typeCount;
};

struct Il2CppAssembly {
    Il2CppImage *image;
    const char *name;
};

struct Il2CppString {
    Il2CppClass *klass;
    void *monitor;
    int32_t m_stringLength;
    uint16_t m_firstChar;

    const char *ToChar() {
        std::u16string s(&m_firstChar, &m_firstChar + m_stringLength);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(s).c_str();
    }
};

#endif
