#include "ezs_plot.h"
#include "ezs_io_fel.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>


void ezs_plot(cyg_uint32 data[], size_t length, cyg_fb_colour foreground,
              cyg_fb_colour background) {
    ezs_fb_clear(background);
    {
        size_t i = 0;
        size_t step = FB_WIDTH/ length;
        for (i = 1; i < length; ++i) {
            assert(data[i] < 256);
            cyg_ucount16 y = (((int) data[i]) * FB_HEIGHT) / 500;

            ezs_fb_vline(i*step, FB_HEIGHT - y, 4, foreground);
        }
    }
}

void ezs_plot_drawCoordSystem(void) {
    // y-axis
    ezs_fb_vline(1, 0, FB_HEIGHT, CYG_FB_DEFAULT_PALETTE_BLACK);
    ezs_fb_hline(0, 1, 3, CYG_FB_DEFAULT_PALETTE_BLACK);

    // x-axis
    ezs_fb_hline(0, FB_HEIGHT - 2, FB_WIDTH, CYG_FB_DEFAULT_PALETTE_BLACK);
    ezs_fb_vline(FB_WIDTH - 2, FB_HEIGHT - 3, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
}

void ezs_plot_markXaxis(short numberOfMarks) {

    int delta = (FB_WIDTH - 1) / (numberOfMarks + 1);
    int shift = delta + 1;

    int i;

    for (i = 0; i < numberOfMarks; ++i) {
        ezs_fb_vline(shift, FB_HEIGHT - 4, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
        shift += delta;
    }
}

void ezs_plot_markYaxis(short numberOfMarks) {

    int delta = (FB_HEIGHT - 1) / (numberOfMarks + 1);
    int shift = FB_HEIGHT - 1 - delta;

    int i;

    for (i = 0; i < numberOfMarks + 1; ++i) {
        ezs_fb_hline(0, shift, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
        shift -= delta;
    }
}

void ezs_plot_drawBarOnCoordSystem(float upperBound, float lowerBound,
                                 float inputArray[], int sizeOfArray) {

    // Clear FrameBuffer
    ezs_fb_clear(CYG_FB_DEFAULT_PALETTE_WHITE);

    // Flag for bufferOverflow/Underflow
    bool overflow;
    bool underflow;

    // Define max / min Area Y-Axis
    int maxValueY = FB_HEIGHT - ((FB_HEIGHT - 1) / 11) * 10 - 1; //=21
    int minValueY = FB_HEIGHT - 1; //=239
    int area = minValueY - maxValueY; //=218

    // Define half block width
    int halfBlockWidth = ((FB_WIDTH - 1) / (sizeOfArray + 1)) / 3;

    // Temp variables for scaling
    assert(upperBound > lowerBound);
    float scaleTo = upperBound - lowerBound;
    float offset = 0 - lowerBound;
    float scaledValue;
    int scaledValueInt;

    // Bar drawing loop
    int delta = (FB_WIDTH - 1) / (sizeOfArray + 1);
    int shift = delta + 1;
    int i;

    for (i = 0; i < sizeOfArray; ++i) {

        scaledValue = (inputArray[i] + offset) / scaleTo;

        // Overflow check!
        if (scaledValue > 1.0) {
            scaledValue = 1.0;
            overflow = true;
        }
        else {
            overflow = false;
        }

        // Underflow check!
        if (scaledValue < 0.0) {
            scaledValue = 0.0;
            underflow = true;
        }
        else {
            underflow = false;
        }

        scaledValueInt = scaledValue * area;

        if (!overflow) {
            if (!underflow) {
                ezs_fb_fill_block(shift - halfBlockWidth, minValueY - scaledValueInt,
                                  halfBlockWidth * 2, scaledValueInt, CYG_FB_DEFAULT_PALETTE_BLUE);
            }
            else {
                ezs_fb_fill_block(shift - halfBlockWidth, minValueY, halfBlockWidth * 2, 2,
                                  CYG_FB_DEFAULT_PALETTE_YELLOW);
            }
        }
        else {
            ezs_fb_fill_block(shift - halfBlockWidth, minValueY - scaledValueInt,
                              halfBlockWidth * 2, scaledValueInt, CYG_FB_DEFAULT_PALETTE_RED);
        }

        shift += delta;
    }

    // Draw coord system
    ezs_plot_drawCoordSystem();
    ezs_plot_markYaxis(10);
    ezs_plot_markXaxis(sizeOfArray);

    /*!
    *  \brief Fill block.
    *  \param x X-Axis start position.
    *  \param y Y-Axis start position.
    *  \param width Width of the box.
    *  \param height Height of the box.
    *  \param color Color.
    */
//void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,cyg_ucount16 height,cyg_fb_colour color);


}
