#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<widget/ui_widget.h>

#define TEST_EPSILON 0.001f

static int test_run=0;
static int test_passed=0;
static int test_failed=0;

static int float_equal(float a,float b){
    return fabsf(a-b)<=TEST_EPSILON;
}
static void test_pass(const char* name){
    test_run++;
    test_passed++;
    printf("[PASS] %s\n",name);
    return;
}
static void test_fail(const char* name){
    test_run++;
    test_failed++;
    printf("[FAILED] %s\n",name);
    return;
}

#define TEST_TRUE(name,condition)   \
  do{                               \
     if(condition){                 \
        test_pass(name);             \
     }                               \
     else{                            \
        test_fail(name);               \
     }                                 \
} while(0)

#define TEST_FLOAT(name,actual,expected) \
   do{                                   \
    if(float_equal((actual),(expected))){ \
     test_pass(name);                    \
    } else{                             \
    test_run++;                         \
    test_failed++;                      \         
    printf("FAILED %s (expected %.3f,got %.3f) \n",name,(double)(expected),(double)(actual));\
    } \  
   } while(0)

static void print_test_summary(void){
    printf("\n");
    printf("=====================================\n");
    printf("Test run:   %d\n",test_run);
    printf("Test passed: %d\n",test_passed);
    printf("Test failed: %d\n",test_failed);
    printf("=====================================\n");
    return;
   }

static void test_get_measure_sized_fixed(void){
    UI_Widget widget={0};
    widget.width_mode=UI_SIZE_FIXED;
    widget.height_mode=UI_SIZE_FIXED;
    widget.preferred_size.width=120.0f;
    widget.preferred_size.height=80.0f;

    UI_Size size=ui_widget_get_measure_size(&widget);

    TEST_FLOAT("get_measure_size fixed width",size.width,120.0f);

    TEST_FLOAT("get_measure_size fixed height",size.height,80.0f);

    return;
   }

static void test_get_measure_size_auto(void){
    UI_Widget widget={0};
    widget.width_mode=UI_SIZE_AUTO;
    widget.height_mode=UI_SIZE_AUTO;
    widget.measured_size.width=200.0f;
    widget.measured_size.height=100.0f;

    UI_Size size=ui_widget_get_measure_size(&widget);
    TEST_FLOAT("get measure_size auto width",size.width,200.0f);

    TEST_FLOAT("get measure_size auto height",size.height,100.0f);
    return;
}
static void test_get_measure_size_fill(void){
    UI_Widget widget={0};
    widget.width_mode=UI_SIZE_FILL;
    widget.height_mode=UI_SIZE_FILL;
    widget.measured_size.width=200.0f;
    widget.measured_size.height=100.0f;

    UI_Size size=ui_widget_get_measure_size(&widget);
    TEST_FLOAT("get measure_size fill width",size.width,0.0);

    TEST_FLOAT("get measure_size fill height",size.height,0.0);
    return;
}

static UI_Size test_measure(UI_Widget* widget,UI_SizeConstraints constraints){
    (void)widget;

    UI_Size size={
        300.0f,
        200.0f
    };
    if(constraints.min_width>size.width){
        size.width=constraints.min_width;
    }
    if(constraints.min_height>size.height){
        size.height=constraints.min_height;
    }
    if(constraints.max_width>0.0 && size.width>constraints.max_width){
       size.width=constraints.max_width;
    }
    if(constraints.max_height>0.0 && size.height>constraints.max_height){
       size.height=constraints.max_height;
    }
    return size;
}
static void test_measure_constraints(void){
    UI_Widget widget={0};

    widget.measure=test_measure;
    UI_SizeConstraints constraints={
        .min_width=100.0f,
        .min_height=50.0f,
        .max_width=150.0f,
        .max_height=120.0f
    };
    UI_Size size=widget.measure(&widget,constraints);

    TEST_FLOAT("measure max width ",size.width,150.0f);

    TEST_FLOAT("measure max height ",size.height,120.0f);
}
static void test_root_size_resolution(void){
    UI_Widget widget={0};

    widget.width_mode=UI_SIZE_FIXED;
    widget.height_mode=UI_SIZE_FIXED;

    widget.preferred_size.width=640.0f;
    widget.preferred_size.height=480.0f;

    ui_widget_resolve_root_size(&widget);
    TEST_FLOAT("root fixed width",widget.rect.width,640.0f);
    TEST_FLOAT("root fixed height",widget.rect.height,480.0f);
}


