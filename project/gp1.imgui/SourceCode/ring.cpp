#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() �� srand() �̂��߂ɃC���N���[�h
#include <algorithm>  // std::sort �̂��߂ɃC���N���[�h

using namespace input;

// �f�t�H���g�̃����O�̐��i�����^�C���ŕύX�\�j
int numRings = 20;  // �����O�̐��̗�
const int START_DELAY = 60 * 5; // 5�b�̒x���i60FPS��5�b�j

RING* goldRings; // �S�[���h�����O�p�̓��I�z��
RING* redRings; // ���b�h�����O�̃I�u�W�F�N�g
RING* rainbowRings;
Sprite* sprRing_gold; // �S�[���h�����O�̃X�v���C�g
Sprite* sprRing_red;  // ���b�h�����O�̃X�v���C�g
Sprite* sprRing_rainbow;

int ring_start_timer = 0;  // �����O�̊J�n�O�̃^�C�}�[

// Z���̍ő�l�ƍŏ��l
const float MAX_Z = 300.0f; // Z�̍ő�l
const float MIN_Z = 100.0f; // Z�̍ŏ��l

const int RING_GROUP_SIZE = rand() % 6;      // �O���[�v���Ƃ̃����O�̐�
const int GROUP_DISPLAY_DELAY = rand() % 180; // �O���[�v�Ԃ̒x���i�t���[���P�ʁA3�b��z��j

// �S�[���h�����O�A�ԃ����O�A�������O�̈ʒu��ʁX�ɐݒ�
std::vector<VECTOR2> ring_positions_gold = {
    {300, 300}, {300, 250}, {300, 300}  // �S�[���h�����O�̈ʒu
};

std::vector<VECTOR2> ring_positions_red = {
    {800, 250}, {800, 300}, {800, 350}  // �ԃ����O�̈ʒu
};

std::vector<VECTOR2> ring_positions_rainbow = {
    {1000, 350}, {1000, 400}, {1000, 450}  // �������O�̈ʒu
};

// �����O�̏������֐�
void ring_init() {
    srand(static_cast<unsigned int>(time(nullptr))); // �����̏�����

    // �X�v���C�g�̓ǂݍ���
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
    sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");

    // �S�[���h�����O�̃��������蓖��
    goldRings = new RING[numRings];
    redRings = new RING[numRings];
    rainbowRings = new RING[numRings];

    // �S�[���h�����O�̏�����
    for (int i = 0; i < numRings; ++i) {
        goldRings[i].scale = { 0.0f, 0.0f };
        goldRings[i].angle = ToRadian(0);
        goldRings[i].texPos = { 0, 0 };
        goldRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        goldRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // �ʒu�̐ݒ�
        if (i < ring_positions_gold.size()) {
            goldRings[i].position.x = ring_positions_gold[i].x;
            goldRings[i].position.y = ring_positions_gold[i].y;
        }
        else {
            goldRings[i].position.x = ring_positions_gold.back().x; // �ǉ������O�͍Ō�̈ʒu���g�p
            goldRings[i].position.y = ring_positions_gold.back().y;
        }

        goldRings[i].position.z = MIN_Z;
        goldRings[i].update_delay = i * 20; // �X�V�x���������O���Ƃɐݒ�
        goldRings[i].update_counter = 0;
        goldRings[i].is_active = true;
    }

    // �ԃ����O�̏�����
    for (int i = 0; i < numRings; ++i) {
        redRings[i].scale = { 0.0f, 0.0f };
        redRings[i].angle = ToRadian(0);
        redRings[i].texPos = { 0, 0 };
        redRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        redRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // �ʒu�̐ݒ�
        if (i < ring_positions_red.size()) {
            redRings[i].position.x = ring_positions_red[i].x;
            redRings[i].position.y = ring_positions_red[i].y;
        }
        else {
            redRings[i].position.x = ring_positions_red.back().x; // �ǉ������O�͍Ō�̈ʒu���g�p
            redRings[i].position.y = ring_positions_red.back().y;
        }

        redRings[i].position.z = MIN_Z;
        redRings[i].update_delay = i * 20; // �X�V�x���������O���Ƃɐݒ�
        redRings[i].update_counter = 0;
        redRings[i].is_active = true;
    }

    // �������O�̏�����
    for (int i = 0; i < numRings; ++i) {
        rainbowRings[i].scale = { 0.0f, 0.0f };
        rainbowRings[i].angle = ToRadian(0);
        rainbowRings[i].texPos = { 0, 0 };
        rainbowRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        rainbowRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // �ʒu�̐ݒ�
        if (i < ring_positions_rainbow.size()) {
            rainbowRings[i].position.x = ring_positions_rainbow[i].x;
            rainbowRings[i].position.y = ring_positions_rainbow[i].y;
        }
        else {
            rainbowRings[i].position.x = ring_positions_rainbow.back().x; // �ǉ������O�͍Ō�̈ʒu���g�p
            rainbowRings[i].position.y = ring_positions_rainbow.back().y;
        }

        rainbowRings[i].position.z = MIN_Z;
        rainbowRings[i].update_delay = i * 20; // �X�V�x���������O���Ƃɐݒ�
        rainbowRings[i].update_counter = 0;
        rainbowRings[i].is_active = true;
    }

    ring_start_timer = 0;
}



