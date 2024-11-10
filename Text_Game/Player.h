#pragma once
#include "Character.h"

typedef struct {
    Character base;  // Character 구조체를 상속받음 (첫 번째 멤버로 포함)
    char items[10][30];  // 플레이어만의 특수한 속성 (아이템 목록)
    int item_count;
} Player;

// 플레이어 전용 함수 선언
void player_attack(void* self, void* target);
void init_player(Player* player);