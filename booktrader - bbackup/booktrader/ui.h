#ifndef ui_h
#define ui_h

#include <stdio.h>
#include <windows.h>
#include <string.h>


void systemafter();
void gotoxy(int x, int y);
void createBox(int posx, int posy, int lenx, int leny, const char* text);
void createOutline();
void uiDefalutSetting();
int checkBoxArea(int x, int y, int lenx, int leny, int cx, int cy);

#endif