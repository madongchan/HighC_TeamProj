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

    // ���ݿ� ���� �Լ� ������(�������� ���� ����)
    void (*attack_func)(void* self, void* target);
} Character;

int character_attack(void* self, void* target) {
    // �⺻ ���� ���� (���Ŀ��� ���� �� ���)
    Character* attacker = (Character*)self;
    Character* defender = (Character*)target;

    int damage = attacker->attack - defender->defense;
    if (damage < 0) damage = 0;

    defender->health -= damage;
    return damage;
}