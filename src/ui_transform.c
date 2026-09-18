#include<core/ui_transform.h>
#include<math.h>

UI_Transform ui_transform_identity(){
     UI_Transform transform={0};

     for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            transform.m[i][j]=0;
        }
     }
     transform.m[0][0]=1.0f;
     transform.m[1][1]=1.0f;
     transform.m[2][2]=1.0f;
     return transform;
}
UI_Transform ui_transform_translation(float x,float y){
     UI_Transform transform=ui_transform_identity();

     transform.m[0][2]=x;
     transform.m[1][2]=y;

     return transform;
}

UI_Transform ui_transform_scale(float x,float y){
    UI_Transform transform=ui_transform_identity();

    transform.m[0][0]=x;
    transform.m[1][1]=y;

    return transform;
}
UI_Transform ui_transform_rotation(float radians){
    float c=cos(radians);
    float s=sin(radians);

    UI_Transform transform=ui_transform_identity();

    transform.m[0][0]=c;
    transform.m[0][1]=-s;
    transform.m[1][0]=s;
    transform.m[1][1]=c;

    return transform;
}

UI_Transform ui_transform_multiply(UI_Transform a,UI_Transform b){
    UI_Transform transform={{0}};

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;++k){
                transform.m[i][j]+=a.m[i][k]*b.m[k][j];
            }
        }
    }
    return transform;
}

UI_Point ui_transform_point(UI_Transform transform,UI_Point point){
    UI_Point result;

    result.x=transform.m[0][0]* point.x + transform.m[0][1]*point.y+transform.m[0][2];

    result.y=transform.m[1][0]*point.x+transform.m[1][1]*point.y+ transform.m[1][2];

    return result;
}