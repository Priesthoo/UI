#ifndef UI_POOL_H
#define UI_POOL_H
#include<memory/ui_memory.h>
#include<stddef.h>
//To fix UI_Arena issue
#define UI_POOL_USED 0
#define UI_POOL_FREED 1
typedef struct{
    unsigned char* buffer;
    size_t block_size; //
    size_t block_count; //block size
    size_t free_count;

    UI_Allocator* back_allocator;
    
    void* free_list;
    //Debug information
    unsigned char* state;
}UI_Pool;

//Initialize a pool
int ui_pool_init(UI_Pool* pool,UI_Allocator* alloc,size_t block_size,size_t block_count);


//allocate one block and remove the allocated block from the free_list
void* ui_pool_alloc(UI_Pool* pool);

//Returns the block to the pool
void ui_pool_free(UI_Pool* pool,void* ptr);


//Reset the pool
void ui_pool_reset(UI_Pool* pool);


//Destroy the pool
void ui_pool_destroy(UI_Pool* pool);
#endif 