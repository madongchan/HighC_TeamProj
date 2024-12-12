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

// �÷��̾� ������ �ʱ�ȭ �Լ�
void init_player(Player* player, const char* filename) {
	// �⺻ �÷��̾� ������ �ʱ�ȭ
	player->base.health = 100;
	player->base.attack = 20;
	player->base.defense = 10;
	player->x = 1;
	player->y = 1;
	player->item_count = 0;
	player->signal_device_count = 0;
	player->base.attack_func = player_attack;

	// �⺻ �÷��̾� ������ �ۼ�
	FILE* file = fopen(filename, "w");
	fprintf(file, "Health: 100\n");
	fprintf(file, "Attack: 20\n");
	fprintf(file, "Defense: 10\n");
	fprintf(file, "X: 1\n");
	fprintf(file, "Y: 1\n");
	fprintf(file, "ItemCount: 0\n");
	fprintf(file, "SignalDeviceCount: 0\n");
	// �κ��丮�� �������� �ʱ�ȭ Max 5��
	for (int i = 0; i < MAX_ITEMS; i++) {
		fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
	}
}

// �÷��̾� ������ ����
void save_player_data(const Player* player, const char* filename) {
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
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (i < player->item_count) {
			fprintf(file, "ItemIndex: %d, Name: %s, Type: %d, Value: %d\n",
				player->inventory[i].index,
				player->inventory[i].name,
				player->inventory[i].type,
				player->inventory[i].value);
		}
		else {
			fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
		}
	}

	fclose(file);
}

// �÷��̾� ������ �ҷ�����
Player* load_player_data(const char* filename) {
	FILE* file = fopen(filename, "r");

	// ������ ������ ���� ����
	if (!file) {
		file = fopen(filename, "w");
		if (!file) return NULL;

		// �⺻ �÷��̾� ������ �ۼ�
		fprintf(file, "Health: 100\n");
		fprintf(file, "Attack: 20\n");
		fprintf(file, "Defense: 10\n");
		fprintf(file, "X: 1\n");
		fprintf(file, "Y: 1\n");
		fprintf(file, "ItemCount: 0\n");
		fprintf(file, "SignalDeviceCount: 0\n");
		// �κ��丮�� �������� �ʱ�ȭ Max 5��
		for (int i = 0; i < MAX_ITEMS; i++) {
			fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
		}


		fclose(file);
		file = fopen(filename, "r");  // �б� ���� �ٽ� ����
		if (!file) return NULL;
	}
	// ���� ũ�� Ȯ��
	fseek(file, 0, SEEK_END); // ���� ������ �̵�
	long file_size = ftell(file); // ���� ũ�� Ȯ��
	rewind(file); // ���� ó������ �̵�

	// ������ ��������� �⺻ ������ �ۼ�
	if (file_size == 0) {
		fclose(file);
		file = fopen(filename, "w");
		if (!file) return NULL;

		// �⺻ �÷��̾� ������ �ۼ� (���� ����)
		fprintf(file, "Health: 100\n");
		fprintf(file, "Attack: 20\n");
		fprintf(file, "Defense: 10\n");
		fprintf(file, "X: 1\n");
		fprintf(file, "Y: 1\n");
		fprintf(file, "ItemCount: 0\n");
		fprintf(file, "SignalDeviceCount: 0\n");
		// �κ��丮�� �������� �ʱ�ȭ Max 5��
		for (int i = 0; i < MAX_ITEMS; i++) {
			fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
		}

		fclose(file);
		file = fopen(filename, "r");  // �б� ���� �ٽ� ����
		if (!file) return NULL;
	}

	// �÷��̾� ��ü ���� ����
	Player* player = (Player*)malloc(sizeof(Player));
	if (player == NULL) {
		fclose(file);
		return NULL;
	}

	// ���Ͽ��� ������ �о�� �÷��̾� �ʱ�ȭ
	if (fscanf(file, "Health: %d\n", &player->base.health) != 1 ||
		fscanf(file, "Attack: %d\n", &player->base.attack) != 1 ||
		fscanf(file, "Defense: %d\n", &player->base.defense) != 1 ||
		fscanf(file, "X: %d\n", &player->x) != 1 ||
		fscanf(file, "Y: %d\n", &player->y) != 1 ||
		fscanf(file, "ItemCount: %d\n", &player->item_count) != 1 ||
		fscanf(file, "SignalDeviceCount: %d\n", &player->signal_device_count) != 1) {

		// �б� ���� �� �޸� ���� �� NULL ��ȯ
		free(player);
		fclose(file);
		return NULL;
	}
	
	// �κ��丮 �ʱ�ȭ
	for (int i = 0; i < player->item_count && i < MAX_ITEMS; i++) {
		if (fscanf(file, "ItemIndex: %d, Name: %[^,], Type: %d, Value: %d\n",
			&player->inventory[i].index,
			player->inventory[i].name,
			&player->inventory[i].type,
			&player->inventory[i].value) != 4) {

			// �б� ���� �� �޸� ���� �� NULL ��ȯ
			free(player);
			fclose(file);
			return NULL;
		}
	}

	// �Լ� ������ ����
	player->base.attack_func = player_attack;
	fclose(file);
	// ���� �÷��̾� ü���� 0 ���ϸ� �ʱ�ȭ ����
	if (player->base.health <= 0) {
		init_player(player, filename);
	}
	return player;
}

Player* create_player() {
	// �÷��̾� �����Ͱ� ����� ������ �ִ��� Ȯ�� 
	Player* player;
	player = load_player_data("player_save.txt");
	if (player != NULL) {
		return player;
	}

	// �� �ҷ������� ���� �޽��� �߻� �� ���� ����
	printf("�÷��̾� �����͸� �ҷ����� �� �����߽��ϴ�.\n");
	exit(1);
	return NULL;
}