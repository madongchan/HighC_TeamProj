#pragma once
typedef struct {
    int health;
    int attack;
    int defense;

    // 행동을 위한 함수 포인터 (다형성을 위한 설정)
    void (*attack_func)(void* self, void* target);
} Character;

// 공통된 행동 함수 선언
void character_attack(void* self, void* target);

