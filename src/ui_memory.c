#include<memory/ui_memory.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
static void* default_alloc(struct UI_Allocator* allocator,size_t size){
 (void)allocator;
 printf("Allocating %z bytes \n",size);
 return malloc(size);
}

static void* default_realloc(struct UI_Allocator* alloc,void* ptr,size_t size){
    (void)alloc;
    printf("Reallocating to %z bytes \n",size);
    return realloc(ptr,size);
}

static void default_free(struct UI_Allocator* alloc,void* ptr){
    (void)alloc;
     printf("Freeing memory \n");
    free(ptr);
}

static void* default_calloc(struct UI_Allocator* alloc,size_t count,size_t sz){
    (void)alloc;
    return calloc(count,sz);
    
}
static struct UI_Allocator default_allocator=
{
 .allocfun=default_alloc,
 .reallocfun=default_realloc,
 .callocfun=default_calloc,
 .freefun=default_free,
 .user_data=NULL

};

struct UI_Allocator* ui_default_allocator(void){
    return &default_allocator;
}
void* ui_alloc(struct UI_Allocator*allocator,size_t size){
   assert(allocator!=NULL); //it only throws,if it becomes false of the condition specified
   assert(allocator->allocfun!=NULL);

    return allocator->allocfun(allocator,size);
}

void* ui_calloc(struct UI_Allocator*alloc,size_t count,size_t size){
     assert(alloc!=NULL);

    assert(alloc->callocfun!=NULL);
    return alloc->callocfun(alloc,count,size);
}

void* ui_realloc(struct UI_Allocator*alloc,void* ptr,size_t newsize){
    assert(alloc!=NULL);

    assert(alloc->reallocfun!=NULL);
    return alloc->reallocfun(alloc,ptr,newsize);
}

void ui_free(struct UI_Allocator* alloc,void* ptr){
    assert(alloc!=NULL);
    assert(alloc->freefun!=NULL);

    alloc->freefun(alloc,ptr);
    return;
}