#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file.h"

/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */
// 사용자 배열 파일에 전부 저장
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
// 라인 하나 넣는
void insertUserTuple(const char* filename, LOGIN login)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%s/%s\n", login.id, login.pwd, login.name, login.creditc, login.phone);
    fclose(file);
}
// 라인 하나 읽어오는
void parseTuple(const char* tuple, LOGIN* login) {
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%[^/]/%s\n", login->id, login->pwd, login->name, login->creditc, login->phone);
}
// 라인 하나 읽어오는 <trade.txt>
void parseTuple_t(const char* tuple, TRADE* t) {
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%[^/]/%d/%d/%d",
        t->idb, t->ids, t->title, t->author, &(t->cost), &(t->bs), &(t->ss));
}
// LOGIN 구조체 동적할당 
LOGIN* readDataFromFile(const char* filename, int* numLines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("파일 열기 오류\n");
        return NULL;
    }

    // 파일 라인 수 계산
    *numLines = 0;
    int ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            (*numLines)++;
        }
    }

    // 파일 포인터를 파일 시작으로 이동
    fseek(file, 0, SEEK_SET);

    // LOGIN 구조체 배열 동적 할당
    LOGIN* loginArray = (LOGIN*)malloc(*numLines * sizeof(LOGIN));
    if (loginArray == NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    // 파일에서 각 튜플을 읽어와 배열에 저장
    for (int i = 0; i < *numLines; ++i) {
        char line[1024];
        if (fgets(line, sizeof(line), file) != NULL) {
            // 개행 문자 제거
            line[strcspn(line, "\n")] = '\0';
            parseTuple(line, &loginArray[i]);
        }
    }

    fclose(file);
    return loginArray;
}
// 로그인 기능 / 일치하는 아이디 비밀번호 / 불일치 = 0 , 일치 = 1
int user_data_check_idpwd(LOGIN login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // 오류 확인
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

    // 메모리 해제
    free(loginArray);

    return 0;
}
// 로그인 기능 / 일치하는 아이디 비밀번호 / 불일치 = 0 , 일치 = 1 / 튜플 리턴
int user_data_check_id(LOGIN *login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // 오류 확인
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

    // 메모리 해제
    free(loginArray);

    return 0;
}
// 로그인 기능 / 일치하는 아이디 비밀번호 / 불일치 = 0 , 일치 = 1 
int check_duplication_id(LOGIN login) {
    int numLines;
    LOGIN* loginArray = readDataFromFile("user.txt", &numLines);
    // 오류 확인
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

    // 메모리 해제
    free(loginArray);

    return 0;
}
// 비밀번호 수정 기능 
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
/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */


// book 배열 파일에 전부 저장
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
// 책 라인 하나 읽어오는
void book_turple(const char* tuple, BOOK* book)
{
    sscanf(tuple, "%[^/]/%[^/]/%[^/]/%d", book->id, book->title, book->author, &(book->cost));
}
// 라인 하나 넣는 <book.txt>
void insert_book_turple(const char* filename, BOOK book)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%d\n", book.id, book.title, book.author, book.cost);
    fclose(file);
}
// book.txt 동적할당
BOOK* read_books_from_file(const char* filename, int* numBooks) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("파일 열기 오류\n");
        return NULL;
    }

    // 파일 라인 수 계산
    *numBooks = 0;
    int ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            (*numBooks)++;
        }
    }

    // 파일 포인터를 파일 시작으로 이동
    fseek(file, 0, SEEK_SET);

    // BOOK 구조체 배열 동적 할당
    BOOK* bookArray = (BOOK*)malloc(*numBooks * sizeof(BOOK));
    if (bookArray== NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    // 파일에서 각 튜플을 읽어와 배열에 저장
    for (int i = 0; i < *numBooks; ++i) {
        char line[1024];
        if (fgets(line, sizeof(line), file) != NULL) {
            // \n 제거
            line[strcspn(line, "\n")] = '\0';
            book_turple(line, &bookArray[i]);
        }
    }

    // 모든 도서 정보 출력

    
    fclose(file);
    return bookArray;
}
// book.txt.에서 id 일치 튜플들 반환
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

    // 파일에서 각 튜플을 읽어와 배열에 저장
    for (int i = 0; i < *userline; ++i)
    {
        strcpy(bookArray[i].id, bookuser[i].id);
        strcpy(bookArray[i].title, bookuser[i].title);
        strcpy(bookArray[i].author, bookuser[i].author);
        bookArray[i].cost = bookuser[i].cost;
    }

    if (*userline > 50)
    {
        // 최대 50개 제한
        return 0;
    }



    return bookArray;
}
// book.txt에서 일치한 튜플 삭제
void del_book_from_file_tuple(int* returnSize, BOOK tmp) {
    int arrSize;
    BOOK* arr = read_books_from_file("book.txt", &arrSize);
    // 요소를 찾아서 삭제
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

    // 요소를 찾은 경우에만 삭제 및 크기 조정
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
    // tmp와 일치하는 요소를 찾아서 배열의 맨 앞으로 이동시키기
    int j = 0;
    char test[64];
    for (int i = 0; i < arrsize; i++) 
    {
        if (strstr(arr[i].title, search)) 
        { // 여기에서 적절한 비교를 수행하세요 (id를 기준으로 예시로 했습니다)
            strcpy(test, arr[i].title);
            swap(&arr[i], &arr[j++]);
        }
    }
}

