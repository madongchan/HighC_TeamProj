#include "Enemy.h"
#include <stdio.h>

void enemy_attack(void* self, void* target) {
    printf("���� �����մϴ�!\n");
    character_attack(self, target);  // �⺻ ���� ���� ȣ��
}

void init_enemy(Enemy* enemy) {
    enemy->base.health = 50;
    enemy->base.attack = 15;
    enemy->base.defense = 5;

    // ���� ���� �Լ� ���� (������ ����)
    enemy->base.attack_func = enemy_attack;

    snprintf(enemy->name, sizeof(enemy->name), "Goblin");  // �� �̸� ����
}