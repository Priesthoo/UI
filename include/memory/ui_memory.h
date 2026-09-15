/*Memory Allocator for Ui*/
#ifndef UI_MEMORY_H
#define UI_MEMORY_H

#include<stddef.h>

//we are creating a struct that allocates much memory 
typedef struct UI_Allocator UI_Allocator;
//This is a function that allocates our memory

//A function pointer that allocates memory 
typedef void* (*UI_AllocFn)(struct UI_Allocator* allocator,size_t size);

//A function that reallocates memory
typedef void*(*UI_ReallocFn)(struct UI_Allocator* allocator,void*ptr,size_t size);

//A function pointer that uses calloc()
typedef void*(*UI_CallocFn) (struct UI_Allocator* allocator,size_t count,size_t size);

typedef void (*UI_FreeFn)(struct UI_Allocator* allocator,void* ptr);

//Allocator interface
struct UI_Allocator{
UI_AllocFn allocfun;
UI_ReallocFn reallocfun;
UI_CallocFn callocfun;
UI_FreeFn freefun;
void* user_data;
};


//Default allocator 
struct UI_Allocator* ui_default_allocator(void);
void* ui_alloc(struct UI_Allocator*allocator,size_t size);

void* ui_calloc(struct UI_Allocator*alloc,size_t count,size_t size);

void* ui_realloc(struct UI_Allocator* alloc,void* ptr,size_t newsize);

void ui_free(struct UI_Allocator* alloc,void* ptr);


#endif