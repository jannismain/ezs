#ifndef __EZS_IO_FEL_H__
#define __EZS_IO_FEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FB_BLACK 0
#define FB_WHITE ((16*4)-1)

#include <stdio.h>
#include <cyg/infra/cyg_type.h>

#define FRAMEBUF fb0
#define FB_WIDTH    CYG_FB_WIDTH(FRAMEBUF)
#define FB_HEIGHT   CYG_FB_HEIGHT(FRAMEBUF)

#define CYG_FB_HEIGHT(a) 600
#define CYG_FB_WIDTH(a)  800

#define CYG_FB_DEFAULT_PALETTE_BLACK        0x00
#define CYG_FB_DEFAULT_PALETTE_BLUE         0x01
#define CYG_FB_DEFAULT_PALETTE_GREEN        0x02
#define CYG_FB_DEFAULT_PALETTE_CYAN         0x03
#define CYG_FB_DEFAULT_PALETTE_RED          0x04
#define CYG_FB_DEFAULT_PALETTE_MAGENTA      0x05
#define CYG_FB_DEFAULT_PALETTE_BROWN        0x06
#define CYG_FB_DEFAULT_PALETTE_LIGHTGREY    0x07
#define CYG_FB_DEFAULT_PALETTE_LIGHTGRAY    0x07
#define CYG_FB_DEFAULT_PALETTE_DARKGREY     0x08
#define CYG_FB_DEFAULT_PALETTE_DARKGRAY     0x08
#define CYG_FB_DEFAULT_PALETTE_LIGHTBLUE    0x09
#define CYG_FB_DEFAULT_PALETTE_LIGHTGREEN   0x0A
#define CYG_FB_DEFAULT_PALETTE_LIGHTCYAN    0x0B
#define CYG_FB_DEFAULT_PALETTE_LIGHTRED     0x0C
#define CYG_FB_DEFAULT_PALETTE_LIGHTMAGENTA 0x0D
#define CYG_FB_DEFAULT_PALETTE_YELLOW       0x0E
#define CYG_FB_DEFAULT_PALETTE_WHITE        0x0F

typedef unsigned char cyg_fb_colour;

void ezs_fel_serial_init(void);

void ezs_printf(const char* fmt, ...)
#ifdef __GNUC__
          __attribute__ (( format( printf, 1, 2 ) ))
#endif
;

void ezs_log_file(const char* file, const char* fmt, ...)
#ifdef __GNUC__
          __attribute__ (( format( printf, 2, 3 ) ))
#endif
;


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
void ezs_fb_print_string(char* c, cyg_ucount16 x, cyg_ucount16 y,
                             cyg_fb_colour color);
///////////////////// TERMINAL FOO //////////////////////////

#ifdef __cplusplus
}
#endif
#endif // __EZS_IO_FEL_H__
