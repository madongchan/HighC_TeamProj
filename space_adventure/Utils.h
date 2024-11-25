#pragma once
#include <windows.h>

// 콘솔 버퍼 크기 설정
void set_console_buffer_size(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { width, height };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}

// 콘솔 창 크기 설정
void set_console_window_size(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rect = { 0, 0, width - 1, height - 1 };  // 창의 시작점은 (0,0)부터이며 창 크기 설정
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

// 콘솔 크기 전체 설정
void set_console_size(int width, int height) {
    set_console_buffer_size(width, height);  // 버퍼 크기 설정
    set_console_window_size(width, height);  // 창 크기 설정
}

// 커서를 특정 위치로 이동시키는 함수
void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 콘솔 창 크기를 가져오는 함수
COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD size;
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

// 콘솔 창 최대화
void maximize_console_window() {
    HWND consoleWindow = GetConsoleWindow();
    SendMessage(consoleWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

// 콘솔 초기화 함수 (커서 숨김)
void init_console() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // 커서 숨김 설정
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;  // 커서 숨기기
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    //maximize_console_window();  // 콘솔 창 최대화

    // 커서 위치 초기화
    set_cursor_position(0, 0);
}