#include "Player.h"
#include <stdio.h>

void player_attack(void* self, void* target) {
    printf("�÷��̾ �����մϴ�!\n");
    character_attack(self, target);  // �⺻ ���� ���� ȣ��
}

void init_player(Player* player) {
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;

    // �÷��̾��� ���� �Լ� ���� (������ ����)
    player->base.attack_func = player_attack;

    player->item_count = 0;  // ������ �ʱ�ȭ
}