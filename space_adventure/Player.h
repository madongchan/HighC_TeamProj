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
    int signal_device_count; // 신호 장치 발견 개수
} Player;

void player_attack(void* self, void* target) {
    character_attack(self, target);  // 기본 공격 로직 호출
}

// 플레이어 데이터 저장
void save_player_data(const char* filename, const Player* player) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    // 기본 정보 저장
    fprintf(file, "Health: %d\n", player->base.health);
    fprintf(file, "Attack: %d\n", player->base.attack);
    fprintf(file, "Defense: %d\n", player->base.defense);
    fprintf(file, "X: %d\n", player->x);
    fprintf(file, "Y: %d\n", player->y);
    fprintf(file, "ItemCount: %d\n", player->item_count);
    fprintf(file, "SignalDeviceCount: %d\n", player->signal_device_count);

    // 인벤토리 저장
    for (int i = 0; i < player->item_count; i++) {
        fprintf(file, "ItemIndex: %d, Name: %s, Type: %d, Value: %d\n",
            player->inventory[i].index,
            player->inventory[i].name,
            player->inventory[i].type,
            player->inventory[i].value);
    }

    fclose(file);
}

// 플레이어 데이터 불러오기
Player* load_player_data(const char* filename) {
    FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fclose(file);
		return NULL;
	}

    Player* player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        fclose(file);
        return NULL;
    }
    char line[256];
    char name[100];

    // 기본 정보 불러오기
    fscanf(file, "Health: %d\n", &player->base.health);
    fscanf(file, "Attack: %d\n", &player->base.attack);
    fscanf(file, "Defense: %d\n", &player->base.defense);
    fscanf(file, "X: %d\n", &player->x);
    fscanf(file, "Y: %d\n", &player->y);
    fscanf(file, "ItemCount: %d\n", &player->item_count);
    fscanf(file, "SignalDeviceCount: %d\n", &player->signal_device_count);

    // 인벤토리 불러오기
    for (int i = 0; i < player->item_count; i++) {
        fscanf(file, "ItemIndex: %d, Name: %[^,], Type: %d, Value: %d\n",
            &player->inventory[i].index,
            player->inventory[i].name,
            &player->inventory[i].type,
            &player->inventory[i].value);
    }

    // 함수 포인터 재설정
    player->base.attack_func = player_attack;
    fclose(file);
	return player;
}

Player* create_player() {
    // 플레이어 데이터가 저장된 파일이 있는지 확인 
    Player* player;
    /*player = load_player_data("player_save.txt");
	if (player != NULL) {
		return player;
	}*/

    player = (Player*)malloc(sizeof(Player));
    player->base.type = PLAYER;
    player->base.health = 100;
    player->base.attack = 20;
    player->base.defense = 10;
    player->base.attack_func = player_attack;
    player->x = 1;
    player->y = 1;
    // 플레이어가 가진 아이템 목록 초기화
    for (int i = 0; i < MAX_ITEMS; ++i) {
        player->inventory[i].index = -1;  // 초기값으로 초기화
        memset(player->inventory[i].name, 0, sizeof(player->inventory[i].name));
        player->inventory[i].type = -1; // 초기값으로 초기화
        player->inventory[i].value = 0;   // 초기값으로 초기화
    }
    player->item_count = 0;
    player->signal_device_count = 4;

    return player;
}