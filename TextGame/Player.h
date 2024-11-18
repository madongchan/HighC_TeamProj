#pragma once
#include "Character.h"
#include "ItemManager.h"

#define MAX_ITEMS 5
#define MAX_SIGNAL_DEVICE 3
typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    int x, y;        // 플레이어의 위치
    Item inventory[MAX_ITEMS];  // 플레이어만의 특수한 속성 (아이템 목록)
    int item_count;
	int signal_device_count; // 신호 장치 발견 갯수
} Player;

void player_attack(void* self, void* target) {
    printf("플레이어가 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
}

Player* create_player() {
    Player* player = (Player*)malloc(sizeof(Player));
    player->base.type = PLAYER;
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;
    player->base.attack_func = player_attack;
    player->x = 0;
    player->y = 0;
	// 플레이어가 가진 아이템 목록 초기화
    for (int i = 0; i < MAX_ITEMS; ++i) {
        player->inventory[i].index = -1;  // 초기값으로 초기화
        memset(player->inventory[i].name, 0, sizeof(player->inventory[i].name));
		player->inventory[i].type = -1; // 초기값으로 초기화
        player->inventory[i].value = 0;   // 초기값으로 초기화
    }
    player->item_count = 0;
	player->signal_device_count = 0;

    return player;
}