/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ */

// 라인 하나 넣는 <trade.txt>
void insert_trade_turple(const char* filename, LOGIN buy, BOOK sell, int buysign, int sellsign)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "%s/%s/%s/%s/%d/%d/%d\n", buy.id, sell.id, sell.title, sell.author, sell.cost, buysign, sellsign);
    fclose(file);
}
// trade 동적할당
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

    // 파일 포인터를 파일 시작으로 이동
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
            // 개행 문자 제거
            line[strcspn(line, "\n")] = '\0';
            parseTuple_t(line, &t[i]);
        }
    }

    fclose(file);
    return t;
}
// trade.txt에서 id 일치 튜플들 반환 <구매자 = user>
TRADE* read_trade_from_file_id(int* num, LOGIN user) {
    int idnum = 0;

    TRADE* t = read_trade_from_file(num);
    if (t == NULL) {
        // 메모리 할당 실패 등으로 인해 t가 NULL인 경우 에러 처리
        fprintf(stderr, "Error reading trades from file.\n");
        return NULL;
    }

    TRADE* r = (TRADE*)malloc(*num * sizeof(TRADE));
    if (r == NULL) {
        // 메모리 할당 실패 등으로 인해 r이 NULL인 경우 에러 처리
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

    // 메모리 크기 조절
    r = realloc(r, idnum * sizeof(TRADE));
    if (r == NULL && idnum > 0) {
        // 메모리 크기 조절 실패인데 idnum이 양수인 경우 에러 처리
        fprintf(stderr, "Error reallocating memory.\n");
        return NULL;
    }

    *num = idnum;
    return r;
}
// TRADE a b 비교 같음 = 1, 다름 = 0
int compareTrades(TRADE* a, TRADE* b) {


    if (strcmp(a->idb, b->idb) == 0 &&
        strcmp(a->ids, b->ids) == 0 &&
        strcmp(a->title, b->title) == 0 &&
        strcmp(a->author, b->author) == 0 &&
        a->cost == b->cost &&
        a->bs == b->bs &&
        a->ss == b->ss) {
        return 1; // 같음
    }
    else {
        return 0; // 다름
    }
}
// trade.txt에서 일치한 튜플 삭제
void del_trade_from_file_tuple(TRADE user)
{
    int num = 0;
    TRADE* t = read_trade_from_file(&num);
    int found = 0; // 일치하는 데이터를 찾았는지 여부를 나타내는 플래그

    for (int i = 0; i < num; i++)
    {
        if (compareTrades(&user, &t[i]))
        {
            found = 1; // 일치하는 데이터를 찾았음을 표시
            // 일치하는 튜플을 찾으면, 해당 튜플을 삭제하고 배열을 당깁니다.
            for (int j = i; j < num - 1; j++)
            {
                t[j] = t[j + 1];
            }
            (num)--;
            i--; // 다음 반복에서 현재 인덱스를 확인하기 위해 감소
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
        printf("일치하는 데이터를 찾을 수 없습니다.\n");
    }
}
// 4.1 일치한 배열 ss 수정
void trade_section_ok(TRADE tmp)
{
    int size;
    TRADE* arr = read_trade_from_file(&size);

    // tmp와 일치하는 요소를 arr 배열에서 찾기
    for (int i = 0; i < size; i++)
    {
        // strcmp 함수를 사용하여 문자열 비교
        if (strcmp(tmp.idb, arr[i].idb) == 0 &&
            strcmp(tmp.ids, arr[i].ids) == 0 &&
            strcmp(tmp.title, arr[i].title) == 0 &&
            strcmp(tmp.author, arr[i].author) == 0 &&
            tmp.cost == arr[i].cost &&
            tmp.bs == arr[i].bs &&
            tmp.ss == arr[i].ss)
        {
            // tmp와 일치하는 경우, 해당 요소의 ss를 1로 설정
            arr[i].ss = 1;
            break; // 일치하는 경우를 찾았으므로 루프 종료
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
