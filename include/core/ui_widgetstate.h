#ifndef U_WIDGET_STATE_H
#define UI_WIDGET_STATE_H
#include<stdbool.h>

typedef struct {
bool visible;
bool enabled;
bool focused;
bool hovered;
bool pressed;

} UI_WidgetState;




#endif