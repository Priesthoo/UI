#ifndef UI_WIDGET_H
#define UI_WIDGET_H
#include<core/ui_rect.h>
#include<memory/ui_memory.h>
#include<core/ui_widgetstate.h>
#include<core/ui_object.h>
#include<core/ui_size.h>
#include<core/ui_point.h>
#include<event/event.h>
 typedef struct UI_Widget UI_Widget;

typedef void(*UI_WidgetDrawFn)( UI_Widget* widget);
typedef void(*UI_WidgetLayoutFn)(UI_Widget* widget);
typedef UI_Size (*UI_WidgetMeasureFn)(UI_Widget* widget);

typedef enum{
 UI_LAYOUT_NONE,
 UI_LAYOUT_VERTICAL,
 UI_LAYOUT_HORIZONTAL
}UI_LayoutType;

typedef enum{
UI_ALIGN_START,
UI_ALIGN_CENTER,
UI_ALIGN_END
} UI_Align;

typedef enum{
UI_SIZE_FIXED,
UI_SIZE_FILL,
UI_SIZE_AUTO
}UI_SizeMode;

struct UI_SizeLimits{
  float min_width;
  float min_height;

  float max_width;
  float max_height;
};
struct UI_Padding{
  float left;
  float top;
  float right;
  float bottom;
};
typedef struct UI_SizeLimits UI_SizeLimits;
typedef UI_SizeLimits UI_SizeConstraints;
typedef UI_Padding UI_Padding;
 struct UI_Widget{
struct UI_Object object;

UI_Rect rect; //the current 
UI_Size preferred_size;

 UI_WidgetState state; //the current widget state
 UI_WidgetEventFn event;
 UI_WidgetDrawFn draw;
 UI_WidgetLayoutFn layout;
 UI_WidgetMeasureFn measure;  //this is used to determine measured size from the content of the widget

 UI_LayoutType layout_type;
 float layout_spacing;

 UI_Padding padding;

 UI_Align horizontal_align;
 UI_Align vertical_align;

 UI_SizeMode width_mode;  
 UI_SizeMode height_mode; 
 
  UI_SizeLimits size_limits;
};
int ui_widget_children_count(const struct UI_Widget* widget);
//For widget creation
struct UI_Widget* ui_widget_create(void);
//For destroy widget objects
void ui_widget_destroy(struct UI_Widget* widget);
//For widget geometry(position and extent)
UI_Point ui_widget_get_position(const struct UI_Widget* widget);

UI_Rect ui_widget_get_rect(const struct UI_Widget* widget);

UI_Point ui_widget_get_global_position(const struct UI_Widget* widget);

int ui_widget_contains_point(const struct UI_Widget* widget,float x,float y);

void ui_widget_set_position(struct  UI_Widget* widget,float x,float y);

void ui_widget_set_size(struct UI_Widget* widget,float width,float height);

//For constructing widget hierarchies
/*
Window
  |_
     >Button
       |_
          >Text
*/

int ui_widget_add_child(struct UI_Widget* parent,struct UI_Widget* child);

int ui_widget_remove_child(struct UI_Widget* parent,struct UI_Widget* child);

//For getting keyboard focus and widget state
void ui_widget_set_enabled(struct UI_Widget* widget,bool state);
bool ui_widget_is_enabled(const struct UI_Widget* widget);



//For event handling
void ui_widget_set_event_handler(struct UI_Widget* widget,UI_WidgetEventFn event);
void ui_widget_handle_event(struct UI_Widget* widget,const UI_Event* event);
void ui_widget_pointer_move(struct UI_Widget*widget,float x,float y); //mouse move
void ui_widget_pointer_press(struct UI_Widget* widget,float x,float y,int button); //mouse press
void ui_widget_pointer_release(struct UI_Widget* widget,float x,float y,int button); //mouse release


//For focus management
bool ui_widget_set_focus(struct UI_Widget* widget);
void ui_widget_clear_focus(struct UI_Widget* widget);
bool ui_widget_has_focus(const struct UI_Widget* widget);
void ui_widget_set_focused(struct UI_Widget* widget,bool state);


//For hovering,for visibiliity
void ui_widget_set_hovered(struct UI_Widget* widget,bool value); 
bool ui_widget_is_hovered(const struct UI_Widget* widget);

void ui_widget_set_pressed(struct UI_Widget* widget,bool value); 
bool ui_widget_is_pressed(const struct  UI_Widget* widget);

void ui_widget_set_visible(struct UI_Widget* widget,bool value); 
bool ui_widget_is_visible(const struct  UI_Widget* widget);


//Drawing functionality
void ui_widget_set_draw_handler(struct UI_Widget* widget,UI_WidgetDrawFn draw);
void ui_widget_draw(struct UI_Widget* widget);
//If this widget object has children,We iterate through it's children and draw them as well
void ui_widget_draw_tree(struct UI_Widget* widget);


//Layout system
void ui_widget_set_layout_handler(struct UI_Widget* widget,UI_WidgetLayoutFn layout);
void ui_widget_layout_tree(struct UI_Widget* widget);
//Vertical Layout
void ui_widget_layout_vertical(UI_Widget* widget,float spacing);
//Horizontal layout
void ui_widget_layout_horizontal(UI_Widget* widget,float spacing);
void ui_widget_set_layout(UI_Widget* widget,UI_LayoutType type,float spacing);
//Layout 
void ui_widget_layout(UI_Widget* widget);  //this function calculates one widget's children

void ui_widget_set_padding(UI_Widget* widget,float top,float bottom,float left,float right);
void ui_widget_set_size_mode(UI_Widget* widget,UI_SizeMode width_mode,UI_SizeMode height_mode);

void ui_widget_set_limits(UI_Widget* widget,float min_width,float min_height,float max_width,float max_height);

void ui_widget_set_preferred_size(UI_Widget* widget,float width,float height);
//Let's measure before we layout the widgets,that is we will seperate the measurement from the layout
/*
Fill uses the parent space width or height
Auto->uses measured width or height
Fixed uses preferred width or height 
*/
/*
      UI Widget Tree
        |
      Measure Phase(bottom up)
         |[Fixed sizes and auto sizes]
       preferred sizes
          |
        Layout Phase(top down)
          |[Fill Size]
        Final Rectangles
          |
        Draw Phase(top down )

  The size modes also affect measurement,
  FIXED:use widget's explicit /preferred size
  AUTO: calculate intrinsic size
  FILL: don't determine size during measurement

*/
//For measurement,measure self,
void ui_widget_measure(UI_Widget* widget);
void ui_widget_measure_tree(UI_Widget* widget);
//Let's the container also measure the children
UI_Size ui_widget_measure_children(UI_Widget* widget);
UI_Size ui_widget_get_measure_size(const UI_Widget* widget);
#endif 