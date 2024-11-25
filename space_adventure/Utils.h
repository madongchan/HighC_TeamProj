#pragma once
#include <windows.h>

// �ܼ� ���� ũ�� ����
void set_console_buffer_size(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { width, height };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}

// �ܼ� â ũ�� ����
void set_console_window_size(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rect = { 0, 0, width - 1, height - 1 };  // â�� �������� (0,0)�����̸� â ũ�� ����
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

// �ܼ� ũ�� ��ü ����
void set_console_size(int width, int height) {
    set_console_buffer_size(width, height);  // ���� ũ�� ����
    set_console_window_size(width, height);  // â ũ�� ����
}

// Ŀ���� Ư�� ��ġ�� �̵���Ű�� �Լ�
void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// �ܼ� â ũ�⸦ �������� �Լ�
COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD size;
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

// �ܼ� â �ִ�ȭ
void maximize_console_window() {
    HWND consoleWindow = GetConsoleWindow();
    SendMessage(consoleWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

// �ܼ� �ʱ�ȭ �Լ� (Ŀ�� ����)
void init_console() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Ŀ�� ���� ����
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;  // Ŀ�� �����
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    //maximize_console_window();  // �ܼ� â �ִ�ȭ

    // Ŀ�� ��ġ �ʱ�ȭ
    set_cursor_position(0, 0);
}