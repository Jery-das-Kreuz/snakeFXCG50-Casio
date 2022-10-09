#include <fxcg/display.h>
#include <string.h>
#include "publicVariables.h"


void drawRect(int x, int y, int width, int height, int color) {
    unsigned short *p = GetVRAMAddress();

    for (int i = 0; i < height; i++) {
        unsigned short *p1 = p+(LCD_WIDTH_PX*(y+i)+x);
        for (int i = 0; i < width; i++) {
            *p1++ = color;
        }
    }
}



void copyPict(const color_t* data, int x, int y, int width, int height) {
	color_t* VRAM = (color_t*)0xAC000000; //For cg- 50
	VRAM += (LCD_WIDTH_PX*y + x);
	short j;
	for(j=y; j<y+height; j++) {
		memcpy(VRAM,data,2*width);
		VRAM +=LCD_WIDTH_PX;
		data += width;
	}
}