#include "ring.h"
#include "common.h"
#include <cstdlib>  // rand() �� srand() �̂��߂ɃC���N���[�h
#include <algorithm>  // std::sort �̂��߂ɃC���N���[�h
#include <ctime>  // time() �̂��߂ɃC���N���[�h

using namespace input;

// �f�t�H���g�̃����O�̐��i�����^�C���ŕύX�\�j
int numRings_gold = 20;  // �����O�̐��̗�
int numRings_red = 10;  // �����O�̐��̗�
int numRings_rainbow = 3;  // �����O�̐��̗�
const int START_DELAY_GOLD = 60 * 5; // 5�b�̒x���i60FPS��5�b�j
const int START_DELAY_RED = 60 * 10; 
const int START_DELAY_RAINBOW = 60 * 15;

RING* goldRings; // �S�[���h�����O�p�̓��I�z��
RING* redRings; 
RING* rainbowRings;
Sprite* sprRing_gold; // �S�[���h�����O�̃X�v���C�g
Sprite* sprRing_red;  // ���b�h�����O�̃X�v���C�g
Sprite* sprRing_rainbow;

int ring_start_timer_gold = 0;  // �����O�̊J�n�O�̃^�C�}�[
int ring_start_timer_red = 0;  
int ring_start_timer_rainbow = 0;  

// Z���̍ő�l�ƍŏ��l
const float MAX_Z = 300.0f; // Z�̍ő�l
const float MIN_Z = 100.0f; // Z�̍ŏ��l

// �O���[�v���Ƃ̃����O�̐��ƒx��
int RING_GROUP_SIZE_GOLD;
int RING_GROUP_SIZE_RED;
int RING_GROUP_SIZE_RAINBOW;
int GROUP_DISPLAY_DELAY_GOLD;
int GROUP_DISPLAY_DELAY_RED;
int GROUP_DISPLAY_DELAY_RAINBOW;

#include <vector>
#include <cstdlib>  // rand() �� srand() �̂��߂ɃC���N���[�h
#include <ctime>    // time() �̂��߂ɃC���N���[�h

// VECTOR2 �\���̂̒�`
struct POSITION {
    float x, y;
};

// �����͈̔͂��w�肷��֐��imin ���� max �̊ԁj
int getRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

// �O���[�v1�ƃO���[�v2�̃����_���Ȉʒu�𐶐�����֐�
void generateRingPositions(std::vector<VECTOR2>& ring_positions_gold) {
    srand(static_cast<unsigned int>(time(nullptr))); // �����̏�����

    // 1�O���[�v�ڂ̃����_���Ȉʒu�ݒ�i�ŏ���3�j
    for (int i = 0; i < 3; ++i) {
        float x = getRandomInRange(450, 650);  // x���W��450����650�͈̔�
        float y = getRandomInRange(200, 400);  // y���W��200����400�͈̔�
        ring_positions_gold.push_back({ x, y });
    }

    // 2�O���[�v�ڂ̃����_���Ȉʒu�ݒ�i����3�j
    for (int i = 0; i < 3; ++i) {
        float x = getRandomInRange(650, 850);  // x���W��650����850�͈̔�
        float y = getRandomInRange(400, 600);  // y���W��400����600�͈̔�
        ring_positions_gold.push_back({ x, y });
    }

    // �Ō�̃S�[���̈ʒu��ǉ�
    ring_positions_gold.push_back({ 700, 200 });  // �S�[���̈ʒu
}




std::vector<VECTOR2> ring_positions_red = {
    {960, 500}, {1000, 550}, {1040, 600},  // Red rings centered, creating a descending stair pattern
    {1080, 550}, {1120, 500}               // Additional rings to extend the stair pattern
};

std::vector<VECTOR2> ring_positions_rainbow = {
    {1500, 300}, {1550, 350}, {1600, 400}, // Rainbow rings on the right, positioned diagonally
    {1650, 350}, {1700, 300}               // Mirror the gold rings' curve for symmetry
};


