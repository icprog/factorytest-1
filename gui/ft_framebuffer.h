
#include <linux/fb.h>

#ifndef _FT_FRAME_BUFFER_H_
#define _FT_FRAME_BUFFER_H_

typedef struct _FBSurface FBSurface;

struct _FBSurface
{
    int     width;
    int     height;
    int     depth;
    int     size;
    char   *buffer;
};

FBSurface *ft_frame_buffer_get_default();

int ft_frame_buffer_get_vinfo(struct fb_var_screeninfo *vinfo);

int ft_frame_buffer_get_finfo(struct fb_fix_screeninfo *finfo);

void ft_frame_buffer_close();

#endif/*_FT_FRAME_BUFFER_H_*/
