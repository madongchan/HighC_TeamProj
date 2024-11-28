#pragma once
#include <windows.h>
#include <stdio.h>
#include "Utils.h"
#include "MessageManager.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

// 더블 버퍼링을 위한 백 버퍼 정의
char** backBuffer;

void free_back_buffer() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        free(backBuffer[i]);
    }
    free(backBuffer);
}

void init_back_buffer() {
    backBuffer = (char**)malloc(SCREEN_HEIGHT * sizeof(char*));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        backBuffer[i] = (char*)malloc((SCREEN_WIDTH + 1) * sizeof(char));
    }
}

// 백 버퍼 초기화 함수 (빈 공간으로 초기화)
void clear_back_buffer() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            backBuffer[i][j] = ' ';  // 빈 공간으로 초기화
        }
        backBuffer[i][SCREEN_WIDTH] = '\0';  // 각 줄의 끝에 널 문자 추가
    }
}

// 백 버퍼에 그리기 함수 (텍스트를 특정 위치에 그리기)
void draw_to_back_buffer(int x, int y, const char* text) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		int length = strlen(text);
		for (int i = 0; i < length && (x + i) < SCREEN_WIDTH; i++) {
			backBuffer[y][x + i] = text[i];
		}
	}
}

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

// 백 버퍼 내용을 실제 화면으로 복사하는 함수 (렌더링)
void render() {
    set_cursor_position(0, 0);
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        fwrite(backBuffer[i], 1, SCREEN_WIDTH, stdout);
        if (i < SCREEN_HEIGHT - 1) {
            fputc('\n', stdout);
        }
    }
    fflush(stdout);
}



void update_console_size() {
	COORD console_size = get_console_size(); // 콘솔 창 크기 가져오기
	set_console_size(console_size.X, console_size.Y); // 콘솔 창 버퍼, 창 크기 설정
	if (backBuffer != NULL) {
		free_back_buffer();
	}
	if (messageSystem != NULL)
	{
		free_message_system();
	}
	SCREEN_WIDTH = console_size.X; // 콘솔 창 너비 설정
	SCREEN_HEIGHT = console_size.Y; // 콘솔 창 높이 설정

	init_back_buffer(); // 백 버퍼 초기화

	// 초기 메시지 시스템 생성
	messageSystem = create_message_system(SCREEN_HEIGHT - 6);  // 화면 하단을 제외한 높이만큼 메시지 저장
}

bool is_console_size_changed() {
	COORD console_size = get_console_size(); // 콘솔 창 크기 가져오기
	if (console_size.X != SCREEN_WIDTH || console_size.Y != SCREEN_HEIGHT) {
		return true;
	}
	return false;
}