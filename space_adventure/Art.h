#pragma once
#include <stdio.h>

typedef enum event_type {
	SIGNAL_DEVICE = 0,
	STORM = 1,
	ITEM_FOUND = 2,
	PREDATOR = 3,
	ALIEN = 4,
	MATTINGA = 5,
	None = 6
} EventType;

const char* get_signal_device_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "     ____"); break;
	case 2: snprintf(message, size, "    |SIGN|"); break;
	case 3: snprintf(message, size, "    |____|"); break;
	case 4: snprintf(message, size, "��ȣ��ġ�� �߰��Ͽ����ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}

const char* get_storm_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "     .-."); break;
	case 2: snprintf(message, size, "    (   )."); break;
	case 3: snprintf(message, size, "   (___(__)"); break;
	case 4: snprintf(message, size, "   ' ' ' ' ' ' "); break;
	case 5: snprintf(message, size, "��ǳ�� �߻��߽��ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}

const char* get_item_found_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "     _______"); break;
	case 2: snprintf(message, size, "    /       \\"); break;
	case 3: snprintf(message, size, "   |  ITEM   |"); break;
	case 4: snprintf(message, size, "    \\_______/"); break;
	case 5: snprintf(message, size, "�������� �߰��Ͽ����ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}

const char* get_predator_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "     _____"); break;
	case 2: snprintf(message, size, "   .'      ' ."); break;
	case 3: snprintf(message, size, "  / oo   oo \\"); break;
	case 4: snprintf(message, size, " \\    \\_//"); break;
	case 5: snprintf(message, size, "  \\ \\___//"); break;
	case 6: snprintf(message, size, "   '-.___.-'"); break;
	case 7: snprintf(message, size, "�������Ͱ� �����߽��ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}

const char* get_alien_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "       .-."); break;
	case 2: snprintf(message, size, "      (o o)"); break;
	case 3: snprintf(message, size, "       | |"); break;
	case 4: snprintf(message, size, "      _|_|_"); break;
	case 5: snprintf(message, size, "���ϸ����� �����߽��ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}

const char* get_mattinga_message(int line, char* message) {
	int size = 100;
	switch (line) {
	case 1: snprintf(message, size, "    ,     ,"); break;
	case 2: snprintf(message, size, "   (\\____/)"); break;
	case 3: snprintf(message, size, "    (_oo_)"); break;
	case 4: snprintf(message, size, "      (O)"); break;
	case 5: snprintf(message, size, "    __||__ \\)"); break;
	case 6: snprintf(message, size, "���ð� �ܰ����� �����߽��ϴ�!"); break;
	default: message[0] = '\0'; break;
	}
	return message;
}