static void test_vertical_fixed_children(void){
    UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();

    root.rect.width=300.0f;
    root.rect.height=500.0f;

    root.layout_type=UI_LAYOUT_VERTICAL;
    root.padding.top=0.0f;
    root.padding.bottom=0.0f;
    root.padding.right=0.0f;
    root.padding.left=0.0f;

    child_a.width_mode=UI_SIZE_FILL;
    child_a.height_mode=UI_SIZE_FIXED;
   
    ui_widget_set_preferred_size(&child_a,0,100.0f);

    child_b.width_mode=UI_SIZE_FILL;
    child_b.height_mode=UI_SIZE_FIXED;
   
     ui_widget_set_preferred_size(&child_b,0.0,150.0f);

    child_c.width_mode=UI_SIZE_FILL;
    child_c.height_mode=UI_SIZE_FIXED;
   
     ui_widget_set_preferred_size(&child_c,0.0,50.0f);

    ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
    printf("Root Children Count: %d \n",root.object.children.count);
    ui_widget_layout(&root);
   
   TEST_FLOAT("Vertical child A height",child_a.rect.height,100.0f);

    TEST_FLOAT("Vertical child B height",child_b.rect.height,150.0f);

    TEST_FLOAT("Vertical child C height",child_c.rect.height,50.0f);

    TEST_FLOAT("Vertical child A y",child_a.rect.y,0.0f);

    TEST_FLOAT("Vertical child B y",child_b.rect.y,100.0f);

    TEST_FLOAT("Vertical child C y",child_c.rect.y,250.0f);
    return;
}
static void test_vertical_spacing(void){
     UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();

      root.rect.width=300.0f;
    root.rect.height=500.0f;
     root.layout_spacing=20.0f;
     root.layout_type=UI_LAYOUT_VERTICAL;

      child_a.width_mode=UI_SIZE_FILL;
    child_a.height_mode=UI_SIZE_FIXED;
   
    ui_widget_set_preferred_size(&child_a,0,100.0f);

    child_b.width_mode=UI_SIZE_FILL;
    child_b.height_mode=UI_SIZE_FIXED;
   
     ui_widget_set_preferred_size(&child_b,0.0,150.0f);

    child_c.width_mode=UI_SIZE_FILL;
    child_c.height_mode=UI_SIZE_FIXED;
   
     ui_widget_set_preferred_size(&child_c,0.0,50.0f);

    ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
      ui_widget_layout(&root);
      TEST_FLOAT("Vertical child A y",child_a.rect.y,0.0f);

    TEST_FLOAT("Vertical child B y",child_b.rect.y,120.0f);

    TEST_FLOAT("Vertical child C y",child_c.rect.y,290.0f);

    return;
}

static void test_vertical_padding(){
     UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();

      root.rect.width=300.0f;
    root.rect.height=500.0f;
     
    root.layout_type=UI_LAYOUT_VERTICAL;
    root.padding.top=20.0f;
    root.padding.bottom=30.0f;
    root.padding.right=10.0f;
    root.padding.left=15.0f;

    child_a.width_mode=UI_SIZE_FILL;
    child_a.height_mode=UI_SIZE_FIXED;
   
    ui_widget_set_preferred_size(&child_a,0,100.0f);

     ui_widget_add_child(&root,&child_a);

      ui_widget_layout(&root);
      TEST_FLOAT("Vertical padding child y",child_a.rect.y,20.0f);

       TEST_FLOAT("Vertical padding child width",child_a.rect.width,275.0f);
        
    return;
}

