#ifndef __EZS_IO_FEL_H__
#define __EZS_IO_FEL_H__

#include <stdio.h>
#include <cyg/infra/cyg_type.h>

typedef unsigned char cyg_fb_colour;

#define EZSMAGIC "5c630ef818d486afb997f307f06a80bf"

#define ezs_printf(fmt, args...) \
	/* TODO: lock scheduler */ \
	printf( EZSMAGIC ":EZZ:MSG\n" fmt "\n" EZSMAGIC ":EZS:ENDMSG:PTY\n", args);

#define ezs_log_file(file, fmt, args...) \
	/* TODO: lock scheduler */ \
	printf( EZSMAGIC ":EZZ:MSG\n" fmt "\n" EZSMAGIC ":EZS:ENDMSG:FILE:\n" file, args);

/*!
 *  \brief Initialize the framebuffer. Call this function exactly once before
 *  using any of the other framebuffer related functions.
 */
void ezs_fb_init(void);


/*!
 *  \brief Fill block.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param width Width of the box.
 *  \param height Height of the box.
 *  \param color Color.
 */
void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,
                       cyg_ucount16 height, cyg_fb_colour color);

/*!
 *  \brief Draw horizontal line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_hline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color);

/*!
 *  \brief Draw vertical line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_vline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color);

/*!
 *  \brief Clear the framebuffer.
 *  \param color Color.
 */
void ezs_fb_clear(cyg_fb_colour color);

/*  and so on.. see: http://ecos.sourceware.org/docs-latest/ref/framebuf-drawing.html   */

/*!
 * \brief Prints a character at a given position.
 * \param c Character to print.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_char(int c, cyg_ucount16 x, cyg_ucount16 y,
                       cyg_fb_colour color);

/*!
 * \brief Prints a string at a given position.
 * \param c Pointer to null terminated string.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_string_cur(char* c, cyg_ucount16 x, cyg_ucount16 y,
                             cyg_fb_colour color);
///////////////////// TERMINAL FOO //////////////////////////

#endif // __EZS_IO_FEL_H__
