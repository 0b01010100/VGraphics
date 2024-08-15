#include <VG.h>
#include <stdarg.h>
#include <stdio.h>
#include <vnew.h>
#include <vslib.h>

typedef void (*api_init)(const char *name, int width, int height, void **vg);
typedef int (*api_create_resource)(void* ctx, void * resource);
typedef void (*api_set_resource)(void* ctx, void * resource);
typedef int (*api_destroy_resource)(void* ctx, void * resource);

typedef struct VG {
    void *api_handle;  
    api_init init;
    api_create_resource resourceCtor;
    api_set_resource resourceSet;
    api_destroy_resource resourceDtor;
} VG;

VG *VG_ctor(VG_Win config)
{
    // Load the dynamic library
    vslib dll_so = vslib_Load("OGL_P", 1, 1);
    if (dll_so.handle == NULL) {
        fprintf(stderr, "Failed to load library.\n");
        return NULL;
    }

    // Get the function pointers from the library
    api_init ftn_init = (api_init)vslib_Getpfn(&dll_so, "OpenGL_Init");
    api_create_resource ftn_create = (api_create_resource)vslib_Getpfn(&dll_so, "OpenGL_InitResource");
    api_set_resource ftn_set = (api_set_resource)vslib_Getpfn(&dll_so, "OpenGL_SetResource");
    api_destroy_resource ftn_destroy = (api_destroy_resource)vslib_Getpfn(&dll_so, "OpenGL_UninitResource");
    

    if (ftn_init == NULL || ftn_create == NULL || ftn_set == NULL || ftn_destroy == NULL) {
        fprintf(stderr, "VG_ctor: Failed to get one or more function pointers.\n");
        vslib_Unload(&dll_so);
        return NULL;
    }

    // Allocate VG object
    VG *this = vnew(VG, 1);
    if (this == NULL) {
        fprintf(stderr, "VG_ctor: Failed to allocate memory for VG.\n");
        vslib_Unload(&dll_so);
        return NULL;
    }

    // Init VG object
    this->init = ftn_init;
    this->resourceCtor = ftn_create;
    this->resourceSet = ftn_set;
    this->resourceDtor = ftn_destroy;
    this->api_handle = NULL; 
    this->init(config.name, config.width, config.height, &this->api_handle);
    
    return this; 
}


VG_RESULT VG_ctor_Resources(VG *this_, unsigned char count, ...) {
    if (!this_ || !this_->resourceCtor) {
        return VG_ERROR_INVALID_PARAMETER;
    }

    va_list args;
    va_start(args, count);

    for (unsigned char i = 0; i < count; i++) {
        VG_Resource* resource = va_arg(args, VG_Resource*);

        int result =this_->resourceCtor(this_->api_handle, resource);
        if (result != 0) {
            va_end(args);
            return VG_ERROR_OUT_OF_MEMORY;  
        }
    }

    va_end(args);
    return VG_SUCCESS;
}


void VG_Set_Resources(VG *this_, unsigned char count, ...)
{
    if (!this_ || !this_->resourceSet) {
        return;
    }

    va_list args;
    va_start(args, count);

    for (unsigned char i = 0; i < count; i++) {
        VG_Resource* resource = va_arg(args, VG_Resource*);

        
        this_->resourceSet(this_->api_handle, resource);

    }

    va_end(args);
}

void VG_dtor_Resources(VG *this_, unsigned char count, ...)
{
    if (!this_ || !this_->resourceDtor) {
        return;
    }

    va_list args;
    va_start(args, count);

    for (unsigned char i = 0; i < count; i++) {
        VG_Resource* resource = va_arg(args, VG_Resource*);

        
        this_->resourceDtor(this_->api_handle, resource);
    }

    va_end(args);
}

void VG_Window_Loop(VG *this_)
{
    if (!this_) return;
    
}


void VG_Render(VG *this_)
{
    if (!this_) return;
    
}

void VG_Clear_Buffer(VG *this_)
{
    if (!this_) return;
    
}

void VG_dtor(VG *this_)
{
    if (!this_ || !this_->api_handle) return;
    
    vslib_Unload((vslib *)this_->api_handle);

    vdelete(this_);
}
