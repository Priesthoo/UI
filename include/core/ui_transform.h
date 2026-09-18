#ifndef UI_TRANSFORM_H
#define UI_TRANSFORM_H
#include<core/ui_point.h>

struct UI_Transform{
 float m[3][3];
};
typedef struct UI_Transform UI_Transform;

UI_Transform ui_transform_identity(void);

UI_Transform ui_transform_translation(float x,float y);

UI_Transform ui_transform_scale(float x,float y);

UI_Transform ui_transform_rotation(float radians);

UI_Transform ui_transform_multiply(UI_Transform a,UI_Transform b);

UI_Point ui_transform_point(UI_Transform transform,UI_Point point);


#endif  /* UI_TRANSFORM_H*/