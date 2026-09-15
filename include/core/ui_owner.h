//For object ownership

#ifndef UI_OWNER_H
#define UI_OWNER_H
//We are trying to create a concept in which a parent own it's children,but removing a child does not destroy it
#include<stddef.h>
//Destroying the parent will also destroy the children
struct UI_Allocator;
struct UI_Object;

struct UI_Owner {
struct UI_Object **objects;
size_t count;
size_t capacity;
struct UI_Allocator* allocator;
};

//initialize an owner
int ui_owner_init(struct UI_Owner* owner,struct UI_Allocator* allocator);



//Add an object to the owner
int ui_owner_add(struct UI_Owner* owner,struct UI_Object* child);

//Remove an object from the owner
//this does not destroy the object
int ui_owner_remove(struct UI_Owner* owner,struct UI_Object* object);

//Destroy the owner
//All objects still owned by it are destroyed
void ui_owner_destroy(struct UI_Owner* owner);
#endif