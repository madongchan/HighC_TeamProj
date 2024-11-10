#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "Player.h"
#include "Enemy.h"

// 화면을 지우는 함수
void clear_screen() {
#ifdef _WIN32
    system("cls");  // Windows 환경에서 화면 지우기
#else
    system("clear"); // Unix/Linux 환경에서 화면 지우기
#endif
}

// 커서를 원하는 위치로 이동시키는 함수 (Windows 전용)
void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 콘솔 창 크기를 가져오는 함수 (Windows 전용)
COORD get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD size;
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

// 상단 게임 로직 공간 출력 함수
void display_game_area(const char* message) {
    // 커서를 상단으로 이동하여 게임 로직 메시지 출력
    set_cursor_position(0, 0);

    printf("===== 게임 로직 공간 =====\n");
    printf("%s\n", message);
    printf("=========================\n\n");
}

// 하단 플레이어 스탯 및 아이템 공간 출력 함수
void display_status_area(Player* player) {
    COORD console_size = get_console_size();

    // 하단으로 커서를 이동하여 플레이어 상태와 아이템 출력
    set_cursor_position(0, console_size.Y - 6);  // 맨 아래에서 6줄 위로 커서 이동 (스탯 및 아이템 공간 확보)

    printf("\n===== 플레이어 상태 =====\n");
    printf("체력: %d\n", player->base.health);
    printf("공격력: %d\n", player->base.attack);
    printf("방어력: %d\n", player->base.defense);

    printf("\n===== 소지 아이템 =====\n");
    for (int i = 0; i < player->item_count; ++i) {
        printf("%s\n", player->items[i]);
    }

    // 마지막 줄까지 채워서 하단 고정 유지
    printf("=========================\n");
}


void battle(Player* player, Enemy* enemy) {
    printf("전투 시작! %s와(과) 싸웁니다!\n", enemy->name);

    while (player->base.health > 0 && enemy->base.health > 0) {
        // 플레이어가 적을 공격
        int damage_to_enemy = player->base.attack;
        enemy->base.health -= damage_to_enemy;
        printf("플레이어가 %s에게 %d 피해를 입혔습니다.\n", enemy->name, damage_to_enemy);

        if (enemy->base.health <= 0) {
            printf("%s를 물리쳤습니다!\n", enemy->name);
            break;
        }

        // 적이 플레이어를 공격
        int damage_to_player = enemy->base.attack - (player->base.defense * 0.4);
        if (damage_to_player < 0) damage_to_player = 0; // 방어력이 높으면 피해가 없을 수도 있음
        player->base.health -= damage_to_player;
        printf("%s가 플레이어에게 %d 피해를 입혔습니다.\n", enemy->name, damage_to_player);

        if (player->base.health <= 0) {
            printf("플레이어가 사망했습니다...\n");
            break;
        }
    }
}

void use_item(Player* player, char* item_name) {
    if (strcmp(item_name, "의료 키트") == 0) {
        player->base.health += 10; // 체력 회복
        printf("의료 키트를 사용하여 체력이 회복되었습니다! 현재 체력: %d\n", player->base.health);
    }
    else if (strcmp(item_name, "에너지 블레이드") == 0) {
        player->base.attack += 5; // 공격력 증가
        printf("에너지 블레이드를 사용하여 공격력이 증가했습니다! 현재 공격력: %d\n", player->base.attack);
    }
    else if (strcmp(item_name, "강화 슈트") == 0) {
        player->base.defense += 5; // 방어력 증가
        printf("강화 슈트를 착용하여 방어력이 증가했습니다! 현재 방어력: %d\n", player->base.defense);
    }
    else {
        printf("알 수 없는 아이템입니다.\n");
    }
}

void random_event(Player* player) {
    int event_type = rand() % 3; // 세 가지 랜덤 이벤트 중 하나 발생

    switch (event_type) {
    case 0:
        printf("외계 광물을 발견했습니다! 공격력이 증가합니다.\n");
        player->base.attack += 2;
        break;
    case 1:
        printf("폭풍이 발생했습니다! 체력이 감소합니다.\n");
        player->base.health -= 5;
        break;
    case 2:
        printf("친근한 외계 생명체와 만났습니다! 정보를 얻었습니다.\n");
        break;
    }
}

void move_room(char direction) {
    switch (direction) {
    case 'N':
        printf("북쪽으로 이동합니다.\n");
        break;
    case 'S':
        printf("남쪽으로 이동합니다.\n");
        break;
    case 'E':
        printf("동쪽으로 이동합니다.\n");
        break;
    case 'W':
        printf("서쪽으로 이동합니다.\n");
        break;
    default:
        printf("잘못된 방향입니다.\n");
        break;
    }
}

int main() {
    Player player;
    Enemy enemy;

    // 플레이어와 적 초기화
    init_player(&player);
    init_enemy(&enemy);

    char command;

    while (player.base.health > 0) {
        // 화면을 먼저 지우고 상단 게임 로직 공간 초기화 및 안내 메시지 출력
        //clear_screen();
        display_game_area("명령을 입력하세요 (N: 북쪽 이동, S: 남쪽 이동, E: 동쪽 이동, W: 서쪽 이동, B: 전투 시작, I: 아이템 사용)");
		scanf("%c", &command);
        // 하단 플레이어 상태 및 아이템 정보 출력 (항상 하단에 고정)
        display_status_area(&player);

        // 명령 입력 받기
        set_cursor_position(0, get_console_size().Y - 1);
        

        switch (command) {
        case 'n':
        case 's':
        case 'e':
        case 'w':
            display_game_area("방향으로 이동 중...");
            random_event(&player); // 이동 중 랜덤 이벤트 발생 가능
            break;

        case 'B':
            display_game_area("프레데터와 전투 중...");
            battle(&player, &enemy); // 프레데터와 전투
            break;

        case 'I':
            use_item(&player, "의료 키트"); // 예시로 의료 키트 사용
            break;

        default:
            display_game_area("잘못된 명령입니다.");
            break;
        }

        if (player.base.health <= 0) {
            display_game_area("게임 오버!");
            break;
        }

        // 명령 실행 후 다시 상단과 하단 갱신
        //clear_screen();
        display_game_area("다음 명령을 기다립니다...");
        display_status_area(&player);
    }

    return 0;
}