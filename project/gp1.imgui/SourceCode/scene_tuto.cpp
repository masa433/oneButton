#include "common.h"
#include "scene_tuto.h"
#include "system.h"
#include "audio.h"
using namespace std;
using namespace input;

Sprite* sprBack;
Sprite* sprPlay;
Sprite* sprSetu[6]; // �`���[�g���A���y�[�W�̔z��
Sprite* sprNextpage;
Sprite* sprBackpage;
Sprite* sprGamestart;
Sprite* sprSkip;


extern TUTO Tuto;
TutoButton tutoNextbutton;
TutoButton tutoBackbutton;
TutoButton gameButton;
TutoButton skipButton;

void tuto_init()
{
    Tuto.tuto_timer = 0;
    Tuto.tuto_state = 0;
    Tuto.scroll_x = 0.0f;
    Tuto.tuto_fadein = 1.0f;
    Tuto.tuto_fadeout = 0.0f;

    Tuto.isTuto_Fadein = false;
    Tuto.isTuto_Fadeout = false;
    Tuto.click_delay_timer = 0.0f;
    Tuto.tuto_fadeTimer = 0.0f;
    Tuto.currentPage = 0; // ���݂̃y�[�W
    tutoNextbutton.clickTimer = 0.0f;
    tutoNextbutton.isClicked = false;

    tutoNextbutton.position = { SCREEN_W*0.95f,SCREEN_H *0.5f };
    tutoNextbutton.scale = { 1.0f, 1.0f };
    tutoNextbutton.texPos = { 0, 0 };
    tutoNextbutton.texSize = { BUTTON_TEX_W, BUTTON_TEX_H };
    tutoNextbutton.pivot = { BUTTON_PIVOT_X, BUTTON_PIVOT_Y };
    tutoNextbutton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    tutoBackbutton.position = { SCREEN_W * 0.05f,SCREEN_H * 0.5f };
    tutoBackbutton.scale = { 1.0f, 1.0f };
    tutoBackbutton.texPos = { 0, 0 };
    tutoBackbutton.texSize = { BUTTON_TEX_W, BUTTON_TEX_H };
    tutoBackbutton.pivot = { BUTTON_PIVOT_X, BUTTON_PIVOT_Y };
    tutoBackbutton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    gameButton.position = { SCREEN_W * 0.5f,SCREEN_H * 0.9f };
    gameButton.scale = { 1.0f, 1.0f };
    gameButton.texPos = { 0, 0 };
    gameButton.texSize = { GAME_BUTTON_TEX_W, GAME_BUTTON_TEX_H };
    gameButton.pivot = { GAME_BUTTON_PIVOT_X, GAME_BUTTON_PIVOT_Y };
    gameButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    skipButton.position = { SCREEN_W * 0.5f,SCREEN_H * 0.9f };
    skipButton.scale = { 1.0f, 1.0f };
    skipButton.texPos = { 0, 0 };
    skipButton.texSize = { GAME_BUTTON_TEX_W, GAME_BUTTON_TEX_H };
    skipButton.pivot = { GAME_BUTTON_PIVOT_X, GAME_BUTTON_PIVOT_Y };
    skipButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void tuto_update()
{
    switch (Tuto.tuto_state)
    {
    case 0:
        
        sprBack = sprite_load(L"./Data/Images/back(��).png");
        sprPlay = sprite_load(L"./Data/Images/�Q�[���摜(��).png");
        sprSetu[0] = sprite_load(L"./Data/Images/����1.png");
        sprSetu[1] = sprite_load(L"./Data/Images/����2.png");
        sprSetu[2] = sprite_load(L"./Data/Images/����3.png");
        sprSetu[3] = sprite_load(L"./Data/Images/����4.png");
        sprSetu[4] = sprite_load(L"./Data/Images/����5.png");
        sprSetu[5] = sprite_load(L"./Data/Images/����1.png");
        sprNextpage = sprite_load(L"./Data/images/nextpage.png");
        sprBackpage = sprite_load(L"./Data/Images/backpage.png");
        sprGamestart=sprite_load(L"./Data/Images/�Q�[���X�^�[�g.png");
        sprSkip = sprite_load(L"./Data/Images/�X�L�b�v.png");
        

        Tuto.tuto_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        music::play(BGM_TUTO, true);
        Tuto.tuto_state++;
        /*fallthrough*/
    case 2:
        fadeIn_act();
        break;

    case 3:
        //���N���b�N�Ńy�[�W�ړ�
        tuto_click_act();
        tuto_click_back_act();
        game_click_act();
        skip_click_act();
        
        break;

    case 4:
        fadeOut_act();
        
        break;
    }

    Tuto.scroll_x += 0.5f;
    if (Tuto.scroll_x >= 5464) {
        Tuto.scroll_x = 0.0f;
    }

    Tuto.tuto_timer++;
}

void tuto_render()
{
    GameLib::clear(1.0, 0.0, 0.0);

    // �w�i�X�N���[��
    sprite_render(sprBack, -Tuto.scroll_x, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);
    sprite_render(sprBack, -Tuto.scroll_x + 5464, SCREEN_H * 0.5f, 1, 1, 0, 0, 5464, 2048, 0, 2048 / 2);

    // �t�F�[�h�C���E�t�F�[�h�A�E�g�G�t�F�N�g
    float scale_up = 1.0f + (1.0f - Tuto.tuto_fadein);
    float scale_down = 1.0f - Tuto.tuto_fadeout * 0.5f;
    VECTOR2 scale = { 1.0f * scale_up * scale_down , 1.0f * scale_up * scale_down };

    // �`���[�g���A���y�[�W�`��
    sprite_render(sprSetu[Tuto.currentPage], SCREEN_W * 0.5f, SCREEN_H * 0.48f, scale.x, scale.y, 0, 0, 800, 550, 800 / 2.0f, 550 / 2.0f);

    // ���{�^���̕\������
    if (scale.x >= 2.0f && scale.y >= 2.0f)
    {
        if (Tuto.currentPage > 0) // �ŏ��̃y�[�W�łȂ��ꍇ�́u�߂�{�^���v��\��
        {
            sprite_render(sprBackpage, tutoBackbutton.position.x, tutoBackbutton.position.y,
                tutoBackbutton.scale.x, tutoBackbutton.scale.y,
                tutoBackbutton.texPos.x, tutoBackbutton.texPos.y,
                tutoBackbutton.texSize.x, tutoBackbutton.texSize.y,
                tutoBackbutton.pivot.x, tutoBackbutton.pivot.y);
        }

        if (Tuto.currentPage < 5) // �Ō�̃y�[�W�łȂ��ꍇ�́u���փ{�^���v��\��
        {
            sprite_render(sprNextpage, tutoNextbutton.position.x, tutoNextbutton.position.y,
                tutoNextbutton.scale.x, tutoNextbutton.scale.y,
                tutoNextbutton.texPos.x, tutoNextbutton.texPos.y,
                tutoNextbutton.texSize.x, tutoNextbutton.texSize.y,
                tutoNextbutton.pivot.x, tutoNextbutton.pivot.y);

            sprite_render(sprSkip, skipButton.position.x, skipButton.position.y,
                skipButton.scale.x, skipButton.scale.y,
                skipButton.texPos.x, skipButton.texPos.y,
                skipButton.texSize.x, skipButton.texSize.y,
                skipButton.pivot.x, skipButton.pivot.y);
                


        }

        if (Tuto.currentPage == 5) 
        {
            sprite_render(sprGamestart, gameButton.position.x, gameButton.position.y,
                gameButton.scale.x, gameButton.scale.y,
                gameButton.texPos.x, gameButton.texPos.y,
                gameButton.texSize.x, gameButton.texSize.y,
                gameButton.pivot.x, gameButton.pivot.y);

            
          
        }
    }

    // �t�F�[�h�C���`��
    if (Tuto.tuto_fadein > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadein);
    }

    // �t�F�[�h�A�E�g�`��
    if (Tuto.tuto_fadeout > 0.0f)
    {
        primitive::rect(0, 0, SCREEN_W, SCREEN_H, 0, 0, ToRadian(0), 0, 0, 0, Tuto.tuto_fadeout);
    }

   
}


