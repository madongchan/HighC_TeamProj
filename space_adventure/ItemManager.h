#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������ Ÿ�� ����
typedef enum {
    CONSUMABLE = 0,  // �Һ� ������ ������
    EQUIPMENT = 1,   // ��� ������
} ItemType;

// ������ ����ü ����
typedef struct {
    int index;        // ������ ���� �ε���
    char name[100];   // ������ �̸�
    ItemType type;    // ������ Ÿ��
    int value;        // ������ ��ġ
} Item;

// ������ �ý��� ����ü ����
typedef struct {
    Item* items;      // ������ ���
    int count;        // ���� ������ ����
} ItemSystem;

// ������ �ý��� ���� �Լ�
ItemSystem* create_item_system() {
    ItemSystem* system = (ItemSystem*)malloc(sizeof(ItemSystem));
    system->items = NULL;
    system->count = 0;
    return system;
}

void remove_newline(char* str) {
    // ���ڿ��� �ٹٲ� ���� ����
    if (str[0] == '\n') {
        memmove(str, str + 1, strlen(str));
    }
}

// ���Ͽ��� ������ �ε� �Լ�
void load_items_from_file(ItemSystem* system, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return;
    }

    char name[100];
    int index, type, value;

    while (fscanf(file, "%d,%[^,],%d,%d", &index, name, &type, &value) == 4) {
        // �ٹٲ� ���� ����
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

// ������ �ý��� ���� �Լ�
void free_item_system(ItemSystem* system) {
    free(system->items); // ������ ��� ����
    free(system);        // �ý��� ����
}