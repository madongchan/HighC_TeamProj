#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    char name[100];   // 적만의 특수한 속성 (이름)
} Enemy;

void enemy_attack(void* self, void* target) {
    printf("적이 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
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
