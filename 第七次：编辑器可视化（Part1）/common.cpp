#include "common.h"

COLORREF _Colors[15] = {
	RGB(128, 128, 128),
	RGB(255, 0, 0),
	RGB(0, 255, 0),
	RGB(0, 0, 255),
	RGB(255, 255, 0),
	RGB(255, 0, 255),
	RGB(0, 255, 255),
	RGB(128, 0, 0),
	RGB(0, 128, 0),
	RGB(0, 0, 128),
	RGB(0,0,0)
};

void _InitWin()
{
	initgraph(WIN_W, WIN_H, EX_SHOWCONSOLE);
	setbkcolor(COLOR3);
	cleardevice();
	setbkmode(0);
}
