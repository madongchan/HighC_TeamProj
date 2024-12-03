#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>


#include "Utils.h"
#include "Player.h"
#include "Enemy.h"
#include "Canvas.h"
#include "ItemManager.h"

// ���� ����
ItemSystem* itemSystem; // ������ �ý���
EnemySystem* enemySystem; // �� �ý���
Player* player; // ���� ����ϴ� �÷��̾� ��ü ���� ������ ����
EventType event_type = None; // �̺�Ʈ Ÿ�� ���� ������ ����

void display_game() {
	display_game_area();
	display_game_art(event_type);
	display_status_area(player);
	render();
}

void use_item(Enemy* enemy) {
	if (player->item_count <= 0) {
		add_message("����� �� �ִ� �������� �����ϴ�.");
		return;
	}

	// �÷��̾��� �κ��丮 ���
	add_message("����� �������� �����ϼ���:");
	for (int i = 0; i < player->item_count; i++) {
		char item_message[100];
		snprintf(item_message, sizeof(item_message), "[%d] %s", i, player->inventory[i].name);
		add_message(item_message);
	}
	display_game(); // ȭ�� ������Ʈ

	// ����� �Է� ���
	char input = _getch(); // ������ �ε����� �Է¹���
	int item_index = input - '0'; // �Էµ� ���� '0'~'9'�� ������ ��ȯ

	if (item_index < 0 || item_index >= player->item_count) {
		add_message("�߸��� �ε����Դϴ�.");
		return;
	}

	// ���õ� ������ ���
	Item* selected_item = &player->inventory[item_index];
	switch (selected_item->index) {
	case 0: // �Ƿ�ŰƮ
		add_message("�Ƿ�ŰƮ�� ����߽��ϴ�.");
		player->base.health += selected_item->value;
		break;
	case 1: // ü��ĸ��
		add_message("ü��ĸ���� ����߽��ϴ�.");
		player->base.health += selected_item->value;
		break;
	case 3: // �ö�� ��
		add_message("�ö�� ���� ����߽��ϴ�.");
		enemy->base.health -= selected_item->value;
		break;
	case 5: // ������ �ǵ�
		add_message("������ �ǵ带 ����߽��ϴ�.");
		player->base.defense += selected_item->value;
		break;
	case 6: // ������ ����
		add_message("������ ������ ����߽��ϴ�.");
		int relic_effect = rand() % 3; // ���� ȿ��
		switch (relic_effect) {
		case 0:
			add_message("���ݷ��� �����߽��ϴ�.");
			player->base.attack += 10;
			break;
		case 1:
			add_message("ü���� ȸ���Ǿ����ϴ�.");
			player->base.health += 10;
			break;
		case 2:
			add_message("������ �����߽��ϴ�.");
			player->base.defense += 10;
			break;
		}
		break;
	default:
		add_message("�� �� ���� �������Դϴ�.");
		return;
	}

	// ����� ������ ����
	for (int i = item_index; i < player->item_count - 1; i++) {
		player->inventory[i] = player->inventory[i + 1];
	}
	player->item_count--;

	add_message("�������� ����߽��ϴ�.");
	display_game(); // ȭ�� ������Ʈ
}


void battle(Enemy* enemy) {
	// ���� ���� �ȳ��� ���
	char message[1000];
	snprintf(message, sizeof(message), "'%s'�� �����߽��ϴ�!", enemy->name);
	add_message(message);

	while (player->base.health > 0 && enemy->base.health > 0) {
		// �÷��̾��� ��
		if (player->item_count > 0) {
			use_item(enemy);
		}
		else {
			player_attack(&player->base, &enemy->base);
			snprintf(message, sizeof(message), "�÷��̾ '%s'�� �����߽��ϴ�.", enemy->name);
			add_message(message);
			snprintf(message, sizeof(message), "�� '%s' ü��: %d", enemy->name, enemy->base.health);
			add_message(message);
		}

		if (enemy->base.health <= 0) {
			snprintf(message, sizeof(message), "�� '%s'�� �����ƽ��ϴ�!", enemy->name);
			add_message(message);
		}

		// ���� ��
		enemy_attack(&enemy->base, &player->base);
		snprintf(message, sizeof(message), "�� '%s'�� �÷��̾ �����߽��ϴ�.", enemy->name);
		add_message(message);

		if (player->base.health <= 0) {
			add_message("�÷��̾ ����߽��ϴ�! ���� ����!");
		}

		Sleep(1000); // �� �� ��� �ð�
	}

	// ���� ���� �� �� �޸� ����
	free(enemy);
}

