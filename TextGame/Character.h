#pragma once

typedef enum {
    PLAYER,
    ENEMY
} CharacterType;

typedef struct {
    CharacterType type;
    int health;
    int attack;
    int defense;

    // 행동을 위한 함수 포인터 (다형성을 위한 설정)
    void (*attack_func)(void* self, void* target);
} Character;

int character_attack(void* self, void* target) {
    // 기본 공격 로직 (자식에서 재정의 가능)
    Character* attacker = (Character*)self;
    Character* defender = (Character*)target;

    int damage = attacker->attack - defender->defense;
    if (damage < 0) damage = 0;

    defender->health -= damage;
    return damage;
}

