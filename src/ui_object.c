#include<core/ui_object.h>
#include<core/ui_owner.h>
#include<stdlib.h>
#include<memory/ui_memory.h>

int ui_object_init( UI_Object* object,struct UI_Allocator* allocator,UI_ObjectType type,UI_ObjectDestroyFun destroy){
    if(object == NULL){
        return 0;
    }
    if(!allocator){
        return 0;
    }
    object->allocator=allocator;
    object->parent=NULL;
    object->type=type;
    object->destroyfun=destroy;
    if(!ui_owner_init(&object->children,allocator)){
        return 0;
    }
    return 1;
}

void ui_object_destroy( UI_Object* object){
    if(!object){
        return;
    }
    //Let's destroy thechildren
     UI_Object* current=object;
    while(current!=NULL){
        if(current->children.count>0){
            current=current->children.objects[current->children.count-1];
            continue;
        }
        UI_Object* parent=current->parent;
        if(parent){
     int output=  ui_object_remove_child(parent,current);
        } 
        if(current->destroyfun){
            current->destroyfun(current);
        }
        if(parent==NULL){
            break;
        }
        current=parent;
    }
    
    return;
}

UI_ObjectType ui_object_get_type(const  UI_Object* object){
    if(!object){
        return UI_OBJECT_UNKNOWN;
    }
    return object->type;
}
int ui_object_is_type(const  UI_Object* object,UI_ObjectType type){
    if(!object){
        return 0;
    }
    if(object->type==type){
        return 1;
    }
    return 0;
}
int ui_object_add_child( UI_Object* parent, UI_Object* child){
  if(!parent){
    return 0;
  }
  if(!child){
    return 0;
  }
  if(child== parent){
    return 0;
  }
  if(!ui_owner_add(&parent->children,child)){
     return 0;
  }
   child->parent=parent;
 return 1;
}

int ui_object_remove_child(UI_Object* parent, UI_Object* child){
    if(!parent){
        return 0;
    }
    if(!child){
        return 0;
    }
    if(child->parent!=parent){
        return 0;
    }
    if(!ui_owner_remove(&parent->children,child)){
        return 0;
    }
    child->parent=NULL;
    return 1;
}