// �����O�̏������֐�
void ring_init() {
    srand(static_cast<unsigned int>(time(nullptr))); // �����̏�����

    std::vector<VECTOR2> ring_positions_gold;

    // �����O�̈ʒu�𐶐�
    generateRingPositions(ring_positions_gold);

    // ���I�ȃ����O�̃O���[�v�T�C�Y�ƒx���̐ݒ�
    RING_GROUP_SIZE_GOLD = 3;         // 4�`6 �͈̔�
    RING_GROUP_SIZE_RED = 2;         // 1�`6 �͈̔�
    RING_GROUP_SIZE_RAINBOW = 1;         // 3�`6 �͈̔�
    GROUP_DISPLAY_DELAY_GOLD = 30 + rand() % 60;       // 60�`119 �͈̔�
    GROUP_DISPLAY_DELAY_RED = 60 + rand() % 60;       // 0�`119 �͈̔�
    GROUP_DISPLAY_DELAY_RAINBOW = rand() % 120;       // 0�`119 �͈̔�

    // �X�v���C�g�̓ǂݍ���
    sprRing_gold = sprite_load(L"./Data/Images/ring_gold.png");
    sprRing_red = sprite_load(L"./Data/Images/ring_red.png");
    sprRing_rainbow = sprite_load(L"./Data/Images/ring_rainbow.png");

    // �S�[���h�����O�̃��������蓖��
    goldRings = new RING[numRings_gold];
    redRings = new RING[numRings_red];
    rainbowRings = new RING[numRings_rainbow];

    // �S�[���h�����O�̏�����
    for (int i = 0; i < numRings_gold; ++i) {
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

    ring_start_timer_gold = 0;


    // �ԃ����O�̏�����
    for (int i = 0; i < numRings_red; ++i) {
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

    ring_start_timer_red = 0;

    // �������O�̏�����
    for (int i = 0; i < numRings_rainbow; ++i) {
        rainbowRings[i].scale = { 0.0f, 0.0f };
        rainbowRings[i].angle = ToRadian(0);
        rainbowRings[i].texPos = { 0, 0 };
        rainbowRings[i].texSize = { RING_TEX_W, RING_TEX_H };
        rainbowRings[i].pivot = { RING_PIVOT_X, RING_PIVOT_Y };

        // �ʒu�̐ݒ�
        if (i < ring_positions_gold.size()) {
            rainbowRings[i].position.x = ring_positions_rainbow[i].x;
            rainbowRings[i].position.y = ring_positions_rainbow[i].y;
        }
        else {
            rainbowRings[i].position.x = ring_positions_rainbow.back().x; // �ǉ������O�͍Ō�̈ʒu���g�p
            rainbowRings[i].position.y = ring_positions_rainbow.back().y;
        }

        rainbowRings[i].position.z = MIN_Z;
        rainbowRings[i].update_delay = i * 100; // �X�V�x���������O���Ƃɐݒ�
        rainbowRings[i].update_counter = 0;
        rainbowRings[i].is_active = true;
    }

    ring_start_timer_rainbow = 0;
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

void update_gold_rings() {
    int active_groups = (ring_start_timer_gold - START_DELAY_GOLD) / GROUP_DISPLAY_DELAY_GOLD + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_GOLD, numRings_gold) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active) {
            if (goldRings[i].update_counter >= goldRings[i].update_delay) {
                goldRings[i].position.z -= 1.0f;

                if (goldRings[i].position.z < MIN_Z) {
                    goldRings[i].position.z = MAX_Z;
                    goldRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - goldRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                goldRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    goldRings[i].is_active = false;
                }
            }
            else {
                goldRings[i].update_counter += 3.0f;
            }
        }
    }
}

void update_red_rings() {
    int active_groups = (ring_start_timer_red - START_DELAY_RED) / GROUP_DISPLAY_DELAY_RED + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RED, numRings_red) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (redRings[i].is_active) {
            if (redRings[i].update_counter >= redRings[i].update_delay) {
                redRings[i].position.z -= 1.0f;

                if (redRings[i].position.z < MIN_Z) {
                    redRings[i].position.z = MAX_Z;
                    redRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - redRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                redRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    redRings[i].is_active = false;
                }
            }
            else {
                redRings[i].update_counter += 2.0f;
            }
        }
    }
}

void update_rainbow_rings() {
    int active_groups = (ring_start_timer_rainbow - START_DELAY_RAINBOW) / GROUP_DISPLAY_DELAY_RAINBOW + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RAINBOW, numRings_rainbow) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (rainbowRings[i].is_active) {
            if (rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
                rainbowRings[i].position.z -= 1.0f;

                if (rainbowRings[i].position.z < MIN_Z) {
                    rainbowRings[i].position.z = MAX_Z;
                    rainbowRings[i].update_counter = 0;
                }

                float scale_factor = (MAX_Z - rainbowRings[i].position.z) / (MAX_Z - MIN_Z) * 4.0f;
                rainbowRings[i].scale = { scale_factor, scale_factor };

                if (scale_factor >= 1.0f) {
                    rainbowRings[i].is_active = false;
                }
            }
            else {
                rainbowRings[i].update_counter++;
            }
        }
    }
}

