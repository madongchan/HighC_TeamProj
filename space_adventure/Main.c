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
Player* player; // ���� ����ϴ� �÷��̾ ���� ������ ����
Enemy* enemy; // ���� ����ϴ� �÷��̾ ���� ������ ����

void display_game() {
	display_game_area();
	display_status_area(player);
	render();
}

void display_map(Player* player) {
	clear_back_buffer(); // ���� ȭ�� �����

	// �� ���� 3x5 ũ��� ����
	const int room_height = 4;
	const int room_width = 12;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int start_x = j * (room_width + 4); // �� ���� ���� x ��ǥ (��� ���� �߰�)
			int start_y = i * (room_height + 2); // �� ���� ���� y ��ǥ (��� ���� �߰�)

			// �� ��輱 �׸���
			draw_to_back_buffer(start_x, start_y, "------------");
			for (int k = 1; k < room_height - 1; k++) {
				char buffer[13]; // �迭 ũ�⸦ ��Ȯ�� ����
				strncpy(buffer, "|          |", sizeof(buffer) - 1);
				buffer[sizeof(buffer) - 1] = '\0';
				draw_to_back_buffer(start_x, start_y + k, buffer);
			}
			draw_to_back_buffer(start_x, start_y + room_height - 1, "------------");

			// �÷��̾� ��ġ ǥ��
			if (i == player->y && j == player->x) {
				draw_to_back_buffer(start_x + room_width / 2, start_y + room_height / 2, "P");
			}

			// ��� �� ������ ��� �׸���
			if (j < 2) { // ���������� ��� �߰� (������ �� ����)
				draw_to_back_buffer(start_x + room_width, start_y + room_height / 2, "####");
			}
			if (i < 2) { // �Ʒ������� ��� �߰� (������ �� ����)
				for (int k = 0; k < 4; k++) {
					draw_to_back_buffer(start_x + room_width / 2, start_y + room_height + k, "#");
				}
			}
		}
	}

	display_status_area(player);
	render();
}




void use_item(Enemy* enemy) {
	add_message("�������� ����Ͻðڽ��ϱ�? : (0 : �Ƿ�ŰƮ ���, 1 : ü��ĸ�� ���)");
	add_message("(2 : �ö�� �� ���, 3 : ������ �ǵ� ���, 4 : ������ ���� ���");
	add_message("������ �ε��� ��ȣ�� �Է����ּ��� : ");
	display_game(); // ȭ�� ���
	int item_index = _getch();
	switch (item_index)
	{
	case '0':
		add_message("�Ƿ�ŰƮ�� ����߽��ϴ�.");
		player->base.health += 10;
		break;
	case '1':
		add_message("ü��ĸ���� ����߽��ϴ�.");
		player->base.health += 5;
		break;
	case '2':
		add_message("�ö�� ���� ����߽��ϴ�");
		enemy->base.health -= 20;
		break;
	case '3':
		add_message("������ �ǵ带 ����߽��ϴ�.");
		player->base.defense += 10;
		break;
	case '4':
		add_message("������ ������ ����߽��ϴ�.");
		int relics = rand() % 3 + 1;
		//������ ������ ����ȿ��
		switch (relics)
		{
		case '1':
			add_message("���ݷ��� �ö󰬽��ϴ�.");
			player->base.attack += 10;
			break;
		case '2':
			add_message("ü���� ȸ���Ǿ����ϴ�.");
			player->base.health += 10;
			break;
		case '3':
			add_message("������ �ö󰬽��ϴ�.");
			player->base.defense += 10;
			break;
		}
	}
}

void battle(Enemy* enemy) {
	// ���� ���� �ȳ��� ���
	char* message[1000];
	snprintf(message, sizeof(message), "'%s'�� �����ߴ�.", enemy->name);
	add_message(message);
	if (player->item_count > 0)
	{
		use_item(enemy);
	}
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
	//int event_type = rand() % 5; // �� ���� ���� �̺�Ʈ �� �ϳ� �߻�
	int event_type = 3; // �� ���� ���� �̺�Ʈ �� �ϳ� �߻�
	char event_message[1000];     // ���� �̺�Ʈ �޽����� ������ ����
	int damage = (rand() % 40) + 10;

	switch (event_type) {
	case 0:
		// ��ȣ��ġ �̺�Ʈ
		snprintf(event_message, sizeof(event_message), "��ȣ��ġ�� �߰��Ͽ����ϴ�!");
		add_message(event_message);
		player->signal_device_count++;
		break;
	case 1:
		// ��ǳ �̺�Ʈ
		snprintf(event_message, sizeof(event_message), "��ǳ�� �߻��߽��ϴ�! ü���� %d �����մϴ�.", damage);
		add_message(event_message);
		player->base.health -= damage;
		break;
	case 2:
		// ������ �̺�Ʈ
		snprintf(event_message, sizeof(event_message), "�������� �߰��Ͽ����ϴ�.");
		add_message(event_message);
		add_random_item_to_player(player);
		break;
	case 3:
		// ���� �̺�Ʈ
		snprintf(event_message, sizeof(event_message), "���� �������ϴ�! ���� ����!");
		add_message(event_message);
		Enemy* enemy = create_enemy();
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

	// �÷��̾� ����
	player = create_player();

	const int FPS = 33;
	const int frameDelay = 1000 / FPS;  // �� �����Ӵ� ��� �ð� (�и���)
	bool is_able_input = true;

	while (player->base.health > 0) { // �÷��̾� ü���� 0 �̻��� ���� �ݺ�
		// �ܼ� â�� ũ�Ⱑ ����Ǿ��� �� �ܼ� â ũ�� ������Ʈ
		if (is_console_size_changed()) {
			update_console_size();
		}
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