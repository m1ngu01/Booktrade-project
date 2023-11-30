#pragma warning(disable : 4996) // ���� C4996�� ����

#include <windows.h>

#include "cak.h"
#include "file.h"

#define outbx 140
#define outby 43
// ���� ����ϴ� �ؽ�Ʈ �ڽ� ������
#define defx 30
#define defy 3

// �� �ڵ�
#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6
#define LIGHTGRAY     7
#define DARKGRAY      8
#define LIGHTBLUE     9
#define LIGHTGREEN    10
#define LIGHTCYAN     11
#define LIGHTRED      12
#define LIGHTMAGENTA  13
#define YELLOW        14
#define WHITE         15

int x = 0;
int y = 0;
char str[8192];

void loginStream();
void buy_section(LOGIN user);



/* ���� �Լ� */

// system �Լ� ��� �� ����ؾ��� �Լ�
void systemafter()
{
	DWORD CIN;
	DWORD mode;
	CIN = GetStdHandle(STD_INPUT_HANDLE); //���콺 ��Ȱ��ȭ
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
}
// goto �Լ�
void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// UI �۾� �� ����
void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// UI ��ȭâ ����� �� �߰�
void createBoxc(int posx, int posy, int lenx, int leny, const char* text, int color)
{
	int tmpy;
	setColor(color);

	// top
	gotoxy(posx, posy);
	printf("��");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("��");
	}
	printf("��");

	// body
	gotoxy(posx, posy + 1);
	tmpy = posy + 1;
	for (int j = 0; j < leny; j++)
	{
		printf("��");
		for (int i = 0; i < lenx - 2; i++)
		{
			printf(" ");
		}
		printf("��");
		gotoxy(posx, ++tmpy);
	}

	//bottom
	gotoxy(posx, tmpy);
	printf("��");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("��");
	}
	printf("��");

	//text

	gotoxy(posx + (lenx / 2) - (strlen(text) / 2), posy + leny / 2 + 1);
	printf("%s", text);
	setColor(WHITE);

}
// UI ��ȭâ �����
void createBox(int posx, int posy, int lenx, int leny, const char* text)
{
	int tmpy;

	// top
	gotoxy(posx, posy);
	printf("��");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("��");
	}
	printf("��");

	// body
	gotoxy(posx, posy + 1);
	tmpy = posy + 1;
	for (int j = 0; j < leny; j++)
	{
		printf("��");
		for (int i = 0; i < lenx - 2; i++)
		{
			printf(" ");
		}
		printf("��");
		gotoxy(posx, ++tmpy);
	}

	//bottom
	gotoxy(posx, tmpy);
	printf("��");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("��");
	}
	printf("��");

	//text

	gotoxy(posx + (lenx / 2) - (strlen(text) / 2), posy + leny / 2 + 1);
	printf("%s", text);

}
// UI �⺻ Ʋ
void createOutline()
{
	int height = 45;
	int line = 180;
	int tmpy;

	gotoxy(2, 2);

	printf("��");
	for (int i = 0; i < line - 2; i++)
	{
		printf("��");
	}
	printf("��");

	gotoxy(2, 3);
	tmpy = 3;
	for (int j = 0; j < height; j++)
	{
		if (j == 39) {
			printf("��");
			for (int i = 0; i < line - 2; i++)
			{
				printf("��");
			}
			printf("��");
		}
		else {
			printf("��");
			gotoxy(181, tmpy);
			printf("��");
			gotoxy(2, tmpy);
		}
		gotoxy(2, ++tmpy);
	}
	
	printf("��");
	for (int i = 0; i < line - 2; i++)
	{
		printf("��");
	}
	printf("��");
}
// UI �ر׸�
void uiDefalutSetting()
{
	system("cls");
	createOutline();
	createBox(140, 43, 30, 3, "������");
}
// ��ȭâ ���� Ȯ��
int checkBoxArea(int x, int y, int lenx, int leny, int cx, int cy)
{

	int tmpx = x + lenx;
	int tmpy = y + leny;

	if ( (cx > x && cx < tmpx ) && (cy > y && cy < tmpy) )
	{
		return 1;
	}
	return 0;
}
// ��ȭâ ���� Ȯ�� �Ű����� ��ư ��ǥ �迭
int check_box_area_func(int bpos[][4], int cx, int cy, int size)
{
	for (int i = 0; i < size; i++)
	{

		int tmpx = bpos[i][2] + bpos[i][0];
		int tmpy = bpos[i][3] + bpos[i][1];

		if ((cx > bpos[i][0] && cx < tmpx) && (cy > bpos[i][1] && cy <= tmpy))
		{
			return i + 1;
		}
	}
	return 0;
}
// ���� �Ǹ� å ��� Ŭ�� ������ ���� �Լ�
static int check_pick_num_pos(int x, int y)
{
	int listcount = 10;
	for (int i = 0; i < listcount; i++)
	{
		if (checkBoxArea(5, 8 + i * 3, 170, 3, x, y))
		{
			return i + 1;
		}
	}
	return 0;
}
// ��� Ŭ�� ������ ���� �Լ� <trade section>
static int check_pick_num_pos_t(int x, int y)
{
	int listcount = 10;
	for (int i = 0; i < listcount; i++)
	{
		if (checkBoxArea(5, 8 + i * 3, 130, 3, x, y))
		{
			return i + 1;
		}
	}
	return 0;
}
// ���� �Ǹ� å ��� ��� <sell_section>
void print_book_list(BOOK book[], int numLines, int page)
{

	int listcount = 10;
	int startcount = (page - 1) * listcount;
	char str[20];



	int boxpos[5][4] =
	{
		{5, 5, 10, 1},
		{15 ,5, 30, 1},
		{45 ,5, 60, 1},
		{105, 5, 60, 1},
		{165, 5, 10, 1}
	};

	createBox(boxpos[0][0], boxpos[0][1], boxpos[0][2], boxpos[0][3], "num");
	createBox(boxpos[1][0], boxpos[1][1], boxpos[1][2], boxpos[1][3], "�̸�");
	createBox(boxpos[2][0], boxpos[2][1], boxpos[2][2], boxpos[2][3], "����");
	createBox(boxpos[3][0], boxpos[3][1], boxpos[3][2], boxpos[3][3], "����");
	createBox(boxpos[4][0], boxpos[4][1], boxpos[4][2], boxpos[4][3], "����");

	for (int i = 0; i < listcount && startcount < numLines; i++, startcount++)
	{
		sprintf(str, "%d", startcount + 1);
		createBox(boxpos[0][0], boxpos[0][1] + 3 * i + 3, boxpos[0][2], boxpos[0][3], str);
		createBox(boxpos[1][0], boxpos[1][1] + 3 * i + 3, boxpos[1][2], boxpos[1][3], book[startcount].id);
		createBox(boxpos[2][0], boxpos[2][1] + 3 * i + 3, boxpos[2][2], boxpos[2][3], book[startcount].title);
		createBox(boxpos[3][0], boxpos[3][1] + 3 * i + 3, boxpos[3][2], boxpos[3][3], book[startcount].author);
		sprintf(str, "%d", book[startcount].cost);
		createBox(boxpos[4][0], boxpos[4][1] + 3 * i + 3, boxpos[4][2], boxpos[4][3], str);
	}
}
// ���� �Ǹ� å ��� ��� <trade_section>
void print_book_list_t(TRADE t[], int numLines, int page, const char *userid)
{
	int listcount = 10;
	int startcount = (page - 1) * listcount;
	char str[20];

	int boxpos[5][4] =
	{
		{5, 4, 10, 1},
		{15 ,4, 30, 1},
		{45 ,4, 60, 1},
		{105, 4, 10, 1},
		{115, 4, 14, 1}
	};

	createBox(boxpos[0][0], boxpos[0][1], boxpos[0][2], boxpos[0][3], "num");
	createBox(boxpos[1][0], boxpos[1][1], boxpos[1][2], boxpos[1][3], "�Ǹ���");
	createBox(boxpos[2][0], boxpos[2][1], boxpos[2][2], boxpos[2][3], "����");
	createBox(boxpos[3][0], boxpos[3][1], boxpos[3][2], boxpos[3][3], "����");
	createBox(boxpos[4][0], boxpos[4][1], boxpos[4][2], boxpos[4][3], "�ŷ� ����");

	for (int i = 0; i < listcount && startcount < numLines; i++, startcount++)
	{
		sprintf(str, "%d", startcount + 1);
		createBox(boxpos[0][0], boxpos[0][1] + 3 * i + 4, boxpos[0][2], boxpos[0][3], str);
		createBox(boxpos[1][0], boxpos[1][1] + 3 * i + 4, boxpos[1][2], boxpos[1][3], t[startcount].ids);
		createBox(boxpos[2][0], boxpos[2][1] + 3 * i + 4, boxpos[2][2], boxpos[2][3], t[startcount].title);
		sprintf(str, "%d", t[startcount].cost);
		createBox(boxpos[3][0], boxpos[3][1] + 3 * i + 4, boxpos[3][2], boxpos[3][3], str);
		// �Ǹ��ڰ� user �϶� ���
		strcpy(str, (t[startcount].ss == 0) ? "x" : "o");
		if (!strcmp(t[startcount].ids ,userid))
		{
			strcpy(str, (t[startcount].ss == 0) ? "��" : "o");
		}
		createBox(boxpos[4][0], boxpos[4][1] + 3 * i + 4, boxpos[4][2], boxpos[4][3], str);
	}
}
// ���� �Ǹ� �ڽ� ����
static void create_ui_buy_section(BOOK book[], int bpos[][4], char btext[][20], int numLines, int page, int size)
{
	system("cls");
	uiDefalutSetting();

	for (int i = 0; i < size; i++)
	{
		createBox(bpos[i][0], bpos[i][1], bpos[i][2], bpos[i][3], btext[i]);
	}
	print_book_list(book, numLines, page);
}
// �迭 ���� ���� ��ü�� ����
void create_box_array(int pos[][4], char text[][20], int size)
{
	for( int i = 0; i < size; i++)
	{
		createBox(pos[i][0], pos[i][1], pos[i][2], pos[i][3], text[i]);
	}
}

