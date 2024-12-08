#pragma once
#include <windows.h>
#include <stdio.h>
#include "Utils.h"
#include "MessageManager.h"
#include "Art.h"

int SCREEN_WIDTH = 80;  // �⺻ �ܼ� �ʺ� ����
int SCREEN_HEIGHT = 25; // �⺻ �ܼ� ���� ����

// ���� ���۸��� ���� �� ���� ����
char** backBuffer = NULL;

void free_back_buffer() {
    if (backBuffer == NULL) return; // �� ���۰� �ʱ�ȭ���� �ʾ����� ����
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        free(backBuffer[i]);
    }
    free(backBuffer);
    backBuffer = NULL;
}

void init_back_buffer() {
    free_back_buffer(); // ���� �� ���� �޸� ����
    backBuffer = (char**)malloc(SCREEN_HEIGHT * sizeof(char*));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        backBuffer[i] = (char*)malloc((SCREEN_WIDTH + 1) * sizeof(char));
    }
}

// �� ���� �ʱ�ȭ �Լ� (�� �������� �ʱ�ȭ)
void clear_back_buffer() {
    if (backBuffer == NULL) return; // �� ���۰� �ʱ�ȭ���� �ʾ����� ����
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            backBuffer[i][j] = ' ';  // �� �������� �ʱ�ȭ
        }
        backBuffer[i][SCREEN_WIDTH] = '\0';  // �� ���� ���� �� ���� �߰�
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

// �� ���ۿ� �׸��� �Լ� (�ؽ�Ʈ�� Ư�� ��ġ�� �׸���)
void draw_to_back_buffer(int x, int y, const char* text) {
    if (backBuffer == NULL || x < 0 || y < 0 || y >= SCREEN_HEIGHT) return; // ��ȿ�� �˻�
    int length = strlen(text);
    for (int i = 0; i < length && (x + i) < SCREEN_WIDTH; i++) {
        backBuffer[y][x + i] = text[i];
    }
}

// ���� ���� ���� ��� �Լ�
void display_game_area() {
    if (backBuffer == NULL) return; // �� ���۰� �ʱ�ȭ���� �ʾ����� ����
    clear_back_buffer();
    draw_to_back_buffer(0, 0, "===== ���� ���� ���� =====");
    draw_to_back_buffer(0, 1, "����� �Է��ϼ��� (N: ���� �̵�, S: ���� �̵�, E: ���� �̵�, W: ���� �̵�), �� ����/�ݱ�: M, ���� ����: Q");

    if (messageSystem == NULL) return; // �޽��� �ý����� �ʱ�ȭ���� �ʾ����� ����
    for (int i = 0; i < messageSystem->count; i++) {
        if (messageSystem->messages[i].is_visible) {
            draw_to_back_buffer(0, i + 2, messageSystem->messages[i].content);
        }
    }
}

