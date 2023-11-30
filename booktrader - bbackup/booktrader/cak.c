
#pragma warning(disable : 4996) // ���� C4996�� ����

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

int cakr(char* str, int* x, int* y);
int inputWithClick(char* str, INPUT_RECORD rec);

void inputMouseSetting() {
	DWORD mode;

	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode); // ���� �ܼ� �Է� ��带 �����´�.
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode | ENABLE_MOUSE_INPUT); // ���콺 �Է��� ����Ѵ�.
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
			int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X�� �޾ƿ�
			int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y�� �޾ƿ�

			*x = mouse_x; //x���� �ѱ�
			*y = mouse_y; //y���� �ѱ�
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