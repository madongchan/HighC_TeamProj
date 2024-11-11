#pragma once
#include <stdlib.h>
#include <windows.h>

// 화면을 지우는 함수
void clear_screen() {
    system("cls");
}

// 커서를 원하는 위치로 이동시키는 함수
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