void tuto_deinit()
{
    safe_delete(sprBack);
    for (int i = 0; i < 6; i++) safe_delete(sprSetu[i]);
    safe_delete(sprPlay);
    safe_delete(sprNextpage);
    safe_delete(sprBackpage);
    safe_delete(sprGamestart);
    music::stop(BGM_TUTO);
    music::stop(BGM_BUTTON);
    safe_delete(sprSkip);
   
}

// �t�F�[�h�C��
void fadeIn_act()
{
    if (!Tuto.isTuto_Fadein)
    {
       
        Tuto.isTuto_Fadein = true;
    }

    if (Tuto.isTuto_Fadein)
    {
        Tuto.tuto_fadein -= 0.03f;
        if (Tuto.tuto_fadein <= 0.0f)
        {
            
            Tuto.tuto_fadein = 0.0f;
            Tuto.isTuto_Fadein = false;
            Tuto.tuto_state++;
        }
    }
}

// �t�F�[�h�A�E�g
void fadeOut_act()
{
    if (Tuto.isTuto_Fadeout)
    {
        Tuto.click_delay_timer += 0.03f;
        if (Tuto.click_delay_timer >= 2.0f)
        {
            Tuto.tuto_fadeout += 0.05f;
            if (Tuto.tuto_fadeout >= 1.0f)
            {
                Tuto.tuto_fadeout = 1.0f;
                Tuto.tuto_fadeTimer += 0.1f;
                if (Tuto.tuto_fadeTimer >= 7.0f)
                {
                    
                    game_start();
                    Tuto.tuto_fadeTimer = 0.0f;
                    Tuto.tuto_state = 0; 
                }
            }
        }
    }
}

