#ifndef UI_EVENT_H
#define UI_EVENT_H
 struct UI_Widget;
typedef enum{
    UI_EVENT_NONE,
    UI_EVENT_POINTER_MOVE,
    UI_EVENT_POINTER_PRESS,
    UI_EVENT_POINTER_RELEASE,
    UI_EVENT_POINTER_ENTER,
    UI_EVENT_POINTER_LEAVE
}UI_EventType;


typedef struct{
    UI_EventType type;
    float x;
    float y;
    int button;
} UI_Event;
typedef void (*UI_WidgetEventFn)(struct UI_Widget* widget,const UI_Event* event);

#endif