static void test_vertical_fill(){
    UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();

      root.rect.width=300.0f;
     root.rect.height=500.0f;
     root.layout_spacing=0.0f;
     root.layout_type=UI_LAYOUT_VERTICAL;

      child_a.width_mode=UI_SIZE_FILL;
    child_a.height_mode=UI_SIZE_FIXED;
   
    ui_widget_set_preferred_size(&child_a,0,100.0f);

    child_b.width_mode=UI_SIZE_FILL;
    child_b.height_mode=UI_SIZE_FILL;
   
    

    child_c.width_mode=UI_SIZE_FILL;
    child_c.height_mode=UI_SIZE_FIXED;
   
     ui_widget_set_preferred_size(&child_c,0.0,100.0f);

    ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
      ui_widget_layout(&root);

       TEST_FLOAT("Vertical fill A height",child_a.rect.height,100.0f);

    TEST_FLOAT("Vertical fill B height",child_b.rect.height,300.0f);

    TEST_FLOAT("Vertical fill C height",child_c.rect.height,100.0f);
      return;
}

static void test_vertical_multiple_fill(){
    UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();
    UI_Widget child_d=*ui_widget_create();


    root.rect.width=300.0f;
    root.rect.height=500.0f;
    root.layout_type=UI_LAYOUT_VERTICAL;

    child_a.height_mode=UI_SIZE_FIXED;
    child_a.width_mode=UI_SIZE_FILL;

     ui_widget_set_preferred_size(&child_a,0,100.0f);

     child_b.width_mode=UI_SIZE_FILL;
     child_b.height_mode=UI_SIZE_FILL;

     child_c.width_mode=UI_SIZE_FILL;
     child_c.height_mode=UI_SIZE_FILL;

     child_d.height_mode=UI_SIZE_FILL;
     child_d.width_mode=UI_SIZE_FILL;
    
      ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
    ui_widget_add_child(&root,&child_d);
      ui_widget_layout(&root);


     
    TEST_FLOAT("multiple fill A",child_a.rect.height,100.0f);

    TEST_FLOAT("Vertical fill B",child_b.rect.height,150.0f);

    TEST_FLOAT("Vertical fil C ",child_c.rect.height,150.0f);

    TEST_FLOAT("Vertical fill D ",child_d.rect.height,100.0f);

    return;
}

static void test_horizontal_alignment(void){
     UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();

    root.rect.width=300.0f;
     root.rect.height=200.0f;
     root.layout_spacing=0.0f;
     root.layout_type=UI_LAYOUT_VERTICAL;
    
     child_a.height_mode=UI_SIZE_FIXED;
     child_a.width_mode=UI_SIZE_FIXED;
     
      ui_widget_set_preferred_size(&child_a,100,50.0f);
        ui_widget_add_child(&root,&child_a);
         

        root.horizontal_align=UI_ALIGN_START;
         ui_widget_layout(&root);
          TEST_FLOAT("horizontal START",child_a.rect.x,0.0f);
    
        root.horizontal_align=UI_ALIGN_CENTER;
         ui_widget_layout(&root);
          TEST_FLOAT("horizontal CENTER",child_a.rect.x,100.0f);

        root.horizontal_align=UI_ALIGN_END;
         ui_widget_layout(&root);
          TEST_FLOAT("horizontal END",child_a.rect.x,200.0f);
    return;
}

static void test_horizontal_fill_min_redistribution(){
     UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();
   

    root.rect.width=500.0f;
    root.rect.height=200.0f;
    root.layout_type=UI_LAYOUT_HORIZONTAL;

    child_a.width_mode=UI_SIZE_FIXED;
    ui_widget_set_preferred_size(&child_a,100.0,0.0f);

    child_b.width_mode=UI_SIZE_FILL;
     child_b.height_mode=UI_SIZE_FILL;

     child_c.width_mode=UI_SIZE_FILL;
     child_c.height_mode=UI_SIZE_FILL;
    
     child_b.size_limits.min_width=300.0f;

      ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
  
      ui_widget_layout(&root);
   
     TEST_FLOAT("minimum redistribution A",child_a.rect.width,100.0f);

    TEST_FLOAT("minimum redistribution B",child_b.rect.width,300.0f);

    TEST_FLOAT("minimum redistribution C ",child_c.rect.width,100.0f);

    return;
}

