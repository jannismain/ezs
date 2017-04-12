#ifndef EZS_PLOT_H
#define EZS_PLOT_H
#include <cyg/infra/cyg_type.h>
#include "ezs_io_fel.h"
/**
 * \defgroup plot Graph plotting
 */


/**
 * \file ezs_plot.h
 * \ingroup plot
 * \brief Helper function to plot a <b>time domain signal</b>
 */

/**
 * \brief Plots a <b>time signal</b> of given length to framebuffer
 *
 * @param data  A pointer of data values
 * @param length The size of the data array
 * @param foreground The foreground color: e.g. FB_WHITE
 * @param background The background color: e.g. FB_BLACK
 */
void ezs_plot(cyg_uint32 data[], size_t length, cyg_fb_colour foreground,
              cyg_fb_colour background);

#endif // EZS_PLOT_H
