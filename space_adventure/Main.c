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
#include "ItemManager.h"

// 전역 변수
ItemSystem* itemSystem; // 아이템 시스템
Player* player; // 자주 사용하는 플레이어도 전역 변수로 선언
Enemy* enemy; // 자주 사용하는 플레이어도 전역 변수로 선언

void display_game() {
	display_game_area();
	display_status_area(player);
	render();
}

void display_map(Player* player) {
	clear_back_buffer(); // 기존 화면 지우기

	// 각 방을 3x5 크기로 정의
	const int room_height = 4;
	const int room_width = 12;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int start_x = j * (room_width + 4); // 각 방의 시작 x 좌표 (통로 공간 추가)
			int start_y = i * (room_height + 2); // 각 방의 시작 y 좌표 (통로 공간 추가)

			// 방 경계선 그리기
			draw_to_back_buffer(start_x, start_y, "------------");
			for (int k = 1; k < room_height - 1; k++) {
				char buffer[13]; // 배열 크기를 명확히 설정
				strncpy(buffer, "|          |", sizeof(buffer) - 1);
				buffer[sizeof(buffer) - 1] = '\0';
				draw_to_back_buffer(start_x, start_y + k, buffer);
			}
			draw_to_back_buffer(start_x, start_y + room_height - 1, "------------");

			// 플레이어 위치 표시
			if (i == player->y && j == player->x) {
				draw_to_back_buffer(start_x + room_width / 2, start_y + room_height / 2, "P");
			}

			// 방과 방 사이의 통로 그리기
			if (j < 2) { // 오른쪽으로 통로 추가 (마지막 열 제외)
				draw_to_back_buffer(start_x + room_width, start_y + room_height / 2, "####");
			}
			if (i < 2) { // 아래쪽으로 통로 추가 (마지막 행 제외)
				for (int k = 0; k < 4; k++) {
					draw_to_back_buffer(start_x + room_width / 2, start_y + room_height + k, "#");
				}
			}
		}
	}

	display_status_area(player);
	render();
}




void use_item(Enemy* enemy) {
	add_message("아이템을 사용하시겠습니까? : (0 : 의료키트 사용, 1 : 체력캡슐 사용)");
	add_message("(2 : 플라즈마 포 사용, 3 : 에너지 실드 사용, 4 : 빛나는 유물 사용");
	add_message("아이템 인덱스 번호를 입력해주세요 : ");
	display_game(); // 화면 출력
	int item_index = _getch();
	switch (item_index)
	{
	case '0':
		add_message("의료키트를 사용했습니다.");
		player->base.health += 10;
		break;
	case '1':
		add_message("체력캡슐을 사용했습니다.");
		player->base.health += 5;
		break;
	case '2':
		add_message("플라즈마 포를 사용했습니다");
		enemy->base.health -= 20;
		break;
	case '3':
		add_message("에너지 실드를 사용했습니다.");
		player->base.defense += 10;
		break;
	case '4':
		add_message("빛나는 유물을 사용했습니다.");
		int relics = rand() % 3 + 1;
		//빛나는 유물의 랜덤효과
		switch (relics)
		{
		case '1':
			add_message("공격력이 올라갔습니다.");
			player->base.attack += 10;
			break;
		case '2':
			add_message("체력이 회복되었습니다.");
			player->base.health += 10;
			break;
		case '3':
			add_message("방어력이 올라갔습니다.");
			player->base.defense += 10;
			break;
		}
	}
}

void battle(Enemy* enemy) {
	// 전투 시작 안내문 출력
	char* message[1000];
	snprintf(message, sizeof(message), "'%s'와 조우했다.", enemy->name);
	add_message(message);
	if (player->item_count > 0)
	{
		use_item(enemy);
	}
}

