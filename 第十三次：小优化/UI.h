#pragma once
#include"common.h"

//��ť�ṹ��
struct Button {
	int x;
	int y;
	int w;
	int h;
	bool flag;
	char text[32];
};

struct Button* CreateButton(int x, int y, int w, int h, const char* text);	//������ť
bool UpdataButton(ExMessage msg, struct Button* but);						//�жϰ�ť�Ƿ񱻵��
void DrawButton(const struct Button* but);									//���ư�ť

