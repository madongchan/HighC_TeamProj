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

// 플레이어 데이터 초기화 함수
void init_player(Player* player, const char* filename) {
	// 기본 플레이어 데이터 초기화
	player->base.health = 100;
	player->base.attack = 20;
	player->base.defense = 10;
	player->x = 1;
	player->y = 1;
	player->item_count = 0;
	player->signal_device_count = 0;
	player->base.attack_func = player_attack;

	// 기본 플레이어 데이터 작성
	FILE* file = fopen(filename, "w");
	fprintf(file, "Health: 100\n");
	fprintf(file, "Attack: 20\n");
	fprintf(file, "Defense: 10\n");
	fprintf(file, "X: 1\n");
	fprintf(file, "Y: 1\n");
	fprintf(file, "ItemCount: 0\n");
	fprintf(file, "SignalDeviceCount: 0\n");
	// 인벤토리는 공백으로 초기화 Max 5개
	for (int i = 0; i < MAX_ITEMS; i++) {
		fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
	}
}

// 플레이어 데이터 저장
void save_player_data(const Player* player, const char* filename) {
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

// 플레이어 데이터 불러오기
Player* load_player_data(const char* filename) {
	FILE* file = fopen(filename, "r");

	// 파일이 없으면 새로 생성
	if (!file) {
		file = fopen(filename, "w");
		if (!file) return NULL;

		// 기본 플레이어 데이터 작성
		fprintf(file, "Health: 100\n");
		fprintf(file, "Attack: 20\n");
		fprintf(file, "Defense: 10\n");
		fprintf(file, "X: 1\n");
		fprintf(file, "Y: 1\n");
		fprintf(file, "ItemCount: 0\n");
		fprintf(file, "SignalDeviceCount: 0\n");
		// 인벤토리는 공백으로 초기화 Max 5개
		for (int i = 0; i < MAX_ITEMS; i++) {
			fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
		}


		fclose(file);
		file = fopen(filename, "r");  // 읽기 모드로 다시 열기
		if (!file) return NULL;
	}
	// 파일 크기 확인
	fseek(file, 0, SEEK_END); // 파일 끝으로 이동
	long file_size = ftell(file); // 파일 크기 확인
	rewind(file); // 파일 처음으로 이동

	// 파일이 비어있으면 기본 데이터 작성
	if (file_size == 0) {
		fclose(file);
		file = fopen(filename, "w");
		if (!file) return NULL;

		// 기본 플레이어 데이터 작성 (위와 동일)
		fprintf(file, "Health: 100\n");
		fprintf(file, "Attack: 20\n");
		fprintf(file, "Defense: 10\n");
		fprintf(file, "X: 1\n");
		fprintf(file, "Y: 1\n");
		fprintf(file, "ItemCount: 0\n");
		fprintf(file, "SignalDeviceCount: 0\n");
		// 인벤토리는 공백으로 초기화 Max 5개
		for (int i = 0; i < MAX_ITEMS; i++) {
			fprintf(file, "ItemIndex: -1, Name: , Type: -1, Value: -1\n");
		}

		fclose(file);
		file = fopen(filename, "r");  // 읽기 모드로 다시 열기
		if (!file) return NULL;
	}

	// 플레이어 객체 동적 생성
	Player* player = (Player*)malloc(sizeof(Player));
	if (player == NULL) {
		fclose(file);
		return NULL;
	}

	// 파일에서 데이터 읽어와 플레이어 초기화
	if (fscanf(file, "Health: %d\n", &player->base.health) != 1 ||
		fscanf(file, "Attack: %d\n", &player->base.attack) != 1 ||
		fscanf(file, "Defense: %d\n", &player->base.defense) != 1 ||
		fscanf(file, "X: %d\n", &player->x) != 1 ||
		fscanf(file, "Y: %d\n", &player->y) != 1 ||
		fscanf(file, "ItemCount: %d\n", &player->item_count) != 1 ||
		fscanf(file, "SignalDeviceCount: %d\n", &player->signal_device_count) != 1) {

		// 읽기 실패 시 메모리 해제 및 NULL 반환
		free(player);
		fclose(file);
		return NULL;
	}
	
	// 인벤토리 초기화
	for (int i = 0; i < player->item_count && i < MAX_ITEMS; i++) {
		if (fscanf(file, "ItemIndex: %d, Name: %[^,], Type: %d, Value: %d\n",
			&player->inventory[i].index,
			player->inventory[i].name,
			&player->inventory[i].type,
			&player->inventory[i].value) != 4) {

			// 읽기 실패 시 메모리 해제 및 NULL 반환
			free(player);
			fclose(file);
			return NULL;
		}
	}

	// 함수 포인터 설정
	player->base.attack_func = player_attack;
	fclose(file);
	// 만약 플레이어 체력이 0 이하면 초기화 해줌
	if (player->base.health <= 0) {
		init_player(player, filename);
	}
	return player;
}

Player* create_player() {
	// 플레이어 데이터가 저장된 파일이 있는지 확인 
	Player* player;
	player = load_player_data("player_save.txt");
	if (player != NULL) {
		return player;
	}

	// 못 불러왔으면 에러 메시지 발생 후 게임 종료
	printf("플레이어 데이터를 불러오는 데 실패했습니다.\n");
	exit(1);
	return NULL;
}