void ring_deinit() {
    delete[] goldRings; // �S�[���h�����O�̃��������
    delete[] redRings;
    delete[] rainbowRings;

    // �X�v���C�g���\�[�X�̃N���[���A�b�v
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
    safe_delete(sprRing_rainbow);
}

void ring_update() {
    // �J�n�x���̊Ԃ̓J�E���g��i�߂邾��
    if (ring_start_timer < START_DELAY) {
        ring_start_timer++;
        return;
    }

    // ���݂̃A�N�e�B�u�O���[�v�����v�Z
    int active_groups = (ring_start_timer - START_DELAY) / GROUP_DISPLAY_DELAY + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE, numRings) - 1;

    // �S�[���h�����O�̍X�V
    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active) {
            // �X�V�x�����o�߂��������O������
            if (goldRings[i].update_counter >= goldRings[i].update_delay) {
                goldRings[i].position.z -= 1.0f; // Z���̈ړ�

                // Z�ʒu�������𒴂����ꍇ�̃��Z�b�g����
                if (goldRings[i].position.z < MIN_Z) {
                    goldRings[i].position.z = MAX_Z;
                    goldRings[i].update_counter = 0;
                }

                // �X�P�[����Z�ʒu�Ɋ�Â��Ē���
                float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z);
                scale_factor *= 3.0f;

                goldRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                // �X�P�[�������ȏ�ɂȂ������A�N�e�B�u�ɂ���
                if (scale_factor >= 1.0f) {
                    goldRings[i].is_active = false;
                }
            }
            else {
                goldRings[i].update_counter++;
            }
        }
    }

    // ���b�h�����O�̍X�V 
   
        
        for (int i = 0; i <= max_active_ring_index; ++i) {
            if (redRings[i].is_active) {
                // �X�V�x�����o�߂��������O������
                if (redRings[i].update_counter >= redRings[i].update_delay) {
                    redRings[i].position.z -= 1.0f; // Z���̈ړ�

                    // Z�ʒu�������𒴂����ꍇ�̃��Z�b�g����
                    if (redRings[i].position.z < MIN_Z) {
                        redRings[i].position.z = MAX_Z;
                        redRings[i].update_counter = 0;
                    }

                    // �X�P�[����Z�ʒu�Ɋ�Â��Ē���
                    float scale_factor = (MAX_Z - redRings[i].position.z) / (MAX_Z - MIN_Z);
                    scale_factor *= 3.0f;

                    redRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                    // �X�P�[�������ȏ�ɂȂ������A�N�e�B�u�ɂ���
                    if (scale_factor >= 1.0f) {
                        redRings[i].is_active = false;
                    }
                }
                else {
                    redRings[i].update_counter++;
                }
            }
        }
    
    // �������O�̍X�V 
    
        
        for (int i = 0; i <= max_active_ring_index; ++i) {
            if (rainbowRings[i].is_active) {
                // �X�V�x�����o�߂��������O������
                if (rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
                    rainbowRings[i].position.z -= 1.0f; // Z���̈ړ�

                    // Z�ʒu�������𒴂����ꍇ�̃��Z�b�g����
                    if (rainbowRings[i].position.z < MIN_Z) {
                        rainbowRings[i].position.z = MAX_Z;
                        rainbowRings[i].update_counter = 0;
                    }

                    // �X�P�[����Z�ʒu�Ɋ�Â��Ē���
                    float scale_factor = (MAX_Z - rainbowRings[i].position.z) / (MAX_Z - MIN_Z);
                    scale_factor *= 3.0f;

                    rainbowRings[i].scale = { 0.1f + scale_factor * 0.5f, 0.1f + scale_factor * 0.5f };

                    // �X�P�[�������ȏ�ɂȂ������A�N�e�B�u�ɂ���
                    if (scale_factor >= 1.0f) {
                        rainbowRings[i].is_active = false;
                    }
                }
                else {
                    rainbowRings[i].update_counter++;
                }
            }
        }
    
    // �^�C�}�[�̑���
    ring_start_timer++;
}


