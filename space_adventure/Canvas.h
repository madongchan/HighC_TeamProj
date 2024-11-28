#pragma once
#include <windows.h>
#include <stdio.h>
#include "Utils.h"
#include "MessageManager.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

// ���� ���۸��� ���� �� ���� ����
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

// �� ���� �ʱ�ȭ �Լ� (�� �������� �ʱ�ȭ)
void clear_back_buffer() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            backBuffer[i][j] = ' ';  // �� �������� �ʱ�ȭ
        }
        backBuffer[i][SCREEN_WIDTH] = '\0';  // �� ���� ���� �� ���� �߰�
    }
}

// �� ���ۿ� �׸��� �Լ� (�ؽ�Ʈ�� Ư�� ��ġ�� �׸���)
void draw_to_back_buffer(int x, int y, const char* text) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		int length = strlen(text);
		for (int i = 0; i < length && (x + i) < SCREEN_WIDTH; i++) {
			backBuffer[y][x + i] = text[i];
		}
	}
}

// �޽����� ����ϴ� �Լ�
void display_game_area() {
	clear_back_buffer();
	draw_to_back_buffer(0, 0, "===== ���� ���� ���� =====");
	draw_to_back_buffer(0, 1, "����� �Է��ϼ��� (N: ���� �̵�, S: ���� �̵�, E: ���� �̵�, W: ���� �̵�)");

	for (int i = 0; i < messageSystem->count; i++) {
		if (messageSystem->messages[i].is_visible) {
			draw_to_back_buffer(0, i + 2, messageSystem->messages[i].content);
		}
	}
}

// �ϴ� �÷��̾� ���� �� ������ ���� ��� �Լ�
void display_status_area(Player* player) {
	COORD size = get_console_size();
	char status[100];
	snprintf(status, sizeof(status), "ü��: %d | ���ݷ�: %d | ����: %d",
		player->base.health, player->base.attack, player->base.defense);

	char massage[1000];
	snprintf(massage, sizeof(massage), "������: %d , %d", size.X, size.Y);
	draw_to_back_buffer(0, size.Y - 6, massage);
	draw_to_back_buffer(0, SCREEN_HEIGHT - 5, "===== �÷��̾� ���� =====");
	draw_to_back_buffer(0, SCREEN_HEIGHT - 4, status);
	draw_to_back_buffer(0, SCREEN_HEIGHT - 3, "===== ���� ������ =====");
	// ���� ������ ���
	if (player->item_count > 0) {
		char item_message[1000] = { 0 };  // ��� �������� ������ �޽���

		for (int i = 0; i < player->item_count; i++) {
			// ������ �̸��� �޽����� �߰�
			strncat(item_message, player->inventory[i].name, sizeof(item_message) - strlen(item_message) - 1);

			// ������ �������� �ƴ� ��� ��ǥ�� ���� �߰�
			if (i != player->item_count - 1) {
				strncat(item_message, ", ", sizeof(item_message) - strlen(item_message) - 1);
			}
		}

		// ���� �޽��� ��� (draw_to_back_buffer�� ȭ�� ��� �Լ���� ����)
		draw_to_back_buffer(0, SCREEN_HEIGHT - 2, item_message);
	}
}

// �� ���� ������ ���� ȭ������ �����ϴ� �Լ� (������)
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
	COORD console_size = get_console_size(); // �ܼ� â ũ�� ��������
	set_console_size(console_size.X, console_size.Y); // �ܼ� â ����, â ũ�� ����
	if (backBuffer != NULL) {
		free_back_buffer();
	}
	if (messageSystem != NULL)
	{
		free_message_system();
	}
	SCREEN_WIDTH = console_size.X; // �ܼ� â �ʺ� ����
	SCREEN_HEIGHT = console_size.Y; // �ܼ� â ���� ����

	init_back_buffer(); // �� ���� �ʱ�ȭ

	// �ʱ� �޽��� �ý��� ����
	messageSystem = create_message_system(SCREEN_HEIGHT - 6);  // ȭ�� �ϴ��� ������ ���̸�ŭ �޽��� ����
}

bool is_console_size_changed() {
	COORD console_size = get_console_size(); // �ܼ� â ũ�� ��������
	if (console_size.X != SCREEN_WIDTH || console_size.Y != SCREEN_HEIGHT) {
		return true;
	}
	return false;
}