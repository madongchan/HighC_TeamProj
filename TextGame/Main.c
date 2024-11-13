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
#include "Canvas.h"

#define MAX_MESSAGES 10
#define MESSAGE_LENGTH 100

// 메시지 구조체 정의
typedef struct {
	char content[MESSAGE_LENGTH];
	bool is_visible;  // 표시 여부를 나타내는 플래그
} Message;

// 메시지 배열 및 메시지 개수
Message messages[MAX_MESSAGES];
int message_count = 0;

bool is_updated = false;  // 상태 변경 플래그

// 메시지 추가 함수
void add_message(const char* text) {
	if (message_count >= MAX_MESSAGES) {
		// 가장 오래된 메시지를 삭제하고 메시지들을 앞으로 이동
		for (int i = 1; i < MAX_MESSAGES; i++) {
			messages[i - 1] = messages[i];
		}
		message_count = MAX_MESSAGES - 1;
	}

	// 새 메시지를 추가하고 표시 상태를 true로 설정
	strncpy(messages[message_count].content, text, MESSAGE_LENGTH - 1);
	messages[message_count].content[MESSAGE_LENGTH - 1] = '\0';
	messages[message_count].is_visible = true;  // 새 메시지를 표시하도록 설정
	message_count++;
}


// 메시지를 출력하는 함수
void display_game_area() {
	clear_back_buffer();
	draw_to_back_buffer(0, 0, "===== 게임 로직 공간 =====");
	draw_to_back_buffer(0, 1, "명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동, B: 전투 시작, I: 아이템 사용)");

	for (int i = 0; i < message_count; i++) {
		if (messages[i].is_visible) {
			draw_to_back_buffer(1, i + 2, messages[i].content);
		}
	}
}



// 하단 플레이어 스탯 및 아이템 공간 출력 함수
void display_status_area(Player* player) {
	COORD console_size = get_console_size();

	char status[100];

	snprintf(status, sizeof(status), "체력: %d | 공격력: %d | 방어력: %d",
		player->base.health, player->base.attack, player->base.defense);

	draw_to_back_buffer(0, console_size.Y - 6, "===== 플레이어 상태 =====");
	draw_to_back_buffer(0, console_size.Y - 5, status);
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
	int event_type = rand() % 4; // 네 가지 랜덤 이벤트 중 하나 발생
	char event_message[100];     // 랜덤 이벤트 메시지를 저장할 변수

	switch (event_type) {
	case 0:
		snprintf(event_message, sizeof(event_message), "외계 광물을 발견했습니다! 공격력이 증가합니다.");
		player->base.attack += 2;
		break;
	case 1:
		snprintf(event_message, sizeof(event_message), "폭풍이 발생했습니다! 체력이 감소합니다.");
		player->base.health -= 5;
		break;
	case 2:
		snprintf(event_message, sizeof(event_message), "친근한 외계 생명체와 만났습니다! 정보를 얻었습니다.");
		break;
	case 3:
		if (player->item_count < 10) {
			snprintf(event_message, sizeof(event_message), "아이템 상자를 발견했습니다! '의료 키트'를 획득합니다.");
			snprintf(player->items[player->item_count], sizeof(player->items[player->item_count]), "의료 키트");
			player->item_count++;
		}
		else {
			snprintf(event_message, sizeof(event_message), "더 이상 아이템을 획득할 수 없습니다.");
		}
		break;
	default:
		snprintf(event_message, sizeof(event_message), "아무 일도 일어나지 않았습니다.");
		break;
	}

	// 백 버퍼에 랜덤 이벤트 메시지 그리기
	display_game_area(event_message);
}

void move_room(char direction) {
	switch (direction) {
	case 'N':
		add_message("북쪽 방향으로 이동 중...");
		break;
	case 'S':
		add_message("남쪽 방향으로 이동 중...");
		break;
	case 'E':
		add_message("동쪽 방향으로 이동 중...");
		break;
	case 'W':
		add_message("서쪽 방향으로 이동 중...");
		break;
	default:
		add_message("잘못된 방향입니다.");
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

	init_console();              // 콘솔 초기화

	while (player->base.health > 0) { // 플레이어 체력이 0 이상인 동안 반복
		clear_back_buffer();

		display_game_area();  // 게임 로직 공간 출력
		display_status_area(player); // 하단 플레이어 스탯 및 아이템 공간 출력

		// 비블로킹 방식으로 입력 받기 (_kbhit() 사용)
		if (_kbhit()) {  // 키보드 입력이 있는지 확인
			char input = toupper(_getch());  // 키 입력을 대문자로 받기
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
				add_message("게임 종료\n");
				player->base.health = 0;
				break;
			default:
				break;
			}
			//random_event(player); // 이동 중 랜덤 이벤트 발생 가능
		}

		if (player->base.health <= 0) {
			display_game_area("게임 오버!");
			free(player);
			break;
		}
		render();			// 백 버퍼 내용을 화면에 렌더링
		Sleep(frameDelay); // 30fps로 고정
	}

	return 0;
}