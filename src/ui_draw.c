#include <render/ui_draw.h>

#include <stdlib.h>


/*
 * Initial number of commands allocated when the draw list
 * needs storage for the first time.
 */
#define UI_DRAW_LIST_INITIAL_CAPACITY 8


/*
 * Grow the command array.
 *
 * The draw list uses a dynamically allocated array.
 *
 * Example:
 *
 *     8 commands
 *         ↓
 *     16 commands
 *         ↓
 *     32 commands
 *         ↓
 *     64 commands
 *
 * Returns:
 *     1 = success
 *     0 = allocation failure
 */
static int ui_draw_list_grow(
    UI_DrawList* draw_list
)
{
    size_t new_capacity;
    UI_DrawCommand* commands;


    /*
     * The draw list itself must exist.
     */
    if (!draw_list)
    {
        return 0;
    }


    /*
     * If this is the first allocation,
     * start with the initial capacity.
     */
    if (draw_list->capacity == 0)
    {
        new_capacity = UI_DRAW_LIST_INITIAL_CAPACITY;
    }
    else
    {
        /*
         * Otherwise double the existing capacity.
         */
        new_capacity = draw_list->capacity * 2;
    }


    /*
     * Resize the command array.
     */
    commands = realloc(
        draw_list->commands,
        new_capacity * sizeof(*commands)
    );


    /*
     * realloc() returns NULL when allocation fails.
     *
     * The original allocation is still valid in this case.
     */
    if (!commands)
    {
        return 0;
    }


    /*
     * Store the new array and capacity.
     */
    draw_list->commands = commands;
    draw_list->capacity = new_capacity;


    return 1;
}


/*
 * Initialize an empty draw list.
 */
void ui_draw_list_init(
    UI_DrawList* draw_list
)
{
    if (!draw_list)
    {
        return;
    }


    /*
     * No command storage has been allocated yet.
     */
    draw_list->commands = NULL;
    draw_list->count = 0;
    draw_list->capacity = 0;
}


/*
 * Destroy the draw list.
 */
void ui_draw_list_destroy(
    UI_DrawList* draw_list
)
{
    if (!draw_list)
    {
        return;
    }


    /*
     * Release the dynamically allocated command array.
     */
    free(draw_list->commands);


    /*
     * Reset the structure so that it no longer
     * refers to the released memory.
     */
    draw_list->commands = NULL;
    draw_list->count = 0;
    draw_list->capacity = 0;
}


/*
 * Clear all commands.
 *
 * The allocated memory is intentionally kept.
 *
 * This allows the draw list to be reused every frame
 * without constantly allocating and freeing memory.
 */
void ui_draw_list_clear(
    UI_DrawList* draw_list
)
{
    if (!draw_list)
    {
        return;
    }


    draw_list->count = 0;
}


/*
 * Add a filled rectangle command.
 */
int ui_draw_list_fill_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    UI_Color color
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    /*
     * Make sure there is enough room for another command.
     */
    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    /*
     * Get the next unused command slot.
     */
    command = &draw_list->commands[draw_list->count];


    /*
     * Store the command type.
     */
    command->type = UI_DRAW_COMMAND_FILL_RECT;


    /*
     * Store the command data.
     */
    command->data.fill_rect.rect = rect;
    command->data.fill_rect.color = color;


    /*
     * The command now belongs to the draw list.
     */
    draw_list->count++;


    return 1;
}


/*
 * Add a filled rounded rectangle command.
 */
int ui_draw_list_fill_rounded_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float radius,
    UI_Color color
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_FILL_ROUNDED_RECT;


    command->data.fill_rounded_rect.rect = rect;
    command->data.fill_rounded_rect.radius = radius;
    command->data.fill_rounded_rect.color = color;


    draw_list->count++;


    return 1;
}


/*
 * Add a rectangle outline command.
 */
int ui_draw_list_stroke_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float thickness,
    UI_Color color
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_STROKE_RECT;


    command->data.stroke_rect.rect = rect;
    command->data.stroke_rect.thickness = thickness;
    command->data.stroke_rect.color = color;


    draw_list->count++;


    return 1;
}


/*
 * Add a rounded rectangle outline command.
 */
int ui_draw_list_stroke_rounded_rect(
    UI_DrawList* draw_list,
    UI_Rect rect,
    float radius,
    float thickness,
    UI_Color color
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_STROKE_ROUNDED_RECT;


    command->data.stroke_rounded_rect.rect = rect;
    command->data.stroke_rounded_rect.radius = radius;
    command->data.stroke_rounded_rect.thickness = thickness;
    command->data.stroke_rounded_rect.color = color;


    draw_list->count++;


    return 1;
}


/*
 * Add a filled quadrilateral command.
 */
int ui_draw_list_fill_quad(
    UI_DrawList* draw_list,
    UI_Quad quad,
    UI_Color color
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_FILL_QUAD;


    command->data.fill_quad.quad = quad;
    command->data.fill_quad.color = color;


    draw_list->count++;


    return 1;
}


/*
 * Add a push-transform command.
 */
