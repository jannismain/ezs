#include "ezs_io_fel.h"

#define EZSMAGIC "5c63"
#define EZSINITMAGIC "1a44"


void ezs_fel_serial_init() {
	printf("%s:INIT\n", EZSINITMAGIC);
}


void ezs_printf(const char *fmt, ...) {
	printf("%s:MSG\n", EZSMAGIC);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n%s:ENDMSG:PTY\n", EZSMAGIC);
}

void ezs_log_file(const char *file, const char *fmt, ...) {
	printf("%s:MSG\n", EZSMAGIC);
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n%s:ENDMSG:FILE:%s\n", EZSMAGIC, file);
}


/*!
 *  \brief Initialize the framebuffer. Call this function exactly once before
 *  using any of the other framebuffer related functions.
 */
void ezs_fb_init(void) {
	printf("%s:RPC:GUI:init()\n", EZSMAGIC);
}


/*!
 *  \brief Fill block.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param width Width of the box.
 *  \param height Height of the box.
 *  \param color Color.
 */
void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,
                       cyg_ucount16 height, cyg_fb_colour color) {
	printf("%s:RPC:GUI:fill_block(%d, %d, %d, %d, %d)\n", EZSMAGIC, x, y, width, height, color);
}

/*!
 *  \brief Draw horizontal line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_hline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color) {
	printf("%s:RPC:GUI:hline(%d, %d, %d, %d)\n", EZSMAGIC, x, y, len, color);
}

/*!
 *  \brief Draw vertical line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_vline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color) {
	printf("%s:RPC:GUI:vline(%d, %d, %d, %d)\n", EZSMAGIC, x, y, len, color);
}

/*!
 *  \brief Clear the framebuffer.
 *  \param color Color.
 */
void ezs_fb_clear(cyg_fb_colour color) {
	printf("%s:RPC:GUI:clear(%d)\n", EZSMAGIC, color);
}

/*  and so on.. see: http://ecos.sourceware.org/docs-latest/ref/framebuf-drawing.html   */

/*!
 * \brief Prints a character at a given position.
 * \param c Character to print.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_char(int c, cyg_ucount16 x, cyg_ucount16 y,
                       cyg_fb_colour color) {
	printf("%s:RPC:GUI:print_char(\"%c\", %d, %d, %d)\n", EZSMAGIC, c, x, y, color);
}

/*!
 * \brief Prints a string at a given position.
 * \param c Pointer to null terminated string.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_string(char* c, cyg_ucount16 x, cyg_ucount16 y,
                             cyg_fb_colour color) {
	// TODO: escaping for newlines here...
	printf("%s:RPC:GUI:print_string_cur(\"%s\", %d, %d, %d)\n", EZSMAGIC, c, x, y, color);
}

///////////////////// TERMINAL FOO //////////////////////////