void add_random_item_to_player() {
	if (player->item_count < MAX_ITEMS) {
		int random_index = rand() % itemSystem->count;
		// ���� �������� ȹ���ߴ��� �޽��� ���
		char message[1000];
		snprintf(message, sizeof(message), "������ '%s'�� ȹ���߽��ϴ�!", itemSystem->items[random_index].name);
		add_message(message);
		// �Ҹ�ǰ �������� ��� ȹ���ϸ� �÷��̾� �κ��丮�� �߰�
		if (itemSystem->items[random_index].type == 0) {
			player->inventory[player->item_count].index = itemSystem->items[random_index].index;
			// ������ �̸��� ����
			strncpy(player->inventory[player->item_count].name, itemSystem->items[random_index].name, sizeof(player->inventory[player->item_count].name) - 1);
			// ���ڿ� ���� NULL ���� �߰�
			player->inventory[player->item_count].name[sizeof(player->inventory[player->item_count].name) - 1] = '\0';

			player->inventory[player->item_count].type = itemSystem->items[random_index].type;
			player->inventory[player->item_count].value = itemSystem->items[random_index].value;
			player->item_count++;
		}
		// ��� �������� ���
		if (itemSystem->items[random_index].type == 1)
		{
			// ������ ���̵��� ���
			if (itemSystem->items[random_index].index == 2)
			{
				player->base.attack += itemSystem->items[random_index].value;
				char message[1000];
				snprintf(message, sizeof(message), "���ݷ��� %d �����Ͽ����ϴ�.", itemSystem->items[random_index].value);
				add_message(message);
				return;
			}
			// ��ȭ ��Ʈ�� ���
			else if (itemSystem->items[random_index].index == 4)
			{
				player->base.defense += itemSystem->items[random_index].value;
				char message[1000];
				snprintf(message, sizeof(message), "������ %d �����Ͽ����ϴ�.", itemSystem->items[random_index].value);
				add_message(message);
				return;
			}
		}
	}
	else {
		add_message("�� �̻� �������� ������ �� �����ϴ�.");
	}
}
void random_event() {
	event_type = (EventType)(rand() % 4); // �� ���� ���� �̺�Ʈ �� �ϳ� �߻�
	//event_type = STORM; // �� ���� ���� �̺�Ʈ �� �ϳ� �߻�
	char event_message[1000];     // ���� �̺�Ʈ �޽����� ������ ����
	int damage = (rand() % 40) + 10;

	int enemy_random_index = rand() % enemySystem->count; // ���� ���� �������� �������� ����

	switch (event_type) {
	case SIGNAL_DEVICE:
		// ��ȣ��ġ �̺�Ʈ
		event_type = SIGNAL_DEVICE;
		snprintf(event_message, sizeof(event_message), "��ȣ��ġ�� �߰��Ͽ����ϴ�!");
		add_message(event_message);
		player->signal_device_count++;
		break;
	case STORM:
		// ��ǳ �̺�Ʈ
		event_type = STORM;
		snprintf(event_message, sizeof(event_message), "��ǳ�� �߻��߽��ϴ�! ü���� %d �����մϴ�.", damage);
		add_message(event_message);
		player->base.health -= damage;
		break;
	case ITEM_FOUND:
		// ������ �̺�Ʈ
		event_type = ITEM_FOUND;
		snprintf(event_message, sizeof(event_message), "�������� �߰��Ͽ����ϴ�.");
		add_message(event_message);
		add_random_item_to_player(player);
		break;
	case 3:
		// ���� �̺�Ʈ
		event_type += enemy_random_index;
		snprintf(event_message, sizeof(event_message), "���� �������ϴ�! ���� ����!");
		add_message(event_message);
		Enemy* enemy = &enemySystem->enemies[enemy_random_index];
		battle(enemy);
		break;
	}

	display_game();

	// ������ ���� ���̶�� �ȳ��ϰ� �� ������ �޽����� ����
	//save_player_data(player, "player_save.dat");
	for (int i = 0; i <= 100; i += (rand() % 10)) {
		char progress_message[100];
		snprintf(progress_message, sizeof(progress_message), "���� ��Ȳ ���� ��... %d%%", i);
		add_message(progress_message);
		display_game();
		remove_last_message();
		Sleep(100); // 0.5�� ���
	}
	remove_messages();
	event_type = None;
}

