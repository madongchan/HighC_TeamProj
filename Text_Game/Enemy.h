#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    char name[30];   // 적만의 특수한 속성 (이름)
} Enemy;

// 적 전용 함수 선언
void enemy_attack(void* self, void* target);
void init_enemy(Enemy* enemy);

