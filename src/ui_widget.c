#include<widget/ui_widget.h>

#include<stdlib.h>
static void ui_widget_object_destroy(struct  UI_Object* object){
  if(object==NULL){
    return;
  }
  struct UI_Widget* widget=(struct UI_Widget*)object; //we were able to cast to an object UI_Object because we use it as the first member of the struct
  ui_owner_destroy(&widget->object.children);
  free(widget);
  return;
}

 struct UI_Widget* ui_widget_create(void){
  struct UI_Widget* widget=malloc(sizeof(struct UI_Widget));
  if(!widget){
    return NULL;
  }
  widget->object.parent=NULL;

  widget->object.children.objects=NULL;
  widget->object.children.count=0;
  widget->object.children.capacity=0;
  widget->object.destroyfun=ui_widget_object_destroy;
  widget->rect.x=0.0f;
  widget->rect.y=0.0;
  widget->rect.width=0.0;
  widget->rect.height=0.0;

  widget->state.visible=true; //it is visible
  widget->state.enabled=true;
  widget->state.focused=false;
  widget->state.hovered=false;
  widget->state.pressed=false;
  widget->event=NULL;
  widget->draw=NULL;
  widget->layout=NULL;
  widget->layout_type=UI_LAYOUT_NONE;
  widget->layout_spacing=0.0f;
  widget->padding.left=0.0f;
  widget->padding.right=0.0f;
  widget->padding.top=0.0f;
  widget->padding.bottom=0.0f;
  widget->horizontal_align=UI_ALIGN_START;
  widget->vertical_align=UI_ALIGN_START;
  widget->height_mode=UI_SIZE_FIXED;
  widget->width_mode=UI_SIZE_FIXED;
  widget->size_limits.min_height=0.0;
  widget->size_limits.min_width=0.0f;
  widget->size_limits.max_height=0.0f;
  widget->size_limits.max_width=0.0f;
  widget->preferred_size.width=0.0;
  widget->preferred_size.height=0.0;
  return widget;
}


void ui_widget_destroy(struct UI_Widget*widget){
    if(!widget){
        return;
    }
    ui_object_destroy(&widget->object);
    return;
}
void ui_widget_set_position(struct  UI_Widget* widget,float x,float y){
    if(!widget){
        return;
    }
    widget->rect.x=x;
    widget->rect.y=y;
    return;
}
void ui_widget_set_size(struct UI_Widget* widget,float width,float height){
   if(!widget){
    return;
   }
   widget->rect.width=width;
   widget->rect.height=height;
   return;
}
void ui_widget_set_enabled(struct UI_Widget* widget,bool state){
    if(!widget){
        return;
    }
    widget->state.enabled=state;
    return;
}
void ui_widgetset_focused(struct UI_Widget* widget,bool state){
    if(!widget){
        return;
    }
    widget->state.focused=state;
    return;
}

int ui_widget_add_child(struct UI_Widget* parent,struct UI_Widget* child){
    if(!parent){
        return 0;
    }
    if(!child){
        return 0;
    }
    //child widget cannot have two parents 
    if(child->object.parent){
        return 0;
    }
   
   //Prevent adding a widget underneath one of it's own descendant
    struct UI_Object* current=&parent->object;

    while(current){
        if(current==&child->object){
              return 0;
        }
        current=current->parent;
    }

    //Grow the children array if necessary
    //if the current children array for parent has count greater than it's capacity,we must expand the children array
    if(parent->object.children.count>=parent->object.children.capacity){
        size_t new_capacity=parent->object.children.capacity==0?4:parent->object.children.capacity*2; 
      struct  UI_Object** new_items=realloc(parent->object.children.objects,new_capacity*sizeof(struct UI_Object*));

        if(!new_items){
            return 0;
        }
        parent->object.children.objects=new_items;
        parent->object.children.capacity=new_capacity;
    }
    //Add the child
    if(parent->object.children.count==0){
         parent->object.children.objects[parent->object.children.count]=&child->object;
    }
    else{
    parent->object.children.objects[parent->object.children.count]=&child->object;
    }
    parent->object.children.count++;
    child->object.parent=&parent->object;
    return 1; //successfull adding
}