// �ߴ� �ƽ�Ű ��Ʈ ��� �Լ�
void display_game_art(EventType event_type) {
    if (backBuffer == NULL) return; // �� ���۰� �ʱ�ȭ���� �ʾ����� ����
    int art_start_row = (SCREEN_HEIGHT / 3) * 2;  // �ƽ�Ű ��Ʈ ���� ��ġ
    int art_height = 7;                    // �ƽ�Ű ��Ʈ�� ����
    char message[100];

    draw_to_back_buffer(0, art_start_row - 1, "===== ��Ȳ�� ���� �ƽ�Ű ��Ʈ =====");

    for (int i = 0; i < art_height; i++) {  // ������ �Ʒ��� ������� ���
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

// �ϴ� �÷��̾� ���� �� ȭ�� ���� ��� �Լ�
void display_status_area(Player* player) {
    if (backBuffer == NULL) return; // �� ���۰� �ʱ�ȭ���� �ʾ����� ����
    int status_start_row = SCREEN_HEIGHT - 4;

    // �÷��̾� ����
    char status[100];
    snprintf(status, sizeof(status), "ü��: %d | ���ݷ�: %d | ����: %d \ ��ȣ��ġ �� ��: %d",
        player->base.health, player->base.attack, player->base.defense, player->signal_device_count);
    draw_to_back_buffer(0, status_start_row, "===== �÷��̾� ���� =====");
    draw_to_back_buffer(0, status_start_row + 1, status);

    // ���� ������
    draw_to_back_buffer(0, status_start_row + 2, "===== ���� ������ =====");
    if (player->item_count > 0) {
        char item_message[1000] = { 0 };  // ��� �������� ������ �޽���

        for (int i = 0; i < player->item_count; i++) {
            strncat(item_message, player->inventory[i].name, sizeof(item_message) - strlen(item_message) - 1);
            if (i != player->item_count - 1) {
                strncat(item_message, ", ", sizeof(item_message) - strlen(item_message) - 1);
            }
        }

        draw_to_back_buffer(0, status_start_row + 3, item_message);
    }
    else {
        draw_to_back_buffer(0, status_start_row + 3, "�������� �����ϴ�.");
    }

    // �ܼ� ���� ���
    char size_info[100];
    snprintf(size_info, sizeof(size_info), "(ȭ�� ũ��: %dx%d)", SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_to_back_buffer(SCREEN_WIDTH - 50, status_start_row, size_info);

    // �÷��̾� ��ġ ����
    char player_info[100];
    snprintf(player_info, sizeof(player_info), "�÷��̾� ��ġ: (%d, %d)", player->x, player->y);
    draw_to_back_buffer(SCREEN_WIDTH - 50, status_start_row + 1, player_info);
}

void display_map(Player* player) {
    clear_back_buffer(); // ���� ȭ�� �����

    // �� ���� 3x5 ũ��� ����
    const int room_height = 4;
    const int room_width = 12;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int start_x = j * (room_width + 4); // �� ���� ���� x ��ǥ (��� ���� �߰�)
            int start_y = i * (room_height + 2); // �� ���� ���� y ��ǥ (��� ���� �߰�)

            // �� ��輱 �׸���
            draw_to_back_buffer(start_x, start_y, "------------");
            for (int k = 1; k < room_height - 1; k++) {
                char buffer[13]; // �迭 ũ�⸦ ��Ȯ�� ����
                strncpy(buffer, "|          |", sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                draw_to_back_buffer(start_x, start_y + k, buffer);
            }
            draw_to_back_buffer(start_x, start_y + room_height - 1, "------------");

            // �÷��̾� ��ġ ǥ��
            if (i == player->y && j == player->x) {
                draw_to_back_buffer(start_x + room_width / 2, start_y + room_height / 2, "P");
            }

            // ��� �� ������ ��� �׸���
            if (j < 2) { // ���������� ��� �߰� (������ �� ����)
                draw_to_back_buffer(start_x + room_width, start_y + room_height / 2, "####");
            }
            if (i < 2) { // �Ʒ������� ��� �߰� (������ �� ����)
                for (int k = 0; k < 4; k++) {
                    draw_to_back_buffer(start_x + room_width / 2, start_y + room_height + k, "#");
                }
            }
        }
    }

    display_status_area(player);
    render();
}

// �ܼ� ũ�� ������Ʈ �Լ�
void update_console_size() {
    // �ܼ� â ũ�� ��������
    COORD console_size = get_console_size();

    // ũ�Ⱑ ������� �ʾ����� ����
    if (console_size.X == SCREEN_WIDTH && console_size.Y == SCREEN_HEIGHT) {
        return;
    }

    // ���� �� ���� ����
    free_back_buffer();

    // �ܼ� ũ�� ������Ʈ
    SCREEN_WIDTH = console_size.X;
    SCREEN_HEIGHT = console_size.Y;

    // ���ο� ũ�⿡ �°� �� ���� �ʱ�ȭ
    init_back_buffer();

    // �޽��� �ý��۵� �� ũ�⿡ �°� �ʱ�ȭ
    if (messageSystem != NULL) {
        free_message_system();
    }
    messageSystem = create_message_system(SCREEN_HEIGHT - 6); // �� ���̿� �´� �޽��� �ý��� ����

    // ���� �Ҵ�� �� ���۸� �ʱ�ȭ
    clear_back_buffer();
}
