/****************************************************************************
 * midres - Portable midres library for retrocomputers                      *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * DRAWING LIBRARY                                                          *
 ****************************************************************************/

 /****************************************************************************
  ** INCLUDE SECTION
  ****************************************************************************/

#include <stdio.h>

#include "midres.h"

  // Overlay management is driven by the definition of the appropriate 
  // compilation symbol (__OVERLAY__). In this case, we enable or disable the 
  // compilation of the relevant code.

#ifdef __OVERLAY__MIDRES__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY4");
    #pragma rodata-name ("OVERLAY4");

#endif

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

#if !defined(__OVERLAY__MIDRES__) || !defined(__VIC20__)

 // The functions defined at this level can only be called up if the current
 // module has been loaded into memory. On the other hand, they can call any 
 // function declared at the resident module level.

char cabs(char _a) {
    if (_a > 0) {
        return _a;
    }
    else {
        return -_a;
    }
}

int iabs(int _a) {
    if (_a > 0) {
        return _a;
    }
    else {
        return -_a;
    }
}


// Clears the bitmap to color BLACK.
void _mr_clear_bitmap(mr_mixel* _screen, mr_color* _colormap) {

    int i;

    for (i = 0; i < WIDTH * HEIGHT; ++i) {
        _screen[i] = RENDERED_MIXELS[0];
    }

}

// Clears the bitmap to the specified color.
void _mr_clear_to_color(mr_mixel* _screen, mr_color* _colormap, mr_color _color) {

    int i;

    for (i = 0; i < WIDTH * HEIGHT; ++i) {
        _screen[i] = RENDERED_MIXELS[15];
        _colormap[i] = _color;
    }

}

// Writes a pixel into a bitmap.
void _mr_putpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_color _color) {

    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x >> 1;
    my = _y >> 1;
    offset = my * WIDTH + mx;

    // x1 * y1 + ( (x1 - 1) * y1) * 2+ ( x1 * (y1-1)) * 4 + ( (x1 - 1 ) * (y1-1)) * 8;
    // x1 * y1 + ((x1 - 1) * y1) * 2 + (x1 * (y1 - 1)) * 4 + ((x1 - 1) * (y1 - 1)) * 8;
    // x1y1 + (x1y1 - y1) * 2 + (x1y1 - x1) * 4 + (x1y1 - y1 + 1 - x1) * 8;
    // x1y1 + 2*x1y1 - 2*y1 + 4*x1y1 - 4*x1 + 8*x1y1 - 8*y1 + 8 - 8*x1;

    abcd = mr_mixel_bits(_x, _y);

    _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset])| abcd];
    _colormap[offset] = _color;

}

// Clear a pixel into a bitmap.
void _mr_clearpixel(mr_mixel* _screen, mr_position _x, mr_position _y) {

    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x >> 1;
    my = _y >> 1;
    offset = my * WIDTH + mx;

    abcd = mr_mixel_bits(_x, _y);

    _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset]) | abcd];

}

// Reads a pixel from a bitmap.
mr_color _mr_getpixel(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y) {

    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x >> 1;
    my = _y >> 1;
    abcd = mr_mixel_bits(_x, _y);
    offset = my * WIDTH + mx;

    if ( ( _screen[offset] & abcd ) == abcd) {
        return _colormap[offset];
    } else {
        return MR_COLOR_BLACK;
    }

}

// Draws a vertical line onto the bitmap.
void _mr_vline(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y1, mr_position _y2, mr_color _color) {

    mr_position y1 = _y1;
    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x >> 1;
    my = _y1 >> 1;
    offset = my * WIDTH + mx;

    if (y1 & 1) {
        abcd = mr_mixel_bits(_x, y1);
        _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset]) | abcd];
        _colormap[offset] = _color;
        offset += WIDTH;
        ++y1;
    }
    for (; y1 <= _y2; y1 += 2) {
        if (y1 + 1 <= _y2) {
            abcd = mr_mixel_bits(_x, y1) | mr_mixel_bits(_x, y1 + 1);
        } else {
            abcd = mr_mixel_bits(_x, y1);
        }
        _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset])|abcd];
        _colormap[offset] = _color;
        offset += WIDTH;
    }

}