/*														SORT SECTION */
/* book arr swap*/
void swapB(BOOK* a, BOOK* b) {
	BOOK temp;
	memcpy(&temp, a, sizeof(BOOK));
	memcpy(a, b, sizeof(BOOK));
	memcpy(b, &temp, sizeof(BOOK));
}
/* trade arr swap*/
void swapT(TRADE* a, TRADE* b) {
	TRADE temp;
	memcpy(&temp, a, sizeof(TRADE));
	memcpy(a, b, sizeof(TRADE));
	memcpy(b, &temp, sizeof(TRADE));
}
/* book bubble sort */
void bubbleSortBook(BOOK arr[], int n, int type) {
	switch (type)
	{
	case 1:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (strcmp(arr[j].id, arr[j + 1].id) > 0) {
					swapB(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (strcmp(arr[j].title, arr[j + 1].title) > 0) {
					swapB(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 3:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (strcmp(arr[j].author, arr[j + 1].author) > 0) {
					swapB(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 4:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (arr[j].cost > arr[j + 1].cost) {
					swapB(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	default:
		break;
	}
	
}
/* print book array */
void printBookArray(BOOK arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("[%d]ID: %s, Title: %s, Author: %s, Cost: %d\n",
			i, arr[i].id, arr[i].title, arr[i].author, arr[i].cost);
	}
	printf("\n");
}
/* trade bubble sort */
void bubbleSortTrade(TRADE arr[], int n, int type) {
	/*
	* 1. ids
	* 2. title
	* 3. author
	* 4. ss
	*/
	switch (type) {
	case 1:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (strcmp(arr[j].ids, arr[j + 1].ids) > 0) {
					swapT(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (strcmp(arr[j].title, arr[j + 1].title) > 0) {
					swapT(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 3:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (arr[j].cost > arr[j + 1].cost) {
					swapT(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	case 4:
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				if (arr[j].ss < arr[j + 1].ss) {
					swapT(&arr[j], &arr[j + 1]);
				}
			}
		}
		break;
	default:
		printf("out number");
		break;
	}
}
/* print trade array */
void printTradeArray(TRADE arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("IDB: %s, IDS: %s, Title: %s, Author: %s, Cost: %d, BS: %d, SS: %d\n",
			arr[i].idb, arr[i].ids, arr[i].title, arr[i].author, arr[i].cost, arr[i].bs, arr[i].ss);
	}
	printf("\n");
}


/*													 MAIN UI SECTION */
// control book.txt
void adminmode_book_add()
{
	system("cls");
	createOutline();
	int bpos[][4] =
	{
		{outbx, outby, defx, defy},

		{10,5,defx,defy},
		{50,5,defx,defy},

		{10,10,defx,defy},
		{50,10,defx,defy},

		{10,15,defx,defy},
		{50,15,defx,defy},

		{10,20,defx,defy},
		{50,20,defx,defy},

		{10,43,defx,defy}

	};
	char btext[][20] =
	{
		" ������ ",
		"���̵�",
		"",
		"����",
		"",
		"����",
		"",
		"����",
		"",
		"�߰�"
		
	};

	char tostr[128] = "";
	int mode = 0;
	int modet = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	/*********************/
	BOOK tmp = { "", "", "", "" };

	create_box_array(bpos, btext, size);

	while (1)
	{
		if (cakr(str, &x, &y))
		{
			modet++;
		}
		else
		{
			mode = check_box_area_func(bpos, x, y, size);
			strcpy(tostr, str);
		}

		switch (mode)
		{
		case 1:
			return;
		case 3:
			strcpy(str, "");
			modet = 1;
			createBox(bpos[2][0], bpos[2][1], bpos[2][2], bpos[2][3], btext[2]);
			gotoxy(53, 7);
			break;
		case 5:
			strcpy(str, "");
			modet = 2;
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], btext[4]);
			gotoxy(53, 12);
			break;
		case 7:
			strcpy(str, "");
			modet = 3;
			createBox(bpos[6][0], bpos[6][1], bpos[6][2], bpos[6][3], btext[6]);
			gotoxy(53, 17);
			break;
		case 9:
			strcpy(str, "");
			modet = 4;
			createBox(bpos[8][0], bpos[8][1], bpos[8][2], bpos[8][3], btext[8]);
			gotoxy(53, 22);
			break;
		case 10:
			/*********************/
			if (strcmp(tmp.id, "") != 0 && strcmp(tmp.title, "") != 0 && strcmp(tmp.author, "") != 0 && tmp.cost > 0) {
				insert_book_turple("book.txt", tmp);
			}
			return;
		}

		/*********************/
		switch (modet)
		{
		case 1:
			strcpy(tmp.id, tostr);
			break;
		case 2:
			strcpy(tmp.title, tostr);
			break;
		case 3:
			strcpy(tmp.author, tostr);
			break;
		case 4:

			tmp.cost = atoi(tostr);
			break;
		}
		
		x = y = 0;
	}

}
void adminmode_book_remove(BOOK arr[], int *arrsize, BOOK bdata)
{
	system("cls");
	createOutline();
	int bpos[][4] =
	{
		{outbx, outby, defx, defy},

		{10,5,defx,defy},
		{50,5,defx,defy},

		{10,10,defx,defy},
		{50,10,defx,defy},

		{10,15,defx,defy},
		{50,15,defx,defy},

		{10,20,defx,defy},
		{50,20,defx,defy},

		{10,43,defx,defy},
		{40,43,defx,defy}

	};
	char btext[][20] =
	{
		" ������ ",

		"���̵�",
		"",

		"����",
		"",

		"����",
		"",

		"����",
		"",

		"����",
		"����"

	};

	

	char tostr[128] = "";
	int mode = 0;
	int modet = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	BOOK tmp = { "", "", "", 0 };
	int arrtmpsize = *arrsize;
	// +â
	/*********************/
	createBox(100, 5, 60, 30, "");
	gotoxy(105, 10);
	printf("���õ� �������Դϴ�.");
	gotoxy(105, 15);
	printf("id : %s", bdata.id);
	gotoxy(105, 20);
	printf("title : %s", bdata.title);
	gotoxy(105, 25);
	printf("author : %s", bdata.author);
	gotoxy(105, 30);
	printf("cost : %d", bdata.cost);

	create_box_array(bpos, btext, size);

	while (1)
	{
		if (cakr(str, &x, &y))
		{
			modet++;
		}
		else
		{
			mode = check_box_area_func(bpos, x, y, size);
			strcpy(tostr, str);
		}

		switch (mode)
		{
		case 1:
			return;
		case 3:
			strcpy(str, "");
			modet = 1;
			createBox(bpos[2][0], bpos[2][1], bpos[2][2], bpos[2][3], btext[2]);
			gotoxy(53, 7);
			break;
		case 5:
			strcpy(str, "");
			modet = 2;
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], btext[4]);
			gotoxy(53, 12);
			break;
		case 7:
			strcpy(str, "");
			modet = 3;
			createBox(bpos[6][0], bpos[6][1], bpos[6][2], bpos[6][3], btext[6]);
			gotoxy(53, 17);
			break;
		case 9:
			strcpy(str, "");
			modet = 4;
			createBox(bpos[8][0], bpos[8][1], bpos[8][2], bpos[8][3], btext[8]);
			gotoxy(53, 22);
			break;
		case 10:
			/*********************/
			if (strcmp(tmp.id, "") == 0) strcpy(tmp.id, bdata.id);
			if (strcmp(tmp.title, "") == 0) strcpy(tmp.title, bdata.title);
			if (strcmp(tmp.author, "") == 0) strcpy(tmp.author, bdata.author);
			if (tmp.cost == 0) tmp.cost = bdata.cost;
			del_book_from_file_tuple(&arrtmpsize, bdata);
			insert_book_turple("book.txt", tmp);
			*arrsize = arrtmpsize;
			return;
		case 11:
			/*********************/
			del_book_from_file_tuple(&arrtmpsize, bdata);
			*arrsize = arrtmpsize;
			return;
			// ����
		}

		switch (modet)
		{
		case 1:
			strcpy(tmp.id, tostr);
			break;
		case 2:
			strcpy(tmp.title, tostr);
			break;
		case 3:
			strcpy(tmp.author, tostr);
			break;
		case 4:
			tmp.cost = atoi(tostr);
			break;
		}

		x = y = 0;
	}

}
void adminmode_book()
{
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		{100,43,defx,defy},
		{outbx, outby, defx, defy},
		{45,39,96,1}
	};
	char btext[][20] =
	{
		" ���� ������ ",
		" ���� ������ ",
		" �߰� ",
		" ����/���� ",
		" ������ ",
		" 1 "
	};

	// ui section
	int mode = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int tstr[3];

	// book txt �����Ҵ� ����ü �迭 ����
	/*********************/
	BOOK* book = read_books_from_file("book.txt", &numLines);
	int endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
	int picknum = 0;		// �����ϱ� ���õ� ��ü
	int pickarray = 0;
	/*********************/
	BOOK tmp = {"","","",0};

	create_ui_buy_section(book, bpos, btext, numLines, page, size);
	sprintf(tstr, "%d", page);
	createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], tstr);

	while (1)
	{
		cakr(str, &x, &y);
		mode = check_box_area_func(bpos, x, y, size);

		// Ŭ��
		picknum = check_pick_num_pos(x, y);

		if (picknum != 0)
		{
			pickarray = picknum + (page - 1) * listcount;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], tstr);
			// ���� �ʷϻ�
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// �ش� ������ ����
			/*********************/
			strcpy(tmp.id, book[pickarray - 1].id);
			strcpy(tmp.title, book[pickarray - 1].title);
			strcpy(tmp.author, book[pickarray - 1].author);
			tmp.cost = book[pickarray - 1].cost;
			picknum = 0;
		}

		// mode
		switch (mode)
		{
		case 1:
			if (--page == 0) page = endpage;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], tstr);
			break;
		case 2:
			if (++page == endpage + 1) page = 1;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], tstr);
			break;
		case 3:
			/*********************/
			adminmode_book_add();
			free(book);
			book = read_books_from_file("book.txt", &numLines);
			endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
			break;
			// �߰�
		case 4:
			/*********************/
			// ����/����
			adminmode_book_remove(book, &numLines, tmp);
			free(book);
			book = read_books_from_file("book.txt", &numLines);
			break;
		case 5:
			return;
		default:
			break;
		}

		x = y = 0;
	}
}
// 5. admin
void adminmode()
{
	uiDefalutSetting();

	int bpos[][4] =
	{
		{outbx, outby, defx, defy},
		{10,5,50,30},
		{65,5,50,30},
		{120,5,50,30}
	};
	char btext[][20] =
	{
		" ������ ",
		" book.txt ",
		" trade.txt ",
		" user.txt "
	};

	int mode = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);

	// createbox line

	while (1)
	{
		x = y = 0;
		create_box_array(bpos, btext, size);
		cakr(str, &x, &y);
		mode = check_box_area_func(bpos, x, y, size);

		switch (mode)
		{
		case 1:
			return; // 'return 0;' ��� 'return;' ���
		case 2:
			// book.txt
			adminmode_book();
			break;
		case 3:
			// trade.txt
			break;
		case 4:
			// user.txt
			break;
		}
	}
}

