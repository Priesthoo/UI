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
  widget->measured_size.width=0.0f;
  widget->measured_size.height=0.0f;
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
   
    parent->object.children.objects[parent->object.children.count++]=&child->object;
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
    size_t visible_count=0;
    //First pass:
    //Determine fixed sizes and count fill children
    for(int i=0;i<child_count;i++){
         UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);
       if(!child) continue;
       if(!child->state.visible) continue;
       visible_count++;
       UI_Size child_size=ui_widget_get_measure_size(child);
       //Cross Axis
       if(child->width_mode==UI_SIZE_FILL){
        child->rect.width=available_width;
       }
       else{
        child->rect.width=child_size.width;
       }
       //Main Axis
       if(child->height_mode==UI_SIZE_FILL){
        fill_count++;
       }
       else{
        child->rect.height=child_size.height;
        ui_widget_apply_size_limits(child);
        fixed_height+=child->rect.height;
       }
       child->rect.width=ui_widget_clamp_size(child->rect.width,child->size_limits.min_width,child->size_limits.max_width);
    }

    
   
    //Total space occupied by spacing
    float total_spacing=0.0f;

   if(visible_count>1){
    total_spacing=spacing*(float)(visible_count-1);
   }
    float remaining=available_height-fixed_height-total_spacing;
    
    //Calculate space available for FILL children 
    float remaining= available_height-fixed_height-total_spacing;
    if(remaining<0.0){
        remaining=0.0f;
    }
    
     ui_widget_distribute_fill(widget,true,remaining);

    //Position children;
    //assign FILL and positions
    float y=widget->padding.top;

   size_t positioned=0;
    

    for(int i=0;i<widget->object.children.count;i++){
       UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);

       if(!child) continue;
       
       if(!child->state.visible){
        continue;
       }
      
       //width of the child uses the cross axis
         
       
       //height uses the main axis
      

        ui_widget_apply_size_limits(child);
       //For alignment
        float offset_x=ui_align_position(widget->horizontal_align,available_width,child->rect.width);

      child->rect.x=widget->padding.left+offset_x;
       
       child->rect.y=y;
       y+=child->rect.height; //compound all heights;

       positioned++;
       if(positioned<visible_count) y+=spacing;
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
    size_t visible_count=0;
    size_t fill_count=0;
    float fixed_width=0.0f;
    /*
     First Pass
    */
    for(int i=0;i<child_count;i++){
         UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);
       if(!child) continue;
       if(!child->state.visible) continue;
       visible_count++;
       UI_Size child_size=ui_widget_get_measure_size(child);
       /*
       Cross Axis 
       */
      if(child->height_mode==UI_SIZE_FILL){
         child->rect.height=available_height;
      }
      else{
        child->rect.height=child_size.height;
      }
      /*
      Main Axis
      */
       if(child->width_mode==UI_SIZE_FILL){
        fill_count++;
       }
       else{
        child->rect.width=child_size.width;
        ui_widget_apply_size_limits(child);
        fixed_width+=child->rect.width;
       }
      child->rect.height=ui_widget_clamp_size(child->rect.height,child->size_limits.min_height,child->size_limits.max_height);
    }
     float total_spacing=0.0f;

    if(visible_count>1){
        total_spacing=spacing* (float)(visible_count-1);
    }
     float remaining= available_width-fixed_width-total_spacing;
    if(remaining<0.0){
        remaining=0.0f;
    }
     ui_widget_distribute_fill(widget,false,remaining);
    
    
    float x=widget->padding.left;
     size_t positioned=0;
   


    for(int i=0;i<widget->object.children.count;i++){
       UI_Object* child_object=widget->object.children.objects[i];

       UI_Widget* child=(UI_Widget*)(child_object);

       if(!child) continue;
       if(!child->state.visible) continue;
       
       ui_widget_apply_size_limits(child);

       float offset_y=ui_align_position(widget->vertical_align,available_height,child->rect.height);
       child->rect.y=widget->padding.top+offset_y;
       child->rect.x=x;
       
       x+=child->rect.width; //compound all heights;
    
       positioned++;
       if(positioned< visible_count){
        x+=spacing;
       }
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
    ui_widget_mark_measure_dirty(widget);
    return;
}
void ui_widget_set_padding(UI_Widget* widget,float top,float bottom,float left,float right){
    if(!widget) return;

    widget->padding.top=top;
    widget->padding.right=right;
    widget->padding.bottom=bottom;
    widget->padding.left=left;

    ui_widget_mark_measure_dirty(widget);
    return;
}

