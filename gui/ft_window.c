#include "ft_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _FWContext FWContext;

struct _FWContext
{
    FTList *windows;
};

static FWContext fw_context;

static void ft_window_event_handler(FTEvent *event, void *data);
static void ft_window_destroy(FTWidget *widget);

FTWindow *ft_window_new()
{
    FTWindow *window = malloc(sizeof(FTWindow));
    FTWidget *widget = (FTWidget *)window;

    memset(window, 0, sizeof(window));

    ft_widget_init_default(widget);

    widget->type = FW_TYPE_WINDOW;
    widget->draw = ft_window_draw;

    widget->rect.x = 0;
    widget->rect.y = 0;
    widget->rect.width = widget->surface->width;
    widget->rect.height = widget->surface->height;

    window->buffer = malloc(widget->surface->size);

    widget->destroy = ft_window_destroy;
    widget->handler = ft_window_event_handler;
    widget->data = window;

    fw_context.windows = ft_list_append(fw_context.windows, window);

    return window;
}

void ft_window_layout(FTWindow *window)
{
    FTWidget *w = (FTWidget *)window;
    FBSurface *s = w->surface;

    FTRect rect;
    int i = 0;

    FTList *iter = window->children;

    for (; iter; iter = iter->next, i++)
    {
        w = (FTWidget *)iter->data;

        if (!w->rect.width || !w->rect.height)
        {
            w->rect.x = (i % 2) ? (s->width / 2 + FT_FONT_W) : FT_FONT_W;
            w->rect.y = (i / 2) * (FT_FONT_H + FT_FONT_W * 3) + FT_FONT_W;

            w->rect.width = s->width / 2 - FT_FONT_W * 2;
            w->rect.height = FT_FONT_H + FT_FONT_W * 2;
        }
    }
}

void ft_window_draw(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTWidget *w;

    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        w = (FTWidget *)iter->data;

        w->draw(w);
    }

    memcpy(window->buffer, widget->surface->buffer, widget->surface->size);

    ft_event_set_key_handler(widget->handler, widget->data);

    window->focus = ft_window_get_focus(window);
}

int ft_window_add_child(FTWindow *window, FTWidget *widget)
{
    assert(widget != NULL);

    window->children = ft_list_append(window->children, widget);

    ft_window_layout(window);

    return FT_SUCCESS;
}

FTWidget *ft_window_get_focus(FTWindow *window)
{
    FTList *iter = window->children;
    FTWidget *widget = NULL;

    if (!iter)
        return NULL;

    for (; iter; iter = iter->next)
    {
        widget = (FTWidget *)iter->data;

        if (widget->focus) break;
    }

    return widget;
}

void ft_window_move_focus(FTWindow *window, int orient)
{
    FTWidget *widget;
    FTList *iter;

    widget = ft_window_get_focus(window);

    if (!widget)
        return;

    iter = ft_list_find(window->children, widget);

    if (orient > 0)
    {
        if (iter && iter->next)
            iter = iter->next;
        else
            iter = window->children;
    }
    else
    {
        if (iter && iter->prev)
            iter = iter->prev;
        else
            iter = ft_list_last(window->children);
    }

    window->focus = (FTWidget *)iter->data;

    ft_widget_set_focus(window->focus);
    ft_widget_unset_focus(widget);
}

void ft_window_close(FTWindow *window)
{
    assert(window != NULL);

    FTList *last = ft_list_last(fw_context.windows);

    ft_window_destroy((FTWidget *)window);

    if (window == last->data)
    {
        if (last->prev)
            ft_window_draw((FTWidget *)last->prev->data);
        else
            exit(FT_SUCCESS);
    }

    fw_context.windows = ft_list_delete(fw_context.windows, window);
}

static void ft_window_destroy(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        FTWidget *w = (FTWidget *)iter->data;

        w->destroy(w);
    }

    free(window->buffer);
    free(window);
}

static FTWidget *ft_window_find_widget(FTWindow *window, FTPoint *point)
{
    FTWidget *widget = NULL;
    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        widget = (FTWidget *)iter->data;

        if (ft_point_in_box(point, &widget->rect))
        {
            return widget;
        }
    }

    return NULL;
}

static void ft_window_event_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    FTMouseEvent *me;
    FTKeyEvent *ke;
    FTPoint point;
    FTWidget *w;

    switch (event->type)
    {
        case FE_KEY_RELEASE:
            ke = (FTKeyEvent *)event;

            if (ke->key == FT_KEY_OK)
            {
                w = ft_window_get_focus(window);

                if (w && w->handler)
                {
                    w->handler(event, w->data);
                }
            }
            else if (ke->key == FT_KEY_DIAL)
            {
                ft_window_move_focus(window, 1);
            }
            else if (ke->key == FT_KEY_END)
            {
                ft_window_move_focus(window, -1);
            }
            else if (ke->key == FT_KEY_BACK)
            {
                ft_window_close(window);
            }

            break;

        case FE_MOUSE_PRESS:
            me = (FTMouseEvent *)event;

            point.x = me->x;
            point.y = me->y;

            w = ft_window_find_widget(window, &point);

            w = ft_window_find_widget(window, &point);

            if (w && w->handler && w->visible)
            {
                //TODO
            }

            break;

        case FE_MOUSE_RELEASE:
            me = (FTMouseEvent *)event;

            point.x = me->x;
            point.y = me->y;

            w = ft_window_find_widget(window, &point);

            if (w && w->handler && w->visible)
            {
                w->handler(event, w->data);
            }

            break;

        default: break;
    }
}

