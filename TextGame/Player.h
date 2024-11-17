#pragma once
#include "Character.h"

#define MAX_ITEMS 5
#define MAX_SIGNAL_DEVICE 3
typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    int x, y;        // 플레이어의 위치
    char items[MAX_ITEMS][30];  // 플레이어만의 특수한 속성 (아이템 목록)
    int item_count;
	int signal_device_count; // 신호 장치 발견 갯수
} Player;

void player_attack(void* self, void* target) {
    printf("플레이어가 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
}

Player* create_player() {
    Player* player = malloc(sizeof(Player));
    player->base.type = PLAYER;
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;
    player->base.attack_func = player_attack;
    player->x = 0;
    player->y = 0;
    player->item_count = 0;
	player->signal_device_count = 0;

    return player;
}