void ui_widget_set_alignment(UI_Widget* widget,UI_Align horizontal,UI_Align vertical){
    if(!widget) return;
    widget->horizontal_align=horizontal;
    widget->vertical_align=vertical;
    ui_widget_mark_measure_dirty(widget);
    return;
}

void ui_widget_set_size_mode(UI_Widget* widget,UI_SizeMode width_mode,UI_SizeMode height_mode){
    if(!widget) return;
    widget->width_mode=width_mode;
    widget->height_mode=height_mode;
    ui_widget_mark_measure_dirty(widget);
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

    ui_widget_mark_measure_dirty(widget);
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
    ui_widget_mark_measure_dirty(widget);
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
    
    UI_SizeMode mode=vertical? child->height_mode:child->width_mode;

    if(mode==UI_SIZE_FILL){
        active[i]=true;
        fill_count++;
    }
    
   }
   if(fill_count==0){
    free(sizes);
    free(active);
    return true;
   }
    /*
    Available space is distributed between the FILL children
    
    */
   float remaining=available;
   if(remaining<0.0f){
    remaining=0.0;
   }
   size_t active_count=fill_count;

   while(active_count>0){
    float share=remaining/(float) active_count;
    bool constrained=false;

    for(int i=0;i<count;i++){
        if(!active[i]) continue;
        UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];

        if(!child){
            active[i]=false;
            active_count--;
            continue;
        }
        float minimum;
        float maximum;
        if(vertical){
            minimum=child->size_limits.min_height;
            maximum=child->size_limits.max_height;
        }
        else{
            minimum=child->size_limits.min_width;
            maximum=child->size_limits.max_width;
        }
       /*
        Minimum Constraint
       */
      if(share<minimum){
        sizes[i]=minimum;
        remaining-=minimum;

        if(remaining<0.0){
            remaining=0.0;
        }
        active[i]=false;
        active_count--;
        constrained=true;
        break;
      }
      /*
      Maximum constraint
      max==0 means unlimited
      */
     if(maximum>0.0 && share<maximum){
        sizes[i]=maximum;
        remaining-=maximum;

        if(remaining<0.0f){
            remaining=0.0f;
        }
        active[i]=false;
        active_count--;
        constrained=true;
        break;
     }

    }
    /*
    Nobody hit a constraint
    Give every remaining FILL child an equal share
    */
   if(!constrained){
    for(size_t i=0;i<count;i++){
        if(!active[i]) continue;
        sizes[i]=share;
        active[i]=false;
    }
    active_count=0;
   }
   }
   /*
   Apply the calculated sizes to the children

   */
  for(size_t i=0;i<count;i++){
    if(sizes[i]<0.0f) sizes[i]=0.0f;

    UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];
    if(!child) continue;
    if(!child->state.visible) continue;

    UI_SizeMode mode=vertical?child->height_mode:child->width_mode;

    if(mode!=UI_SIZE_FILL){
        continue;
    }
    if(vertical) child->rect.height=sizes[i];
    else 
       child->rect.width=sizes[i];
  }
  free(sizes);
  free(active);
  sizes=NULL;
  active=NULL;
    return true;
}

