#include <VG.h>
#include <stdarg.h>
#include <stdio.h>
#include <vnew.h>
#include <vslib.h>

typedef void (*api_Init)(const char *name, int width, int height, void **vg);
typedef int (*api_LoadResource)(void* ctx, void * desc, void** out);
typedef void (*api_SetResource)(void* ctx, void * resource);
typedef int (*api_DestroyResource)(void* ctx, void * resource);

typedef struct VG {
    void *api_handle;  
    api_Init init;
    api_LoadResource resourceCtor;
    api_SetResource resourceSet;
    api_DestroyResource resourceDtor;
} VG;

// typedef struct VG_Resource{
//     void * UnUsed;
// }VG_Resource;

VG *VG_ctor(VG_Win config)
{
    // Load the dynamic library
    vslib dll_so = vslib_Load("OGL_P", 1, 1);
    if (dll_so.handle == NULL) {
        fprintf(stderr, "Failed to load library.\n");
        return NULL;
    }

    // Get the function pointers from the library
    api_Init ftn_init = (api_Init)vslib_Getpfn(&dll_so, "OGL_Init");
    api_LoadResource ftn_create = (api_LoadResource)vslib_Getpfn(&dll_so, "OGL_LoadResource");
    api_SetResource ftn_set = (api_SetResource)vslib_Getpfn(&dll_so, "OGL_SetResource");
    api_DestroyResource ftn_destroy = (api_DestroyResource)vslib_Getpfn(&dll_so, "OGL_UninitResource");
    

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


VG_Resource* VG_LoadResource(VG *this_, VG_Resource_Desc* desc, VG_RESULT* err) 
{
    VG_RESULT e = 0;
    if (!this_ || !this_->resourceCtor) {
        e =  VG_ERROR_INVALID_PARAMETER;
        if(err) *err = e;
        return NULL;
    }

    VG_Resource* out = NULL;
    e = this_->resourceCtor(this_->api_handle, desc, (void**)&out);
    if (err && err != 0) {
        e  = VG_ERROR_OUT_OF_MEMORY; 
        if(err) *err = e;
        return NULL;
    }
    return out;
}


void VG_SetResources(VG *this_, unsigned int count, ...)
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

void VG_UnLoadResources(VG *this_, unsigned int count, ...)
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
