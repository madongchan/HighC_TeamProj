#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "Player.h"
#include "Enemy.h"

// ȭ���� ����� �Լ�
void clear_screen() {
#ifdef _WIN32
    system("cls");  // Windows ȯ�濡�� ȭ�� �����
#else
    system("clear"); // Unix/Linux ȯ�濡�� ȭ�� �����
#endif
}

// Ŀ���� ���ϴ� ��ġ�� �̵���Ű�� �Լ� (Windows ����)
void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// �ܼ� â ũ�⸦ �������� �Լ� (Windows ����)
COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD size;
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

// ��� ���� ���� ���� ��� �Լ�
void display_game_area(const char* message) {
    // Ŀ���� ������� �̵��Ͽ� ���� ���� �޽��� ���
    set_cursor_position(0, 0);

    printf("===== ���� ���� ���� =====\n");
    printf("%s\n", message);
    printf("=========================\n\n");
}

// �ϴ� �÷��̾� ���� �� ������ ���� ��� �Լ�
void display_status_area(Player* player) {
    COORD console_size = get_console_size();

    // �ϴ����� Ŀ���� �̵��Ͽ� �÷��̾� ���¿� ������ ���
    set_cursor_position(0, console_size.Y - 6);  // �� �Ʒ����� 6�� ���� Ŀ�� �̵� (���� �� ������ ���� Ȯ��)

    printf("\n===== �÷��̾� ���� =====\n");
    printf("ü��: %d\n", player->base.health);
    printf("���ݷ�: %d\n", player->base.attack);
    printf("����: %d\n", player->base.defense);

    printf("\n===== ���� ������ =====\n");
    for (int i = 0; i < player->item_count; ++i) {
        printf("%s\n", player->items[i]);
    }

    // ������ �ٱ��� ä���� �ϴ� ���� ����
    printf("=========================\n");
}


void battle(Player* player, Enemy* enemy) {
    printf("���� ����! %s��(��) �ο�ϴ�!\n", enemy->name);

    while (player->base.health > 0 && enemy->base.health > 0) {
        // �÷��̾ ���� ����
        int damage_to_enemy = player->base.attack;
        enemy->base.health -= damage_to_enemy;
        printf("�÷��̾ %s���� %d ���ظ� �������ϴ�.\n", enemy->name, damage_to_enemy);

        if (enemy->base.health <= 0) {
            printf("%s�� �����ƽ��ϴ�!\n", enemy->name);
            break;
        }

        // ���� �÷��̾ ����
        int damage_to_player = enemy->base.attack - (player->base.defense * 0.4);
        if (damage_to_player < 0) damage_to_player = 0; // ������ ������ ���ذ� ���� ���� ����
        player->base.health -= damage_to_player;
        printf("%s�� �÷��̾�� %d ���ظ� �������ϴ�.\n", enemy->name, damage_to_player);

        if (player->base.health <= 0) {
            printf("�÷��̾ ����߽��ϴ�...\n");
            break;
        }
    }
}

void use_item(Player* player, char* item_name) {
    if (strcmp(item_name, "�Ƿ� ŰƮ") == 0) {
        player->base.health += 10; // ü�� ȸ��
        printf("�Ƿ� ŰƮ�� ����Ͽ� ü���� ȸ���Ǿ����ϴ�! ���� ü��: %d\n", player->base.health);
    }
    else if (strcmp(item_name, "������ ���̵�") == 0) {
        player->base.attack += 5; // ���ݷ� ����
        printf("������ ���̵带 ����Ͽ� ���ݷ��� �����߽��ϴ�! ���� ���ݷ�: %d\n", player->base.attack);
    }
    else if (strcmp(item_name, "��ȭ ��Ʈ") == 0) {
        player->base.defense += 5; // ���� ����
        printf("��ȭ ��Ʈ�� �����Ͽ� ������ �����߽��ϴ�! ���� ����: %d\n", player->base.defense);
    }
    else {
        printf("�� �� ���� �������Դϴ�.\n");
    }
}

void random_event(Player* player) {
    int event_type = rand() % 3; // �� ���� ���� �̺�Ʈ �� �ϳ� �߻�

    switch (event_type) {
    case 0:
        printf("�ܰ� ������ �߰��߽��ϴ�! ���ݷ��� �����մϴ�.\n");
        player->base.attack += 2;
        break;
    case 1:
        printf("��ǳ�� �߻��߽��ϴ�! ü���� �����մϴ�.\n");
        player->base.health -= 5;
        break;
    case 2:
        printf("ģ���� �ܰ� ����ü�� �������ϴ�! ������ ������ϴ�.\n");
        break;
    }
}

void move_room(char direction) {
    switch (direction) {
    case 'N':
        printf("�������� �̵��մϴ�.\n");
        break;
    case 'S':
        printf("�������� �̵��մϴ�.\n");
        break;
    case 'E':
        printf("�������� �̵��մϴ�.\n");
        break;
    case 'W':
        printf("�������� �̵��մϴ�.\n");
        break;
    default:
        printf("�߸��� �����Դϴ�.\n");
        break;
    }
}

int main() {
    Player player;
    Enemy enemy;

    // �÷��̾�� �� �ʱ�ȭ
    init_player(&player);
    init_enemy(&enemy);

    char command;

    while (player.base.health > 0) {
        // ȭ���� ���� ����� ��� ���� ���� ���� �ʱ�ȭ �� �ȳ� �޽��� ���
        //clear_screen();
        display_game_area("����� �Է��ϼ��� (N: ���� �̵�, S: ���� �̵�, E: ���� �̵�, W: ���� �̵�, B: ���� ����, I: ������ ���)");
		scanf("%c", &command);
        // �ϴ� �÷��̾� ���� �� ������ ���� ��� (�׻� �ϴܿ� ����)
        display_status_area(&player);

        // ��� �Է� �ޱ�
        set_cursor_position(0, get_console_size().Y - 1);
        

        switch (command) {
        case 'n':
        case 's':
        case 'e':
        case 'w':
            display_game_area("�������� �̵� ��...");
            random_event(&player); // �̵� �� ���� �̺�Ʈ �߻� ����
            break;

        case 'B':
            display_game_area("�������Ϳ� ���� ��...");
            battle(&player, &enemy); // �������Ϳ� ����
            break;

        case 'I':
            use_item(&player, "�Ƿ� ŰƮ"); // ���÷� �Ƿ� ŰƮ ���
            break;

        default:
            display_game_area("�߸��� ����Դϴ�.");
            break;
        }

        if (player.base.health <= 0) {
            display_game_area("���� ����!");
            break;
        }

        // ��� ���� �� �ٽ� ��ܰ� �ϴ� ����
        //clear_screen();
        display_game_area("���� ����� ��ٸ��ϴ�...");
        display_status_area(&player);
    }

    return 0;
}