void tuto_click_act()
{

    // 5���ڂ̃y�[�W�Ȃ�u�߂�v�{�^���̏������X�L�b�v
    if (Tuto.currentPage == 5)
    {
        tutoNextbutton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�T�C�Y�Ƀ��Z�b�g
        return;
    }
    // �N���b�N����Ĕ͈͓��ɂ���ꍇ
    if ((TRG(0) & L_CLICK) && tuto_click())
    {
        tutoNextbutton.isClicked = true;          // �N���b�N��Ԃ��Z�b�g
        tutoNextbutton.clickTimer = 0.0f;         // �^�C�}�[�����Z�b�g
        tutoNextbutton.scale = { 1.0f, 1.0f };  // �X�P�[��������������

        // �y�[�W��i�߂�
        if (Tuto.currentPage < 5) {
            Tuto.currentPage++;
            music::play(BGM_BUTTON, false);
        }
        else if (Tuto.currentPage == 5) {  // �Ō�̃y�[�W�̂Ƃ�
            Tuto.isTuto_Fadeout = true;
            Tuto.click_delay_timer = 0.0f;
            Tuto.tuto_state++;
        }
    }

    // �N���b�N���܂��̓N���b�N��ԂŔ͈͓��ɂ���ꍇ
    if (tutoNextbutton.isClicked && tuto_click())
    {
        tutoNextbutton.clickTimer += 0.2f;  // �^�C�}�[��i�߂�

        // �N���b�N�^�C�}�[��2�b�ȏ�Ȃ�X�P�[����傫������
        if (tutoNextbutton.clickTimer >= 2.0f)
        {
            tutoNextbutton.scale = { 1.3f, 1.3f };
        }
    }
    else if (tuto_click())
    {
        // �͈͓��ɂ��邪�N���b�N����Ă��Ȃ��ꍇ
        tutoNextbutton.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
    }
    else
    {
        // �͈͊O�ɂ���ꍇ
        tutoNextbutton.scale = { 1.0f, 1.0f }; // �ʏ�̃X�P�[���ɖ߂�
    }

    // �^�C�}�[��10.0�ȏ�ɂȂ�����N���b�N��Ԃ����Z�b�g
    if (tutoNextbutton.clickTimer >= 5.0f)
    {
        tutoNextbutton.isClicked = false;
        tutoNextbutton.clickTimer = 0.0f;
    }

}



