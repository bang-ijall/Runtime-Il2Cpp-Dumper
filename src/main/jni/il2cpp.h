void init_il2cpp(void *handle) {
#define DO_API(r, n, p) {                      \
    n = (r (*) p) xdl_sym(handle, #n, 0);      \
    if(!n) {                                   \
        LOGE("api not found %s", #n);          \
    }                                          \
}

#include "il2cpp-api.h"
#undef DO_API
}

void init_setup() {
    void *handle = xdl_open("libil2cpp.so", 0);

    while (handle == nullptr) {
        sleep(1);
        handle = xdl_open("libil2cpp.so", 0);
    }

    LOGI("il2cpp_handle: %p", handle);
    init_il2cpp(handle);
    xdl_close(handle);

    if (il2cpp_domain_get_assemblies) {
        Dl_info dlInfo;

        if (dladdr((void *) il2cpp_domain_get_assemblies, &dlInfo)) {
            il2cpp_base = (uintptr_t) dlInfo.dli_fbase;
        }

        LOGI("il2cpp has been loaded at: %p", il2cpp_base);
    } else {
        LOGE("Failed to initialize il2cpp api");
    }

    while (!il2cpp_is_vm_thread(nullptr)) {
        sleep(1);
        LOGI("Waiting for il2cpp_init...");
    }

    auto domain = il2cpp_domain_get();
    il2cpp_thread_attach(domain);
}
