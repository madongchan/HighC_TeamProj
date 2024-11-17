#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct {
	char* content; // 메시지 내용
	bool is_visible; // 메시지가 화면에 출력되는지 여부
} Message;

typedef struct {
	Message* messages; // 메시지 목록
	int capacity; // 메시지 목록의 용량
	int count; // 현재 메시지 개수
} MessageSystem;

MessageSystem* create_message_system(int initial_capacity) {
    MessageSystem* system = (MessageSystem*)malloc(sizeof(MessageSystem));
    system->messages = (Message*)malloc(sizeof(Message) * initial_capacity);
    system->capacity = initial_capacity;
    system->count = 0;

    // 메시지 배열 초기화
    for (int i = 0; i < initial_capacity; i++) {
        system->messages[i].content = NULL;
        system->messages[i].is_visible = false;
    }
    return system;
}


void add_message(MessageSystem* system, const char* text) {
    if (system->count >= system->capacity) {
        return;
    }
    system->messages[system->count].content = (char*)malloc(strlen(text) + 1);
	strncpy(system->messages[system->count].content, text, strlen(text) + 1); // 크기는 널 문자까지 해서 텍스트 크기 + 1
    system->messages[system->count].is_visible = true;
    system->count++;
}

void remove_messages(MessageSystem* system) {
	for (int i = 0; i < system->count; i++) {
		free(system->messages[i].content);
        system->messages[i].content = NULL;
		system->messages[i].is_visible = false;
	}
	system->count = 0; 
}

void free_message_system(MessageSystem* system) {
    for (int i = 0; i < system->count; i++) {
        free(system->messages[i].content);
    }
    free(system->messages);
    free(system);
}