int ui_widget_remove_child(struct UI_Widget* parent,struct UI_Widget* child){
    if(!parent){
        return 0;
    }
    if(!child){
        return 0;
    }
    if(child->object.parent!=&parent->object){
        return 0;
    }
    size_t i;
   
   

    for( i=0;i<parent->object.children.count;i++){
        if(parent->object.children.objects[i]==&child->object){
            /*
            Shift everything after the child
            one position to the left
            */
           for(size_t j=i;j+1<parent->object.children.count;j++){
             parent->object.children.objects[j]=parent->object.children.objects[j+1];
           }
           --parent->object.children.count;
           //The child no longer belongs to this parent
            child->object.parent=NULL;
          return 1;
        }
    }
    return 0;
}
UI_Point ui_widget_get_position(const struct  UI_Widget* widget){
    UI_Point position={0.0f,0.0f};

    if(!widget){
        return position;
    }
    position.x=widget->rect.x;
    position.y=widget->rect.y;

    return position;
}

UI_Rect ui_widget_get_rect(const struct  UI_Widget* widget){
    UI_Rect rect={0.0f,0.0f,0.0f,0.0f};
    if(!widget){
        return rect;
    }
    return widget->rect;
}

UI_Point ui_widget_get_global_position(const struct  UI_Widget* widget){
    UI_Point position={0.0f,0.0f};

    if(!widget){
        return position;
    }
    const struct UI_Object* current=&widget->object;
    while(current!=NULL){
        const struct UI_Widget* curr_widget=(const struct  UI_Widget*)current;
        position.x+=curr_widget->rect.x;
        position.y+=curr_widget->rect.y;
        current=current->parent;
    }
    return position;
}

int ui_widget_contains_point(const struct  UI_Widget* widget,float x,float y){
    if(!widget){
        return 0;
    }
    if(!widget->state.visible){
        return 0;
    }
    UI_Point point= ui_widget_get_global_position(widget);

    return (x>=point.x && x<point.x+widget->rect.width) && (y>=point.y && y<point.y+widget->rect.height);
}

void ui_widget_set_event_handler(struct UI_Widget* widget,UI_WidgetEventFn event){
    if(!widget){
        return;
    }
    widget->event=event;
    return;
}
void ui_widget_handle_event(struct UI_Widget* widget,const UI_Event* event){
    if(!widget || !event){
        return;
    }
    if(!widget->state.visible){
        return;
    }
    if(!widget->state.enabled){
        return;
    }
    if(!widget->event){
        widget->event(widget,event);
    }
    return;
}


void ui_widget_pointer_press(struct UI_Widget* widget,float x,float y,int button){
    if(!widget){
        return;
    }
    if(!ui_widget_contains_point(widget,x,y)){
        return;
    }
    widget->state.pressed=true;
    UI_Event event={
      .type=UI_EVENT_POINTER_PRESS,
      .x=x,
      .y=y,
      .button=button
    };
    ui_widget_handle_event(widget,&event);
    return;
}

void ui_widget_pointer_release(struct UI_Widget* widget,float x,float y,int button){
    if(!widget){
        return;
    }
    if(!widget->state.pressed) return;

    widget->state.pressed=false;

     UI_Event event={
      .type=UI_EVENT_POINTER_RELEASE,
      .x=x,
      .y=y,
      .button=button
    };
    ui_widget_handle_event(widget,&event);
    return;
}
void ui_widget_pointer_move(struct UI_Widget*widget,float x,float y){
    if(!widget) return;

    bool inside=ui_widget_contains_point(widget,x,y);

    if(inside && !widget->state.hovered){
        widget->state.hovered=true;

        UI_Event event={
            .type=UI_EVENT_POINTER_ENTER,
            .x=x,
            .y=y,
            .button=0
        };
          ui_widget_handle_event(widget,&event);
    }
    else if(!inside && widget->state.hovered){
        widget->state.hovered=false;
         UI_Event event={
            .type=UI_EVENT_POINTER_LEAVE,
            .x=x,
            .y=y,
            .button=0
        };
         ui_widget_handle_event(widget,&event);
    }
    return;
}


bool ui_widget_has_focus(const struct UI_Widget* widget){
    if(!widget){
        return false;
    }
    return widget->state.focused;
}