// 4 �ŷ� �� ���� Ȯ��
void trade_section(LOGIN user)
{
	uiDefalutSetting();
	int bpos[][4] =
	{
		{140, 43, 30, 3},
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		{100,43,defx,defy},
		{130,4,50,35},
		{5,39,100,1},
		{140, 35, defx, defy},
		// ���� ��ư
		{105,39,24,1}
	};
	char btext[][20] =
	{
		" ������ ",
		" ���� ������ ",
		" ���� ������ ",
		" ���� ",
		" ���� ",
		"",
		"1",
		"           ",
		"����"
	};

	int numLines = 0;
	int page = 1;
	int mode = 0;
	int listcount = 10;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int pick = 0;
	char tstr[5];
	int selleron = 0;
	int sortnum = 1;
	
	LOGIN tuser;

	// createbox line
	TRADE *t = read_trade_from_file_id(&numLines, user);
	TRADE tmp;

	create_box_array(bpos, btext, size);
	print_book_list_t(t, numLines, page, user.id);

	int endpage = (numLines % listcount == 0 && numLines >= listcount) ? numLines / listcount : (numLines / listcount) + 1;

	while (1)
	{
		gotoxy(0, 0);
		cakr(str, &x, &y);
		

		pick = check_pick_num_pos_t(x, y);
		mode = check_box_area_func(bpos, x, y, size);

		if (pick != 0 && (page - 1) * listcount + pick <= numLines)
		{
			// ���õ� ��� �����
			print_book_list_t(t, numLines, page, user.id);

			// ���ÿ�� ���
			sprintf(tstr, "%d", (page - 1) * listcount + pick);
			createBoxc(5, 8 + (pick - 1) * 3, 10, 1, tstr, GREEN);
			// ���ÿ�� ������ �ӽ� ����
			
			strcpy(tuser.id, t[pick - 1].ids);
			tmp = t[(page - 1) * listcount + pick - 1];

			// ��� ��� Ŭ���ϴ� �κ�
			if (t[(page - 1) * listcount + pick - 1].ss == 0 && !strcmp(t[(page - 1) * listcount + pick - 1].ids, user.id))
			{
				gotoxy(136, 7);
				printf(" ���� ��û�� ���Խ��ϴ�.\n");
				gotoxy(136, 8);
				printf(" ������ id�Դϴ� : %s.\n", t[(page - 1) * listcount + pick - 1].idb);
				gotoxy(136, 9);
				printf("                                       \n");
				gotoxy(136, 10);
				printf("                                       \n");
				gotoxy(136, 11);
				printf("                                       \n");
				gotoxy(136, 7);
				createBox(140, 35, defx, defy, "           ");
			}
			else if( t[(page - 1) * listcount + pick - 1].ss == 1)
			{
				
				user_data_check_id(&tuser);
				gotoxy(136, 7);
				printf("                                       \n");
				gotoxy(136, 8);
				printf("                                       \n");
				gotoxy(136, 9);
				printf("                                       \n");
				gotoxy(136, 10);
				printf("                                       \n");
				gotoxy(136, 11);
				printf("                                       \n");
				gotoxy(136, 7);
				printf(" �Ǹ��ڰ� ���� ��û�� �����ϼ̽��ϴ�.\n");
				gotoxy(136, 8);
				printf(" �Ǹ����� ���������Դϴ�.\n");
				gotoxy(136, 9);
				printf(" �̸� :\t\t%s\n", tuser.name);
				gotoxy(136, 10);
				printf(" ���� ��ȣ :\t\t%s\n", tuser.creditc);
				gotoxy(136, 11);
				printf(" �ڵ��� ��ȣ :\t\t%s\n", tuser.phone);
				createBox(140, 35, defx, defy, " Ȯ��/���� ");
				tmp = t[(page - 1) * listcount + pick - 1];
				selleron = 1;
			}
			else 
			{
				gotoxy(136, 7);
				printf(" �Ǹ����� ������ ����ϰ� �ֽ��ϴ�.   \n");
				gotoxy(136, 8);
				printf("                                       \n");
				gotoxy(136, 9);
				printf("                                       \n");
				gotoxy(136, 10);
				printf("                                       \n");
				gotoxy(136, 11);
				printf("                                       \n");
				gotoxy(136, 7);
				createBox(140, 35, defx, defy, "           ");
				selleron = 0;
			}
			
			
			pick = 0;
			x = y = 0;
		}		

		
		switch(mode)
		{
			case 1:
				return 0;
			case 2:
				if (--page == 0) page = endpage;

				system("cls");
				create_box_array(bpos, btext, size);
				print_book_list_t(t, numLines, page, user.id);
				sprintf(tstr, "%d", page);
				createBox(bpos[6][0], bpos[6][1], bpos[6][2], bpos[6][3], tstr);
				break;
			case 3:
				if (++page == endpage + 1) page = 1;

				system("cls");
				create_box_array(bpos, btext, size);
				print_book_list_t(t, numLines, page, user.id);
				sprintf(tstr, "%d", pick + page);
				createBox(bpos[6][0], bpos[6][1], bpos[6][2], bpos[6][3], tstr);
				break;
			case 4:
				if(!strcmp(tmp.ids, user.id) && tmp.ss == 0)
				{
					trade_section_ok(tmp);
					free(t);
					t = read_trade_from_file_id(&numLines, user);
					create_box_array(bpos, btext, size);
					print_book_list_t(t, numLines, page, &user.id);
				}
				break;
			case 5:
				// ����
				break;
			case 6:
				// o �϶� Ȯ��â
				// ���� : Ȯ�� �� �� txt ���Ͽ� �ش� ������ ��� ����
				if (selleron == 1)
				{
					// ���� : ���� �� ������ �����ϰ� ����ϰ�
					del_trade_from_file_tuple(tmp);
					free(t);
					t = read_trade_from_file_id(&numLines, user);
					system("cls");
					create_box_array(bpos, btext, size);
					print_book_list_t(t, numLines, page, &user.id);
				}
				break;
			case 9:
				// ���� ��ư�� ���Ͽ�
				bubbleSortTrade(t, numLines, sortnum++);
				if (sortnum == 5)
				{
					sortnum = 1;
				}
				print_book_list_t(t, numLines, page, &user.id);
				break;
		}
		x = y = 0;
		mode = 0;
	}
	
}

