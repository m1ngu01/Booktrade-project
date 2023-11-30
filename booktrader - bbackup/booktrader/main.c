#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include "file.h"

void systemSetting()
{
	system("mode con:cols=200 lines=50");
	systemafter();
}

void mainStream()
{
	systemSetting();
	loginStream();
}

int main()
{
	
	

	/*
	LOGIN user = {"1234","1234","1234","1234"};
	choose_bs_(user);
	return 0;
	*/

	mainStream();
}

/*
* 삭제 후 endpage 재설정	o
* 등록 후 endapge 재설정	o
* trade.txt에서 구매요청은 수락해야 하는가?	o
* 거래요청 -> 나가기 좌표 초기화	o
* 정렬 page옆에 작게 버튼 하나 만드는걸로 예정	o
* trade section에 user id 관련된 데이터 출력 부분에서 요소 클릭 시 구매 상태 꼬이는거 해결	o
* 
* 2023.11.30 최종 버전 완
* -> 로직 오류 해결해야함
* -> 정렬시 시각적 효과 추가
*/