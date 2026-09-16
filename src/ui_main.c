#include<stdio.h>

#include<widget/ui_widget.h>
typedef struct UI_Widget UI_Widget;
//A simple c program
int main(void){
    UI_Widget window=*ui_widget_create();
     UI_Widget button=*ui_widget_create();
    UI_Widget text=*ui_widget_create();
    
    int v=ui_widget_add_child(&window,&button);
    ui_widget_add_child(&button,&text);
    ui_widget_set_size_mode(&window,UI_SIZE_AUTO,UI_SIZE_AUTO); //this will use the measured size from the children(measured_size.width,measured_size.height);
    ui_widget_set_preferred_size(&button,150.0f,60.0f);

                                    //Use preferred width,use preferred height
    ui_widget_set_size_mode(&button,UI_SIZE_FIXED,UI_SIZE_FIXED);
    
    UI_SizeConstraints window_constraint={
       .min_width=0.0f,
       .min_height=0.0f,
       .max_width=800.0f,
       .max_height=600.0f
    };
   
    getchar();
    return 0;
}