// 3.1 �Ǹ� ���
void sell_section_puton_book(LOGIN user)
{
	uiDefalutSetting();

	int bpos[][4] =
	{

		{5,5,defx,defy},
		{5,10,defx,defy},
		{5,15,defx,defy},
		// input box
		{50,5,defx,defy},
		{50,10,defx,defy},
		{50,15,defx,defy},

		// ����ϱ�
		{5, 20, 75, 5}
	};

	char btext[][20] =
	{
		" å ���� ",
		" å ���� ",
		" ���� ",
		"",
		"",
		"",
		" ����ϱ� "
	};

	BOOK tmp = { "","","",0 };

	// � ��ư�� �νĵǾ����� ��ȯ�ϴ� ����
	int mode = 0;
	// str ���� ��ġ
	int smode = 0;

	// ��ư ����
	int size = sizeof(bpos) / sizeof(bpos[0]);

	// createbox line
	create_box_array(bpos, btext, size);




	while (1)
	{
		cakr(str, &x, &y);
		mode = check_box_area_func(bpos, x, y, size);
		switch (mode)
		{
		case 4:
			strcpy(str, "");
			createBox(bpos[3][0], bpos[3][1], bpos[3][2], bpos[3][3], btext[3]);
			gotoxy(bpos[3][0] + 5, bpos[3][1] + 2);
			smode = 4;
			break;
		case 5:
			strcpy(str, "");
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], btext[4]);
			gotoxy(bpos[4][0] + 5, bpos[4][1] + 2);
			smode = 5;
			break;
		case 6:
			strcpy(str, "");
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], btext[5]);
			gotoxy(bpos[5][0] + 5, bpos[5][1] + 2);
			smode = 6;
			break;
		case 7:
			// Ȯ��â �߰��ϱ�
			gotoxy(0, 0);
			printf("\n 1. %d", strcmp(tmp.title, ""));
			printf("\n 2. %d", strcmp(tmp.author, ""));
			printf("\n 3. %d", (tmp.cost != 0));
			if ((strcmp(tmp.title, "")) && (strcmp(tmp.author, "")) && (tmp.cost != 0))
			{
				strcpy(tmp.id, user.id);
				insert_book_turple("book.txt", tmp);
				return 0;
			}
		defalut:
			smode = 0;
		}

		// ������ ��ư
		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			x = y = 0;
			return 0;
		}

		// ���� ����
		switch (smode)
		{
		case 4:
			strcpy(tmp.title, str);
			break;
		case 5:
			strcpy(tmp.author, str);
			break;
		case 6:
			tmp.cost = atoi(str);
			break;
		}


		mode = x = y = 0;
	}

}
// 3 �Ǹ�
void sell_section(LOGIN user)
{
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		{100,43,defx,defy},
		// ���� ��ư
		{150,39,24,1}
	};
	char btext[][20] =
	{
		" ���� ������ ",
		" ���� ������ ",
		" ��� ",
		" ��� ��� ",
		" ���� "
	};

	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int mode = 0;

	int picknum = 1;		// �����ϱ� ���õ� ��ü
	int pickarray = 0;
	BOOK tmp;
	int tstr[3];
	int sortnum = 1;

	// tmp.id ���� ��ġ�� �迭�� 
	BOOK* book = read_books_from_file_id(&numLines, user);
	int endpage = (numLines % listcount == 0 ) ? numLines / listcount : (numLines / listcount) + 1;

	
	while (1)
	{
		gotoxy(0, 0);
		cakr(str, &x, &y);

		mode = check_box_area_func(bpos, x, y, size);
		picknum = check_pick_num_pos(x, y);

		if (picknum != 0)
		{
			pickarray = picknum + (page - 1) * listcount;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(5, 39, 140, 1, tstr);
			// ���� �ʷϻ�
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// �ش� ������ ����
			tmp = book[pickarray - 1];
			picknum = 0;
		}


		switch (mode)
		{
		case 1:
			if (--page == 0) page = endpage;

			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(5, 39, 140, 1, tstr);
			break;
		case 2:
			if (++page == endpage + 1) page = 1;

			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(5, 39, 140, 1, tstr);
			break;
		case 3:
			sell_section_puton_book(user);
			free(book);
			book = read_books_from_file_id(&numLines, user);
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
			picknum = 1;
			break;
		case 4:
			// �����Ѱ� book.txt���� ���� 
			del_book_from_file_tuple(&numLines, tmp);
			free(book);
			book = read_books_from_file_id(&numLines, user);
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
			picknum = 1;
			break;
		case 5:
			bubbleSortBook(book, numLines, sortnum++);
			if (sortnum == 5)
			{
				sortnum = 1;
			}
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			break;
		}

		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			x = y = 0;
			strcpy(tstr, "");
			return 0;
		}

		mode = 0;
		x = y = 0;
		strcpy(tstr, "");
	}


	free(book);
}