void move_room(char direction) {
	for (int i = 0; i < 3; i++) {
		char message[1000];
		snprintf(message, sizeof(message), "%s �������� �̵� ��%.*s",
			direction == 'N' ? "����" :
			direction == 'S' ? "����" :
			direction == 'E' ? "����" : "����",
			i + 1, "...");

		remove_messages();
		add_message(message);  // �޽��� �ý��ۿ� �޽��� �߰�
		display_game(); // ȭ�� ���
		Sleep(333);           // 0.333�� ���

		// ����� �޽��� ���� (�� �� �޽����� ����� �迭�� ������ ���)
		if (i < 2) { // ������ �޽����� �������� ����
			remove_messages();
		}
	}
	random_event(player); // ���� �̺�Ʈ �߻�
}



void game_loop() {
	player = create_player();
	while (player->base.health > 0) {
		srand((unsigned int)time(NULL));  // ���� ������ �ʱ�ȭ
		clear_back_buffer();
		display_game(); // ȭ�� ���
		Sleep(1000); // 1�� ���
	}
	free(player);
}


bool is_map_open = false; // �� ���¸� ��Ÿ���� ����

void handle_map_key(Player* player) {
	if (is_map_open) {
		// ���� ���������� ���� �ݰ� ���� ȭ������ ���ư�
		is_map_open = false;
		display_game_area();
		display_status_area(player);
		render();
	}
	else {
		// ���� ����
		is_map_open = true;
		display_map(player);
	}
}



int main() {
	init_console(); // �ܼ� �ʱ�ȭ ( Ŀ�� ����, �ܼ� â �ִ�ȭ)
	update_console_size(); // �ܼ� â ũ�� ����

	// ������ �ý��� ����
	itemSystem = create_item_system();
	load_items_from_file(itemSystem, "items.txt");
	enemySystem = create_enemy_system();
	load_enemies_from_file(enemySystem, "Enemies.txt");

	// �÷��̾� ����
	player = create_player();

	const int FPS = 33;
	const int frameDelay = 1000 / FPS;  // �� �����Ӵ� ��� �ð� (�и���)
	bool is_able_input = true;

	while (player->base.health > 0) { // �÷��̾� ü���� 0 �̻��� ���� �ݺ�
		// �ܼ� â�� ũ�Ⱑ ����Ǿ��� �� �ܼ� â ũ�� ������Ʈ
		update_console_size();

		srand((unsigned int)time(NULL));  // ���� ������ �ʱ�ȭ
		is_able_input = true; // �Է� ���� ���·� ����
		clear_back_buffer();


		// ����ŷ ������� �Է� �ޱ� (_kbhit() ���)
		if (_kbhit() && is_able_input) {  // Ű���� �Է��� �ִ��� Ȯ�� && �Է� ���� �������� Ȯ��
			is_able_input = false; // �Է� �Ұ��� ���·� ����
			char input = toupper(_getch());  // Ű �Է��� �빮�ڷ� �ޱ�
			switch (input)
			{
			case 'W':
				move_room('N');
				player->y--;
				break;
			case 'S':
				move_room('S');
				player->y++;
				break;
			case 'D':
				move_room('E');
				player->x++;
				break;
			case 'A':
				move_room('W');
				player->x--;
				break;
			case 'Q': // 'q'�� ������ ���� ����
				add_message("���� ����!");
				display_game();
				Sleep(1000);
				exit(1);
				break;
			case 'M':
				handle_map_key(player);
				break;
			default:
				break;
			}
		}
		if (player->base.health <= 0) {
			add_message("�÷��̾� ����!");
			free(player);
			break;
		}
		if (player->signal_device_count == MAX_SIGNAL_DEVICE)
		{
			add_message("���� Ŭ����!");
			free(player);
			break;
		}
		if (is_map_open == false)
		{
			display_game(); // ȭ�� ���
		}
		Sleep(frameDelay); // 30fps�� ����
	}
	free_back_buffer();
	free_message_system();
	return 0;
}