#include"UI.h"

struct Button* CreateButton(int x, int y, int w, int h, const char* text)
{
	Button* pNewBut = (Button*)malloc(sizeof(Button));
	memset(pNewBut, 0, sizeof(Button));
	pNewBut->x = x;
	pNewBut->y = y;
	pNewBut->w = w;
	pNewBut->h = h;
	strcpy(pNewBut->text, text);
	return pNewBut;
}

bool UpdataButton(ExMessage msg, struct Button* button)
{
	if (msg.x > button->x && msg.x<button->x + button->w && msg.y>button->y && msg.y < button->y + button->h) {
		if (msg.message == WM_LBUTTONDOWN) {
			return true;
		}
		button->flag = true;
	}
	else {
		button->flag = false;
	}
	return false;
}

void DrawButton(const struct Button* button) {
	setlinecolor(BLACK);
	setlinestyle(0, 1);
	if (button->flag) {
		setfillcolor(COLOR2);
	}
	else {
		setfillcolor(COLOR1);
	}
	settextcolor(LIGHTCYAN);
	settextstyle(button->h * 2 / 3, 0, "ºÚÌå");
	fillrectangle(button->x, button->y, button->x + button->w, button->y + button->h);
	outtextxy(button->x + (button->w - textwidth(button->text)) / 2, button->y + button->h / 6, button->text);
	
}