// 2 / ���� �Ǹ� ����
void choose_bs(LOGIN user)
{
	uiDefalutSetting();

	int bpos[][4] =
	{
		{outbx, outby, defx, defy},
		{10,5,50,30},
		{65,5,50,30},
		{120,5,50,30}
	};
	char btext[][20] =
	{
		" ������ ",
		" ���� ",
		" �Ǹ� ",
		" �ŷ� ��û "
	};

	int mode = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);

	// createbox line

	while (1)
	{
		x = y = 0;
		create_box_array(bpos, btext, size);
		cakr(str, &x, &y);
		mode = check_box_area_func(bpos, x, y, size);

		switch (mode)
		{
		case 1:
			return; // 'return 0;' ��� 'return;' ���
		case 2:
			buy_section(user);
			break;
		case 3:
			sell_section(user);
			break;
		case 4:
			trade_section(user);
			break;
		}
		x = y = 0;
		mode = 0;
	}
}
// 2 .1 .2 // ���� ��û ������
void buy_section_yesno(BOOK tmp, BOOK book, LOGIN user)
{
	int bpos[][4] =
	{
		{65,25,10,defy},
		{95,25,10,defy},
		{60,10,50,20}
	};
	char btext[][20] =
	{
		" YES ",
		" NO "
	};

	char tstr[50];
	strcpy(tstr, tmp.id);
	strcat(tstr, "���� ������ �����Ͻʴϱ�?");

	createBox(bpos[2][0], bpos[2][1], bpos[2][2], bpos[2][3], tstr);
	createBox(bpos[0][0], bpos[0][1], bpos[0][2], bpos[0][3], btext[0]);
	createBox(bpos[1][0], bpos[1][1], bpos[1][2], bpos[1][3], btext[1]);

	while (1)
	{
		cakr(str, &x, &y);
		//say yes
		if (checkBoxArea(bpos[0][0], bpos[0][1], bpos[0][2], bpos[0][3], x, y))
		{
			insert_trade_turple("trade.txt", user, book, 1, 0);
			return 0;
		}
		// say no
		if (checkBoxArea(bpos[1][0], bpos[1][1], bpos[1][2], bpos[1][3], x, y))
		{
			return 0;
		}
	}
}
// 2 .1 / ����
void buy_section(LOGIN user)
{
	/*

	numm box ->> {5, 8, 10, 1},

	1	(5,8,170,3)
	2	(5,11,)
	3	(5,14,)
	4	(5,17)
	5	(5,20)
	6	(5,23)
	7	(5,26)
	8	(5,29)
	9	(5,32)
	10	(5,35)

	*/

	// ��µ� ��� ����
	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int tstr[3];
	// �ڽ� �ν�
	int mode = 3;

	// ��ư ��ġ, ���� ����
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		// ���� ��ư
		{151,39,24,1},
		// page
		{5, 39, 40,1},
		// 6. �˻�â
		{45,39,96,1},
		// enter
		{141,39,10,1}
	};
	char btext[][20] =
	{
		" ���� ������ ",
		" ���� ������ ",
		" �����ϱ� ",
		" ���� ",
		" 1 ",
		"  ",
		" ENTER "
	};

	// �ν� ��ư ����
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int sortnum = 1;
	int enter = 0;
	char search[128];

	// book txt �����Ҵ� ����ü �迭 ����
	BOOK* book = read_books_from_file("book.txt", &numLines);
	int endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
	int picknum = 0;		// �����ϱ� ���õ� ��ü
	int pickarray = 0;
	BOOK tmp;		// ���� ���õ� ��ü�� ���̵�

	// ui �׸���
	create_ui_buy_section(book, bpos, btext, numLines, page, size);

	sprintf(tstr, "%d", page);
	createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);

	// �� �Լ� ����
	while (1)
	{
		enter = cakr(str, &x, &y);

		if (enter == 1)
		{
			// enter �϶� 7 �� �˻� �ǵ���
			mode = 7;
		}
		else
		{
			mode = check_box_area_func(bpos, x, y, size);
			strcpy(search, str);
		}
		picknum = check_pick_num_pos(x, y);

		if (picknum != 0)
		{
			pickarray = picknum + (page - 1) * listcount;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);
			// ���� �ʷϻ�
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// �ش� ������ ����
			strcpy(tmp.id, book[pickarray - 1].id);
			picknum = 0;
		}


		switch (mode)
		{
		case 1:
			if (--page == 0) page = endpage;

			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);
			break;
		case 2:
			if (++page == endpage + 1) page = 1;

			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);
			break;
		case 3:
			if( strcmp(tmp.id, user.id) != 0 )
			{
				buy_section_yesno(tmp, book[pickarray - 1], user);
				create_ui_buy_section(book, bpos, btext, numLines, page, size);
			}
			break;
		case 4:
			bubbleSortBook(book, numLines, sortnum++);
			if (sortnum == 5)
			{
				sortnum = 1;
			}
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);
			break;
		case 6:
			strcpy(search, "");
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], "");
			gotoxy(50, 40);
			break;
		case 7:
			buy_section_search(book, numLines, search);
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);
			gotoxy(50, 40);
			break;
		}
		

		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			return 0;
		}

		mode = 0;
		x = y = 0;
		strcpy(tstr, "");
	}


	free(book);
}

