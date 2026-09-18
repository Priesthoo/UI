#include<geometry/ui_geometry.h>

UI_Quad ui_quad_from_rect(UI_Rect rect){
    UI_Quad quad={
     .points={
        {
            .x=rect.x,
            .y=rect.y
        },
        {
            .x=rect.x+rect.width,
            .y=rect.y
        },
        {
            .x=rect.x+rect.width,
            .y=rect.y+rect.height
        },
        {
            .x=rect.x,
            .y=rect.y+rect.height
        }
     }
   
    };
    return quad;
}
UI_Quad ui_quad_transform(UI_Quad quad,UI_Transform transform){
    UI_Quad result;

    for(int i=0;i<4;++i){
        result.points[i]=ui_transform_point(transform,quad.points[i]);
    }

    return result;
}

UI_Rect ui_quad_bounds(UI_Quad quad){
    float min_x=quad.points[0].x;
    float max_x=quad.points[0].x;

    float min_y=quad.points[0].y;
    float max_y=quad.points[0].y;

    for(int i=1;i<4;i++){
        if(quad.points[i].x < min_x){
            min_x=quad.points[i].x;
        }
        if(quad.points[i].x> max_x){
            max_x=quad.points[i].x;
        }
        if(quad.points[i].y < min_y){
            min_y=quad.points[i].y;
        }
        if(quad.points[i].y> max_y){
            max_y=quad.points[i].y;
        }
    }

    UI_Rect rect={
      .x=min_x,
      .y=min_y,
      .width=max_x-min_x,
      .height=max_y-min_y
    };

    return rect;
}