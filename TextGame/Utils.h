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
	SMALL_RECT rect = { 0, 0, width - 1, height - 1 };  // 창의 좌상단 (0,0)부터 크기 설정
	SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

// 콘솔 크기 통합 설정
void set_console_size(int width, int height) {
	set_console_buffer_size(width, height);  // 버퍼 크기 설정
	set_console_window_size(width, height);  // 창 크기 설정
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

// 콘솔 글꼴 크기 설정
void set_console_font_size(int fontWidth, int fontHeight) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };

	if (GetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
		cfi.dwFontSize.X = fontWidth;  // 글꼴 너비
		cfi.dwFontSize.Y = fontHeight; // 글꼴 높이
		wcscpy(cfi.FaceName, L"Consolas");  // 글꼴 설정 (Consolas 사용)

		if (!SetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
			printf("Error setting console font size.\n");
		}
	}
	else {
		printf("Error retrieving current console font info.\n");
	}
}

// 콘솔 창 최대화
void maximize_console_window() {
	HWND consoleWindow = GetConsoleWindow();  // 현재 콘솔 창 핸들 가져오기
	ShowWindow(consoleWindow, SW_MAXIMIZE);  // 콘솔 창 최대화
}

// 콘솔 초기화 함수 (커서 숨김)
void init_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// 커서 숨김 설정
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = FALSE;  // 커서 숨기기
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	maximize_console_window();  // 콘솔 창 최대화
	// 커서 위치 초기화
	set_cursor_position(0, 0);
}
