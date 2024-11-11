#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() �� srand() �̂��߂ɃC���N���[�h
#include <algorithm>  // std::sort �̂��߂ɃC���N���[�h

using namespace input;

// �f�t�H���g�̃����O�̐��i�����^�C���ŕύX�\�j
int numRings = 50;  // �����O�̐��̗�
const int START_DELAY = 60 * 7; // 7�b�̒x���i60FPS��7�b�j

RING* goldRings; // �S�[���h�����O�p�̓��I�z��
RING redRing;
Sprite* sprRing_gold;
Sprite* sprRing_red;

int ring_start_timer = 0;  // �����O�̊J�n�O�̃^�C�}�[


// Z���̍ő�l�ƍŏ��l
const float MAX_Z = 0.0f; // Z�̍ő�l
const float MIN_Z = -50.0f; // Z�̍ŏ��l

// �蓮�Ŏw�肷��ʒu�̔z��
std::vector<VECTOR2> ring_positions = {
    {100, 100}, {200, 150}, {300, 200}, {400, 250}, {500, 300},
    {600, 350}, {700, 400}, {800, 450}, {900, 500}, {1000, 550}
    // �K�v�ɉ����č��W��ǉ�
};

// �����O�̏�����
void ring_init() {
    // �����_���̂��߂ɗ����̃V�[�h��ݒ�
    srand(static_cast<unsigned int>(time(nullptr)));

    // �����O�̃e�N�X�`�������[�h
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");

    // �S�[���h�����O�p�̃������𓮓I�Ɋm��
    goldRings = new RING[numRings];

    // �����O�̏�����
    for (int i = 0; i < numRings; ++i) {
        goldRings[i].scale = { 0.1f, 0.1f };  // �X�P�[��
        goldRings[i].angle = ToRadian(0);  // �p�x
        goldRings[i].texPos = { 0, 0 };  // �e�N�X�`���̈ʒu
        goldRings[i].texSize = { RING_TEX_W, RING_TEX_H };  // �e�N�X�`���̃T�C�Y
        goldRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };  // �s�{�b�g�ʒu

        // �蓮�Ŏw�肵���ʒu���g���ă����O�̈ʒu��ݒ�
        if (i < ring_positions.size()) {
            goldRings[i].position.x = ring_positions[i].x;  // x���W���w��
            goldRings[i].position.y = ring_positions[i].y;  // y���W���w��
        }
        else {
            // �����ʒu������Ȃ��ꍇ�A�����_���Ŕz�u�i�K�v�Ȑ������蓮�Ŏw�肵�A����Ȃ��ꍇ�̂݁j
            goldRings[i].position.x = rand() % SCREEN_W;
            goldRings[i].position.y = rand() % SCREEN_H;
        }

        goldRings[i].position.z = MIN_Z; // ����Z�ʒu
        goldRings[i].update_delay = i * 60; // �X�V�������x�点��
        goldRings[i].update_counter = 0;  // �X�V�J�E���^
    }

    // �Ԃ������O�̏����ʒu��ݒ�
    redRing.position = { SCREEN_W * 0.5f, SCREEN_H * 0.7f };
    redRing.scale = { 0.5f, 0.5f };
    redRing.angle = ToRadian(0);
    redRing.texPos = { 0, 0 };
    redRing.texSize = { RING_TEX_W, RING_TEX_H };
    redRing.pivot = { RING_PIVOT_X, RING_PIVOT_Y };

    ring_start_timer = 0;  // �^�C�}�[�����Z�b�g
}


// �����O�̌�n��
void ring_deinit() {
    // �S�[���h�����O�̃����������
    delete[] goldRings;

    // �X�v���C�g�̃��\�[�X���N���[���A�b�v
    safe_delete(sprRing_gold);
    safe_delete(sprRing_red);
}

// �����O�̍X�V
void ring_update() {
    // �^�C�}�[���J�n�x�����Ԃ��Z���ꍇ�A�J�E���g�A�b�v���Ė߂�
    if (ring_start_timer < START_DELAY) {
        ring_start_timer++;
        return;
    }

    // �e�����O���X�V
    for (int i = 0; i < numRings; ++i) {
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            // Z�ʒu���X�V�i�����O���J�����Ɍ������Ĉړ��A��O�������l���������j
            goldRings[i].position.z -= 1.0f;

            // �ŏ�Z�ɒB�����烊���O�̈ʒu�����Z�b�g
            if (goldRings[i].position.z < MIN_Z) {
                goldRings[i].position.z = MAX_Z;
                goldRings[i].update_counter = 0; // �J�E���^�����Z�b�g
            }

            // Z�ʒu�Ɋ�Â��ăX�P�[���𒲐��i�߂��̃����O���傫��������悤�Ɂj
            float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z);
            scale_factor *= 3.0f;  // �X�P�[�����O�������i���̒l�𑝉�������Ƒ����Ȃ�j

            goldRings[i].scale = { 0.1f + scale_factor * 0.2f, 0.1f + scale_factor * 0.2f };
        }
        else {
            goldRings[i].update_counter++;  // �X�V�J�E���^���C���N�������g
        }
    }
}

// �����O�̕`��
void ring_render() {
    // �^�C�}�[���J�n�x�����Ԃ��Z���ꍇ�A�`����X�L�b�v
    if (ring_start_timer < START_DELAY) {
        return;
    }

    // �����O��Z�ʒu�Ɋ�Â��ă\�[�g�i���������O�͌�ɕ`�悳���j
    std::sort(goldRings, goldRings + numRings, compareRingsByZ);

    // �\�[�g��A�����O��`��
    for (int i = 0; i < numRings; ++i) {
        if (goldRings[i].update_counter >= goldRings[i].update_delay) {
            sprite_render(
                sprRing_gold,
                goldRings[i].position.x, goldRings[i].position.y, // �����O�̈ʒu
                goldRings[i].scale.x, goldRings[i].scale.y,
                goldRings[i].texPos.x, goldRings[i].texPos.y,
                goldRings[i].texSize.x, goldRings[i].texSize.y,
                goldRings[i].pivot.x, goldRings[i].pivot.y
            );
           
        }
    }

    // ���ׂẴS�[���h�����O��`�悵����ɐԂ������O��`��
    sprite_render(
        sprRing_red,
        redRing.position.x, redRing.position.y,
        redRing.scale.x, redRing.scale.y,
        redRing.texPos.x, redRing.texPos.y,
        redRing.texSize.x, redRing.texSize.y,
        redRing.pivot.x, redRing.pivot.y
    );
   
}

// Z���Ŕ�r���邽�߂̊֐�
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z�ʒu���傫���قǎ�O�ɕ`�悳���
}
