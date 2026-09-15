#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include<core/ui_owner.h>
typedef  struct UI_Object UI_Object;
 struct UI_Allocator;
typedef enum{
    UI_OBJECT_UNKNOWN,
    UI_OBJECT_WIDGET,
    UI_OBJECT_BUTTON,
    UI_OBJECT_LABEL,
    UI_OBJECT_PANEL,
    UI_OBJECT_IMAGE
} UI_ObjectType;


typedef void(*UI_ObjectDestroyFun)(struct UI_Object* object);

 struct UI_Object {
 struct UI_Allocator* allocator;
 struct UI_Object* parent;
 struct UI_Owner children;
  UI_ObjectType type;
  UI_ObjectDestroyFun destroyfun;
} ;

//initialize a ui object
int ui_object_init(struct UI_Object* object,struct UI_Allocator* allocator,UI_ObjectType type,UI_ObjectDestroyFun destroy);

//Destroy a ui object
void ui_object_destroy(struct UI_Object* object);

//returns the object type
UI_ObjectType ui_object_get_type(const struct  UI_Object* object);

//returns if the object's type is of type
int ui_object_is_type(const struct UI_Object* object,UI_ObjectType type);

int ui_object_add_child( struct UI_Object* parent,struct UI_Object* child);

int ui_object_remove_child(struct UI_Object* parent,struct UI_Object* child);
#endif