int ui_draw_list_push_transform(
    UI_DrawList* draw_list,
    UI_Transform transform
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_PUSH_TRANSFORM;


    command->data.push_transform.transform = transform;


    draw_list->count++;


    return 1;
}


/*
 * Add a pop-transform command.
 */
int ui_draw_list_pop_transform(
    UI_DrawList* draw_list
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_POP_TRANSFORM;


    draw_list->count++;


    return 1;
}


/*
 * Add a push-clip command.
 */
int ui_draw_list_push_clip(
    UI_DrawList* draw_list,
    UI_Rect rect
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_PUSH_CLIP;


    command->data.push_clip.rect = rect;


    draw_list->count++;


    return 1;
}


/*
 * Add a pop-clip command.
 */
int ui_draw_list_pop_clip(
    UI_DrawList* draw_list
)
{
    UI_DrawCommand* command;


    if (!draw_list)
    {
        return 0;
    }


    if (draw_list->count >= draw_list->capacity)
    {
        if (!ui_draw_list_grow(draw_list))
        {
            return 0;
        }
    }


    command = &draw_list->commands[draw_list->count];


    command->type = UI_DRAW_COMMAND_POP_CLIP;


    draw_list->count++;


    return 1;
}


/*
 * Execute the entire draw list.
 *
 * The draw list itself does not perform rendering.
 *
 * It simply translates stored commands into calls
 * to the backend-independent UI_Renderer.
 */
void ui_draw_list_execute(
    const UI_DrawList* draw_list,
    UI_Renderer* renderer
)
{
    size_t i;


    if (!draw_list)
    {
        return;
    }


    if (!renderer)
    {
        return;
    }


    /*
     * Tell the renderer that drawing is beginning.
     */
    ui_renderer_begin(renderer);


    /*
     * Execute commands in exactly the order in which
     * they were inserted.
     *
     * This ordering is important because transforms
     * and clipping affect subsequent commands.
     */
    for (i = 0; i < draw_list->count; ++i)
    {
        const UI_DrawCommand* command =
            &draw_list->commands[i];


        switch (command->type)
        {
            /*
             * -------------------------------------------------
             * FILL RECTANGLE
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_FILL_RECT:
            {
                ui_renderer_fill_rect(
                    renderer,
                    command->data.fill_rect.rect,
                    command->data.fill_rect.color
                );

                break;
            }


            /*
             * -------------------------------------------------
             * FILL ROUNDED RECTANGLE
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_FILL_ROUNDED_RECT:
            {
                ui_renderer_fill_rounded_rect(
                    renderer,
                    command->data.fill_rounded_rect.rect,
                    command->data.fill_rounded_rect.radius,
                    command->data.fill_rounded_rect.color
                );

                break;
            }


            /*
             * -------------------------------------------------
             * STROKE RECTANGLE
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_STROKE_RECT:
            {
                ui_renderer_stroke_rect(
                    renderer,
                    command->data.stroke_rect.rect,
                    command->data.stroke_rect.thickness,
                    command->data.stroke_rect.color
                );

                break;
            }


            /*
             * -------------------------------------------------
             * STROKE ROUNDED RECTANGLE
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_STROKE_ROUNDED_RECT:
            {
                ui_renderer_stroke_rounded_rect(
                    renderer,
                    command->data.stroke_rounded_rect.rect,
                    command->data.stroke_rounded_rect.radius,
                    command->data.stroke_rounded_rect.thickness,
                    command->data.stroke_rounded_rect.color
                );

                break;
            }


            /*
             * -------------------------------------------------
             * FILL QUAD
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_FILL_QUAD:
            {
                ui_renderer_fill_quad(
                    renderer,
                    command->data.fill_quad.quad,
                    command->data.fill_quad.color
                );

                break;
            }


            /*
             * -------------------------------------------------
             * PUSH TRANSFORM
             * -------------------------------------------------
             *
             * Every drawing operation after this command
             * is affected by the new transform until the
             * corresponding POP_TRANSFORM command.
             */
            case UI_DRAW_COMMAND_PUSH_TRANSFORM:
            {
                ui_renderer_push_transform(
                    renderer,
                    command->data.push_transform.transform
                );

                break;
            }


            /*
             * -------------------------------------------------
             * POP TRANSFORM
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_POP_TRANSFORM:
            {
                ui_renderer_pop_transform(
                    renderer
                );

                break;
            }


            /*
             * -------------------------------------------------
             * PUSH CLIP
             * -------------------------------------------------
             *
             * The current implementation records the clip
             * operation. Nested intersection is handled later.
             */
            case UI_DRAW_COMMAND_PUSH_CLIP:
            {
                ui_renderer_push_clip(
                    renderer,
                    command->data.push_clip.rect
                );

                break;
            }


            /*
             * -------------------------------------------------
             * POP CLIP
             * -------------------------------------------------
             */
            case UI_DRAW_COMMAND_POP_CLIP:
            {
                ui_renderer_pop_clip(
                    renderer
                );

                break;
            }
        }
    }


    /*
     * Tell the renderer that drawing is finished.
     */
    ui_renderer_end(renderer);
}