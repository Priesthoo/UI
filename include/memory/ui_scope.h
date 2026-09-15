//This defines memory scope that contains group of allocations

#ifndef UI_SCOPE_H
#define UI_SCOPE_H
#include<stddef.h>

//Tackling memory ownership and lifetime
struct UI_Allocator;

struct UI_MemoryScope;

 struct UI_MemoryScope{
  struct UI_Allocator* allocator;
  struct UI_MemoryScope* parent; //parent scope
   void** allocations; //List of allocations
   size_t count;
   size_t capacity;

};

int ui_memoryscope_init(struct UI_MemoryScope* scope,struct UI_Allocator* allocator);

void* ui_memoryscope_alloc(struct UI_MemoryScope* scope,size_t size);

//First parameter is the child while the second parameter is the parent
int ui_memoryscope_init_child(struct UI_MemoryScope* scope,struct UI_MemoryScope* parent);

void ui_memoryscope_free(struct UI_MemoryScope* scope,void* ptr);

void ui_memoryscope_destroy(struct UI_MemoryScope* scope);





#endif