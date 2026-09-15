#include<core/ui_object.h>
#include<memory/ui_memory.h>
#include<core/ui_owner.h>
#include<stdlib.h>
static int ui_owner_grow(struct UI_Owner* owner){
    size_t newcapacity=0;
    struct UI_Object **new_objects;

    if(!owner){
        return 0;
    }
    if(owner->capacity == 0){
        newcapacity= 8;
    }
    else{
        if(owner->capacity > SIZE_MAX/2){
            return 0;
        }
        newcapacity=owner->capacity*2;
    }
    if(newcapacity > SIZE_MAX/sizeof( struct UI_Object*)){
        return 0;
    }
   new_objects=realloc(owner->objects,newcapacity*sizeof(struct UI_Object*));
    if(!new_objects){
      return 0;
    }
    owner->objects=new_objects;
    owner->capacity=newcapacity;
    return 1;
}

int ui_owner_init(struct UI_Owner* owner, struct UI_Allocator* allocator){
    if(!owner){
        return 0;
    }
    if(!allocator){
        return 0;
    }
    owner->objects=NULL;
    owner->count=0;
    owner->capacity=0;
    owner->allocator=allocator;
    return 1;

}

int ui_owner_add(struct UI_Owner* owner,struct UI_Object* child){
    if(!owner){
        return 0;
    }
    if(!child){
        return 0;
    }
    if(owner->count >=owner->capacity){
        if(!ui_owner_grow(owner)){
            return 0;
        }
    }
    owner->objects[owner->count]=child;
    owner->count++;
    return 1;
}

int ui_owner_remove(struct UI_Owner* owner,struct  UI_Object* object){

 /* size_t i;

    if(owner == NULL){
        return 0;
    }
    if(object == NULL){
        return 0;
    }

    for( i=0;i<owner->count;i++){
        if(owner->objects[i]==object){
            /*
            Shift everything after the child
            one position to the left
            
           for(size_t j=i;j+1<owner->count;j++){
             owner->objects[j]=owner->objects[j+1];
           }
           owner->count--;
           //The child no longer belongs to this parent
          object->parent=NULL;
          return 1;
        }
*/
        
    
  return 0;
}

void ui_owner_destroy(struct UI_Owner* owner){
    
      size_t i;
    if(!owner){
        return;
    }
    //Free every allocation still owned by the scope
    for( i=0; i< owner->count;i++){
        ui_free(owner->allocator,owner->objects[i]);
    }
     //Free the allocations tracking array itself

     free(owner->objects);
      owner->objects=NULL;
      owner->count=0;
      
      owner->capacity=0;
      owner->allocator=NULL;
    
    return;
    
    
}