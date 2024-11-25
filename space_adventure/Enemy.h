#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character ����ü�� ��ӹ��� (ù ��° ����� ����)
    char name[100];   // ������ Ư���� �Ӽ� (�̸�)
} Enemy;

void enemy_attack(void* self, void* target) {
    printf("���� �����մϴ�\n");
    character_attack(self, target);  // �⺻ ���� ���� ȣ��
}

Enemy* create_enemy() {
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->base.type = ENEMY;
    enemy->base.health = 50;
    enemy->base.attack = 15;
    enemy->base.defense = 5;
    enemy->base.attack_func = enemy_attack;

    snprintf(enemy->name, sizeof(enemy->name), "Test Enemy");

    return enemy;
}