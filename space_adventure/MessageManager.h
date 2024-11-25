#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* content; // �޽��� ����
    bool is_visible; // �޽����� ȭ�鿡 ��µǴ��� ����
} Message;

typedef struct {
    Message* messages; // �޽��� ���
    int capacity; // �޽��� ����� �ִ� �뷮
    int count; // ���� �޽��� ����
} MessageSystem;

MessageSystem* messageSystem; // �޽��� �ý���

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

    // �޽��� �迭 �ʱ�ȭ
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
    strncpy(messageSystem->messages[messageSystem->count].content, text, strlen(text) + 1); // ���ڿ��� ���̸�ŭ ���� + 1
    messageSystem->messages[messageSystem->count].is_visible = true;
    messageSystem->count++;
}

// ������ �޽����� �����ϴ� �Լ�
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