void ui_widget_set_hovered(struct UI_Widget* widget,bool value){
    if(!widget){
        return;
    }
    widget->state.hovered=value;
    return;
}
bool ui_widget_is_hovered(const struct UI_Widget* widget){
    if(!widget) return false;

    return widget->state.hovered;
}

void ui_widget_set_pressed(struct UI_Widget* widget,bool value){
    if(!widget){
        return;
    }
    widget->state.pressed=value;
    return;
}
bool ui_widget_is_pressed(const struct UI_Widget* widget){
    if(!widget) return false;

    return widget->state.pressed;
}

void ui_widget_set_visible(struct UI_Widget* widget,bool value){
    if(!widget){
        return;
    }
    widget->state.visible=value;
    return;
}
bool ui_widget_is_visible(const struct UI_Widget* widget){
    if(!widget) return false;

    return widget->state.visible;
}
bool ui_widget_is_enabled(const struct UI_Widget* widget){
    if(!widget) return false;

    return widget->state.enabled;
}
void ui_widget_set_draw_handler(struct UI_Widget* widget,UI_WidgetDrawFn draw){
    if(!widget) return;

    widget->draw=draw;
    return;
}
void ui_widget_draw(struct UI_Widget* widget){
    if(!widget) return;

    if(!widget->state.visible) return;

    if(widget->draw){
        widget->draw(widget);
    }
    return;
}

void ui_widget_draw_tree(struct UI_Widget* widget){
    if(!widget) return;
 struct  UI_Widget **stack=NULL;
     
    size_t count=0;
    size_t capacity=0;

    capacity=8;
    stack=malloc(capacity* sizeof(struct UI_Widget*));

    if(stack==NULL) return;

    stack[count++] = widget; //push

    while(count> 0){
        //pop
      struct UI_Widget* curr_widget=stack[--count];

       if(!curr_widget) continue;
       if(!curr_widget->state.visible) continue;

       if(curr_widget->draw){
        curr_widget->draw(curr_widget);
       }
        //Push children in reverse order,this keeps normal child drawing order
         if(curr_widget->object.children.count==0) continue;

        for(int i=curr_widget->object.children.count;i>0;i--){
        struct  UI_Object* child=curr_widget->object.children.objects[i-1];

         struct UI_Widget* childwidget=(struct UI_Widget*)child;

            //Grow the stack if necessary
            if(count>=capacity){
                size_t new_capacity=capacity*2;
              struct  UI_Widget** newstack=realloc(stack,new_capacity*sizeof(struct UI_Widget*));
                if(!newstack){
                    free(stack);
                    stack=NULL;
                    return;
                }
                stack=newstack;
                capacity=new_capacity;
            }
            stack[count++]=childwidget;
        }
       
    }

   free(stack);
   stack=NULL;
   return;
}

void ui_widget_set_layout_handler(struct UI_Widget* widget,UI_WidgetLayoutFn layout){
    if(!widget) return;

    widget->layout=layout;
    return;
}