static void test_horizontal_fill_max_redistribution(){
     UI_Widget root=*ui_widget_create();
    UI_Widget child_a=*ui_widget_create();
    UI_Widget child_b=*ui_widget_create();
    UI_Widget child_c=*ui_widget_create();
   

    root.rect.width=600.0f;
    root.rect.height=200.0f;
    root.layout_type=UI_LAYOUT_HORIZONTAL;

    child_a.width_mode=UI_SIZE_FIXED;
    ui_widget_set_preferred_size(&child_a,100.0,0.0f);

    child_b.width_mode=UI_SIZE_FILL;
     child_b.height_mode=UI_SIZE_FILL;

     child_c.width_mode=UI_SIZE_FILL;
     child_c.height_mode=UI_SIZE_FILL;
    
     child_b.size_limits.max_width=150.0f;

      ui_widget_add_child(&root,&child_a);
    ui_widget_add_child(&root,&child_b);
    ui_widget_add_child(&root,&child_c);
  
      ui_widget_layout(&root);
   
     TEST_FLOAT("maximum redistribution A",child_a.rect.width,100.0f);

    TEST_FLOAT("maximum redistribution B",child_b.rect.width,150.0f);

    TEST_FLOAT("maximum redistribution C ",child_c.rect.width,350.0f);

    return;
}
static void test_nested_layouts(){
   UI_Widget root=*ui_widget_create();
   UI_Widget left=*ui_widget_create();
   UI_Widget right=*ui_widget_create();
   UI_Widget a=*ui_widget_create();
   UI_Widget b=*ui_widget_create();
   UI_Widget c=*ui_widget_create();
   UI_Widget d=*ui_widget_create();

   root.rect.width=600.0f;
   root.rect.height=400.0f;

   root.layout_type=UI_LAYOUT_HORIZONTAL;
   left.width_mode=UI_SIZE_FIXED;
   left.height_mode=UI_SIZE_FILL;

   ui_widget_set_preferred_size(&left,300.0f,0.0);

   right.width_mode=UI_SIZE_FIXED;
   right.height_mode=UI_SIZE_FILL;

   ui_widget_set_preferred_size(&right,300.0f,0.0);

   left.layout_type=UI_LAYOUT_VERTICAL;
   right.layout_type=UI_LAYOUT_VERTICAL;

   a.width_mode=UI_SIZE_FILL;
   a.height_mode=UI_SIZE_FIXED;
    ui_widget_set_preferred_size(&a,0.0,100.0f);

   b.width_mode=UI_SIZE_FILL;
   b.height_mode=UI_SIZE_FIXED;
   ui_widget_set_preferred_size(&b,0.0,200.0f);

   c.width_mode=UI_SIZE_FILL;
   c.height_mode=UI_SIZE_FIXED;
   ui_widget_set_preferred_size(&c,0.0,150.0f);

  d.width_mode=UI_SIZE_FILL;
   d.height_mode=UI_SIZE_FIXED;
   ui_widget_set_preferred_size(&d,0.0,100.0f);

  ui_widget_add_child(&left,&a);
  ui_widget_add_child(&left,&b);

  ui_widget_add_child(&right,&c);
  ui_widget_add_child(&right,&d);

  ui_widget_add_child(&root,&left);
  ui_widget_add_child(&root,&right);

  ui_widget_layout_tree(&root);
  

  TEST_FLOAT("Nested left width",left.rect.width,300.0f);

  TEST_FLOAT("Nested right width",right.rect.width,300.0f);

  TEST_FLOAT("Nested left child A height",a.rect.height,100.0f);

  TEST_FLOAT("Nested left child B height",b.rect.height,200.0f);

  TEST_FLOAT("Nested left child C height",c.rect.height,150.0f);

   TEST_FLOAT("Nested left child D height",d.rect.height,100.0f);

   TEST_FLOAT("Nested right x",right.rect.x,300.0f);


    return;
}





int main(){
      test_get_measure_sized_fixed();
      test_get_measure_size_auto();
      test_get_measure_size_fill();
      test_measure_constraints();
      test_root_size_resolution();
      test_vertical_fixed_children();
      test_vertical_spacing();
      test_vertical_padding();
      test_vertical_fill();
      test_horizontal_alignment();
      test_horizontal_fill_min_redistribution();
      test_horizontal_fill_max_redistribution();
      test_nested_layouts();
      print_test_summary();
     if(test_failed==0){
        printf("Test passed\n");
     }
     else{
        printf("Test failed \n");
     }
      getchar();
      return 1;
   }