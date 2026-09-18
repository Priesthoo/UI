#ifndef UI_DRAW_H
#define UI_DRAW_H

#include <stddef.h>

#include <core/ui_rect.h>
#include <core/ui_color.h>
#include <core/ui_geometry.h>
#include <core/ui_transform.h>

#include <render/ui_render.h>


/*
 * Every operation that can be stored inside a UI_DrawList
 * is represented by one of these command types.
 */
typedef enum UI_DrawCommandType
{
    /* Draw a solid rectangular shape. */
    UI_DRAW_COMMAND_FILL_RECT,

    /* Draw a solid rounded rectangle. */
    UI_DRAW_COMMAND_FILL_ROUNDED_RECT,

    /* Draw the outline of a rectangle. */
    UI_DRAW_COMMAND_STROKE_RECT,

    /* Draw the outline of a rounded rectangle. */
    UI_DRAW_COMMAND_STROKE_ROUNDED_RECT,

    /* Draw a four-point quadrilateral. */
    UI_DRAW_COMMAND_FILL_QUAD,

    /* Push a transformation onto the renderer transform stack. */
    UI_DRAW_COMMAND_PUSH_TRANSFORM,

    /* Remove the current transformation from the renderer stack. */
    UI_DRAW_COMMAND_POP_TRANSFORM,

    /* Push a rectangular clipping region. */
    UI_DRAW_COMMAND_PUSH_CLIP,

    /* Remove the current clipping region. */
    UI_DRAW_COMMAND_POP_CLIP

} UI_DrawCommandType;


/*
 * Data required by FILL_RECT.
 */
typedef struct Fill_rect
{
    UI_Rect rect;
    UI_Color color;

} Fill_rect;


/*
 * Data required by FILL_ROUNDED_RECT.
 */
typedef struct Fill_rounded_rect
{
    UI_Rect rect;
    float radius;
    UI_Color color;

} Fill_rounded_rect;


/*
 * Data required by STROKE_RECT.
 */
typedef struct Stroke_rect
{
    UI_Rect rect;
    float thickness;
    UI_Color color;

} Stroke_rect;


/*
 * Data required by STROKE_ROUNDED_RECT.
 */
typedef struct Stroke_rounded_rect
{
    UI_Rect rect;
    float radius;
    float thickness;
    UI_Color color;

} Stroke_rounded_rect;


/*
 * Data required by FILL_QUAD.
 */
typedef struct Fill_quad
{
    UI_Quad quad;
    UI_Color color;

} Fill_quad;


/*
 * Data required by PUSH_TRANSFORM.
 */
typedef struct Push_transform
{
    UI_Transform transform;

} Push_transform;


/*
 * Data required by PUSH_CLIP.
 */
typedef struct Push_clip
{
    UI_Rect rect;

} Push_clip;


/*
 * A draw command contains:
 *
 * 1. The command type.
 * 2. The data associated with that command.
 *
 * The union allows one command to store only the data
 * required by its specific operation.
 */
typedef union Data
{
    Fill_rect fill_rect;

    Fill_rounded_rect fill_rounded_rect;

    Stroke_rect stroke_rect;

    Stroke_rounded_rect stroke_rounded_rect;

    Fill_quad fill_quad;

    Push_transform push_transform;

    Push_clip push_clip;

} Data;


/*
 * One command in the draw list.
 */
typedef struct UI_DrawCommand
{
    UI_DrawCommandType type;
    Data data;

} UI_DrawCommand;


/*
 * Dynamic array containing all drawing commands.
 */
typedef struct UI_DrawList
{
    UI_DrawCommand* commands;

    size_t count;
    size_t capacity;

} UI_DrawList;


/*
 * Initialize an empty draw list.
 */
void ui_draw_list_init(
    UI_DrawList* draw_list
);


/*
 * Destroy the draw list and release its memory.
 */
void ui_draw_list_destroy(
    UI_DrawList* draw_list
);


/*
 * Remove all commands from the draw list.
 *
 * This does not release the allocated command array.
 * The memory remains available for future commands.
 */
void ui_draw_list_clear(
    UI_DrawList* draw_list
);


/*
 * Add a filled rectangle command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_fill_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    UI_Color color
);


/*
 * Add a filled rounded rectangle command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_fill_rounded_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float radius,
    UI_Color color
);


/*
 * Add a rectangle outline command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_stroke_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float thickness,
    UI_Color color
);


/*
 * Add a rounded rectangle outline command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_stroke_rounded_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float radius,
    float thickness,
    UI_Color color
);


/*
 * Add a filled quadrilateral command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_fill_quad(
    UI_DrawList* draw_list,
    UI_Quad quad,
    UI_Color color
);


/*
 * Add a transformation command.
 *
 * The transform is pushed onto the renderer's
 * transformation stack when the draw list executes.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_push_transform(
    UI_DrawList* draw_list,
    UI_Transform transform
);


/*
 * Add a pop-transform command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_pop_transform(
    UI_DrawList* draw_list
);


/*
 * Add a rectangular clipping command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_push_clip(
    UI_DrawList* draw_list,
    UI_Rect rect
);


/*
 * Add a pop-clip command.
 *
 * Returns:
 *     1 = success
 *     0 = failure
 */
int ui_draw_list_pop_clip(
    UI_DrawList* draw_list
);


/*
 * Execute every command in the draw list through
 * the supplied renderer.
 */
void ui_draw_list_execute(
    const UI_DrawList* draw_list,
    UI_Renderer* renderer
);


#endif /* UI_DRAW_H */