void ui_widget_layout_tree(struct UI_Widget* widget){
      if(!widget) return;
    struct UI_Widget **stack=NULL;
     
    size_t count=0;
    size_t capacity=0;

    capacity=8;
    stack=malloc(capacity* sizeof(struct UI_Widget*));

    if(stack==NULL) return;

    stack[count++] = widget; //push

    while(count> 0){
        //pop
      struct UI_Widget* curr_widget=stack[--count];

       if(!curr_widget) continue;
       if(!curr_widget->state.visible) continue;
       ui_widget_layout(widget);
        //Push children in reverse order,this keeps normal child drawing order
         if(curr_widget->object.children.count==0) continue;

        for(int i=curr_widget->object.children.count;i>0;i--){
            struct UI_Object* child=curr_widget->object.children.objects[i-1];

           struct UI_Widget* childwidget=(struct UI_Widget*)child;

            //Grow the stack if necessary
            if(count>=capacity){
                size_t new_capacity=capacity*2;
             struct   UI_Widget** newstack=realloc(stack,new_capacity*sizeof(struct UI_Widget*));
                if(!newstack){
                    free(stack);
                    stack=NULL;
                    return;
                }
                stack=newstack;
                capacity=new_capacity;
            }
            stack[count++]=childwidget;
        }
       
    }

   free(stack);
   stack=NULL;
   
 return;
}
void ui_widget_layout_vertical(UI_Widget* widget,float spacing){
    if(!widget) return;
     
    size_t child_count=widget->object.children.count;
    if(child_count==0){
        return;
    }
   float available_width=widget->rect.width-widget->padding.left-widget->padding.right;


   float available_height=widget->rect.height-widget->padding.top-widget->padding.bottom;
  if(available_width<0.0f){
        available_width=0.0f;
    }
   if(available_height<0.0f){
        available_height=0.0f;
    }
    //Count children that fill vertically
    size_t fill_count=0;
    float fixed_height=0.0f;
    for(int i=0;i<child_count;i++){
         UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);
       if(!child) continue;
       if(child->width_mode==UI_SIZE_AUTO || child->height_mode==UI_SIZE_AUTO){
           if(child->measure){
           widget->preferred_size= child->measure(child);
           }
       }
       if(child->width_mode==UI_SIZE_FIXED || child->width_mode==UI_SIZE_AUTO){
        child->rect.width=child->preferred_size.width;
       }
         if(child->height_mode==UI_SIZE_FIXED || child->height_mode==UI_SIZE_AUTO){
        child->rect.height=child->preferred_size.height;
       }
       if(child->height_mode==UI_SIZE_FILL){
        fill_count++;
       }
       else{
        fixed_height+=child->rect.height;
       }
    }
   
    //Total space occupied by spacing
    float total_spacing=0.0f;

    if(child_count>1){
        total_spacing=spacing* (float)(child_count-1);
    }
    float remaining=available_height-fixed_height-total_spacing;
    
    //Calculate space available for FILL children 
    float remaining= available_height-fixed_height-total_spacing;
    if(remaining<0.0){
        remaining=0.0f;
    }
    float fill_height=0.0f;
    if(fill_count>0){
     fill_height=remaining/(float)fill_count;
    }

    //Position children;
    float y=widget->padding.top;

 
    

    for(int i=0;i<widget->object.children.count;i++){
       UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);

       if(!child) continue;
       //width of the child uses the cross axis
       if(child->width_mode==UI_SIZE_FILL){
        child->rect.width=available_width;
       }
       //height uses the main axis
       if(child->height_mode==UI_SIZE_FILL){
          child->rect.height=fill_height;
       }
        ui_widget_apply_size_limits(child);
       //For alignment
        float offset_x=ui_align_position(widget->horizontal_align,available_width,child->rect.width);

      child->rect.x=widget->padding.left+offset_x;
       child->rect.x=0.0f;
       child->rect.y=y;
       y+=child->rect.height; //compound all heights;
       y+=spacing;
    }
    return;
}
void ui_widget_layout_horizontal(UI_Widget* widget,float spacing){
    if(!widget) return;

      size_t child_count=widget->object.children.count;
    if(child_count==0){
        return;
    }
   float available_width=widget->rect.width-widget->padding.left-widget->padding.right;


   float available_height=widget->rect.height-widget->padding.top-widget->padding.bottom;
  if(available_width<0.0f){
        available_width=0.0f;
    }
   if(available_height<0.0f){
        available_height=0.0f;
    }
    //Count children that fill horizontally
    size_t fill_count=0;
    float fixed_width=0.0f;
    for(int i=0;i<child_count;i++){
         UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);
       if(!child) continue;
       if(child->width_mode==UI_SIZE_AUTO || child->height_mode==UI_SIZE_AUTO){
           if(child->measure){
           widget->preferred_size= child->measure(child);
           }
       }
       if(child->width_mode==UI_SIZE_FIXED || child->width_mode==UI_SIZE_AUTO){
        child->rect.width=child->preferred_size.width;
       }
         if(child->height_mode==UI_SIZE_FIXED || child->height_mode==UI_SIZE_AUTO){
        child->rect.height=child->preferred_size.height;
       }
       if(child->width_mode==UI_SIZE_FILL){
        fill_count++;
       }
       else{
        fixed_width+=child->rect.width;
       }
    }
     float total_spacing=0.0f;

    if(child_count>1){
        total_spacing=spacing* (float)(child_count-1);
    }
     float remaining= available_width-fixed_width-total_spacing;
    if(remaining<0.0){
        remaining=0.0f;
    }
    float fill_width=0.0f;
    if(fill_count>0){
     fill_width=remaining/(float)fill_count;
    }
    
    
    
    float x=widget->padding.left;
    
   


    for(int i=0;i<widget->object.children.count;i++){
       UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);

       if(!child) continue;
       //width uses the main axis 
        if(child->width_mode==UI_SIZE_FILL){
        child->rect.width=fill_width;
       }
       //height uses the cross axis
       if(child->height_mode==UI_SIZE_FILL){
          child->rect.height=available_height;
       }
       
       ui_widget_apply_size_limits(child);

       float offset_y=ui_align_position(widget->vertical_align,available_height,child->rect.height);
       child->rect.y=widget->padding.top+offset_y;
       child->rect.x=x;
       child->rect.y=0.0f;
       x+=child->rect.width; //compound all heights;
       x+=spacing;
    }
    return;
}

