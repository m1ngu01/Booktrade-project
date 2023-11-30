#pragma warning(disable : 4996) // 에러 C4996을 무시

#include <windows.h>

#include "cak.h"
#include "file.h"

#define outbx 140
#define outby 43
// 자주 사용하는 텍스트 박스 사이즈
#define defx 30
#define defy 3

// 색 코드
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



/* 공통 함수 */

// system 함수 사용 후 사용해야할 함수
void systemafter()
{
	DWORD CIN;
	DWORD mode;
	CIN = GetStdHandle(STD_INPUT_HANDLE); //마우스 재활성화
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
}
// goto 함수
void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// UI 글씨 색 변경
void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// UI 대화창 만들기 색 추가
void createBoxc(int posx, int posy, int lenx, int leny, const char* text, int color)
{
	int tmpy;
	setColor(color);

	// top
	gotoxy(posx, posy);
	printf("┏");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("━");
	}
	printf("┓");

	// body
	gotoxy(posx, posy + 1);
	tmpy = posy + 1;
	for (int j = 0; j < leny; j++)
	{
		printf("┃");
		for (int i = 0; i < lenx - 2; i++)
		{
			printf(" ");
		}
		printf("┃");
		gotoxy(posx, ++tmpy);
	}

	//bottom
	gotoxy(posx, tmpy);
	printf("┗");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("━");
	}
	printf("┛");

	//text

	gotoxy(posx + (lenx / 2) - (strlen(text) / 2), posy + leny / 2 + 1);
	printf("%s", text);
	setColor(WHITE);

}
// UI 대화창 만들기
void createBox(int posx, int posy, int lenx, int leny, const char* text)
{
	int tmpy;

	// top
	gotoxy(posx, posy);
	printf("┏");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("━");
	}
	printf("┓");

	// body
	gotoxy(posx, posy + 1);
	tmpy = posy + 1;
	for (int j = 0; j < leny; j++)
	{
		printf("┃");
		for (int i = 0; i < lenx - 2; i++)
		{
			printf(" ");
		}
		printf("┃");
		gotoxy(posx, ++tmpy);
	}

	//bottom
	gotoxy(posx, tmpy);
	printf("┗");
	for (int i = 0; i < lenx - 2; i++)
	{
		printf("━");
	}
	printf("┛");

	//text

	gotoxy(posx + (lenx / 2) - (strlen(text) / 2), posy + leny / 2 + 1);
	printf("%s", text);

}
// UI 기본 틀
void createOutline()
{
	int height = 45;
	int line = 180;
	int tmpy;

	gotoxy(2, 2);

	printf("┏");
	for (int i = 0; i < line - 2; i++)
	{
		printf("━");
	}
	printf("┓");

	gotoxy(2, 3);
	tmpy = 3;
	for (int j = 0; j < height; j++)
	{
		if (j == 39) {
			printf("┠");
			for (int i = 0; i < line - 2; i++)
			{
				printf("━");
			}
			printf("┫");
		}
		else {
			printf("┃");
			gotoxy(181, tmpy);
			printf("┃");
			gotoxy(2, tmpy);
		}
		gotoxy(2, ++tmpy);
	}
	
	printf("┗");
	for (int i = 0; i < line - 2; i++)
	{
		printf("━");
	}
	printf("┛");
}
// UI 밑그림
void uiDefalutSetting()
{
	system("cls");
	createOutline();
	createBox(140, 43, 30, 3, "나가기");
}
// 대화창 범위 확인
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
// 대화창 범위 확인 매개변수 버튼 좌표 배열
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
// 구매 판매 책 목록 클릭 데이터 리턴 함수
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
// 목록 클릭 데이터 리턴 함수 <trade section>
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
// 구매 판매 책 목록 출력 <sell_section>
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
	createBox(boxpos[1][0], boxpos[1][1], boxpos[1][2], boxpos[1][3], "이름");
	createBox(boxpos[2][0], boxpos[2][1], boxpos[2][2], boxpos[2][3], "제목");
	createBox(boxpos[3][0], boxpos[3][1], boxpos[3][2], boxpos[3][3], "저자");
	createBox(boxpos[4][0], boxpos[4][1], boxpos[4][2], boxpos[4][3], "가격");

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
// 구매 판매 책 목록 출력 <trade_section>
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
	createBox(boxpos[1][0], boxpos[1][1], boxpos[1][2], boxpos[1][3], "판매자");
	createBox(boxpos[2][0], boxpos[2][1], boxpos[2][2], boxpos[2][3], "제목");
	createBox(boxpos[3][0], boxpos[3][1], boxpos[3][2], boxpos[3][3], "가격");
	createBox(boxpos[4][0], boxpos[4][1], boxpos[4][2], boxpos[4][3], "거래 상태");

	for (int i = 0; i < listcount && startcount < numLines; i++, startcount++)
	{
		sprintf(str, "%d", startcount + 1);
		createBox(boxpos[0][0], boxpos[0][1] + 3 * i + 4, boxpos[0][2], boxpos[0][3], str);
		createBox(boxpos[1][0], boxpos[1][1] + 3 * i + 4, boxpos[1][2], boxpos[1][3], t[startcount].ids);
		createBox(boxpos[2][0], boxpos[2][1] + 3 * i + 4, boxpos[2][2], boxpos[2][3], t[startcount].title);
		sprintf(str, "%d", t[startcount].cost);
		createBox(boxpos[3][0], boxpos[3][1] + 3 * i + 4, boxpos[3][2], boxpos[3][3], str);
		// 판매자가 user 일때 출력
		strcpy(str, (t[startcount].ss == 0) ? "x" : "o");
		if (!strcmp(t[startcount].ids ,userid))
		{
			strcpy(str, (t[startcount].ss == 0) ? "△" : "o");
		}
		createBox(boxpos[4][0], boxpos[4][1] + 3 * i + 4, boxpos[4][2], boxpos[4][3], str);
	}
}
// 구매 판매 박스 생성
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
// 배열 선언 상자 객체들 생성
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
		" 나가기 ",
		"아이디",
		"",
		"제목",
		"",
		"저자",
		"",
		"가격",
		"",
		"추가"
		
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
		" 나가기 ",

		"아이디",
		"",

		"제목",
		"",

		"저자",
		"",

		"가격",
		"",

		"수정",
		"삭제"

	};

	

	char tostr[128] = "";
	int mode = 0;
	int modet = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	BOOK tmp = { "", "", "", 0 };
	int arrtmpsize = *arrsize;
	// +창
	/*********************/
	createBox(100, 5, 60, 30, "");
	gotoxy(105, 10);
	printf("선택된 데이터입니다.");
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
			// 삭제
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
		" 이전 페이지 ",
		" 다음 페이지 ",
		" 추가 ",
		" 수정/삭제 ",
		" 나가기 ",
		" 1 "
	};

	// ui section
	int mode = 0;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int tstr[3];

	// book txt 동적할당 구조체 배열 생성
	/*********************/
	BOOK* book = read_books_from_file("book.txt", &numLines);
	int endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
	int picknum = 0;		// 구매하기 선택된 객체
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

		// 클릭
		picknum = check_pick_num_pos(x, y);

		if (picknum != 0)
		{
			pickarray = picknum + (page - 1) * listcount;
			create_ui_buy_section(book, bpos, btext, numLines, page, size);
			sprintf(tstr, "%d", page);
			createBox(bpos[5][0], bpos[5][1], bpos[5][2], bpos[5][3], tstr);
			// 고른거 초록색
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// 해당 데이터 저장
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
			// 추가
		case 4:
			/*********************/
			// 수정/삭제
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
		" 나가기 ",
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
			return; // 'return 0;' 대신 'return;' 사용
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

// 4 거래 현 상태 확인
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
		// 정렬 버튼
		{105,39,24,1}
	};
	char btext[][20] =
	{
		" 나가기 ",
		" 이전 페이지 ",
		" 다음 페이지 ",
		" 수락 ",
		" 거절 ",
		"",
		"1",
		"           ",
		"정렬"
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
			// 선택된 요소 지우기
			print_book_list_t(t, numLines, page, user.id);

			// 선택요소 출력
			sprintf(tstr, "%d", (page - 1) * listcount + pick);
			createBoxc(5, 8 + (pick - 1) * 3, 10, 1, tstr, GREEN);
			// 선택요소 데이터 임시 저장
			
			strcpy(tuser.id, t[pick - 1].ids);
			tmp = t[(page - 1) * listcount + pick - 1];

			// 출력 요소 클릭하는 부분
			if (t[(page - 1) * listcount + pick - 1].ss == 0 && !strcmp(t[(page - 1) * listcount + pick - 1].ids, user.id))
			{
				gotoxy(136, 7);
				printf(" 구매 요청이 들어왔습니다.\n");
				gotoxy(136, 8);
				printf(" 구매자 id입니다 : %s.\n", t[(page - 1) * listcount + pick - 1].idb);
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
				printf(" 판매자가 구매 요청을 수락하셨습니다.\n");
				gotoxy(136, 8);
				printf(" 판매자의 인적정보입니다.\n");
				gotoxy(136, 9);
				printf(" 이름 :\t\t%s\n", tuser.name);
				gotoxy(136, 10);
				printf(" 계좌 번호 :\t\t%s\n", tuser.creditc);
				gotoxy(136, 11);
				printf(" 핸드폰 번호 :\t\t%s\n", tuser.phone);
				createBox(140, 35, defx, defy, " 확인/삭제 ");
				tmp = t[(page - 1) * listcount + pick - 1];
				selleron = 1;
			}
			else 
			{
				gotoxy(136, 7);
				printf(" 판매자의 응답을 대기하고 있습니다.   \n");
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
				// 거절
				break;
			case 6:
				// o 일때 확인창
				// 수정 : 확인 일 시 txt 파일에 해당 데이터 모두 삭제
				if (selleron == 1)
				{
					// 수정 : 삭제 후 데이터 갱신하고 출력하게
					del_trade_from_file_tuple(tmp);
					free(t);
					t = read_trade_from_file_id(&numLines, user);
					system("cls");
					create_box_array(bpos, btext, size);
					print_book_list_t(t, numLines, page, &user.id);
				}
				break;
			case 9:
				// 정렬 버튼에 대하영
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

// 3.1 판매 등록
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

		// 등록하기
		{5, 20, 75, 5}
	};

	char btext[][20] =
	{
		" 책 제목 ",
		" 책 저자 ",
		" 가격 ",
		"",
		"",
		"",
		" 등록하기 "
	};

	BOOK tmp = { "","","",0 };

	// 어떤 버튼이 인식되었는지 반환하는 변수
	int mode = 0;
	// str 복사 위치
	int smode = 0;

	// 버튼 개수
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
			// 확인창 추가하기
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

		// 나가기 버튼
		if (checkBoxArea(outbx, outby, defx, defy, x, y))
		{
			x = y = 0;
			return 0;
		}

		// 변수 저장
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
// 3 판매
void sell_section(LOGIN user)
{
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		{100,43,defx,defy},
		// 정렬 버튼
		{150,39,24,1}
	};
	char btext[][20] =
	{
		" 이전 페이지 ",
		" 다음 페이지 ",
		" 등록 ",
		" 등록 취소 ",
		" 정렬 "
	};

	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int mode = 0;

	int picknum = 1;		// 구매하기 선택된 객체
	int pickarray = 0;
	BOOK tmp;
	int tstr[3];
	int sortnum = 1;

	// tmp.id 에와 일치한 배열들 
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
			// 고른거 초록색
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// 해당 데이터 저장
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
			// 선택한거 book.txt에서 삭제 
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

// 2 / 구매 판매 선택
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
		" 나가기 ",
		" 구매 ",
		" 판매 ",
		" 거래 요청 "
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
			return; // 'return 0;' 대신 'return;' 사용
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
// 2 .1 .2 // 구매 요청 보내기
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
	strcat(tstr, "님의 물건을 구매하십니까?");

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
// 2 .1 / 구매
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

	// 출력될 목록 개수
	int listcount = 10;
	int numLines = 0;
	int page = 1;
	int tstr[3];
	// 박스 인식
	int mode = 3;

	// 버튼 위치, 범위 설정
	int bpos[][4] =
	{
		{10,43,defx,defy},
		{40,43,defx,defy},
		{70,43,defx,defy},
		// 정렬 버튼
		{151,39,24,1},
		// page
		{5, 39, 40,1},
		// 6. 검색창
		{45,39,96,1},
		// enter
		{141,39,10,1}
	};
	char btext[][20] =
	{
		" 이전 페이지 ",
		" 다음 페이지 ",
		" 구매하기 ",
		" 정렬 ",
		" 1 ",
		"  ",
		" ENTER "
	};

	// 인식 버튼 개수
	int size = sizeof(bpos) / sizeof(bpos[0]);
	int sortnum = 1;
	int enter = 0;
	char search[128];

	// book txt 동적할당 구조체 배열 생성
	BOOK* book = read_books_from_file("book.txt", &numLines);
	int endpage = (numLines % listcount == 0) ? numLines / listcount : (numLines / listcount) + 1;
	int picknum = 0;		// 구매하기 선택된 객체
	int pickarray = 0;
	BOOK tmp;		// 구매 선택된 객체의 아이디

	// ui 그리기
	create_ui_buy_section(book, bpos, btext, numLines, page, size);

	sprintf(tstr, "%d", page);
	createBox(bpos[4][0], bpos[4][1], bpos[4][2], bpos[4][3], tstr);

	// 본 함수 시작
	while (1)
	{
		enter = cakr(str, &x, &y);

		if (enter == 1)
		{
			// enter 일때 7 즉 검색 되도록
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
			// 고른거 초록색
			sprintf(tstr, "%d", pickarray);
			createBoxc(5, 8 + (picknum - 1) * 3, 10, 1, tstr, GREEN);
			// 해당 데이터 저장
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

// 1.1 / 로그인 ★ 구매 판매 쪾으로 이동 필요
void login_func()
{
	system("cls");
	systemafter();

	LOGIN tmp;
	int mode = 0;

	uiDefalutSetting();


	char strr[256] = "아이디 / 비밀번호를 입력해주세요";
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
				// 결제 판매 창으로 이동
			}
			else
			{
				createBox(50, 5, defx, defy, "");
				createBox(50, 10, defx, defy, "");
				createBox(5, 35, 174, 3, "아이디 비밀번호를 재입력 해주세요");
				mode = 0;
			}
		}

		// 아이디 입력 박스
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

		// 비밀번호 입력 박스
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
// 1.2 / 아이디 생성
void create_id() 
{
	system("cls");
	systemafter();
	uiDefalutSetting();

	LOGIN tmp = { "id" , "pwd" , "name" , "credic" , "phone"};
	int mode = 0;

	strcpy(str, "");
	char strr[256] = "아이디 / 비밀번호를 입력해주세요";
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
				createBox(5, 35, 174, 3, "아이디 비밀번호를 재입력 해주세요");
				strcpy(tmp.id, "");
				strcpy(tmp.pwd, "");
				
				mode = 0;
			}
		}

		// 아이디 입력 박스
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

		// 비밀번호 입력 박스
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
			//조건 추가 하자
			insertUserTuple("user.txt", tmp);
			return 0;

		}

		// 아이디 입력 박스
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

		// 비밀번호 입력 박스
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
// 1.3.1 / 비밀번호 특수 문자 / 조건 만족 = 0 , 불만족 = 1
int check_secial_word_in_password(char* password)
{
	int special = 0;
	for (int i = 0; i < strlen(password); i++)
	{
		char c = password[i];

		// 특수문자 여부 확인
		if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*')
		{
			special = 1;
			break;
		}
	}

	// 비밀번호는 최소 8자 이상이며, 특수문자를 포함해야 함
	if (special == 1 && (strlen(password) >= 8))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
// 1.3 / 비밀번호 수정
void edit_password()
{
	system("cls");
	//systemafter();
	uiDefalutSetting();
	int mode = 0;
	LOGIN tmp;

	char strr[256] = "아이디 입력해주세요";
	createBox(10, 5, defx, defy, "아이디");
	createBox(50, 5, defx, defy, "");
	createBox(5, 35, 174, 3, strr);

	// 아이디 일치/불일치 검사
	while (1) {
		// 엔터 입력
		if (cakr(str, &x, &y))
		{
			if (user_data_check_id(&tmp))
			{
				break;
			}
			else
			{
				createBox(5, 35, 174, 3, "존재하지 않는 아이디입니다");
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

	// 이름 전화번호 일치/불일치 검사
	LOGIN comp;
	mode = 0;
	strcpy(str, "개인정보 인증 단계입니다.");
	createBox(10, 5, defx, defy, "이름");
	createBox(50, 5, defx, defy, "");
	createBox(10, 10, defx, defy, "전화번호");
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
				createBox(5, 35, 174, 3, "사용자 정보를 다시 입력해주세요.");
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

	// 비밀번호 재설정
	strcpy(str, "");
	strcpy(comp.pwd, tmp.pwd);
	strcpy(tmp.pwd, "");

	strcpy(str, "새로운 비밀번호를 입력해주세요");
	createBox(10, 15, defx, defy, "새로운 비밀번호");
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
				createBox(5, 35, 174, 3, "기존의 비밀번호와 일치합니다");
				mode = 0;
				continue;
			}
			else
			{
				strcpy(str, "");
				createBox(5, 35, 174, 3, "8자리 이상 / 특수문자를 포함해야합니다");
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

// login 메인 함수
void loginStream()
{
	uiDefalutSetting();
	createBox(10, 43, defx, defy, "로그인");
	createBox(40, 43, defx, defy, "아이디 생성");
	createBox(70, 43, defx, defy, "비밀번호 찾기");

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
			createBox(10, 43, defx, defy, "로그인");
			createBox(40, 43, defx, defy, "아이디 생성");
			createBox(70, 43, defx, defy, "비밀번호 찾기");
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
		" 이전 페이지 ",
		" 다음 페이지 ",
		" 등록 ",
		" 등록 취소 "
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