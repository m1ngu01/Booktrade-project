
#pragma warning(disable : 4996) // 에러 C4996을 무시

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

int cakr(char* str, int* x, int* y);
int inputWithClick(char* str, INPUT_RECORD rec);

void inputMouseSetting() {
	DWORD mode;

	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode); // 현재 콘솔 입력 모드를 가져온다.
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode | ENABLE_MOUSE_INPUT); // 마우스 입력을 허용한다.
}

int cakr(char* str, int* x, int* y)
{
	INPUT_RECORD rec;
	DWORD mode;


	while (TRUE)
	{
		inputMouseSetting();
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &mode);

		if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown)
		{
			if (inputWithClick(str, rec))
			{
				return 1;
			}
			break;
		}

		if (rec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
			int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴

			*x = mouse_x; //x값을 넘김
			*y = mouse_y; //y값을 넘김
			break;
		}
	}
	return 0;
}

int inputWithClick(char* str, INPUT_RECORD rec)
{
	// detect enter
	if (rec.Event.KeyEvent.uChar.AsciiChar == 13)
	{
		strcpy(str, "");
		return 1;
	}
	// backspace
	if (rec.Event.KeyEvent.uChar.AsciiChar == 8)
	{
		int len = strlen(str);
		if (len > 0)
		{
			str[len - 1] = '\0';
			rec.Event.KeyEvent.uChar.AsciiChar = 0;

			printf("\b");
			printf(" ");
			printf("\b");

		}
		return 0;
	}
	printf("%c", rec.Event.KeyEvent.uChar.AsciiChar);
	strncat(str, &rec.Event.KeyEvent.uChar.AsciiChar, 1);

	return 0;

}