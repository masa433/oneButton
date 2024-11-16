#include "bird.h"
#include "common.h"
#include <cstdlib>
#include <ctime>
#include "sign.h"
using namespace std;

BIRD bird[BIRD_MAX];
Sprite* sprBird;

// ���[����y���W��ݒ�i���E�ňقȂ�ʒu��5�̃��[������ʂɍ��킹�Ĕz�u�j
const float lane_positions_left[NUM_LANES] = { 100.0f, 275.0f, 450.0f, 625.0f, 800.0f };  // �����̃��[��
const float lane_positions_right[NUM_LANES] = { 150.0f, 325.0f, 500.0f, 675.0f, 850.0f };  // �E���̃��[��


void bird_init() {
    srand((unsigned)time(NULL));
    sprBird = sprite_load(L"./Data/Images/�e��.png");

    for (int i = 0; i < BIRD_MAX; i++) {
        bird[i].bird_state = 1; // ��Ԃ�1�ɐݒ肵�ĕ`���L���ɂ���
        bird[i].speed = { 0.0f, 0.0f };
        bird[i].position = { 0.0f, 0.0f };
        bird[i].scale = { 0.3f, 0.3f };
        bird[i].texPos = { 0.0f, 0.0f };
        bird[i].texSize = { BIRD_TEX_W, BIRD_TEX_H };
        bird[i].pivot = { 0.5f, 0.5f };
        bird[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };

        // ���̏����ʒu��ݒ�i�����_���Ƀ��[����I���j
        int lane = rand() % NUM_LANES;  // ���[���ԍ��������_���őI��
        bird[i].position.y = lane_positions_left[lane];  // �����̃��[���̈ʒu�ɐݒ�

        // ���̏����ʒu��ݒ�i���E�����_���j
        if (rand() % 2 == 0) {
            // ������E�ɔ��
            bird[i].position.x = -BIRD_TEX_W;  // ��ʊO�i�����j
            bird[i].speed.x = (rand() % (int)BIRD_SPEED_X_MAX) + .0f;  // �����_�����x
            bird[i].scale.x = -0.3f;  // �E����
        }
        else {
            // �E���獶�ɔ��
            bird[i].position.x = 1920 + BIRD_TEX_W;  // ��ʊO�i�E���j
            bird[i].speed.x = -(rand() % (int)BIRD_SPEED_X_MAX) - 5.0f;  // �����_�����x
            bird[i].scale.x = 0.3f;  // �������i���]�j
            bird[i].position.y = lane_positions_right[lane];  // �E���̃��[���̈ʒu�ɐݒ�
        }
    }
}

void bird_deinit() {
    safe_delete(sprBird);
}

void bird_update() {
    for (int i = 0; i < BIRD_MAX; i++) {
        if (bird[i].bird_state == 0) continue;  // ��Ԃ�0�̏ꍇ�͉������Ȃ�

        // ���̈ړ������i�������̂݁j
        bird[i].position.x += bird[i].speed.x;

        // ��ʊO�ɏo���ꍇ�A�摜�������i��Ԃ�0�ɐݒ�j
        if (bird[i].position.x < -BIRD_TEX_W || bird[i].position.x > 1920 + BIRD_TEX_W) {
            bird[i].bird_state = 0;  // ����������Ԃɐݒ�
        }
    }
}

void bird_render() {
    for (int i = 0; i < BIRD_MAX; i++) {
        if (bird[i].bird_state == 0) continue;  // ��Ԃ�0�̏ꍇ�͕`�悵�Ȃ�

        sprite_render(
            sprBird,
            bird[i].position.x, bird[i].position.y,
            bird[i].scale.x, bird[i].scale.y,
            bird[i].texPos.x, bird[i].texPos.y,
            bird[i].texSize.x, bird[i].texSize.y,
            bird[i].pivot.x, bird[i].pivot.y,
            0.0f, // ��]�Ȃ�
            bird[i].color.x, bird[i].color.y, bird[i].color.z, bird[i].color.w
        );
    }
}
