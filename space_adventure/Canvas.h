#pragma once
#include <windows.h>
#include <stdio.h>
#include "Utils.h"
#include "MessageManager.h"
#include "Art.h"

int SCREEN_WIDTH = 80;  // 기본 콘솔 너비 설정
int SCREEN_HEIGHT = 25; // 기본 콘솔 높이 설정

// 더블 버퍼링을 위한 백 버퍼 정의
char** backBuffer = NULL;

void free_back_buffer() {
    if (backBuffer == NULL) return; // 백 버퍼가 초기화되지 않았으면 종료
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        free(backBuffer[i]);
    }
    free(backBuffer);
    backBuffer = NULL;
}

void init_back_buffer() {
    free_back_buffer(); // 기존 백 버퍼 메모리 해제
    backBuffer = (char**)malloc(SCREEN_HEIGHT * sizeof(char*));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        backBuffer[i] = (char*)malloc((SCREEN_WIDTH + 1) * sizeof(char));
    }
}

// 백 버퍼 초기화 함수 (빈 공간으로 초기화)
void clear_back_buffer() {
    if (backBuffer == NULL) return; // 백 버퍼가 초기화되지 않았으면 종료
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            backBuffer[i][j] = ' ';  // 빈 공간으로 초기화
        }
        backBuffer[i][SCREEN_WIDTH] = '\0';  // 각 줄의 끝에 널 문자 추가
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

// 백 버퍼에 그리기 함수 (텍스트를 특정 위치에 그리기)
void draw_to_back_buffer(int x, int y, const char* text) {
    if (backBuffer == NULL || x < 0 || y < 0 || y >= SCREEN_HEIGHT) return; // 유효성 검사
    int length = strlen(text);
    for (int i = 0; i < length && (x + i) < SCREEN_WIDTH; i++) {
        backBuffer[y][x + i] = text[i];
    }
}

// 게임 로직 공간 출력 함수
void display_game_area() {
    if (backBuffer == NULL) return; // 백 버퍼가 초기화되지 않았으면 종료
    clear_back_buffer();
    draw_to_back_buffer(0, 0, "===== 게임 로직 공간 =====");
    draw_to_back_buffer(0, 1, "명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동), 맵 열기/닫기: M, 게임 종료: Q");

    if (messageSystem == NULL) return; // 메시지 시스템이 초기화되지 않았으면 종료
    for (int i = 0; i < messageSystem->count; i++) {
        if (messageSystem->messages[i].is_visible) {
            draw_to_back_buffer(0, i + 2, messageSystem->messages[i].content);
        }
    }
}

// 중단 아스키 아트 출력 함수
void display_game_art(EventType event_type) {
    if (backBuffer == NULL) return; // 백 버퍼가 초기화되지 않았으면 종료
    int art_start_row = (SCREEN_HEIGHT / 3) * 2;  // 아스키 아트 시작 위치
    int art_height = 7;                    // 아스키 아트의 높이
    char message[100];

    draw_to_back_buffer(0, art_start_row - 1, "===== 상황에 따른 아스키 아트 =====");

    for (int i = 0; i < art_height; i++) {  // 위에서 아래로 순서대로 출력
        switch (event_type) {
        case SIGNAL_DEVICE:
            draw_to_back_buffer(0, art_start_row + i, get_signal_device_message(i + 1, message));
            break;
        case STORM:
            draw_to_back_buffer(0, art_start_row + i, get_storm_message(i + 1, message));
            break;
        case ITEM_FOUND:
            draw_to_back_buffer(0, art_start_row + i, get_item_found_message(i + 1, message));
            break;
        case PREDATOR:
            draw_to_back_buffer(0, art_start_row + i, get_predator_message(i + 1, message));
            break;
        case ALIEN:
            draw_to_back_buffer(0, art_start_row + i, get_alien_message(i + 1, message));
            break;
        case MATTINGA:
            draw_to_back_buffer(0, art_start_row + i, get_mattinga_message(i + 1, message));
            break;
        default:
            draw_to_back_buffer(0, art_start_row + i, "");
            break;
        }
    }
}

// 하단 플레이어 상태 및 화면 정보 출력 함수
void display_status_area(Player* player) {
    if (backBuffer == NULL) return; // 백 버퍼가 초기화되지 않았으면 종료
    int status_start_row = SCREEN_HEIGHT - 4;

    // 플레이어 상태
    char status[100];
    snprintf(status, sizeof(status), "체력: %d | 공격력: %d | 방어력: %d \ 신호장치 갯 수: %d",
        player->base.health, player->base.attack, player->base.defense, player->signal_device_count);
    draw_to_back_buffer(0, status_start_row, "===== 플레이어 상태 =====");
    draw_to_back_buffer(0, status_start_row + 1, status);

    // 보유 아이템
    draw_to_back_buffer(0, status_start_row + 2, "===== 보유 아이템 =====");
    if (player->item_count > 0) {
        char item_message[1000] = { 0 };  // 모든 아이템을 저장할 메시지

        for (int i = 0; i < player->item_count; i++) {
            strncat(item_message, player->inventory[i].name, sizeof(item_message) - strlen(item_message) - 1);
            if (i != player->item_count - 1) {
                strncat(item_message, ", ", sizeof(item_message) - strlen(item_message) - 1);
            }
        }

        draw_to_back_buffer(0, status_start_row + 3, item_message);
    }
    else {
        draw_to_back_buffer(0, status_start_row + 3, "아이템이 없습니다.");
    }

    // 콘솔 정보 출력
    char size_info[100];
    snprintf(size_info, sizeof(size_info), "(화면 크기: %dx%d)", SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_to_back_buffer(SCREEN_WIDTH - 50, status_start_row, size_info);

    // 플레이어 위치 정보
    char player_info[100];
    snprintf(player_info, sizeof(player_info), "플레이어 위치: (%d, %d)", player->x, player->y);
    draw_to_back_buffer(SCREEN_WIDTH - 50, status_start_row + 1, player_info);
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

// 콘솔 크기 업데이트 함수
void update_console_size() {
    // 콘솔 창 크기 가져오기
    COORD console_size = get_console_size();

    // 크기가 변경되지 않았으면 종료
    if (console_size.X == SCREEN_WIDTH && console_size.Y == SCREEN_HEIGHT) {
        return;
    }

    // 기존 백 버퍼 해제
    free_back_buffer();

    // 콘솔 크기 업데이트
    SCREEN_WIDTH = console_size.X;
    SCREEN_HEIGHT = console_size.Y;

    // 새로운 크기에 맞게 백 버퍼 초기화
    init_back_buffer();

    // 메시지 시스템도 새 크기에 맞게 초기화
    if (messageSystem != NULL) {
        free_message_system();
    }
    messageSystem = create_message_system(SCREEN_HEIGHT - 6); // 새 높이에 맞는 메시지 시스템 생성

    // 새로 할당된 백 버퍼를 초기화
    clear_back_buffer();
}
