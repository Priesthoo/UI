
#include<memory/ui_scope.h>
#include<memory/ui_memory.h>
#include<stdlib.h>
#define UI_SCOPE_INITIAL_CAPACITY 16 //the initial reallocation size is 16,

int ui_memoryscope_init(struct UI_MemoryScope* scope,struct UI_Allocator* allocator){
    if(scope==NULL){
        return 0;
    }
    if(allocator==NULL){
        return 0;
    }
    scope->allocator=allocator;
    scope->parent=NULL;
    scope->allocations=NULL;
    scope->count=0;
    scope->capacity=0;
    return 1;
}
static int ui_memoryscope_grow(struct UI_MemoryScope*scope){
    if(scope==NULL){
        return 0;
    }
    
   size_t new_capacity;
   void **new_allocations;
    
    if(scope->capacity ==0 ){
        new_capacity=UI_SCOPE_INITIAL_CAPACITY;
    }
    else{
      //Prevent integer overflow

      if(scope->capacity > SIZE_MAX/2){
           return 0;
      }
     new_capacity=scope->capacity*2;
    }
    if(new_capacity>SIZE_MAX/sizeof(void*)){
         return 0;
    }
    new_allocations=realloc(scope->allocations,new_capacity* sizeof(void *));

    if(new_allocations == NULL){
        return 0;
    }
    scope->allocations=new_allocations;
    scope->capacity=new_capacity;
    
    return 1;
}

void* ui_memoryscope_alloc(struct UI_MemoryScope* scope,size_t size){
   void* ptr;

   if(scope==NULL){
    return NULL;
   }

   if(scope->allocator== NULL){
    return NULL;
   }

   if(size == 0){
    return NULL;
   }

   //Make room for another allocation
   if(scope->count >= scope->capacity){
    if(!ui_memoryscope_grow(scope)){
        return NULL;
    }
   }
   ptr=ui_alloc(scope->allocator,size);

   if(ptr== NULL){
    return NULL;
   }
   scope->allocations[scope->count]=ptr;
   scope->count++;
   return ptr;
}


void ui_memoryscope_free(struct UI_MemoryScope* scope,void* ptr){
    size_t i;

    if(scope == NULL){
        return;
    }
    if(ptr == NULL){
        return;
    }

    for( i=0;i<scope->count;i++){
        if(scope->allocations[i]==ptr){
            //Free the actual allocation
            ui_free(scope->allocator,ptr);

            //Move the last allocation into the removed slot , this avoids shifting every element after it
        scope->allocations[i]=scope->allocations[scope->count-1];
        scope->count--;
        break;
        }
        
    }
    return;
}


int ui_memoryscope_init_child(struct UI_MemoryScope* scope,struct UI_MemoryScope* parent){
    if(scope==NULL){
        return 0;
    }
    if(parent ==NULL){
        return 0;
    }
    if(parent->allocator==NULL){
        return 0;
    }
    scope->allocator=parent->allocator;
    scope->parent=parent;
    scope->allocations=NULL;
    scope->count=0;
    scope->capacity=0;
    return 1;
}


void ui_memoryscope_destroy(struct UI_MemoryScope* scope){
    size_t i;
    if(!scope){
        return;
    }
    //Free every allocation still owned by the scope
    for( i=0; i< scope->count;i++){
        ui_free(scope->allocator,scope->allocations[i]);
    }
     //Free the allocations tracking array itself

     free(scope->allocations);
      scope->allocations=NULL;
      scope->count=0;
      scope->parent=NULL;
      scope->capacity=0;
      scope->allocator=NULL;
    return;
}
