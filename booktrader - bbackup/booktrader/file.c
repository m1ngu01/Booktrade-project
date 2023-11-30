#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file.h"

/* �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� */
// ����� �迭 ���Ͽ� ���� ����
void user_data_insert_array(const char* filename, const LOGIN* login, int numLines)
{
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        exit(1);
    }

    for (int i = 0; i < numLines; i++) {
        fprintf(file, "%s/%s/%s/%s/%s\n", login[i].id, login[i].pwd, login[i].name, login[i].creditc, login[i].phone);
    }

    fclose(file);
}
// ���� �ϳ� �ִ�
void insertUserTuple(const char* filename, LOGIN login)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%s/%s\n", login.id, login.pwd, login.name, login.creditc, login.phone);
    fclose(file);
}
// ���� �ϳ� �о����
void parseTuple(const char* tuple, LOGIN* login) {
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%[^/]/%s\n", login->id, login->pwd, login->name, login->creditc, login->phone);
}
// ���� �ϳ� �о���� <trade.txt>
void parseTuple_t(const char* tuple, TRADE* t) {
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%[^/]/%d/%d/%d",
        t->idb, t->ids, t->title, t->author, &(t->cost), &(t->bs), &(t->ss));
}
// LOGIN ����ü �����Ҵ� 
LOGIN* readDataFromFile(const char* filename, int* numLines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("���� ���� ����\n");
        return NULL;
    }

    // ���� ���� �� ���
    *numLines = 0;
    int ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            (*numLines)++;
        }
    }

    // ���� �����͸� ���� �������� �̵�
    fseek(file, 0, SEEK_SET);

    // LOGIN ����ü �迭 ���� �Ҵ�
    LOGIN* loginArray = (LOGIN*)malloc(*numLines * sizeof(LOGIN));
    if (loginArray == NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    // ���Ͽ��� �� Ʃ���� �о�� �迭�� ����
    for (int i = 0; i < *numLines; ++i) {
        char line[1024];
        if (fgets(line, sizeof(line), file) != NULL) {
            // ���� ���� ����
            line[strcspn(line, "\n")] = '\0';
            parseTuple(line, &loginArray[i]);
        }
    }

    fclose(file);
    return loginArray;
}
// �α��� ��� / ��ġ�ϴ� ���̵� ��й�ȣ / ����ġ = 0 , ��ġ = 1
int user_data_check_idpwd(LOGIN login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // ���� Ȯ��
    if (loginArray == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < numLines; i++)
    {
        if (!strcmp(loginArray[i].id, login.id))
        {
            if (!strcmp(loginArray[i].pwd, login.pwd))
            {
                return 1;
            }
        }
    }

    // �޸� ����
    free(loginArray);

    return 0;
}
// �α��� ��� / ��ġ�ϴ� ���̵� ��й�ȣ / ����ġ = 0 , ��ġ = 1 / Ʃ�� ����
int user_data_check_id(LOGIN *login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // ���� Ȯ��
    if (loginArray == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < numLines; i++)
    {
        if (!strcmp(loginArray[i].id, login->id))
        {
            strcpy(login->id, loginArray[i].id);
            strcpy(login->pwd, loginArray[i].pwd);
            strcpy(login->name, loginArray[i].name);
            strcpy(login->creditc, loginArray[i].creditc);
            strcpy(login->phone, loginArray[i].phone);
            return 1;
        }
    }

    // �޸� ����
    free(loginArray);

    return 0;
}
// �α��� ��� / ��ġ�ϴ� ���̵� ��й�ȣ / ����ġ = 0 , ��ġ = 1 
int check_duplication_id(LOGIN login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // ���� Ȯ��
    if (loginArray == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < numLines; i++)
    {
        if (!strcmp(loginArray[i].id, login.id))
        {
            return 1;
        }
    }

    // �޸� ����
    free(loginArray);

    return 0;
}
// ��й�ȣ ���� ��� 
void user_data_edit_pwd(LOGIN tmp)
{
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);

    for (int i = 0; i < numLines; i++)
    {
        if (!strcmp(tmp.id, loginArray[i].id))
        {
            strcpy(loginArray[i].pwd, tmp.pwd);
        }
    }

    user_data_insert_array("user.txt", loginArray, numLines);

    free(loginArray);
}
/* �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� */


