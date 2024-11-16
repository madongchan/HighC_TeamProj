#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct {
    char* content;
    bool is_visible;
} Message;

typedef struct {
    Message* messages;
    int capacity;
    int count;
} MessageSystem;

MessageSystem* create_message_system(int initial_capacity) {
    MessageSystem* system = (MessageSystem*)malloc(sizeof(MessageSystem));
    system->messages = (Message*)malloc(sizeof(Message) * initial_capacity);
    system->capacity = initial_capacity;
    system->count = 0;
    return system;
}

void add_message(MessageSystem* system, const char* text) {
    if (system->count >= system->capacity) {
        // 용량 증가
        system->capacity *= 2;
        system->messages = (Message*)realloc(system->messages, sizeof(Message) * system->capacity);
    }

    system->messages[system->count].content = strdup(text);
    system->messages[system->count].is_visible = true;
    system->count++;
}

void remove_oldest_message(MessageSystem* system) {
    if (system->count > 0) {
        free(system->messages[0].content);
        for (int i = 1; i < system->count; i++) {
            system->messages[i - 1] = system->messages[i];
        }
        system->count--;
    }
}

void free_message_system(MessageSystem* system) {
    for (int i = 0; i < system->count; i++) {
        free(system->messages[i].content);
    }
    free(system->messages);
    free(system);
}