#pragma once
#include <windows.h>
#include <stdio.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// 이중 버퍼링을 위한 백 버퍼 정의
char backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1];

// 콘솔 핸들 정의
HANDLE consoleHandle;

// 콘솔 초기화 함수 (커서 숨김)
void init_console() {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;  // 커서 숨기기
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
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
    COORD cursorPos = { 0, 0 };  // 커서를 (0, 0)으로 이동
    SetConsoleCursorPosition(consoleHandle, cursorPos);

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        printf("%s\n", backBuffer[i]);  // 백 버퍼 내용을 화면에 출력
    }
}