#include<memory/ui_pool.h>
#include<stdint.h>
#include<string.h>

typedef struct UI_PoolNode{
   struct UI_PoolNode* next;
} UI_PoolNode;

static int ui_pool_contains(UI_Pool* pool,void* ptr){
   if(pool==NULL){
     return 0;
   }
   if(ptr == NULL){
      return 0;
   }
   unsigned char* address=(unsigned char*)ptr;

   unsigned char* begin=pool->buffer;
   unsigned char* end=pool->buffer+(pool->block_size+pool->block_count);

   if(address<begin){
    return 0;
   }
   if(address>=end){
    return 0;
   }
   size_t offset=(size_t)(address-begin);

   //the pointer must point exactly to the beginning of a block
 if(offset% pool->block_size){
    return 0;
 }
 return 1;
}

int ui_pool_init(UI_Pool* pool,UI_Allocator* alloc,size_t block_size,size_t block_count){
    if(pool==NULL){
        return 0;
    }
    if(alloc==NULL){
        return 0;
    }
    if(block_size==0){
        return 0;
    }
    if(block_count==0){
        return 0;
    }
    //it is either block_size is equal to sizeof(UI_PoolNode) or greater than it
    if(block_size<sizeof(UI_PoolNode)){
        block_size=sizeof(UI_PoolNode);
    }
    //
    if(block_count> SIZE_MAX/block_size){
        return 0;
    }
    size_t totalsize=block_count*block_size;

    pool->buffer=ui_alloc(alloc,totalsize);
    if(pool->buffer==NULL){
        return 0;
    }

    pool->block_size=block_size;
    pool->block_count=block_count;
    pool->free_count=block_count;
    
    pool->back_allocator=alloc;
    
    pool->free_list=NULL;
    //Build the freelist
    for(int i=0;i<block_count;i++){
        UI_PoolNode* node=(UI_PoolNode*)(pool->buffer+(i* block_size));
        node->next=pool->free_list;
        pool->free_list=node;
    }
    pool->state=ui_calloc(alloc,block_count,sizeof(unsigned char));
    
    if(pool->state==NULL){
        ui_free(alloc,pool->buffer);
    
    }
    pool->buffer=NULL;
   return 1; 
}


void* ui_pool_alloc(UI_Pool* pool){
   if(pool==NULL){
     return NULL;
   }

   if(pool->free_list==NULL){
    return NULL;
   }
   UI_PoolNode* node=(UI_PoolNode*)pool->free_list;
    if(node==NULL){
        return NULL;
    }
    pool->free_list=node->next; // this points to the node->next
    
    size_t offset=(unsigned char*)node -pool->buffer;

    size_t index=offset/pool->block_size;

    pool->state[index]=UI_POOL_USED;
    pool->free_count--;
    return node;

}

void ui_pool_free(UI_Pool* pool,void* ptr){
    if(pool == NULL){
        return;
    }
   if(ptr== NULL){
    return;
   }
   if(ui_pool_contains(pool,ptr)==0){
     return;
   }
   unsigned char* address=(unsigned char*)ptr;

   size_t offset=(size_t)(address-pool->buffer);
   size_t index=offset/pool->block_size;

   //Detect Double freeing

   if(pool->state[index] != UI_POOL_USED) {
      return;
   }
   UI_PoolNode* node=(UI_PoolNode*)ptr;

   node->next=pool->free_list;

   pool->free_list=node;

   pool->state[index]=UI_POOL_FREED;
   pool->free_count++;
   return;
}

void ui_pool_reset(UI_Pool* pool){
    if(pool==NULL){
        return;
    }
    pool->free_list=NULL;
    for(int i=0;i<pool->block_count;i++){
        UI_PoolNode* node=(UI_PoolNode*)(pool->buffer+(i*pool->block_size));

        node->next=pool->free_list;
        pool->free_list=node;
    }
    pool->free_count=pool->block_count;
    return;
}
void ui_pool_destroy(UI_Pool* pool){
    if(pool==NULL){
        return;
    }
    if(pool->buffer!=NULL){
        ui_free(pool->back_allocator,pool->buffer);

    }
    pool->buffer=NULL;
    pool->block_size=0;
    pool->block_count=0;
    pool->free_count=0;

    pool->back_allocator=NULL;
    pool->free_list=NULL;
    pool->state=NULL;
}