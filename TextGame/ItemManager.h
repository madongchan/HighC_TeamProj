#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 아이템 타입 열거형
typedef enum {
    CONSUMABLE = 0,  // 소모품
    EQUIPMENT = 1,   // 장비
} ItemType;

// 아이템 구조체
typedef struct {
    char* name;       // 아이템 이름
    ItemType type;    // 아이템 타입
    int value;        // 효과 값
} Item;

// 아이템 시스템 구조체
typedef struct {
    Item* items;    // 아이템 목록
    int count;      // 현재 아이템 개수
} ItemSystem;

// 아이템 시스템 생성
ItemSystem* create_item_system() {
    ItemSystem* system = (ItemSystem*)malloc(sizeof(ItemSystem));
    system->items = NULL;
    system->count = 0;
    return system;
}

void remove_newline(char* str) {
    // 맨 앞의 개행 문자 제거
    if (str[0] == '\n') {
        memmove(str, str + 1, strlen(str));
    }
}

// 파일에서 아이템 로드
void load_items_from_file(ItemSystem* system, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return;
    }

    char name[100];
    int type, value;

    while (fscanf(file, "%[^,],%d,%d", name, &type, &value) == 3) {
        // 개행 문자 제거
        remove_newline(name);

        system->items = (Item*)realloc(system->items, sizeof(Item) * ((size_t)system->count + 1));
        if (!system->items) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            return;
        }

        Item* item = &system->items[system->count];
        item->name = (char*)malloc(strlen(name) + 1);
        if (!item->name) {
            fprintf(stderr, "Error: Memory allocation failed for item name\n");
            fclose(file);
            return;
        }

        strncpy(item->name, name, strlen(name) + 1);
        item->type = (ItemType)type;
        item->value = value;
        system->count++;
    }


    fclose(file);
}

// 아이템 시스템 메모리 해제
void free_item_system(ItemSystem* system) {
    for (int i = 0; i < system->count; i++) {
        free(system->items[i].name);
    }
    free(system->items);
    free(system);
}
