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
	draw_to_back_buffer(0, 1, "명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동)");

	for (int i = 0; i < messageSystem->count; i++) {
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
	
	char massage[1000];
	snprintf(massage, sizeof(massage), "사이즈: %d , %d", size.X, size.Y);
	draw_to_back_buffer(0, size.Y - 6, massage);
	draw_to_back_buffer(0, SCREEN_HEIGHT - 5, "===== 플레이어 상태 =====");
	draw_to_back_buffer(0, SCREEN_HEIGHT - 4, status);
	draw_to_back_buffer(0, SCREEN_HEIGHT - 3, "===== 보유 아이템 =====");
	// 보유 아이템 출력
	if (player->item_count > 0) {
		char item_message[1000] = { 0 };  // 모든 아이템을 저장할 메시지

		for (int i = 0; i < player->item_count; i++) {
			// 아이템 이름을 메시지에 추가
			strncat(item_message, player->inventory[i].name, sizeof(item_message) - strlen(item_message) - 1);

			// 마지막 아이템이 아닌 경우 쉼표와 공백 추가
			if (i != player->item_count - 1) {
				strncat(item_message, ", ", sizeof(item_message) - strlen(item_message) - 1);
			}
		}

		// 최종 메시지 출력 (draw_to_back_buffer는 화면 출력 함수라고 가정)
		draw_to_back_buffer(0, SCREEN_HEIGHT - 2, item_message);
	}

}


void use_item(Player* player, Enemy* enemy) {
	bool is_able_input = true;
	add_message(messageSystem, "아이템을 사용하시겠습니까? : (0 : 의료키트 사용, 1 : 체력캡슐 사용)");
	add_message(messageSystem, "(2 : 플라즈마 포 사용, 3 : 에너지 실드 사용, 4 : 빛나는 유물 사용");
	add_message(messageSystem, "아이템 인덱스 번호를 입력해주세요 : ");
	display_game_area();
	display_status_area(player);
	render();
	is_able_input = false;
	int item_index = _getch();
	switch (item_index)
	{
	case '0':
		add_message(messageSystem, "의료키트를 사용했습니다.");
		player->base.health += 10;
		break;
	case '1':
		add_message(messageSystem, "체력캡슐을 사용했습니다.");
		player->base.health += 5;
		break;
	case '2':
		add_message(messageSystem, "플라즈마 포를 사용했습니다");
		enemy->base.health -= 20;
		break;
	case '3':
		add_message(messageSystem, "에너지 실드를 사용했습니다.");
		player->base.defense += 10;
		break;
	case '4':
		add_message(messageSystem, "빛나는 유물을 사용했습니다.");
		int relics = rand() % 3 + 1;
		//빛나는 유물의 랜덤효과
		switch (relics)
		{
		case '1':
			add_message(messageSystem, "공격력이 올라갔습니다.");
			player->base.attack += 10;
			break;
		case '2':
			add_message(messageSystem, "체력이 회복되었습니다.");
			player->base.health += 10;
			break;
		case '3':
			add_message(messageSystem, "방어력이 올라갔습니다.");
			player->base.defense += 10;
			break;
		}
	}
}

void battle(Player* player, Enemy* enemy) {
	// 전투 시작 안내문 출력
	char* message[1000];
	snprintf(message, sizeof(message), "'%s'와 조우했다.", enemy->name);
	add_message(messageSystem, message);
	if (player->item_count > 0)
	{
		use_item(player, enemy);
	}
}