// 1.1 / �α��� �� ���� �Ǹ� �U���� �̵� �ʿ�
void login_func()
{
	system("cls");
	systemafter();

	LOGIN tmp;
	int mode = 0;

	uiDefalutSetting();


	char strr[256] = "���̵� / ��й�ȣ�� �Է����ּ���";
	createBox(10, 5, defx, defy, "input id");
	createBox(50, 5, defx, defy, "");
	createBox(10, 10, defx, defy, "input pw");
	createBox(50, 10, defx, defy, "");
	createBox(10, 15, 70, 3, strr);
	createBox(5, 35, 174, 3, "");

	while (1) 
	{
		if (cakr(str, &x, &y))
		{
			if (!strcmp(tmp.id, "admin") && !strcmp(tmp.pwd, "admin"))
			{
				adminmode();
				break;
			}
			if (user_data_check_idpwd(tmp))
			{
				choose_bs(tmp);
				x = y = 0;
				break;
				// ���� �Ǹ� â���� �̵�
			}
			else
			{
				createBox(50, 5, defx, defy, "");
				createBox(50, 10, defx, defy, "");
				createBox(5, 35, 174, 3, "���̵� ��й�ȣ�� ���Է� ���ּ���");
				mode = 0;
			}
		}

		// ���̵� �Է� �ڽ�
		if (checkBoxArea(50, 5, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 5, defx, defy, "");
				gotoxy(55, 7);
				mode = 1;
			}
			strcpy(tmp.id, str);
		}

		// ��й�ȣ �Է� �ڽ�
		if (checkBoxArea(50, 10, defx, defy, x, y))
		{
			if (mode != 2)
			{
				strcpy(str, "");
				createBox(50, 10, defx, defy, "");
				gotoxy(55, 12);
				mode = 2;
			}
			strcpy(tmp.pwd, str);

		}

		if (checkBoxArea(5, 35, 174, 3, x, y))
		{
			if (mode != 3)
			{
				createBox(5, 35, defx, defy, str);
				gotoxy(10, 37);
				mode = 3;
			}
			strcpy(tmp.pwd, str);

		}
		
		if (checkBoxArea(outbx, outby, defx, defy, x, y)) return 0;
	}
}
// 1.2 / ���̵� ����
void create_id() 
{
	system("cls");
	systemafter();
	uiDefalutSetting();

	LOGIN tmp = { "id" , "pwd" , "name" , "credic" , "phone"};
	int mode = 0;

	strcpy(str, "");
	char strr[256] = "���̵� / ��й�ȣ�� �Է����ּ���";
	createBox(10, 5, defx, defy, "input id");
	createBox(50, 5, defx, defy, "");
	createBox(10, 10, defx, defy, "input pw");
	createBox(50, 10, defx, defy, "");
	createBox(10, 15, 70, 3, strr);
	createBox(5, 35, 174, 3, "");

	// id password chekc
	while (1) 
	{
		if (cakr(str, &x, &y))
		{
			if ( !check_duplication_id(tmp.id) && strcmp(tmp.id, "") != 0 && !check_secial_word_in_password(tmp.pwd))
			{
				break;
			}
			else
			{
				createBox(50, 5, defx, defy, "");
				createBox(50, 10, defx, defy, "");
				createBox(5, 35, 174, 3, "���̵� ��й�ȣ�� ���Է� ���ּ���");
				strcpy(tmp.id, "");
				strcpy(tmp.pwd, "");
				
				mode = 0;
			}
		}

		// ���̵� �Է� �ڽ�
		if (checkBoxArea(50, 5, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 5, defx, defy, "");
				gotoxy(55, 7);
				mode = 1;
			}
			strcpy(tmp.id, str);
		}

		// ��й�ȣ �Է� �ڽ�
		if (checkBoxArea(50, 10, defx, defy, x, y))
		{
			if (mode != 2)
			{
				strcpy(str, "");
				createBox(50, 10, defx, defy, "");
				gotoxy(55, 12);
				mode = 2;
			}
			strcpy(tmp.pwd, str);

		}

		if (checkBoxArea(outbx, outby, defx, defy, x, y)) return 0;
	}

	system("cls");
	systemafter();
	uiDefalutSetting();

	strcpy(strr, "please input user info");
	createBox(10, 5, defx, defy, "Name");
	createBox(50, 5, defx, defy, "");
	createBox(10, 10, defx, defy, "Creditcode");
	createBox(50, 10, defx, defy, "");
	createBox(10, 15, defx, defy, "Phone");
	createBox(50, 15, defx, defy, "");
	createBox(5, 35, 174, 3, str);

	// user info input
	while (1) {

		if (cakr(str, &x, &y))
		{
			//���� �߰� ����
			insertUserTuple("user.txt", tmp);
			return 0;

		}

		// ���̵� �Է� �ڽ�
		if (checkBoxArea(50, 5, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 5, defx, defy, "");
				gotoxy(55, 7);
				mode = 1;
			}
			strcpy(tmp.name, str);
		}

		// ��й�ȣ �Է� �ڽ�
		if (checkBoxArea(50, 10, defx, defy, x, y))
		{
			if (mode != 2)
			{
				strcpy(str, "");
				createBox(50, 10, defx, defy, "");
				gotoxy(55, 12);
				mode = 2;
			}
			strcpy(tmp.creditc, str);

		}

		if (checkBoxArea(50, 15,defx, defy, x, y))
		{
			if (mode != 3)
			{
				strcpy(str, "");
				createBox(50, 15, defx, defy, str);
				gotoxy(55, 17);
				mode = 3;
			}
			strcpy(tmp.phone, str);

		}

		if (checkBoxArea(outbx, outby, defx, defy, x, y)) return 0;
	}
}
// 1.3.1 / ��й�ȣ Ư�� ���� / ���� ���� = 0 , �Ҹ��� = 1
int check_secial_word_in_password(char* password)
{
	int special = 0;
	for (int i = 0; i < strlen(password); i++)
	{
		char c = password[i];

		// Ư������ ���� Ȯ��
		if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*')
		{
			special = 1;
			break;
		}
	}

	// ��й�ȣ�� �ּ� 8�� �̻��̸�, Ư�����ڸ� �����ؾ� ��
	if (special == 1 && (strlen(password) >= 8))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
// 1.3 / ��й�ȣ ����
void edit_password()
{
	system("cls");
	//systemafter();
	uiDefalutSetting();
	int mode = 0;
	LOGIN tmp;

	char strr[256] = "���̵� �Է����ּ���";
	createBox(10, 5, defx, defy, "���̵�");
	createBox(50, 5, defx, defy, "");
	createBox(5, 35, 174, 3, strr);

	// ���̵� ��ġ/����ġ �˻�
	while (1) {
		// ���� �Է�
		if (cakr(str, &x, &y))
		{
			if (user_data_check_id(&tmp))
			{
				break;
			}
			else
			{
				createBox(5, 35, 174, 3, "�������� �ʴ� ���̵��Դϴ�");
				mode = 0;
			}
		}

		if (checkBoxArea(50, 5, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 5, defx, defy, "");
				gotoxy(55, 7);
				mode = 1;
			}
			strcpy(tmp.id, str);
		}


		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			return 0;
		}
	}

	// �̸� ��ȭ��ȣ ��ġ/����ġ �˻�
	LOGIN comp;
	mode = 0;
	strcpy(str, "�������� ���� �ܰ��Դϴ�.");
	createBox(10, 5, defx, defy, "�̸�");
	createBox(50, 5, defx, defy, "");
	createBox(10, 10, defx, defy, "��ȭ��ȣ");
	createBox(50, 10, defx, defy, "");

	createBox(5, 35, 174, 3, str);

	while (1)
	{
		// enter
		if (cakr(str, &x, &y))
		{
			if ( !strcmp(comp.name, tmp.name) && !strcmp(comp.phone, tmp.phone))
			{
				break;
			}
			else
			{
				createBox(5, 35, 174, 3, "����� ������ �ٽ� �Է����ּ���.");
				createBox(50, 5, defx, defy, "");
				createBox(50, 10, defx, defy, "");
			}
		}

		if (checkBoxArea(50, 5, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 5, defx, defy, "");
				gotoxy(55, 7);
				mode = 1;
			}
			strcpy(comp.name, str);
		}

		if (checkBoxArea(50, 10, defx, defy, x, y))
		{
			if (mode != 2)
			{
				strcpy(str, "");
				createBox(50, 10, defx, defy, "");
				gotoxy(55, 12);
				mode = 2;
			}
			strcpy(comp.phone, str);
		}


		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			return 0;
		}
		
	}

	// ��й�ȣ �缳��
	strcpy(str, "");
	strcpy(comp.pwd, tmp.pwd);
	strcpy(tmp.pwd, "");

	strcpy(str, "���ο� ��й�ȣ�� �Է����ּ���");
	createBox(10, 15, defx, defy, "���ο� ��й�ȣ");
	createBox(50, 15, defx, defy, "");
	gotoxy(0, 0);

	while (1)
	{
		// enter
		if (cakr(str, &x, &y))
		{
			if (check_secial_word_in_password(tmp.pwd) == 0)
			{
				user_data_edit_pwd(tmp);
				return 0;
			}
			else if (!strcmp(tmp.pwd, comp.pwd))
			{
				createBox(5, 35, 174, 3, "������ ��й�ȣ�� ��ġ�մϴ�");
				mode = 0;
				continue;
			}
			else
			{
				strcpy(str, "");
				createBox(5, 35, 174, 3, "8�ڸ� �̻� / Ư�����ڸ� �����ؾ��մϴ�");
				mode = 0;
			}
		}

		if (checkBoxArea(50, 15, defx, defy, x, y))
		{
			if (mode != 1)
			{
				strcpy(str, "");
				createBox(50, 15, defx, defy, "");
				gotoxy(55, 17);
				mode = 1;
			}
			strcpy(tmp.pwd, str);
		}


		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			return 0;
		}

	}

}