void ui_widget_set_layout(UI_Widget* widget,UI_LayoutType type,float spacing){
    if(!widget) return;
     
      if(spacing<0.0){
        spacing=0.0f;
      }

    widget->layout_type=type;
    widget->layout_spacing=spacing;
    return;
}
void ui_widget_set_padding(UI_Widget* widget,float top,float bottom,float left,float right){
    if(!widget) return;

    widget->padding.top=top;
    widget->padding.right=right;
    widget->padding.bottom=bottom;
    widget->padding.left=left;
    return;
}

void ui_widget_set_alignment(UI_Widget* widget,UI_Align horizontal,UI_Align vertical){
    if(!widget) return;
    widget->horizontal_align=horizontal;
    widget->vertical_align=vertical;
    return;
}

void ui_widget_set_size_mode(UI_Widget* widget,UI_SizeMode width_mode,UI_SizeMode height_mode){
    if(!widget) return;
    widget->width_mode=width_mode;
    widget->height_mode=height_mode;
    return;
}

static float ui_align_position(UI_Align alignment,float available,float size){
    switch(alignment){
        case UI_ALIGN_CENTER:{
            return (available-size)*0.5f;
        }
        case UI_ALIGN_END:
           return available-size;
        case UI_ALIGN_START:
          return 0.0f;
    }
    return 0.0f;
}
static void ui_widget_apply_size(UI_Widget* parent,UI_Widget* child){
    if(!parent || !child) return;

    float available_width=parent->rect.width-parent->padding.left-parent->padding.right;
    float available_height=parent->rect.height-parent->padding.top-parent->padding.bottom;

    if(available_height<0.0f){
        available_height=0.0f;
    }
    if(available_width<0.0f){
        available_width=0.0f;
    }
    if(child->width_mode==UI_SIZE_FILL){
        child->rect.width=available_width;
    }
    if(child->height_mode==UI_SIZE_FILL){
        child->rect.height=available_height;
    }
    return;
}
void ui_widget_set_limits(UI_Widget* widget,float min_width,float min_height,float max_width,float max_height){
    if(!widget) return;
    if(min_width<0.0){
        min_width=0.0;
    }
    if(min_height<0.0){
        min_height=0.0;
    }
   if(max_width<0.0){
        max_width=0.0;
    }
    if(max_height<0.0){
        max_height=0.0;
    }
    /*
    A maximum of 0 means "no maximum"
    if a maximum exists but is smaller than 
    the minimum,make it equal to the minimum 
    
    
    */
   if(max_width>0.0f && max_width <min_width){
    max_width=min_width;
   }
   if(max_height>0.0f && max_height <min_height){
    max_height=min_height;
   }
    widget->size_limits.min_width=min_width;
    widget->size_limits.min_height=min_height;
    widget->size_limits.max_width=max_width;
    widget->size_limits.max_height=max_height;

    ui_widget_apply_size_limits(widget);
  return;
}
void ui_widget_set_preferred_size(UI_Widget* widget,float width,float height){
    if(!widget) return;
    if(width<0.0){
        width=0.0;
    }
    if(height<0.0){
        height=0.0;
    }
    widget->preferred_size.width=width;
    widget->preferred_size.height=height;

    //Fixed widgets use their preferred size,width and height
    if(widget->width_mode!=UI_SIZE_FILL){
        widget->rect.width=width;
    }
    if(widget->height_mode!=UI_SIZE_FILL){
        widget->rect.height=height;
    }
    ui_widget_apply_size_limits(widget);
    return;
}
static float ui_widget_clamp_size(float size,float min,float max){
    if(size<min){
        size=min;
    }
    if(max>0.0f && size>max){
        size=max;
    }
    return size;
}
static void ui_widget_apply_size_limits(UI_Widget* widget){
    if(!widget) return;

    widget->rect.width=ui_widget_clamp_size(widget->rect.width,widget->size_limits.min_width,widget->size_limits.max_width);
    widget->rect.height=ui_widget_clamp_size(widget->rect.height,widget->size_limits.min_height,widget->size_limits.max_height);
    return;
}
static bool ui_widget_distribute_fill(UI_Widget* widget,bool vertical,float available){
    if(!widget) return false;

    size_t count=widget->object.children.count;
    if(count==0){
        return true;
    }
   float* sizes=malloc(count* sizeof(float));
   bool* active=malloc(count* sizeof(bool));
   if(!sizes || !active){
    free(sizes);
    free(active);
    return false;
   }
   float fixed_space=0.0;
   size_t fill_count=0;

   for(size_t i=0;i<count;i++){
    UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];
    sizes[i]=0.0f;
    active[i]=false;
    if(!child) continue;
    if(!child->state.visible) continue;

    float current_size;

    if(vertical){
        current_size=child->rect.height;
    }
    else{
        current_size=child->rect.width;
    }
    if((vertical && child->height_mode==UI_SIZE_FILL) || (!vertical && child->width_mode==UI_SIZE_FILL)){
        active[i]=true;
        fill_count++;
        sizes[i]=current_size;
    }
    else{
        fixed_space+=current_size;
    }
   }
    return true;
}