// book �迭 ���Ͽ� ���� ����
void book_data_insert_array(const char* filename, const BOOK* arr, int numLines)
{
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        exit(1);
    }


    for (int i = 0; i < numLines; i++) {
        fprintf(file, "%s/%s/%s/%d\n", arr[i].id, arr[i].title, arr[i].author, arr[i].cost);
    }

    fclose(file);
}
// å ���� �ϳ� �о����
void book_turple(const char* tuple, BOOK* book)
{
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%d", book->id, book->title, book->author, &(book->cost));
}
// ���� �ϳ� �ִ� <book.txt>
void insert_book_turple(const char* filename, BOOK book)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%d\n", book.id, book.title, book.author, book.cost);
    fclose(file);
}
// book.txt �����Ҵ�
BOOK* read_books_from_file(const char* filename, int* numBooks) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("���� ���� ����\n");
        return NULL;
    }

    // ���� ���� �� ���
    *numBooks = 0;
    int ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            (*numBooks)++;
        }
    }

    // ���� �����͸� ���� �������� �̵�
    fseek(file, 0, SEEK_SET);

    // BOOK ����ü �迭 ���� �Ҵ�
    BOOK* bookArray = (BOOK*)malloc(*numBooks * sizeof(BOOK));
    if (bookArray== NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    // ���Ͽ��� �� Ʃ���� �о�� �迭�� ����
    for (int i = 0; i < *numBooks; ++i) {
        char line[1024];
        if (fgets(line, sizeof(line), file) != NULL) {
            // \n ����
            line[strcspn(line, "\n")] = '\0';
            book_turple(line, &bookArray[i]);
        }
    }

    // ��� ���� ���� ���

    
    fclose(file);
    return bookArray;
}
// book.txt.���� id ��ġ Ʃ�õ� ��ȯ
BOOK* read_books_from_file_id(int* userline, LOGIN user) {

    int numBooks = 0;
    *userline = 0;

    BOOK* book = read_books_from_file("book.txt", &numBooks);
    BOOK bookuser[50];

    for (int i = 0; i < numBooks; i++)
    {
        if (!strcmp(user.id, book[i].id))
        {
            strcpy(bookuser[*userline].id, book[i].id);
            strcpy(bookuser[*userline].title, book[i].title);
            strcpy(bookuser[*userline].author, book[i].author);
            bookuser[*userline].cost = book[i].cost;
            (*userline)++;
        }
    }

    free(book);

    BOOK* bookArray = (BOOK*)malloc(*userline * sizeof(BOOK));
    if (bookArray == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    // ���Ͽ��� �� Ʃ���� �о�� �迭�� ����
    for (int i = 0; i < *userline; ++i)
    {
        strcpy(bookArray[i].id, bookuser[i].id);
        strcpy(bookArray[i].title, bookuser[i].title);
        strcpy(bookArray[i].author, bookuser[i].author);
        bookArray[i].cost = bookuser[i].cost;
    }

    if (*userline > 50)
    {
        // �ִ� 50�� ����
        return 0;
    }



    return bookArray;
}
// book.txt���� ��ġ�� Ʃ�� ����
void del_book_from_file_tuple(int* returnSize, BOOK tmp) {
    int arrSize;
    BOOK* arr = read_books_from_file("book.txt", &arrSize);
    // ��Ҹ� ã�Ƽ� ����
    int foundIndex = -1;
    for (int i = 0; i < arrSize; i++) {
        if (strcmp(arr[i].id, tmp.id) == 0 &&
            strcmp(arr[i].title, tmp.title) == 0 &&
            strcmp(arr[i].author, tmp.author) == 0 &&
            (arr[i].cost == tmp.cost) ) {
            foundIndex = i;
            break;
        }
    }

    // ��Ҹ� ã�� ��쿡�� ���� �� ũ�� ����
    if (foundIndex != -1) {
        for (int i = foundIndex; i < arrSize - 1; i++) {
            arr[i] = arr[i + 1];
        }
        (arrSize)--;
        (*returnSize)--;
    }

    book_data_insert_array("book.txt", arr, arrSize);
}
// search tmp in arr
void swap(BOOK* a, BOOK* b) {
    BOOK temp = *a;
    *a = *b;
    *b = temp;
}
void buy_section_search(BOOK arr[], int arrsize, const char* search) {
    // tmp�� ��ġ�ϴ� ��Ҹ� ã�Ƽ� �迭�� �� ������ �̵���Ű��
    int j = 0;
    char test[64];
    for (int i = 0; i < arrsize; i++) 
    {
        if (strstr(arr[i].title, search)) 
        { // ���⿡�� ������ �񱳸� �����ϼ��� (id�� �������� ���÷� �߽��ϴ�)
            strcpy(test, arr[i].title);
            swap(&arr[i], &arr[j++]);
        }
    }
}

/* �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ� */

// ���� �ϳ� �ִ� <trade.txt>
void insert_trade_turple(const char* filename, LOGIN buy, BOOK sell, int buysign, int sellsign)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%s/%d/%d/%d\n", buy.id, sell.id, sell.title, sell.author, sell.cost, buysign, sellsign);
    fclose(file);
}
// trade �����Ҵ�
TRADE* read_trade_from_file(int* num)
{
    FILE* file = fopen("trade.txt", "r");
    // check error
    if (file == NULL)
    {
        return NULL;
    }

    //return file line
    *num = 0;
    int ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            (*num)++;
        }
    }
    if (*num != 0)
    {
        (*num)++;
    }

    // ���� �����͸� ���� �������� �̵�
    fseek(file, 0, SEEK_SET);

    TRADE* t = (TRADE*)malloc(*num * sizeof(TRADE));
    if (t == NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    char line[1024];
    for (int i = 0; i < *num; ++i) {
        if (fgets(line, sizeof(line), file) != NULL) {
            // ���� ���� ����
            line[strcspn(line, "\n")] = '\0';
            parseTuple_t(line, &t[i]);
        }
    }

    fclose(file);
    return t;
}
// trade.txt���� id ��ġ Ʃ�õ� ��ȯ <������ = user>
TRADE* read_trade_from_file_id(int* num, LOGIN user) {
    int idnum = 0;

    TRADE* t = read_trade_from_file(num);
    if (t == NULL) {
        // �޸� �Ҵ� ���� ������ ���� t�� NULL�� ��� ���� ó��
        fprintf(stderr, "Error reading trades from file.\n");
        return NULL;
    }

    TRADE* r = (TRADE*)malloc(*num * sizeof(TRADE));
    if (r == NULL) {
        // �޸� �Ҵ� ���� ������ ���� r�� NULL�� ��� ���� ó��
        fprintf(stderr, "Error allocating memory.\n");
        free(t);
        return NULL;
    }

    for (int i = 0; i < *num; i++) {
        if (!strcmp(user.id, t[i].idb) || !strcmp(user.id, t[i].ids)) {
            r[idnum] = t[i];
            idnum++;
        }
    }
    free(t);

    // �޸� ũ�� ����
    r = realloc(r, idnum * sizeof(TRADE));
    if (r == NULL && idnum > 0) {
        // �޸� ũ�� ���� �����ε� idnum�� ����� ��� ���� ó��
        fprintf(stderr, "Error reallocating memory.\n");
        return NULL;
    }

    *num = idnum;
    return r;
}
// TRADE a b �� ���� = 1, �ٸ� = 0
int compareTrades(TRADE* a, TRADE* b) {


    if (strcmp(a->idb, b->idb) == 0 &&
        strcmp(a->ids, b->ids) == 0 &&
        strcmp(a->title, b->title) == 0 &&
        strcmp(a->author, b->author) == 0 &&
        a->cost == b->cost &&
        a->bs == b->bs &&
        a->ss == b->ss) {
        return 1; // ����
    }
    else {
        return 0; // �ٸ�
    }
}
// trade.txt���� ��ġ�� Ʃ�� ����
void del_trade_from_file_tuple(TRADE user)
{
    int num = 0;
    TRADE* t = read_trade_from_file(&num);
    int found = 0; // ��ġ�ϴ� �����͸� ã�Ҵ��� ���θ� ��Ÿ���� �÷���

    for (int i = 0; i < num; i++)
    {
        if (compareTrades(&user, &t[i]))
        {
            found = 1; // ��ġ�ϴ� �����͸� ã������ ǥ��
            // ��ġ�ϴ� Ʃ���� ã����, �ش� Ʃ���� �����ϰ� �迭�� ���ϴ�.
            for (int j = i; j < num - 1; j++)
            {
                t[j] = t[j + 1];
            }
            (num)--;
            i--; // ���� �ݺ����� ���� �ε����� Ȯ���ϱ� ���� ����
            break;
        }
    }

    //TRADE checktmp;

    if (found)
    {
        FILE* file = fopen("trade.txt", "w");

        for (int i = 0; i < num - 1; i++)
        {
            //checktmp = t[i];
            fprintf(file, "%s/%s/%s/%s/%d/%d/%d\n", t[i].idb, t[i].ids, t[i].title, t[i].author, t[i].cost, t[i].bs, t[i].ss);
        }
        fclose(file);
    }
    else
    {
        printf("��ġ�ϴ� �����͸� ã�� �� �����ϴ�.\n");
    }
}
// 4.1 ��ġ�� �迭 ss ����
void trade_section_ok(TRADE tmp)
{
    int size;
    TRADE* arr = read_trade_from_file(&size);

    // tmp�� ��ġ�ϴ� ��Ҹ� arr �迭���� ã��
    for (int i = 0; i < size; i++)
    {
        // strcmp �Լ��� ����Ͽ� ���ڿ� ��
        if (strcmp(tmp.idb, arr[i].idb) == 0 &&
            strcmp(tmp.ids, arr[i].ids) == 0 &&
            strcmp(tmp.title, arr[i].title) == 0 &&
            strcmp(tmp.author, arr[i].author) == 0 &&
            tmp.cost == arr[i].cost &&
            tmp.bs == arr[i].bs &&
            tmp.ss == arr[i].ss)
        {
            // tmp�� ��ġ�ϴ� ���, �ش� ����� ss�� 1�� ����
            arr[i].ss = 1;
            break; // ��ġ�ϴ� ��츦 ã�����Ƿ� ���� ����
        }
    }
    FILE* file = fopen("trade.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < size - 1; i++) {
        fprintf(file, "%s/%s/%s/%s/%d/%d/%d\n",
            arr[i].idb, arr[i].ids, arr[i].title, arr[i].author, arr[i].cost, arr[i].bs, arr[i].ss);
    }

    free(arr);
    fclose(file);
}