bool tuto_click() 
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = tutoNextbutton.position.x + (tutoNextbutton.texSize.x * tutoNextbutton.scale.x)/2 ;
    float button_left = tutoNextbutton.position.x - (tutoNextbutton.texSize.x * tutoNextbutton.scale.x)/2 ;
    float button_top = tutoNextbutton.position.y - (tutoNextbutton.texSize.y * tutoNextbutton.scale.y) /2;
    float button_bottom = tutoNextbutton.position.y + (tutoNextbutton.texSize.y * tutoNextbutton.scale.y)/2 ;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void tuto_click_back_act()
{
    // 1���ڂ̃y�[�W�Ȃ�u�߂�v�{�^���̏������X�L�b�v
    if (Tuto.currentPage == 0)
    {
        tutoBackbutton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�T�C�Y�Ƀ��Z�b�g
        return;
    }

    // �N���b�N����Ĕ͈͓��ɂ���ꍇ
    if ((TRG(0) & L_CLICK) && tuto_click_back())
    {
        tutoBackbutton.isClicked = true;          // �N���b�N��Ԃ��Z�b�g
        tutoBackbutton.clickTimer = 0.0f;         // �^�C�}�[�����Z�b�g
        tutoBackbutton.scale = { 1.0f, 1.0f };    // �X�P�[��������������

        // �y�[�W��߂�
        if (Tuto.currentPage > 0) {
            Tuto.currentPage--;
            music::play(BGM_BUTTON, false);
        }
    }

    // �N���b�N���܂��̓N���b�N��ԂŔ͈͓��ɂ���ꍇ
    if (tutoBackbutton.isClicked && tuto_click_back())
    {
        tutoBackbutton.clickTimer += 0.2f;  // �^�C�}�[��i�߂�

        // �N���b�N�^�C�}�[��2�b�ȏ�Ȃ�X�P�[����傫������
        if (tutoBackbutton.clickTimer >= 2.0f)
        {
            tutoBackbutton.scale = { 1.3f, 1.3f };
        }
    }
    else if (tuto_click_back())
    {
        // �͈͓��ɂ��邪�N���b�N����Ă��Ȃ��ꍇ
        tutoBackbutton.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
    }
    else
    {
        // �͈͊O�ɂ���ꍇ
        tutoBackbutton.scale = { 1.0f, 1.0f }; // �ʏ�̃X�P�[���ɖ߂�
    }

    // �^�C�}�[��5.0�ȏ�ɂȂ�����N���b�N��Ԃ����Z�b�g
    if (tutoBackbutton.clickTimer >= 5.0f)
    {
        tutoBackbutton.isClicked = false;
        tutoBackbutton.clickTimer = 0.0f;
    }
}


bool tuto_click_back()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = tutoBackbutton.position.x + (tutoBackbutton.texSize.x * tutoBackbutton.scale.x)/2;
    float button_left = tutoBackbutton.position.x - (tutoBackbutton.texSize.x * tutoBackbutton.scale.x)/2;
    float button_top = tutoBackbutton.position.y - (tutoBackbutton.texSize.y * tutoBackbutton.scale.y)/2;
    float button_bottom = tutoBackbutton.position.y + (tutoBackbutton.texSize.y * tutoBackbutton.scale.y)/2;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void game_click_act()
{
    if (Tuto.currentPage != 5)
    {
       
        gameButton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�T�C�Y�Ƀ��Z�b�g
        gameButton.isClicked = false;     // �N���b�N��Ԃ����Z�b�g
        gameButton.clickTimer = 0.0f;     // �^�C�}�[�����Z�b�g
        gameButton.clickCount = 0;        // �N���b�N�J�E���g�����Z�b�g
        return;
    }

    if (gameButton.clickCount == 0 && (TRG(0) & L_CLICK) && game_click())
    {
        // �N���b�N����Ă��Ȃ� && ���N���b�N���͈͓��Ŕ��������ꍇ
        gameButton.isClicked = true;
        gameButton.clickCount = 1;        // �N���b�N�J�E���g�𑝂₷
        gameButton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�T�C�Y��
        music::play(BGM_BUTTON, false);
    }

    if (gameButton.isClicked)
    {
        gameButton.clickTimer += 0.2f;
        if (gameButton.clickTimer >= 2.0f)  // �N���b�N�^�C�}�[��2�b�ȏ�ɂȂ�����
        {
            gameButton.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
        }

        if (gameButton.clickTimer >= 5.0f)  // 5�b�o�߂Ńt�F�[�h�A�E�g������
        {
            Tuto.isTuto_Fadeout = true;
            gameButton.clickTimer = 0.0f; // �^�C�}�[�����Z�b�g
            gameButton.isClicked = false; // �N���b�N��Ԃ����Z�b�g
            gameButton.clickCount = 0;    // �N���b�N�J�E���g�����Z�b�g
            Tuto.tuto_state++;            // �`���[�g���A���̏�Ԃ�i�߂�
        }
    }
    else if (gameButton.clickCount == 0 && !game_click()) // �N���b�N����Ă��Ȃ� && �͈͊O�̏ꍇ
    {
        gameButton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�ɖ߂�
    }

    if (gameButton.clickCount == 0 && game_click()) // �N���b�N����Ă��Ȃ� && �͈͓��̏ꍇ
    {
        gameButton.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
    }
}