void ui_widget_layout(UI_Widget* widget){
    if(!widget) return;
    if(widget->layout){
        widget->layout(widget);
        return;
    }
    switch(widget->layout_type){
        case UI_LAYOUT_VERTICAL:{
            ui_widget_layout_vertical(widget,widget->layout_spacing);
            break;
        }
        case UI_LAYOUT_HORIZONTAL:{
           ui_widget_layout_horizontal(widget,widget->layout_spacing);
            break;
        }
        case UI_LAYOUT_NONE:{
            break;
        }
        default:{
            break;
        }
    }
    return;
}

static void ui_widget_measure_self(UI_Widget* widget){
    if(!widget) return;

    if(!widget->measure) return;

    UI_Size size=widget->measure(widget);

    if(widget->width_mode==UI_SIZE_AUTO){
        widget->preferred_size.width=size.width;
    }
      if(widget->height_mode==UI_SIZE_AUTO){
        widget->preferred_size.height=size.height;
    }
   return;
}

void ui_widget_measure(UI_Widget* widget){
    if(!widget) return;

    if(widget->measure){
        UI_Size size=widget->measure(widget);
        if(widget->width_mode==UI_SIZE_AUTO){
            widget->preferred_size.width=size.width;
        }
        if(widget->height_mode==UI_SIZE_AUTO){
            widget->preferred_size.height=size.height;
        }
        return;
    }
    /*
    Otherwise,Containers can measure themselves from their children
    */
    if(widget->object.children.count==0){
        return;
    }
    if(widget->layout_type!=UI_LAYOUT_NONE){
        if(widget->width_mode!=UI_SIZE_AUTO || widget->height_mode!=UI_SIZE_AUTO){
             return;
        }
        UI_Size size=ui_widget_measure_children(widget);
        if(widget->width_mode==UI_SIZE_AUTO){
           widget->preferred_size.width=size.width;
        }
        if(widget->height_mode==UI_SIZE_AUTO){
            widget->preferred_size.height=size.height;
        }
    }
    return;
}


