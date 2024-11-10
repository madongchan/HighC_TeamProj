#include "Character.h"
#include <stdio.h>

int character_attack(void* self, void* target) {
    // 기본 공격 로직 (자식에서 재정의 가능)
    Character* attacker = (Character*)self;
    Character* defender = (Character*)target;

    int damage = attacker->attack - defender->defense;
    if (damage < 0) damage = 0;

    defender->health -= damage;
	return damage;
}