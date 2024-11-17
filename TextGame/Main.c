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
#include "MessageManager.h"
#include "ItemManager.h"

// 전역 변수
MessageSystem* messageSystem; // 메시지 시스템
ItemSystem* itemSystem; // 아이템 시스템

// 메시지를 출력하는 함수
void display_game_area() {
	clear_back_buffer();
	draw_to_back_buffer(0, 0, "===== 게임 로직 공간 =====");
	draw_to_back_buffer(0, 1, "명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동, B: 전투 시작, I: 아이템 사용)");

	for (int i = 0; i < messageSystem->capacity; i++) {
		if (messageSystem->messages[i].is_visible) {
			draw_to_back_buffer(0, i + 2, messageSystem->messages[i].content);
		}
	}
}

// 하단 플레이어 스탯 및 아이템 공간 출력 함수
void display_status_area(Player* player) {
	COORD size = get_console_size();
	char status[100];
	snprintf(status, sizeof(status), "체력: %d | 공격력: %d | 방어력: %d",
		player->base.health, player->base.attack, player->base.defense);

	draw_to_back_buffer(0, SCREEN_HEIGHT - 5, "===== 플레이어 상태 =====");
	draw_to_back_buffer(0, SCREEN_HEIGHT - 4, status);
	draw_to_back_buffer(0, SCREEN_HEIGHT - 3, "===== 보유 아이템 =====");
	// 보유 아이템 출력
	if (player->item_count > 0) {
		char item_message[1000] = { 0 };  // 모든 아이템을 저장할 메시지

		for (int i = 0; i < player->item_count; i++) {
			// 아이템 이름을 메시지에 추가
			strncat(item_message, player->items[i], sizeof(item_message) - strlen(item_message) - 1);

			// 마지막 아이템이 아닌 경우 쉼표와 공백 추가
			if (i != player->item_count - 1) {
				strncat(item_message, ", ", sizeof(item_message) - strlen(item_message) - 1);
			}
		}

		// 최종 메시지 출력 (draw_to_back_buffer는 화면 출력 함수라고 가정)
		draw_to_back_buffer(0, SCREEN_HEIGHT - 2, item_message);
	}

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


void add_random_item_to_player(Player* player) {
    if (player->item_count < MAX_ITEMS) {
        int random_index = rand() % itemSystem->count;
        snprintf(player->items[player->item_count], sizeof(player->items[player->item_count]), "%s", itemSystem->items[random_index].name);
        player->item_count++;
		// 아이템 이름을 포함한 메시지 추가
		char message[1000];
		snprintf(message, sizeof(message), "아이템 '%s'를 획득했습니다!", itemSystem->items[random_index].name);
		add_message(messageSystem, message);
	}
	else {
		add_message(messageSystem, "더 이상 아이템을 가져올 수 없습니다.");
	}
}
void random_event(Player* player) {
	//int event_type = rand() % 5; // 네 가지 랜덤 이벤트 중 하나 발생
	int event_type = 2; // 네 가지 랜덤 이벤트 중 하나 발생
	char event_message[1000];     // 랜덤 이벤트 메시지를 저장할 변수
	int damage = (rand() % 40) + 10;

	switch (event_type) {
	case 0:
		snprintf(event_message, sizeof(event_message), "신호장치를 발견하였습니다!");
		add_message(messageSystem, event_message);
		player->signal_device_count++;
		break;
	case 1:
		snprintf(event_message, sizeof(event_message), "폭풍이 발생했습니다! 체력이 %d 감소합니다.", damage);
		add_message(messageSystem, event_message);
		player->base.health -= damage;
		break;
	case 2:
		snprintf(event_message, sizeof(event_message), "아이템을 발견하였습니다.");
		add_message(messageSystem, event_message);
		add_random_item_to_player(player);
		break;
	case 3:
		if (player->item_count < MAX_ITEMS) {
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

void move_room(char direction, Player* player) {
	for (int i = 0; i < 3; i++) {
		char message[1000];
		snprintf(message, sizeof(message), "%s 방향으로 이동 중%.*s",
			direction == 'N' ? "북쪽" :
			direction == 'S' ? "남쪽" :
			direction == 'E' ? "동쪽" : "서쪽",
			i + 1, "...");

		remove_messages(messageSystem);
		add_message(messageSystem, message);  // 메시지 시스템에 메시지 추가
		display_game_area(messageSystem);     // 메시지 시스템을 화면에 출력
		display_status_area(player);          // 하단 플레이어 스탯 출력
		render();
		Sleep(333);           // 0.333초 대기

		// 출력한 메시지 삭제 (맨 앞 메시지를 지우고 배열을 앞으로 당김)
		if (i < 2) { // 마지막 메시지는 삭제하지 않음
			remove_messages(messageSystem);
		}
	}
	random_event(player); // 랜덤 이벤트 발생
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	//** 전체 화면에서 플레이 해야 버그가 없음 **//
	init_console(); // 콘솔 초기화 ( 커서 숨김, 콘솔 창 최대화)
	COORD console_size = get_console_size(); // 콘솔 창 크기 가져오기
	set_console_size(console_size.X, console_size.Y); // 콘솔 창 버퍼, 창 크기 설정
	SCREEN_WIDTH = console_size.X; // 콘솔 창 너비 설정
	SCREEN_HEIGHT = console_size.Y; // 콘솔 창 높이 설정
	init_back_buffer(); // 백 버퍼 초기화

	// 초기 메시지 시스템 생성
	messageSystem = create_message_system(SCREEN_HEIGHT - 6);  // 화면 하단을 제외한 높이만큼 메시지 저장

	// 아이템 시스템 생성
	itemSystem = create_item_system();
	load_items_from_file(itemSystem, "items.txt");

	// 플레이어 생성
	Player* player = create_player(&player);

	const int FPS = 33;
	const int frameDelay = 1000 / FPS;  // 각 프레임당 대기 시간 (밀리초)


	while (player->base.health > 0) { // 플레이어 체력이 0 이상인 동안 반복
		srand((unsigned int)time(NULL));  // 난수 생성기 초기화
		clear_back_buffer();

		display_game_area();  // 게임 로직 공간 출력
		display_status_area(player); // 하단 플레이어 스탯 및 아이템 공간 출력

		// 비블로킹 방식으로 입력 받기 (_kbhit() 사용)
		if (_kbhit()) {  // 키보드 입력이 있는지 확인
			char input = toupper(_getch());  // 키 입력을 대문자로 받기
			switch (input)
			{
			case 'W':
				move_room('N', player);
				break;
			case 'S':
				move_room('S', player);
				break;
			case 'D':
				move_room('E', player);
				break;
			case 'A':
				move_room('W', player);
				break;
			case 'Q': // 'q'를 누르면 게임 종료
				add_message("게임 종료!", messageSystem);
				player->base.health = 0;
				break;
			default:
				break;
			}
		}

		if (player->base.health <= 0) {
			display_game_area("게임 오버!");
			free(player);
			break;
		}
		if (player->signal_device_count == MAX_SIGNAL_DEVICE)
		{
			display_game_area("게임 클리어!");
			free(player);
			break;
		}
		render();			// 백 버퍼 내용을 화면에 렌더링
		Sleep(frameDelay); // 30fps로 고정
	}
	free_back_buffer();
	free_message_system(messageSystem);
	return 0;
}