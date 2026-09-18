#ifndef UI_GEOMETRY_H
#define UI_GEOMETRY_H

#include<core/ui_point.h>
#include<core/ui_rect.h>
#include<core/ui_transform.h>
struct UI_Quad{
    UI_Point points[4];
};

typedef struct UI_Quad UI_Quad;

/*
Corner Order

0 --------1
|         |
|         |
3----------2

*/

UI_Quad ui_quad_from_rect(UI_Rect rect);

UI_Quad ui_quad_transform(UI_Quad quad,UI_Transform transform);

UI_Rect ui_quad_bounds(UI_Quad quad);
#endif