// Draws a horizontal line onto the bitmap.
void _mr_hline(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _x2, mr_position _y, mr_color _color) {

    mr_position x1 = _x1;
    mr_position mx, my;
    mr_mixelbits abcd;
    int offset;

    mx = _x1 >> 1;
    my = _y >> 1;
    offset = my * WIDTH + mx;

    if (x1 & 1) {
        abcd = mr_mixel_bits(x1, _y);
        _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset]) | abcd];
        _colormap[offset] = _color;
        ++offset;
        ++x1;
    }
    for (; x1 <= _x2; x1 += 2) {
        if (x1 + 1 <= _x2) {
            abcd = mr_mixel_bits(x1, _y) | mr_mixel_bits(x1+1, _y);
        }
        else {
            abcd = mr_mixel_bits(x1, _y);
        }
        _screen[offset] = RENDERED_MIXELS[get_mixel_bits(_screen[offset]) | abcd];
        _colormap[offset] = _color;
        ++offset;
    }

}

// Draws a line onto the bitmap.
void _mr_line(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color) {
    int x1 = _x1, y1 = _y1;
    int x2 = _x2, y2 = _y2;
    int dx = iabs(x2 - x1), sx = _x1 < _x2 ? 1 : -1;
    int dy = -iabs(y2 - y1), sy = _y1 < _y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
    for (;;) {  /* loop */
        _mr_putpixel(_screen, _colormap, (mr_position)x1, (mr_position)y1, _color);
        if (x1 == _x2 && y1 == _y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy+e_y < 0 */
    }
}

// Draws a triangle.
void _mr_triangle(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_position _x3, mr_position _y3, mr_color _color) {
    _mr_line(_screen, _colormap, _x1, _y1, _x2, _y2, _color);
    _mr_line(_screen, _colormap, _x2, _y2, _x3, _y3, _color);
    _mr_line(_screen, _colormap, _x3, _y3, _x1, _y1, _color);
}

// Draws a polygon.
void _mr_polygon(mr_mixel* _screen, mr_color* _colormap, mr_position _vertices, mr_position _points[], mr_color _color) {
    int i = 0;
    for (i = 0; i < (_vertices-1); ++i) {
        _mr_line(_screen, _colormap, _points[i*2], _points[(i*2)+1], _points[(i * 2)+2], _points[(i * 2) + 3], _color);
    }
}

// Draws an outline rectangle.
void _mr_rect(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color) {
    _mr_hline(_screen, _colormap, _x1, _x2, _y1, _color);
    _mr_hline(_screen, _colormap, _x1, _x2, _y2, _color);
    _mr_vline(_screen, _colormap, _x1, _y1, _y2, _color);
    _mr_vline(_screen, _colormap, _x2, _y1, _y2, _color);
}

// Draws a filled rectangle.
void _mr_rectfill(mr_mixel* _screen, mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color) {
    mr_position w = (_x2 - _x1);
    mr_position h = (_y2 - _y1);
    if (w > h) {
        for (--h; h != 255; --h) {
            _mr_hline(_screen, _colormap,
                _x1, _x2,
                _y1 + h,
                _color);
        }
    } else {
        for (--w; w != 255; --w) {
            _mr_vline(_screen, _colormap,
                _x1 + w,
                _y1, _y2,
                _color);
        }

    }

}

// Draws a circle.
void _mr_circle(mr_mixel* _screen, mr_color* _colormap, mr_position _x, mr_position _y, mr_position _radius, mr_color _color) {
    int x = -_radius, y = 0, err = 2 - 2 * _radius; /* II. Quadrant */
    do {
        _mr_putpixel(_screen, _colormap, _x - x, _y + y, _color); /*   I. Quadrant */
        _mr_putpixel(_screen, _colormap, _x - y, _y - x, _color); /*  II. Quadrant */
        _mr_putpixel(_screen, _colormap, _x + x, _y - y, _color); /* III. Quadrant */
        _mr_putpixel(_screen, _colormap, _x + y, _y + x, _color); /*  IV. Quadrant */
        _radius = err;
        if (_radius <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (_radius > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

// Color a rectangle.
void _mr_colorfill(mr_color* _colormap, mr_position _x1, mr_position _y1, mr_position _x2, mr_position _y2, mr_color _color) {
    mr_position x1b = (_x1 >> 1), y1b = (_y1 >> 1);
    mr_position wb = (( _x2>>1) -x1b );
    mr_position hb = ((_y2 >> 1) - y1b );
    _colormap += (_y1 >> 1) * SCREEN_WIDTH + _x1 >> 1;

    for (; hb != 255; --hb) {
        mr_position w = wb;
        for (--w; w != 255; --w) {
            *_colormap = _color;
            ++_colormap;
        }
        _colormap += SCREEN_WIDTH - wb;
    }

}

#endif