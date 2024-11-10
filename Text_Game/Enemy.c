#include "Enemy.h"
#include <stdio.h>

void enemy_attack(void* self, void* target) {
    printf("적이 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
}

void init_enemy(Enemy* enemy) {
    enemy->base.health = 50;
    enemy->base.attack = 15;
    enemy->base.defense = 5;

    // 적의 공격 함수 설정 (다형성 구현)
    enemy->base.attack_func = enemy_attack;

    snprintf(enemy->name, sizeof(enemy->name), "Goblin");  // 적 이름 설정
}