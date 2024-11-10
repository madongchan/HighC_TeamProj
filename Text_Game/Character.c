#include "Character.h"
#include <stdio.h>

int character_attack(void* self, void* target) {
    // �⺻ ���� ���� (�ڽĿ��� ������ ����)
    Character* attacker = (Character*)self;
    Character* defender = (Character*)target;

    int damage = attacker->attack - defender->defense;
    if (damage < 0) damage = 0;

    defender->health -= damage;
	return damage;
}