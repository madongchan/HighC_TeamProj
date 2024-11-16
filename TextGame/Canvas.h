#pragma once
#include <windows.h>
#include <stdio.h>
#include "Utils.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

// 이중 버퍼링을 위한 백 버퍼 정의
char** backBuffer;

void init_back_buffer() {
    backBuffer = (char**)malloc(SCREEN_HEIGHT * sizeof(char*));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        backBuffer[i] = (char*)malloc((SCREEN_WIDTH + 1) * sizeof(char));
    }
}

void free_back_buffer() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        free(backBuffer[i]);
    }
    free(backBuffer);
}

// 백 버퍼 초기화 함수 (빈 공간으로 초기화)
void clear_back_buffer() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            backBuffer[i][j] = ' ';  // 빈 공간으로 초기화
        }
        backBuffer[i][SCREEN_WIDTH] = '\0';  // 각 줄 끝에 널 문자 추가
    }
}

// 백 버퍼에 그리기 함수 (텍스트를 특정 위치에 그리기)
void draw_to_back_buffer(int x, int y, const char* text) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        snprintf(backBuffer[y], SCREEN_WIDTH + 1, "%s", text);  // 텍스트를 백 버퍼에 그리기
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