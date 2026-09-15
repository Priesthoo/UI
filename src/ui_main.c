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
    int u=ui_widget_remove_child(&window,&button);
    if(u==0){
      printf("It cannot be removed");
    }
   if(v!=0){
    printf("Window's Children count: %d\n",window.object.children.count);
   }
   printf("Button's children count: %d\n",button.object.children.count);

   
    getchar();
    return 0;
}