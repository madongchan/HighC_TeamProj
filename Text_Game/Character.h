#pragma once
typedef struct {
    int health;
    int attack;
    int defense;

    // �ൿ�� ���� �Լ� ������ (�������� ���� ����)
    void (*attack_func)(void* self, void* target);
} Character;

// ����� �ൿ �Լ� ����
void character_attack(void* self, void* target);

