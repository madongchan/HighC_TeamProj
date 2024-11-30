#pragma once
#include "Character.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 적 구조체 정의
typedef struct {
    Character base;      // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    char name[100];      // 적의 이름
} Enemy;

// 적 시스템 구조체
typedef struct {
    Enemy* enemies;      // 적 목록
    int count;           // 현재 적 개수
} EnemySystem;

// 적의 공격 함수
int enemy_attack(void* self, void* target) {
    return character_attack(self, target);  // 기본 공격 로직 호출 후 피해량 반환
}

// 적 시스템 생성 함수
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

// 파일에서 적 로드 함수
void load_enemies_from_file(EnemySystem* system, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open enemy file %s\n", filename);
        return;
    }

    int index, health, attack, defense;
    char name[100];

    while (fscanf(file, "%d,%[^,],%d,%d,%d", &index, name, &health, &attack, &defense) == 5) {
        // 메모리 재할당
        Enemy* temp = realloc(system->enemies, sizeof(Enemy) * (system->count + 1));
        if (!temp) {
            fprintf(stderr, "Error: Memory allocation failed while loading enemies\n");
            fclose(file);
            return;
        }
        system->enemies = temp;

        // 적 초기화
        Enemy* enemy = &system->enemies[system->count];
        enemy->base.type = ENEMY;
        enemy->base.health = health;
        enemy->base.attack = attack;
        enemy->base.defense = defense;
        enemy->base.attack_func = enemy_attack; // 적의 공격 함수 설정
        strncpy(enemy->name, name, sizeof(enemy->name) - 1);
        enemy->name[sizeof(enemy->name) - 1] = '\0';
        system->count++;
    }

    fclose(file);
}

// 적 시스템 메모리 해제 함수
void free_enemy_system(EnemySystem* system) {
    if (system->enemies) {
        free(system->enemies);
    }

    free(system);
}

// Enemy.h 파일에 추가
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
    enemy->base.attack_func = enemy_attack; // 공격 함수 설정
    strncpy(enemy->name, "기본 적", sizeof(enemy->name) - 1);
    enemy->name[sizeof(enemy->name) - 1] = '\0';

    return enemy;
}