void add_random_item_to_player() {
	if (player->item_count < MAX_ITEMS) {
		int random_index = rand() % itemSystem->count;
		// 무슨 아이템을 획득했는지 메시지 출력
		char message[1000];
		snprintf(message, sizeof(message), "아이템 '%s'를 획득했습니다!", itemSystem->items[random_index].name);
		add_message(message);
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
				add_message(message);
				return;
			}
			// 강화 슈트인 경우
			else if (itemSystem->items[random_index].index == 4)
			{
				player->base.defense += itemSystem->items[random_index].value;
				char message[1000];
				snprintf(message, sizeof(message), "방어력이 %d 증가하였습니다.", itemSystem->items[random_index].value);
				add_message(message);
				return;
			}
		}
	}
	else {
		add_message("더 이상 아이템을 가져올 수 없습니다.");
	}
}
void random_event() {
	//int event_type = rand() % 5; // 네 가지 랜덤 이벤트 중 하나 발생
	int event_type = 3; // 네 가지 랜덤 이벤트 중 하나 발생
	char event_message[1000];     // 랜덤 이벤트 메시지를 저장할 변수
	int damage = (rand() % 40) + 10;

	switch (event_type) {
	case 0:
		// 신호장치 이벤트
		snprintf(event_message, sizeof(event_message), "신호장치를 발견하였습니다!");
		add_message(event_message);
		player->signal_device_count++;
		break;
	case 1:
		// 폭풍 이벤트
		snprintf(event_message, sizeof(event_message), "폭풍이 발생했습니다! 체력이 %d 감소합니다.", damage);
		add_message(event_message);
		player->base.health -= damage;
		break;
	case 2:
		// 아이템 이벤트
		snprintf(event_message, sizeof(event_message), "아이템을 발견하였습니다.");
		add_message(event_message);
		add_random_item_to_player(player);
		break;
	case 3:
		// 전투 이벤트
		snprintf(event_message, sizeof(event_message), "적을 만났습니다! 전투 시작!");
		add_message(event_message);
		Enemy* enemy = create_enemy();
		battle(enemy);
		break;
	}

	display_game();

	// 게임을 저장 중이라고 안내하고 다 끝나면 메시지들 삭제
	//save_player_data(player, "player_save.dat");
	for (int i = 0; i <= 100; i += (rand() % 10)) {
		char progress_message[100];
		snprintf(progress_message, sizeof(progress_message), "진행 상황 저장 중... %d%%", i);
		add_message(progress_message);
		display_game();
		remove_last_message();
		Sleep(100); // 0.5초 대기
	}
	remove_messages();
}

void move_room(char direction) {
	for (int i = 0; i < 3; i++) {
		char message[1000];
		snprintf(message, sizeof(message), "%s 방향으로 이동 중%.*s",
			direction == 'N' ? "북쪽" :
			direction == 'S' ? "남쪽" :
			direction == 'E' ? "동쪽" : "서쪽",
			i + 1, "...");

		remove_messages();
		add_message(message);  // 메시지 시스템에 메시지 추가
		display_game(); // 화면 출력
		Sleep(333);           // 0.333초 대기

		// 출력한 메시지 삭제 (맨 앞 메시지를 지우고 배열을 앞으로 당김)
		if (i < 2) { // 마지막 메시지는 삭제하지 않음
			remove_messages();
		}
	}
	random_event(player); // 랜덤 이벤트 발생
}



void game_loop() {
	player = create_player();
	while (player->base.health > 0) {
		srand((unsigned int)time(NULL));  // 난수 생성기 초기화
		clear_back_buffer();
		display_game(); // 화면 출력
		Sleep(1000); // 1초 대기
	}
	free(player);
}


bool is_map_open = false; // 맵 상태를 나타내는 변수

void handle_map_key(Player* player) {
	if (is_map_open) {
		// 맵이 열려있으면 맵을 닫고 원래 화면으로 돌아감
		is_map_open = false;
		display_game_area();
		display_status_area(player);
		render();
	}
	else {
		// 맵을 열음
		is_map_open = true;
		display_map(player);
	}
}



int main() {
	init_console(); // 콘솔 초기화 ( 커서 숨김, 콘솔 창 최대화)
	update_console_size(); // 콘솔 창 크기 설정

	// 아이템 시스템 생성
	itemSystem = create_item_system();
	load_items_from_file(itemSystem, "items.txt");

	// 플레이어 생성
	player = create_player();

	const int FPS = 33;
	const int frameDelay = 1000 / FPS;  // 각 프레임당 대기 시간 (밀리초)
	bool is_able_input = true;

	while (player->base.health > 0) { // 플레이어 체력이 0 이상인 동안 반복
		// 콘솔 창의 크기가 변경되었을 때 콘솔 창 크기 업데이트
		if (is_console_size_changed()) {
			update_console_size();
		}
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
				move_room('N');
				player->y--;
				break;
			case 'S':
				move_room('S');
				player->y++;
				break;
			case 'D':
				move_room('E');
				player->x++;
				break;
			case 'A':
				move_room('W');
				player->x--;
				break;
			case 'Q': // 'q'를 누르면 게임 종료
				add_message("게임 종료!");
				display_game();
				Sleep(1000);
				exit(1);
				break;
			case 'M':
				handle_map_key(player);
				break;
			default:
				break;
			}
		}
		if (player->base.health <= 0) {
			add_message("플레이어 죽음!");
			free(player);
			break;
		}
		if (player->signal_device_count == MAX_SIGNAL_DEVICE)
		{
			add_message("게임 클리어!");
			free(player);
			break;
		}
		if (is_map_open == false)
		{
		display_game(); // 화면 출력
		}
		Sleep(frameDelay); // 30fps로 고정
	}
	free_back_buffer();
	free_message_system();
	return 0;
}