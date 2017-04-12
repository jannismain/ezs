#include "ezs_fb.h"

typedef struct {
	int x;
	int y;
} cursor_t;

static cursor_t cursor; 
void ezs_fb_init(void)
{
    CYG_FB_ON(FRAMEBUF);
    CYG_FB_FILL_BLOCK(FRAMEBUF, 0, 0, FB_WIDTH, FB_HEIGHT, 0);
	cursor.x = 0;
	cursor.y = 0;
}


void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,cyg_ucount16 height,cyg_fb_colour color)
{
	CYG_FB_FILL_BLOCK(FRAMEBUF, x, y, width, height, color);
}


void ezs_fb_hline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour color)
{
	CYG_FB_WRITE_HLINE(FRAMEBUF, x, y, len, color);
}


void ezs_fb_vline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len, cyg_fb_colour color)
{

	CYG_FB_WRITE_VLINE(FRAMEBUF, x, y, len, color);
}

void ezs_fb_clear(cyg_fb_colour color)
{
	ezs_fb_fill_block(0, 0, FB_WIDTH, FB_HEIGHT, color);
}


void ezs_fb_print_char( int ch, cyg_ucount16 x, cyg_ucount16 y, cyg_fb_colour color){
	int i, j;
	for (i = ch*8; i < (ch*8)+8; i++){
		unsigned char row = g_8x8_font[i];
		for (j = 0; j < 8; j++){
			if((row & (0x80 >> j)) != 0){
				ezs_fb_fill_block(x+j, y+(i-ch*8), 1, 1, color);
			}
		}
	}
	
}

void ezs_fb_print_string(char*c,  cyg_fb_colour color){
	ezs_fb_print_string_cur(c, cursor.x, cursor.y, color);
}

void ezs_fb_print_string_cur(char* c, cyg_ucount16 x, cyg_ucount16 y, cyg_fb_colour color){
	char* ch = c;
	
	while(*ch != '\0'){
		if(*ch == '\n'){
			ezs_fb_newline();
		}else{
			ezs_fb_print_char((*ch)+0, x , y, color);
			x = x+8;
			if( x >= FB_WIDTH - 8){
				y += 8;
				x = 0;
			}
		}
		ch++;
	}
	cursor.x = x;
	cursor.y = y;
}

void ezs_fb_newline(void){
	cursor.x = 0;
	cursor.y += 8;
}

void ezs_fb_setpos(cyg_ucount16 x, cyg_ucount16 y){
	cursor.x = x;
	cursor.y = y;
}

