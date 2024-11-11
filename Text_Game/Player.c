#include "Player.h"
#include <stdio.h>

void player_attack(void* self, void* target) {
    printf("플레이어가 공격합니다!\n");
    character_attack(self, target);  // 기본 공격 로직 호출
}

void init_player(Player* player) {
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;

    // 플레이어의 공격 함수 설정 (다형성 구현)
    player->base.attack_func = player_attack;

    player->item_count = 0;  // 아이템 초기화
}