bool game_click()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = gameButton.position.x + (gameButton.texSize.x * gameButton.scale.x) / 2;
    float button_left = gameButton.position.x - (gameButton.texSize.x * gameButton.scale.x) / 2;
    float button_top = gameButton.position.y - (gameButton.texSize.y * gameButton.scale.y) / 1.5;
    float button_bottom = gameButton.position.y + (gameButton.texSize.y * gameButton.scale.y) / 3.1;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

void skip_click_act() 
{
    if (Tuto.currentPage == 5)
    {
        skipButton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�T�C�Y�Ƀ��Z�b�g
        return;
    }

    if (skipButton.clickCount == 0 && (TRG(0) & L_CLICK) && skip_click())
    {
        // �N���b�N����Ă��Ȃ� && �t�F�[�h�A�E�g���Ă��Ȃ� && ���N���b�N���͈͓��Ŕ��������ꍇ
        skipButton.isClicked = true;
        skipButton.clickCount = 1; // �N���b�N�J�E���g�𑝂₷
        skipButton.scale = { 1.0f, 1.0f }; // �X�P�[��������������
        music::play(BGM_BUTTON, false);
    }

    if (skipButton.isClicked)
    {
        skipButton.clickTimer += 0.2f;

        if (skipButton.clickTimer >= 2.0f)  // �N���b�N�^�C�}�[��2�b�ȏ�ɂȂ�����
        {
            // �X�P�[����傫������
            skipButton.scale = { 1.3f, 1.3f };
        }

        if (skipButton.clickTimer >= 5.0f)  // 5�b�o�߂Ńt�F�[�h�A�E�g������
        {
            Tuto.isTuto_Fadeout = true;
            skipButton.clickTimer = 0.0f;
            skipButton.isClicked = false; // �N���b�N��Ԃ����Z�b�g
            skipButton.clickCount = 0;    // �N���b�N�J�E���g�����Z�b�g
            Tuto.tuto_state++;
        }
    }


    else if (skipButton.clickCount == 0 && !skip_click()) // �N���b�N����Ă��Ȃ� && �͈͊O�̏ꍇ
    {
        skipButton.scale = { 1.0f, 1.0f }; // �X�P�[����ʏ�ɖ߂�
    }

    if (skipButton.clickCount == 0 && skip_click()) // �N���b�N����Ă��Ȃ� && �͈͓��̏ꍇ
    {
        skipButton.scale = { 1.3f, 1.3f }; // �X�P�[����傫������
    }
}

bool skip_click()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window::getHwnd(), &point);

    float button_right = skipButton.position.x + (skipButton.texSize.x * skipButton.scale.x) / 2;
    float button_left = skipButton.position.x - (skipButton.texSize.x * skipButton.scale.x) / 2;
    float button_top = skipButton.position.y - (skipButton.texSize.y * skipButton.scale.y) / 1.5;
    float button_bottom = skipButton.position.y + (skipButton.texSize.y * skipButton.scale.y) / 3.1;

    bool isWithinX = (point.x >= button_left && point.x <= button_right);
    bool isWithinY = (point.y >= button_top && point.y <= button_bottom);

    return isWithinX && isWithinY;
}