void ui_widget_measure_tree(UI_Widget* widget){
       if(!widget) return;
    UI_Widget** stack;
    UI_Widget** order=NULL;

    size_t stack_count=0;
    size_t order_count=0;

    size_t stack_capacity=8;
    size_t order_capacity=8;

    stack=malloc(stack_capacity*sizeof(UI_Widget*));

    order=malloc(order_capacity*sizeof(UI_Widget*));

    if(stack==NULL || order==NULL){
        free(stack);
        free(order);
        stack=NULL;
        order=NULL;
        return;
    }
    /*
    First pass
    Build the traversal order
    */
   stack[stack_count++]=widget;

   while(stack_count>0){
    UI_Widget* curr_widget=stack[--stack_count];

    if(!curr_widget) continue;
    
    if(!curr_widget->state.visible) continue;

    /*
    Store the widget
    */
   if(order_count >=order_capacity){
     size_t new_capacity=order_capacity*2;
    
    UI_Widget** new_order=realloc(order,new_capacity* sizeof(UI_Widget*));

    if(new_order==NULL){
        free(stack);
        free(order);
        stack=NULL;
        order=NULL;
        return;
    }
    order=new_order;
    order_capacity=new_capacity;
   }
   order[order_count++]=curr_widget;
   //Add Children 
   for(int i=curr_widget->object.children.count;i>0;i--){
    UI_Object *child_object=curr_widget->object.children.objects[i-1];
    UI_Widget* child=(UI_Widget*)child_object;

    if(stack_count>=stack_capacity){
        size_t new_capacity=stack_capacity*2;
        UI_Widget** new_stack=realloc(stack,new_capacity*sizeof(UI_Widget*));
     if(new_stack==NULL){
        free(stack);
        free(order);
        return;
     }
     stack=new_stack;
     stack_capacity=new_capacity;
}
   stack[stack_count++]=child;
   }
   }
   /*
   Second pass.
   Measure from the bottom of the tree upward
   */
  while(order_count>0){
    UI_Widget* widget=order[--order_count];
    ui_widget_measure(widget);
  }
  free(stack);
  free(order);
 return;
    
}



UI_Size ui_widget_measure_children(UI_Widget* widget){
    UI_Size size={0.0f,0.0f};
    
    if(!widget) return size;
   size_t visible_Count=0; //this is for the visible ones
    switch(widget->layout_type){
        case UI_LAYOUT_VERTICAL:{
            if(widget->object.children.count==0){
                break;
            }
            for(int i=0;i<widget->object.children.count;i++){
                 UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];
                 if(!child)  continue;
                 if(!child->state.visible) continue;
                 UI_Size child_size=ui_widget_get_measure_size(child);

                 visible_Count++;
                 if(child_size.width>size.width){  //Get the highest width
                    size.width=child_size.width; 
                 }
                 size.height+=child_size.height;  //this is the cumulative height of all edges
        
            }
            if(visible_Count>1){
                size.height+=widget->layout_spacing*(float)(visible_Count-1);
            
            }
            break;
        }
         case UI_LAYOUT_HORIZONTAL:{
            if(widget->object.children.count==0){
                break;
            }
            for(int i=0;i<widget->object.children.count;i++){
                 UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];
                 if(!child)  continue;
                 if(!child->state.visible) continue;
                  UI_Size child_size=ui_widget_get_measure_size(child);

                 visible_Count++;
                 if(child_size.height>size.height){  //Get the highest height
                    size.height=child_size.height; 
                 }
                 size.width+=child_size.width;  //this is the cumulative width of all child
        
            }
            if(visible_Count>1){
                size.height+=widget->layout_spacing*(float)(visible_Count-1);
            
            }
            break;
        }
        case UI_LAYOUT_NONE:{
            break;
        }
        default:{
            break;
        }

    }
    size.width+=widget->padding.left+widget->padding.right;
    size.height+=widget->padding.top+widget->padding.bottom;
    return size;
}
UI_Size ui_widget_get_measure_size(const UI_Widget* widget){
    UI_Size size={0.0f,0.0f};
    if(!widget) return size;

    if(widget->width_mode!=UI_SIZE_FILL){
        size.width=widget->preferred_size.width;
    }
    if(widget->height_mode==UI_SIZE_FILL){
        size.height=widget->preferred_size.height;
    }
    return size;
}

static float ui_widget_resolve_size(float preferred,UI_SizeMode mode,float available){
 switch(mode){
    case UI_SIZE_FILL:{
        return available;
    }
    case UI_SIZE_AUTO:{
        return preferred;
    }
    case UI_SIZE_FIXED:{
        return preferred;
    }

 }
 return 0.0;
}