// �����O�̕`�揈��
void ring_render() {
    if (ring_start_timer < START_DELAY) {
        return;
    }

    // Z���ŕ��בւ�
    std::sort(goldRings, goldRings + numRings, compareRingsByZ);
    std::sort(redRings, redRings + numRings, compareRingsByZ);
    std::sort(rainbowRings, rainbowRings + numRings, compareRingsByZ);

    // ���݃A�N�e�B�u�ȃO���[�v�����v�Z
    int active_groups = (ring_start_timer - START_DELAY) / GROUP_DISPLAY_DELAY + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE, numRings) - 1;

    // �A�N�e�B�u�ȃ����O��`��
    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active && goldRings[i].update_counter >= goldRings[i].update_delay) {
            // �S�[���h�����O��`��
            sprite_render(sprRing_gold, goldRings[i].position.x, goldRings[i].position.y, goldRings[i].scale.x, goldRings[i].scale.y, goldRings[i].texPos.x,goldRings[i].texPos.y,goldRings[i].texSize.x,goldRings[i].texSize.y,goldRings[i].pivot.x, goldRings[i].pivot.y);
        }
        if (redRings[i].is_active && redRings[i].update_counter >= redRings[i].update_delay) {
            // ���b�h�����O��`��i�m���ŕ\���j
            sprite_render(sprRing_red, redRings[i].position.x, redRings[i].position.y, redRings[i].scale.x, redRings[i].scale.y, redRings[i].texPos.x, redRings[i].texPos.y, redRings[i].texSize.x, redRings[i].texSize.y, redRings[i].pivot.x, redRings[i].pivot.y);
        }
        if (rainbowRings[i].is_active && rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
            // �������O��`��i�m���ŕ\���j
            sprite_render(sprRing_rainbow, rainbowRings[i].position.x, rainbowRings[i].position.y, rainbowRings[i].scale.x, rainbowRings[i].scale.y, rainbowRings[i].texPos.x, rainbowRings[i].texPos.y, rainbowRings[i].texSize.x, rainbowRings[i].texSize.y, rainbowRings[i].pivot.x, rainbowRings[i].pivot.y);
        }
    }
}



// Z���ł̔�r�֐�
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z�ʒu���傫���قǎ�O�ɕ`�悳���
}
