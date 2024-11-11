#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>


#include "Utils.h"
#include "Player.h"
#include "Enemy.h"

#define MAX_MESSAGES 10          // 메시지 저장할 최대 개수
#define MESSAGE_LENGTH 100       // 각 메시지의 최대 길이

// 게임 로직 메시지 저장 공간
char messages[MAX_MESSAGES][MESSAGE_LENGTH];
int message_count = 0;

void display_game_area(const char* message) {
	// 메시지가 가득 찬 경우 가장 오래된 메시지 제거
	if (message_count >= MAX_MESSAGES) {
		for (int i = 1; i < MAX_MESSAGES; i++) {
			strcpy(messages[i - 1], messages[i]);
		}
		message_count = MAX_MESSAGES - 1;
	}

	// 새로운 메시지 추가
	strncpy(messages[message_count], message, MESSAGE_LENGTH - 1);
	messages[message_count][MESSAGE_LENGTH - 1] = '\0'; // 문자열 끝을 널 문자로 설정
	message_count++;

	// 화면 상단에 메시지 출력
	set_cursor_position(0, 0);
	printf("===== 게임 로직 공간 =====\n");
	for (int i = 0; i < message_count; i++) {
		printf("%s\n", messages[i]);
	}
	printf("=========================\n\n");
}

// 하단 플레이어 스탯 및 아이템 공간 출력 함수
void display_status_area(Player* player) {
	COORD console_size = get_console_size();
	set_cursor_position(0, console_size.Y - 6); // 맨 아래에서 6줄 위로 커서 이동 (스탯 및 아이템 공간 확보)

	printf("\n===== 플레이어 상태 =====\n");
	printf("체력: %d\n", player->base.health);
	printf("공격력: %d\n", player->base.attack);
	printf("방어력: %d\n", player->base.defense);

	printf("\n===== 소지 아이템 =====\n");
	for (int i = 0; i < player->item_count; ++i) {
		printf("%s\n", player->items[i]);
	}
	printf("=========================\n");
}


void battle(Player* player, Enemy* enemy) {
	printf("전투 시작! %s와(과) 싸웁니다!\n", enemy->name);

	while (player->base.health > 0 && enemy->base.health > 0) {
		player->base.attack_func(&player, &enemy);  // 플레이어가 적을 공격
		printf("적 남은 체력: %d\n", enemy->base.health);

		printf("적 남은 체력: %d\n", enemy->base.health);
		enemy->base.attack_func(&enemy, &player);   // 적이 플레이어를 공격
		printf("플레이어 남은 체력: %d\n", player->base.health);

		if (player->base.health <= 0) {
			printf("플레이어가 사망했습니다...\n");
			break;
		}
	}
}

void use_item(Player* player, char* item_name) {
	if (strcmp(item_name, "의료 키트") == 0) {
		player->base.health += 10; // 체력 회복
		printf("의료 키트를 사용하여 체력이 회복되었습니다! 현재 체력: %d\n", player->base.health);
	}
	else if (strcmp(item_name, "에너지 블레이드") == 0) {
		player->base.attack += 5; // 공격력 증가
		printf("에너지 블레이드를 사용하여 공격력이 증가했습니다! 현재 공격력: %d\n", player->base.attack);
	}
	else if (strcmp(item_name, "강화 슈트") == 0) {
		player->base.defense += 5; // 방어력 증가
		printf("강화 슈트를 착용하여 방어력이 증가했습니다! 현재 방어력: %d\n", player->base.defense);
	}
	else {
		printf("알 수 없는 아이템입니다.\n");
	}
}

void random_event(Player* player) {
	int event_type = rand() % 4; // 세 가지 랜덤 이벤트 중 하나 발생

	switch (event_type) {
	case 0:
		printf("외계 광물을 발견했습니다! 공격력이 증가합니다.\n");
		player->base.attack += 2;
		break;
	case 1:
		printf("폭풍이 발생했습니다! 체력이 감소합니다.\n");
		player->base.health -= 5;
		break;
	case 2:
		printf("친근한 외계 생명체와 만났습니다! 정보를 얻었습니다.\n");
		break;
	case 3:
		printf("아이템 상자를 발견했습니다! 아이템을 획득합니다.\n");
		if (player->item_count < 10) {
			snprintf(player->items[player->item_count], sizeof(player->items[player->item_count]), "의료 키트");
			player->item_count++;
		}
		else {
			printf("더 이상 아이템을 획득할 수 없습니다.\n");
		}
		break;
	default:
		//battle(player, createEnemy());
		break;
	}
}

void move_room(char direction) {
	switch (direction) {
	case 'N':
		display_game_area("북쪽 방향으로 이동 중...");
		break;
	case 'S':
		display_game_area("남쪽 방향으로 이동 중...");
		break;
	case 'E':
		display_game_area("동쪽 방향으로 이동 중...");
		break;
	case 'W':
		display_game_area("서쪽 방향으로 이동 중...");
		break;
	default:
		display_game_area("잘못된 방향입니다.");
		break;
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	// 플레이어 생성
	Player* player = createPlayer(&player);

	const int FPS = 33;
	const int frameDelay = 1000 / FPS;  // 각 프레임당 대기 시간 (밀리초)

	char input = '\0';  // 사용자 입력 변수

	while (player->base.health > 0) { // 플레이어 체력이 0 이상인 동안 반복

		clear_screen();
		display_game_area("명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동, B: 전투 시작, I: 아이템 사용)");
		// 하단 플레이어 상태 및 아이템 정보 출력 (항상 하단에 고정)
		display_status_area(player);

		// 비블로킹 방식으로 입력 받기 (_kbhit() 사용)
		if (_kbhit()) {  // 키보드 입력이 있는지 확인
			input = toupper(_getch());  // 키 입력을 대문자로 받기
			switch (input)
			{
			case 'N':
				move_room('N');
				break;
			case 'S':
				move_room('S');
				break;
			case 'E':
				move_room('E');
				break;
			case 'W':
				move_room('W');
				break;
			case 'Q': // 'q'를 누르면 게임 종료
				display_game_area("게임 종료\n");
				break;
			default:
				break;
			}
			random_event(player); // 이동 중 랜덤 이벤트 발생 가능
		}

		if (player->base.health <= 0) {
			display_game_area("게임 오버!");
			free(player);
			break;
		}
		Sleep(frameDelay); // 30fps로 고정
	}

	return 0;
}