void ring_update() {
    // �^�C�}�[�̑���
    ring_start_timer_gold++;
    ring_start_timer_red++;
    ring_start_timer_rainbow++;

    // �J�n�x�����I��������A�F���ƂɃ����O���X�V
    if (ring_start_timer_gold >= START_DELAY_GOLD) {
        update_gold_rings();
    }

    if (ring_start_timer_red >= START_DELAY_RED) {
        update_red_rings();
    }

    if (ring_start_timer_rainbow >= START_DELAY_RAINBOW) {
        update_rainbow_rings();
    }
}


        


// �S�[���h�����O�̕`�揈��
void render_gold_rings() {
    std::sort(goldRings, goldRings + numRings_gold, compareRingsByZ);

    int active_groups = (ring_start_timer_gold - START_DELAY_GOLD) / GROUP_DISPLAY_DELAY_GOLD + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_GOLD, numRings_gold) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (goldRings[i].is_active && goldRings[i].update_counter >= goldRings[i].update_delay) {
            sprite_render(sprRing_gold, goldRings[i].position.x, goldRings[i].position.y,
                goldRings[i].scale.x, goldRings[i].scale.y,
                goldRings[i].texPos.x, goldRings[i].texPos.y,
                goldRings[i].texSize.x, goldRings[i].texSize.y,
                goldRings[i].pivot.x, goldRings[i].pivot.y);
        }
    }
}

// ���b�h�����O�̕`�揈��
void render_red_rings() {
    std::sort(redRings, redRings + numRings_red, compareRingsByZ);

    int active_groups = (ring_start_timer_red - START_DELAY_RED) / GROUP_DISPLAY_DELAY_RED + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RED, numRings_red) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (redRings[i].is_active && redRings[i].update_counter >= redRings[i].update_delay) {
            sprite_render(sprRing_red, redRings[i].position.x, redRings[i].position.y,
                redRings[i].scale.x, redRings[i].scale.y,
                redRings[i].texPos.x, redRings[i].texPos.y,
                redRings[i].texSize.x, redRings[i].texSize.y,
                redRings[i].pivot.x, redRings[i].pivot.y);
        }
    }
}

// �������O�̕`�揈��
void render_rainbow_rings() {
    std::sort(rainbowRings, rainbowRings + numRings_rainbow, compareRingsByZ);

    int active_groups = (ring_start_timer_rainbow - START_DELAY_RAINBOW) / GROUP_DISPLAY_DELAY_RAINBOW + 1;
    int max_active_ring_index = std::min(active_groups * RING_GROUP_SIZE_RAINBOW, numRings_rainbow) - 1;

    for (int i = 0; i <= max_active_ring_index; ++i) {
        if (rainbowRings[i].is_active && rainbowRings[i].update_counter >= rainbowRings[i].update_delay) {
            sprite_render(sprRing_rainbow, rainbowRings[i].position.x, rainbowRings[i].position.y,
                rainbowRings[i].scale.x, rainbowRings[i].scale.y,
                rainbowRings[i].texPos.x, rainbowRings[i].texPos.y,
                rainbowRings[i].texSize.x, rainbowRings[i].texSize.y,
                rainbowRings[i].pivot.x, rainbowRings[i].pivot.y);
        }
    }
}

// �S�����O�̕`�揈��
void ring_render() {
    // �S�[���h�����O�̕`��^�C�~���O���`�F�b�N
    if (ring_start_timer_gold >= START_DELAY_GOLD) {
        render_gold_rings();
    }

    // ���b�h�����O�̕`��^�C�~���O���`�F�b�N
    if (ring_start_timer_red >= START_DELAY_RED) {
        render_red_rings();
    }

    // ���C���{�[�����O�̕`��^�C�~���O���`�F�b�N
    if (ring_start_timer_rainbow >= START_DELAY_RAINBOW) {
        render_rainbow_rings();
    }
}





// Z���ł̔�r�֐�
bool compareRingsByZ(const RING& a, const RING& b) {
    return a.position.z > b.position.z;  // Z�ʒu���傫���قǎ�O�ɕ`�悳���
}