void ui_widget_layout(UI_Widget* widget){
    if(!widget) return;
    if(widget->layout){
        widget->layout(widget);
        widget->layout_dirty=false;
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
    widget->layout_dirty=false;
    return;
}

static void ui_widget_measure_self(UI_Widget* widget){
    if(!widget) return;

    if(!widget->measure) return;

    UI_Size size=widget->measure(widget,widget->constraint);

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
    UI_Size size={
        0.0,0.0
    };
    if(widget->measure){
       size=widget->measure(widget,widget->constraint); 
       
    }
    /*
    Otherwise,Containers can measure themselves from their children
    */
    if(widget->object.children.count==0){
        return;
    }
    if(widget->layout_type!=UI_LAYOUT_NONE){
         size=ui_widget_measure_children(widget);
        
    }
    size=ui_widget_clamp_measure_size(size,widget->constraint);
    widget->measured_size=size;
    widget->measure_dirty=false;
    return;
}


void ui_widget_measure_tree(UI_Widget* widget,UI_SizeConstraints constraints){
       if(!widget) return;
    
    /*
    First pass:
    propagate constraints from parent to child
    */
   ui_widget_propagate_measure_constraints(widget,constraints);
   /*
   Build a preorder list
   *We will reverse it so children are measured before their  parents
   
   
   */
  size_t capacity=16;
  size_t count=0;

  UI_Widget** stack=malloc(capacity * sizeof(UI_Widget*));
  UI_Widget** order=malloc(capacity * sizeof(UI_Widget*));

  if(!stack || !order){
    free(stack);
    free(order);
    stack=NULL;
    order=NULL;
    return;
  }
  stack[count++]=widget;
  size_t order_count=0;
  while(count>0){
    UI_Widget* curr_widget=stack[--count];
    if(!curr_widget) continue;
    if(order_count>= capacity){
        size_t new_capacity=capacity*2;
        UI_Widget** new_order=realloc(order,new_capacity*sizeof(UI_Widget*));
        if(!new_order){
            free(stack);
            free(order);
            free(new_order);
            stack=NULL;
            order=NULL;
            new_order=NULL;
        }
        order=new_order;
        capacity=new_capacity;
    }
    order[order_count++]=curr_widget;
    for(size_t i=0;i<curr_widget->object.children.count;i++){
        UI_Widget* child=(UI_Widget*)curr_widget->object.children.objects[i];
        if(!child) continue;
        if(!child->state.visible) continue;

        if(count>=capacity){
            size_t new_capacity=capacity*2;
            UI_Widget** new_stack=realloc(stack,new_capacity*sizeof(UI_Widget*));
            if(!new_stack){
                free(stack);
                free(order);
                free(new_stack);
                stack=NULL;
                order=NULL;
                new_stack=NULL;
            }
            stack=new_stack;
            capacity=new_capacity;
        }
        stack[count++]=child;
    }
  }
  /*
  Reversal traversal 
  child to parent;
  */
 for(size_t i=order_count;i>0;i--){
    UI_Widget* curr_widget=order[i-1];
    if(!curr_widget) continue;
     if(!curr_widget->measure_dirty) continue;
     ui_widget_measure(curr_widget);
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
                size.width+=widget->layout_spacing*(float)(visible_Count-1);
            
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
    
    if(widget->width_mode==UI_SIZE_FIXED){
        size.width=widget->preferred_size.width;
    }
    else if(widget->width_mode==UI_SIZE_AUTO){
        size.width=widget->measured_size.width;
    }
    else if(widget->width_mode==UI_SIZE_FILL){
        size.width=0.0f;
    }
    if(widget->height_mode==UI_SIZE_FIXED){
        size.height=widget->preferred_size.height;
    }
    else if(widget->height_mode==UI_SIZE_AUTO){
       size.height=widget->measured_size.height;
    }
    else if(widget->height_mode==UI_SIZE_FILL){
        size.height=0.0f;
    }
    return size;
}

static UI_Size my_measure(UI_Widget* widget,UI_SizeConstraints constraints){
    (void) widget;

    UI_Size size={
        0.0,0.0f
    };
    if(constraints.max_width>0.0f && size.width>constraints.max_width){
        size.width=constraints.max_width;
    }
     if(constraints.max_height>0.0f && size.width>constraints.max_height){
        size.height=constraints.max_height;
    }
    return size;
}

void ui_widget_set_constraints(UI_Widget* widget,const float min_width,const float max_width,const float min_height,const float max_height){
    if(!widget) return;

    widget->constraint.min_width=min_width;
    widget->constraint.min_height=min_height;
    widget->constraint.max_height=max_height;
    widget->constraint.max_width=max_width;

    return;
}

static float ui_widget_clamp_measure_dimension(float size,float min,float max){
    if(size< min){
        size=min;
    }
    if(max> 0.0 && size>max){
        size=max;
    }
    return size;
}
static UI_Size ui_widget_clamp_measure_size(UI_Size size,UI_SizeConstraints constraint){
    size.width=ui_widget_clamp_measure_dimension(size.width,constraint.min_width,constraint.max_width);
    size.height=ui_widget_clamp_measure_dimension(size.height,constraint.min_height,constraint.max_height);

    return size;
}
void ui_widget_mark_measure_dirty(UI_Widget* widget){
    if(!widget) return;

    UI_Object* current=&widget->object;

    while(current!=NULL){
        UI_Widget* current_widget=(UI_Widget*) current;
        current_widget->measure_dirty=true;
        current_widget->layout_dirty=true;
        current=current->parent;
    }
    return;
}
void ui_widget_mark_layout_dirty(UI_Widget* widget){
    if(!widget) return;

    UI_Object* current=&widget->object;

    while(current!=NULL){
        UI_Widget* current_widget=(UI_Widget*)current;

        current_widget->layout_dirty=true;
        current=current->parent;
    }
    return;
}

static UI_SizeConstraints ui_widget_make_child_constraints(UI_Widget* parent,UI_Widget* child){
    UI_SizeConstraints constraints={
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };
    if(!parent || !child) return constraints;

    float available_width=parent->constraint.max_width;
    float available_height=parent->constraint.max_height;

    if(available_width>0.0f){
        available_width-=parent->padding.left+parent->padding.right;
        if(available_width<0.0f) available_width=0.0f;
    }
    if(available_height>0.0f){
        available_height-=parent->padding.top+parent->padding.bottom;
        if(available_height) available_height=0.0f;
    }
    if(parent->layout_type==UI_LAYOUT_VERTICAL){
        constraints.max_width=available_width;
        constraints.max_height=0.0f;
    }
    else if(parent->layout_type==UI_LAYOUT_HORIZONTAL){
        constraints.max_height=available_height;
        constraints.max_width=0.0f;
    }
    else{
        constraints.max_width=available_width;
        constraints.max_height=available_height;
    }
    constraints.min_width=child->size_limits.min_width;
    constraints.min_height=child->size_limits.min_height;

    if(child->size_limits.max_width>0.0){
        if(constraints.max_width==0.0f || child->size_limits.max_width<constraints.max_width){
            constraints.max_width=child->size_limits.max_width;
        }
    }
    if(child->size_limits.max_height>0.0){
        if(constraints.max_height==0.0f || child->size_limits.max_height<constraints.max_height){
            constraints.max_height=child->size_limits.max_height;
        }
    }
    if(child->width_mode== UI_SIZE_FIXED){
        constraints.min_width=child->preferred_size.width;
        constraints.max_width=child->preferred_size.width;
    }
    if(child->height_mode==UI_SIZE_FIXED){
        constraints.min_height=child->preferred_size.height;
        constraints.max_height=child->preferred_size.height;
    }
    if(constraints.max_width>0.0f && constraints.max_width< constraints.min_width){
        constraints.max_width=constraints.min_width;
    }
    if(constraints.max_height>0.0f && constraints.max_height<constraints.min_height){
        constraints.max_height=constraints.min_height;
    }
    return constraints;

}
static void ui_widget_propagate_measure_constraints(UI_Widget* root,UI_SizeConstraints constraints){
    if(!root) return;

    size_t capacity=16;
    size_t count=0;

    UI_Widget** stack=malloc(capacity * sizeof(UI_Widget*));

    if(!stack){
        free(stack);
        return;
    }
    root->constraint=constraints;

    stack[count++] =root;

    while(count > 0){
        UI_Widget* widget=stack[--count];
       if(!widget) continue;
       if(!widget->state.visible) continue;
       if(widget->object.children.count==0) continue;

       for(size_t i=0;i<widget->object.children.count;i++){
         UI_Widget* child=(UI_Widget*)widget->object.children.objects[i];
         if(child) continue;
         child->constraint=ui_widget_make_child_constraints(widget,child);
        if(count>=capacity){
            size_t new_capacity=capacity*2;
            UI_Widget** new_stack=realloc(stack,new_capacity*sizeof(UI_Widget*));

            if(new_stack==NULL){
                free(stack);
                return;
            }
            stack=new_stack;
            capacity=new_capacity;
        }
        stack[count++]=child;
       }
    }
    free(stack);
    return;
}
static void ui_widget_resolve_root_size(UI_Widget* root){
    if(!root) return;

    if(root->width_mode==UI_SIZE_FIXED){
        root->rect.width=root->preferred_size.width;
    }
    else if(root->width_mode==UI_SIZE_AUTO){
        root->rect.width=root->measured_size.width;
    }
    if(root->height_mode==UI_SIZE_FIXED){
        root->rect.height=root->preferred_size.height;
    }
    else if(root->height_mode==UI_SIZE_AUTO){
        root->rect.height=root->measured_size.height;
    }
    ui_widget_apply_size_limits(root);
    return;
}

UI_Widget* ui_widget_find_child_by_index(UI_Widget* parent,size_t index){
    if(!parent) return NULL;
    if(index>=0 && index<parent->object.children.count){
        return (UI_Widget*)parent->object.children.objects[index];
    }
   return NULL;
}

int ui_widget_get_index_by_child(UI_Widget* parent,UI_Widget* child){
    if(!parent) return -1;
    if(!child) return -1;

    if(child->object.parent!=&parent->object){
        return -1;
    }
    bool isFound=false;
    int j=-1;
    for(int i=0;i<parent->object.children.count;i++){
        if((UI_Widget*)parent->object.children.objects[i]==child){
            isFound=true;
            j=i;
            break;
        }
    }
    if(!isFound){
        return j;  //returning j at this point because it is -1;
    }
    return j;
}