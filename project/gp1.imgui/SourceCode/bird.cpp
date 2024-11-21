#include "bird.h"
#include "common.h"
#include <cstdlib>
#include <ctime>
#include "sign.h"
using namespace std;

BIRD bird[BIRD_MAX];
Sprite* sprBird;

// ���[����y���W��ݒ�
const float lane_positions_left[NUM_LANES] = { 100.0f, 275.0f, 450.0f, 625.0f, 800.0f };
const float lane_positions_right[NUM_LANES] = { 150.0f, 325.0f, 500.0f, 675.0f, 850.0f };

float spawn_timer ;             // �X�|�[���^�C�}�[
float next_spawn_time ;         // ���̃X�|�[���^�C�~���O
const float min_spawn_interval = 1.0f; // �X�|�[���Ԋu�̍ŏ��l
const float max_spawn_interval = 5.0f; // �X�|�[���Ԋu�̍ő�l

void bird_init() {
    srand((unsigned)time(NULL));
    sprBird = sprite_load(L"./Data/Images/�e��.png");
    spawn_timer = 0.0f;
    next_spawn_time = 0.0f;

    for (int i = 0; i < BIRD_MAX; i++) {
        bird[i].bird_state = 0; // �ŏ��͔�A�N�e�B�u
        bird[i].speed = { 0.0f, 0.0f };
        bird[i].position = { -100.0f, -100.0f };
        bird[i].scale = { 0.3f, 0.3f };
        bird[i].texPos = { 0.0f, 0.0f };
        bird[i].texSize = { BIRD_TEX_W, BIRD_TEX_H };
        bird[i].pivot = { 0.5f, 0.5f };
        bird[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
        bird[i].radius = 30;
        
        bird[i].isColliding = false;
    }

    // ����X�|�[���^�C�~���O��ݒ�
    next_spawn_time = min_spawn_interval + (rand() / (float)RAND_MAX) * (max_spawn_interval - min_spawn_interval);
}

void spawn_birds(bool from_right, int count) {
    for (int i = 0; i < count; i++) { // �w�肳�ꂽ����������
        for (int j = 0; j < BIRD_MAX; j++) {
            if (bird[j].bird_state == 0) { // ��A�N�e�B�u�Ȓ���T��
                bird[j].bird_state = 1; // �����A�N�e�B�u�ɂ���
                int lane = rand() % NUM_LANES;
                if (from_right) {
                    bird[j].position.x = 1920 + BIRD_TEX_W; // �E������o��
                    bird[j].speed.x = -(7 + rand() % (int)BIRD_SPEED_X_MAX) - 3.0f;
                    bird[j].scale.x = 0.3f; // ������
                    bird[j].position.y = lane_positions_right[lane];
                    bird[i].offset = { 60,60 };
                }
                else {
                    bird[j].position.x = -BIRD_TEX_W; // ��������o��
                    bird[j].speed.x = (7 + rand() % (int)BIRD_SPEED_X_MAX) + 3.0f;
                    bird[j].scale.x = -0.3f; // �E����
                    bird[j].position.y = lane_positions_left[lane];
                    bird[i].offset = { -60,60 };
                }
                break; // 1�H���������烋�[�v�𔲂���
            }
        }
    }
}

void bird_deinit() {
    safe_delete(sprBird);
}

void bird_update() {
    spawn_timer += 1.0f / 60.0f; // �X�|�[���^�C�}�[�����Z

    //// �����_���^�C�~���O�Œ����X�|�[��
    //if (spawn_timer >= next_spawn_time) {
    //    bool from_right = rand() % 2; // �����_���ō��E������
    //    int spawn_count = 1 + rand() % 5; // 1�`10�H�̃����_���Ȑ��𐶐�
    //    spawn_birds(from_right, spawn_count); // �����X�|�[��

    //    spawn_timer = 0.0f; // �^�C�}�[�����Z�b�g

    //    // ���̃X�|�[���^�C�~���O�������_���ɐݒ�
    //    next_spawn_time = min_spawn_interval + (rand() / (float)RAND_MAX) * (max_spawn_interval - min_spawn_interval);
    //}

    // ���̈ړ�����
    for (int i = 0; i < BIRD_MAX; i++) {
        if (bird[i].bird_state == 0) continue; // ��Ԃ�0�̏ꍇ�͉������Ȃ�

        bird[i].position.x += bird[i].speed.x;

        // ��ʊO�ɏo���ꍇ�A�摜�������i��Ԃ�0�ɐݒ�j
        if (bird[i].position.x < -BIRD_TEX_W || bird[i].position.x > 1920 + BIRD_TEX_W) {
            bird[i].bird_state = 0; // ����������Ԃɐݒ�
        }
    }
}

void bird_render() {
    for (int i = 0; i < BIRD_MAX; i++) {
        if (bird[i].bird_state == 0) continue; // ��Ԃ�0�̏ꍇ�͕`�悵�Ȃ�

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

        /*primitive::circle(bird[i].position.x + bird[i].offset.x,
            bird[i].position.y + bird[i].offset.y,
            bird[i].radius, 1, 1, ToRadian(0), 1, 0, 0, 0.2f);*/
    }
}
