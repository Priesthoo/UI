#ifndef UI_ARENA_H
#define UI_ARENA_H
#include<memory/ui_memory.h>
#include<stddef.h>
//Create an arena of memory 
/*
[



]


*/

typedef struct{
unsigned char* buffer;
size_t capacity;
size_t used;
UI_Allocator* backing_allocator;
} UI_Arena;

//Create an arena from which we will get memory of different sizes
int ui_arena_init(UI_Arena* arena,UI_Allocator* backalloc,size_t capacity);

//Allocate memory from the arena
void* ui_arena_alloc(UI_Arena* arena,size_t size);

/*
Reset the arena 
*Memory is not returned to the operating system(OS)
*All allocations simply become available again

*/
void ui_arena_reset(UI_Arena* arena);

//Destroy the arena
void ui_arena_destroy(UI_Arena* arena);
#endif
//UI_Arena has different alignments issues