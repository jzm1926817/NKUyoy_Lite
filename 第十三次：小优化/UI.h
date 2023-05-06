#pragma once
#include"common.h"

//按钮结构体
struct Button {
	int x;
	int y;
	int w;
	int h;
	bool flag;
	char text[32];
};

struct Button* CreateButton(int x, int y, int w, int h, const char* text);	//创建按钮
bool UpdataButton(ExMessage msg, struct Button* but);						//判断按钮是否被点击
void DrawButton(const struct Button* but);									//绘制按钮

