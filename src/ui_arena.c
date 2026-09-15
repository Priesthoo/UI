#include<memory/ui_arena.h>

int ui_arena_init(UI_Arena*arena,UI_Allocator* backalloc,size_t capacity){
    if(arena==NULL){
        return 0;
    }
    if(backalloc==NULL){
        return 0;
    }
    if(capacity==0){
        return 0;
    }
    arena->buffer=ui_alloc(backalloc,capacity);
    if(arena->buffer==NULL){
        return 0;
    }
    arena->used=0;
    arena->capacity=capacity;
    arena->backing_allocator=backalloc;
    return 1;
}
//allocate block of memory from the arena
void* ui_arena_alloc(UI_Arena* arena,size_t size){
    if(arena==NULL){
        return NULL;
    }
    if(size==0){
        return NULL;
    }

    if(arena->used+size> arena->capacity){
        return NULL;
    }
    void* memory=arena->buffer+arena->used;
    arena->used+=size;

    return memory;
}

void ui_arena_reset(UI_Arena* arena){
    if(arena==NULL){
        return;
    }
    arena->used=0;
    return;
}

void ui_arena_destroy(UI_Arena* arena){
    if(arena==NULL){
        return;
    }
    if(arena->buffer!=NULL){
        ui_free(arena->backing_allocator,arena->buffer);
    }
    arena->buffer=NULL;
    arena->capacity=0;
    arena->used=0;
    arena->backing_allocator=NULL;
    return;
}