#include "ezs_plot_pds.h"

#include "ezs_io_fel.h"

//#include <cyg/infra/cyg_type.h>
//#include <cyg/io/framebuf.h>

#include <assert.h>
#include <stddef.h>

static void plot_spectrum(float* spectrum, int length,
                          cyg_ucount16 widget_x, cyg_ucount16 widget_y,
                          cyg_ucount16 widget_width, cyg_ucount16 widget_height,
                          cyg_fb_colour widget_color) {
    assert(widget_x + widget_width <= FB_WIDTH);
    assert(widget_y + widget_height <= FB_HEIGHT);

    int bar_width = widget_width / length;
    int bar_unit = widget_height / 140;
    {
        int i;

        for (i = 0; i < length; ++i) {
            int x = i * bar_width + widget_x;
            assert(x < widget_x + widget_width);
            assert(x >= widget_x);

            int width = bar_width;
            assert(width >= 0);

            int height = bar_unit * (spectrum[i] + 140.0f);
            assert(height >= 0);

            int y = widget_y + widget_height - height;
            assert(y <= widget_y + widget_height);
            assert(y >= widget_y);


            ezs_fb_fill_block(x, y, width, height, widget_color);
        }
    }
}

void ezs_plot_pds(float spectrum[], int length, cyg_fb_colour foreground,
                  cyg_fb_colour background) {
    assert(length <= FB_WIDTH);

    ezs_fb_clear(background);
    plot_spectrum(spectrum, length, 0, 0, FB_WIDTH, FB_HEIGHT, foreground);
}