// login ���� �Լ�
void loginStream()
{
	uiDefalutSetting();
	createBox(10, 43, defx, defy, "�α���");
	createBox(40, 43, defx, defy, "���̵� ����");
	createBox(70, 43, defx, defy, "��й�ȣ ã��");

	int mode = 1;

	while (1)
	{

		cakr(str, &x, &y);
		gotoxy(5, 5);
		if (checkBoxArea(10, 43, defx, defy, x, y))
		{
			login_func();		// 1.1
			mode = 0;
		}
		if (checkBoxArea(40, 43, defx, defy, x, y))
		{
			create_id();		// 1.2
			mode = 0;
		}
		if (checkBoxArea(70, 43, defx, defy, x, y))
		{
			edit_password();	// 1.3
			mode = 0;
		}
		

		if (mode == 0)
		{
			system("cls");
			systemafter();
			uiDefalutSetting();
			createBox(10, 43, defx, defy, "�α���");
			createBox(40, 43, defx, defy, "���̵� ����");
			createBox(70, 43, defx, defy, "��й�ȣ ã��");
			mode = 1;
			x = 0;
			y = 0;
		}

		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			exit(1);
		}
	}
}




void setupfunction()
{
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		{100,43,defx,defy}
	};
	char btext[][20] =
	{
		" ���� ������ ",
		" ���� ������ ",
		" ��� ",
		" ��� ��� "
	};

	int mode = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);

	// createbox line
	create_box_array(bpos, btext, size);

	while (1)
	{
		cakr(str, &x, &y);
		mode = check_box_area_func(bpos, x, y, size);
		x = y = 0;
	}
}