#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character ����ü�� ��ӹ��� (ù ��° ����� ����)
    char name[30];   // ������ Ư���� �Ӽ� (�̸�)
} Enemy;

// �� ���� �Լ� ����
void enemy_attack(void* self, void* target);
void init_enemy(Enemy* enemy);

