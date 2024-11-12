#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    int x, y;        // 플레이어의 위치
    char items[10][30];  // 플레이어만의 특수한 속성 (아이템 목록)
    int item_count;
} Player;

void player_attack(void* self, void* target) {
    printf("플레이어가 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
}

// 스택 공간에 플레이어 객체를 생성하고 초기화하는 함수
void init_player(Player* player) {
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;

    // 플레이어의 공격 함수 설정 (다형성 구현)
    player->base.attack_func = player_attack;

    player->x = 0;
    player->y = 0;
    player->item_count = 0;  // 아이템 초기화
}

// 힙 공간에 플레이어 객체를 생성하고 초기화하는 함수
Player* createPlayer() {
    Player* player = malloc(sizeof(Player));
    (*player).base.type = PLAYER;
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;
    player->base.attack_func = player_attack;
    player->x = 0;
    player->y = 0;
    player->item_count = 0;

    return player;
}