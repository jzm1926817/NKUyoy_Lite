#include "common.h"



void _InitWin()
{
	initgraph(WIN_W, WIN_H, EX_SHOWCONSOLE);
	setbkcolor(COLOR3);
	cleardevice();
	setbkmode(0);
}