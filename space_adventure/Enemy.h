#pragma once
#include "Character.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �� ����ü ����
typedef struct {
    Character base;      // Character ����ü�� ��ӹ��� (ù ��° ����� ����)
    char name[100];      // ���� �̸�
} Enemy;

// �� �ý��� ����ü
typedef struct {
    Enemy* enemies;      // �� ���
    int count;           // ���� �� ����
} EnemySystem;

// ���� ���� �Լ�
int enemy_attack(void* self, void* target) {
    return character_attack(self, target);  // �⺻ ���� ���� ȣ�� �� ���ط� ��ȯ
}

// �� �ý��� ���� �Լ�
EnemySystem* create_enemy_system() {
    EnemySystem* system = (EnemySystem*)malloc(sizeof(EnemySystem));
    if (!system) {
        fprintf(stderr, "Error: Memory allocation failed for EnemySystem\n");
        exit(EXIT_FAILURE);
    }
    system->enemies = NULL;
    system->count = 0;
    return system;
}

// ���Ͽ��� �� �ε� �Լ�
void load_enemies_from_file(EnemySystem* system, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open enemy file %s\n", filename);
        return;
    }

    int index, health, attack, defense;
    char name[100];

    while (fscanf(file, "%d,%[^,],%d,%d,%d", &index, name, &health, &attack, &defense) == 5) {
        // �޸� ���Ҵ�
        Enemy* temp = realloc(system->enemies, sizeof(Enemy) * (system->count + 1));
        if (!temp) {
            fprintf(stderr, "Error: Memory allocation failed while loading enemies\n");
            fclose(file);
            return;
        }
        system->enemies = temp;

        // �� �ʱ�ȭ
        Enemy* enemy = &system->enemies[system->count];
        enemy->base.type = ENEMY;
        enemy->base.health = health;
        enemy->base.attack = attack;
        enemy->base.defense = defense;
        enemy->base.attack_func = enemy_attack; // ���� ���� �Լ� ����
        strncpy(enemy->name, name, sizeof(enemy->name) - 1);
        enemy->name[sizeof(enemy->name) - 1] = '\0';
        system->count++;
    }

    fclose(file);
}

// �� �ý��� �޸� ���� �Լ�
void free_enemy_system(EnemySystem* system) {
    if (system->enemies) {
        free(system->enemies);
    }

    free(system);
}

// Enemy.h ���Ͽ� �߰�
Enemy* create_enemy() {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    if (!enemy) {
        fprintf(stderr, "Error: Memory allocation failed for Enemy\n");
        exit(EXIT_FAILURE);
    }
    enemy->base.type = ENEMY;
    enemy->base.health = 100;
    enemy->base.attack = 15;
    enemy->base.defense = 5;
    enemy->base.attack_func = enemy_attack; // ���� �Լ� ����
    strncpy(enemy->name, "�⺻ ��", sizeof(enemy->name) - 1);
    enemy->name[sizeof(enemy->name) - 1] = '\0';

    return enemy;
}