void add_random_item_to_player(Player* player) {
    if (player->item_count < MAX_ITEMS) {
        int random_index = rand() % itemSystem->count;
		// 무슨 아이템을 획득했는지 메시지 출력
		char message[1000];
		snprintf(message, sizeof(message), "아이템 '%s'를 획득했습니다!", itemSystem->items[random_index].name);
		add_message(messageSystem, message);
		// 소모품 아이템인 경우 획득하면 플레이어 인벤토리에 추가
		if (itemSystem->items[random_index].type == 0) {
			player->inventory[player->item_count].index = itemSystem->items[random_index].index;
			// 아이템 이름을 복사
			strncpy(player->inventory[player->item_count].name, itemSystem->items[random_index].name, sizeof(player->inventory[player->item_count].name) - 1);
			// 문자열 끝에 NULL 문자 추가
			player->inventory[player->item_count].name[sizeof(player->inventory[player->item_count].name) - 1] = '\0';

			player->inventory[player->item_count].type = itemSystem->items[random_index].type;
			player->inventory[player->item_count].value = itemSystem->items[random_index].value;
			player->item_count++;
		}
		// 장비 아이템인 경우
		if (itemSystem->items[random_index].type == 1)
		{
			// 에너지 블레이드인 경우
			if (itemSystem->items[random_index].index == 2)
			{
				player->base.attack += itemSystem->items[random_index].value;
				char message[1000];
				snprintf(message, sizeof(message), "공격력이 %d 증가하였습니다.", itemSystem->items[random_index].value);
				add_message(messageSystem, message);
				return;
			}
			// 강화 슈트인 경우
			else if (itemSystem->items[random_index].index == 4)
			{
				player->base.defense += itemSystem->items[random_index].value;
				char message[1000];
				snprintf(message, sizeof(message), "방어력이 %d 증가하였습니다.", itemSystem->items[random_index].value);
				add_message(messageSystem, message);
				return;
			}
		}
	}
	else {
		add_message(messageSystem, "더 이상 아이템을 가져올 수 없습니다.");
	}
}
void random_event(Player* player) {
	//int event_type = rand() % 5; // 네 가지 랜덤 이벤트 중 하나 발생
	int event_type = 3; // 네 가지 랜덤 이벤트 중 하나 발생
	char event_message[1000];     // 랜덤 이벤트 메시지를 저장할 변수
	int damage = (rand() % 40) + 10;

	switch (event_type) {
	case 0:
		// 신호장치 이벤트
		snprintf(event_message, sizeof(event_message), "신호장치를 발견하였습니다!");
		add_message(messageSystem, event_message);
		player->signal_device_count++;
		break;
	case 1:
		// 폭풍 이벤트
		snprintf(event_message, sizeof(event_message), "폭풍이 발생했습니다! 체력이 %d 감소합니다.", damage);
		add_message(messageSystem, event_message);
		player->base.health -= damage;
		break;
	case 2:
		// 아이템 이벤트
		snprintf(event_message, sizeof(event_message), "아이템을 발견하였습니다.");
		add_message(messageSystem, event_message);
		add_random_item_to_player(player);
		break;
	case 3:
		// 전투 이벤트
		snprintf(event_message, sizeof(event_message), "적을 만났습니다! 전투 시작!");
		add_message(messageSystem, event_message);
		Enemy* enemy = create_enemy();
		battle(player, enemy);
		break;
	}

	display_game_area();
	display_status_area(player);
	render();

	// 메시지 삭제 진행 표시
	//for (int i = 0; i <= 100; i += 5) {
	//	char progress_message[100];
	//	snprintf(progress_message, sizeof(progress_message), "이벤트 종료 중... %d%%", i);
	//	add_message(messageSystem, progress_message);
	//	display_game_area();
	//	display_status_area(player);
	//	render();
	//	remove_last_message(messageSystem);
	//	Sleep(50); // 0.5초 대기
	//}
	//Sleep(1000); // 1초 대기
	//remove_messages(messageSystem);
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
	bool is_able_input = true;

	while (player->base.health > 0) { // 플레이어 체력이 0 이상인 동안 반복
		srand((unsigned int)time(NULL));  // 난수 생성기 초기화
		is_able_input = true; // 입력 가능 상태로 변경
		clear_back_buffer();


		// 비블로킹 방식으로 입력 받기 (_kbhit() 사용)
		if (_kbhit() && is_able_input) {  // 키보드 입력이 있는지 확인 && 입력 가능 상태인지 확인
			is_able_input = false; // 입력 불가능 상태로 변경
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
				add_message(messageSystem, "게임 종료!");
				break;
			default:
				break;
			}
		}
		if (player->base.health <= 0) {
			add_message(messageSystem, "플레이어 죽음!");
			free(player);
			break;
		}
		if (player->signal_device_count == MAX_SIGNAL_DEVICE)
		{
			add_message(messageSystem, "게임 클리어!");
			free(player);
			break;
		}
		display_game_area();  // 게임 로직 공간 출력
		display_status_area(player); // 하단 플레이어 스탯 및 아이템 공간 출력
		render();			// 백 버퍼 내용을 화면에 렌더링
		Sleep(frameDelay); // 30fps로 고정
	}
	Sleep(2000);
	free_back_buffer();
	free_message_system(messageSystem);
	return 0;
}