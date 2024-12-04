#pragma once
#include "Character.h"
#include "ItemManager.h"

#define MAX_ITEMS 5
#define MAX_SIGNAL_DEVICE 3
typedef struct {
    Character base;  // Character ����ü�� ��ӹ��� (ù ��° ����� ����)
    int x, y;        // �÷��̾��� ��ġ
    Item inventory[MAX_ITEMS];  // �÷��̾�� Ư���� �Ӽ� (������ ���)
    int item_count;
    int signal_device_count; // ��ȣ ��ġ �߰� ����
} Player;

void player_attack(void* self, void* target) {
    character_attack(self, target);  // �⺻ ���� ���� ȣ��
}

// �÷��̾� ������ ����
void save_player_data(const char* filename, const Player* player) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    // �⺻ ���� ����
    fprintf(file, "Health: %d\n", player->base.health);
    fprintf(file, "Attack: %d\n", player->base.attack);
    fprintf(file, "Defense: %d\n", player->base.defense);
    fprintf(file, "X: %d\n", player->x);
    fprintf(file, "Y: %d\n", player->y);
    fprintf(file, "ItemCount: %d\n", player->item_count);
    fprintf(file, "SignalDeviceCount: %d\n", player->signal_device_count);

    // �κ��丮 ����
    for (int i = 0; i < player->item_count; i++) {
        fprintf(file, "ItemIndex: %d, Name: %s, Type: %d, Value: %d\n",
            player->inventory[i].index,
            player->inventory[i].name,
            player->inventory[i].type,
            player->inventory[i].value);
    }

    fclose(file);
}

// �÷��̾� ������ �ҷ�����
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

    // �⺻ ���� �ҷ�����
    fscanf(file, "Health: %d\n", &player->base.health);
    fscanf(file, "Attack: %d\n", &player->base.attack);
    fscanf(file, "Defense: %d\n", &player->base.defense);
    fscanf(file, "X: %d\n", &player->x);
    fscanf(file, "Y: %d\n", &player->y);
    fscanf(file, "ItemCount: %d\n", &player->item_count);
    fscanf(file, "SignalDeviceCount: %d\n", &player->signal_device_count);

    // �κ��丮 �ҷ�����
    for (int i = 0; i < player->item_count; i++) {
        fscanf(file, "ItemIndex: %d, Name: %[^,], Type: %d, Value: %d\n",
            &player->inventory[i].index,
            player->inventory[i].name,
            &player->inventory[i].type,
            &player->inventory[i].value);
    }

    // �Լ� ������ �缳��
    player->base.attack_func = player_attack;
    fclose(file);
	return player;
}

Player* create_player() {
    // �÷��̾� �����Ͱ� ����� ������ �ִ��� Ȯ�� 
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
    // �÷��̾ ���� ������ ��� �ʱ�ȭ
    for (int i = 0; i < MAX_ITEMS; ++i) {
        player->inventory[i].index = -1;  // �ʱⰪ���� �ʱ�ȭ
        memset(player->inventory[i].name, 0, sizeof(player->inventory[i].name));
        player->inventory[i].type = -1; // �ʱⰪ���� �ʱ�ȭ
        player->inventory[i].value = 0;   // �ʱⰪ���� �ʱ�ȭ
    }
    player->item_count = 0;
    player->signal_device_count = 4;

    return player;
}