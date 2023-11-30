#ifndef file_h
#define file_h

#define _CRT_SECURE_NO_WARNINGS

#define INFODATASIZE 20
#define USERDATASIZE 100
#define BOOKDATASIZE 100

typedef struct
{
	char id[INFODATASIZE];
	char pwd[INFODATASIZE];
	char name[INFODATASIZE];
	char creditc[INFODATASIZE];
	char phone[INFODATASIZE];
}LOGIN;

typedef struct
{
	char id[20];
	char title[20];
	char author[20];
	int cost;
}BOOK;

typedef struct
{
	char idb[20];
	char ids[20];
	char title[20];
	char author[20];
	int cost;
	int bs;
	int ss;
}TRADE;


BOOK* read_books_from_file(const char* filename, int* numBooks);
BOOK* read_books_from_file_id(int* userline, LOGIN user);
TRADE* read_trade_from_file(int* num);
TRADE* read_trade_from_file_id(int* num, LOGIN user);
void book_turple(const char* tuple, BOOK* book);
void del_trade_from_file_tuple(TRADE user);
void del_book_from_file_tuple(int* returnSize, BOOK tmp);


#endif