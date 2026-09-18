#ifndef UI_RENDER_H
#define UI_RENDER_H
#include<core/ui_color.h>
#include<core/ui_rect.h>
typedef struct UI_Renderer UI_Renderer;

typedef void(*UI_RenderBeginFn)(UI_Renderer* renderer);
typedef void (*UI_RenderEndFn)(UI_Renderer* renderer);

typedef void (*UI_RenderFillRectFn)(UI_Renderer* renderer,UI_Rect rect,UI_Color color);


struct UI_Renderer{
 void* user_data;

 UI_RenderBeginFn begin;
 UI_RenderEndFn end;
 UI_RenderFillRectFn fill_rect;
};

void ui_renderer_begin(UI_Renderer* renderer);

void ui_renderer_end(UI_Renderer* renderer);

void ui_renderer_fill_rect(UI_Renderer* renderer,UI_Rect rect,UI_Color color);





#endif 