#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 아이템 타입 정의
typedef enum {
    CONSUMABLE = 0,  // 소비 가능한 아이템
    EQUIPMENT = 1,   // 장비 아이템
} ItemType;

// 아이템 구조체 정의
typedef struct {
    int index;        // 아이템 고유 인덱스
    char name[100];   // 아이템 이름
    ItemType type;    // 아이템 타입
    int value;        // 아이템 가치
} Item;

// 아이템 시스템 구조체 정의
typedef struct {
    Item* items;      // 아이템 목록
    int count;        // 현재 아이템 개수
} ItemSystem;

// 아이템 시스템 생성 함수
ItemSystem* create_item_system() {
    ItemSystem* system = (ItemSystem*)malloc(sizeof(ItemSystem));
    system->items = NULL;
    system->count = 0;
    return system;
}

void remove_newline(char* str) {
    // 문자열의 줄바꿈 문자 제거
    if (str[0] == '\n') {
        memmove(str, str + 1, strlen(str));
    }
}

// 파일에서 아이템 로드 함수
void load_items_from_file(ItemSystem* system, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return;
    }

    char name[100];
    int index, type, value;

    while (fscanf(file, "%d,%[^,],%d,%d", &index, name, &type, &value) == 4) {
        // 줄바꿈 문자 제거
        remove_newline(name);

        system->items = (Item*)realloc(system->items, sizeof(Item) * ((size_t)system->count + 1));
        if (!system->items) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            return;
        }

        Item* item = &system->items[system->count];

        item->index = index;
        strncpy(item->name, name, strlen(name) + 1);
        item->type = (ItemType)type;
        item->value = value;
        system->count++;
    }

    fclose(file);
}

// 아이템 시스템 해제 함수
void free_item_system(ItemSystem* system) {
    free(system->items); // 아이템 목록 해제
    free(system);        // 시스템 해제
}