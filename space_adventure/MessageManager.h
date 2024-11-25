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
    int capacity; // 메시지 목록의 최대 용량
    int count; // 현재 메시지 개수
} MessageSystem;

MessageSystem* messageSystem; // 메시지 시스템

void free_message_system() {
    for (int i = 0; i < messageSystem->count; i++) {
        free(messageSystem->messages[i].content);
    }
    free(messageSystem->messages);
    free(messageSystem);
}

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

void add_message(const char* text) {
    if (messageSystem->count >= messageSystem->capacity) {
        return;
    }
    messageSystem->messages[messageSystem->count].content = (char*)malloc(strlen(text) + 1);
    strncpy(messageSystem->messages[messageSystem->count].content, text, strlen(text) + 1); // 문자열의 길이만큼 복사 + 1
    messageSystem->messages[messageSystem->count].is_visible = true;
    messageSystem->count++;
}

// 마지막 메시지를 제거하는 함수
void remove_last_message() {
    if (messageSystem->count > 0) {
        free(messageSystem->messages[messageSystem->count - 1].content);
        messageSystem->messages[messageSystem->count - 1].content = NULL;
        messageSystem->messages[messageSystem->count - 1].is_visible = false;
        messageSystem->count--;
    }
}

void remove_messages() {
    for (int i = 0; i < messageSystem->count; i++) {
        free(messageSystem->messages[i].content);
        messageSystem->messages[i].content = NULL;
        messageSystem->messages[i].is_visible = false;
    }
    messageSystem->count = 0;
}