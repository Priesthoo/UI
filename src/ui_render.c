#include<render/ui_render.h>

void ui_renderer_begin(UI_Renderer* renderer){
    if(!renderer){
        return;
    }
    if(!renderer->begin ){
        return;
    }
    renderer->begin(renderer);
    return;
}
void ui_renderer_end(UI_Renderer* renderer){
    if(!renderer){
        return;
    }
    if(!renderer->end) return;

    renderer->end(renderer);
    return;
}

void ui_renderer_fill_rect(UI_Renderer* renderer,UI_Rect rect,UI_Color color){
    if(!renderer){
        return;
    }
    if(!renderer->fill_rect) return;

    renderer->fill_rect(renderer,rect,color);
}


