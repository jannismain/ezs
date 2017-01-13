#ifndef EZS_PLOT_PDS_H
#define EZS_PLOT_PDS_H
#include "ezs_io_fel.h"

/**
 * \file ezs_plot_pds.h
 * \ingroup plot
 * \brief plot an previously calculated <b>power density spectrum</b>
 *
 * @param spectrum The caculated pds data
 * @param length The length of the pds array
 * @param foreground The framebuffer foreground color
 * @param background The framebuffer background color
 */
void ezs_plot_pds(float spectrum[], int length, cyg_fb_colour foreground,
                  cyg_fb_colour background);

#endif // EZS_PLOT_PDS_H
