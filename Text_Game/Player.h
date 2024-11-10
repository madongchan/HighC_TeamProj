#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character ����ü�� ��ӹ��� (ù ��° ����� ����)
    char items[10][30];  // �÷��̾�� Ư���� �Ӽ� (������ ���)
    int item_count;
} Player;

// �÷��̾� ���� �Լ� ����
void player_attack(void* self, void* target);
void init_player(Player* player);