//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< �C���N���[�h >---------------------------------------------------------
#include "common.h"
#include"scene_game.h"
#include"scene_title.h"
#include"scene_tuto.h"
#include"scene_result.h"

//------< namespace >-----------------------------------------------------------
using namespace GameLib;

//------< �ϐ� >----------------------------------------------------------------
int curScene = SCENE_NONE;
int nextScene = SCENE_TITLE;
SCENE Scene;

//------------------------------------------------------------------------------
//  WinMain�i�G���g���|�C���g�j
//------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// �g�p���Ȃ��ϐ��͋L�q���Ȃ�
{
    // �Q�[�����C�u�����̏����ݒ�
    GameLib::init(L"�Q�[���v���O���~���O�T", SCREEN_W, SCREEN_H, true);



    while (GameLib::gameLoop())
    {
        if (curScene != nextScene) 
        {
            switch (curScene) 
            {
            case SCENE_TITLE:
                title_deinit();
                break;
            case SCENE_TUTO:
                tuto_deinit();
                break;
            case SCENE_GAME:
                game_deinit();
                break;
            case SCENE_RESULT:
                result_deinit();
                break;
            }
       
            // ���̃V�[���ɉ����������ݒ菈��
            switch (nextScene)
            {
            case SCENE_TITLE: 
                title_init();
                break;
            case SCENE_TUTO:
                tuto_init();
                break;
            case SCENE_GAME:
                game_init();
                break;
            case SCENE_RESULT:
                result_init();
                break;
            }
            // nextScene �� curScene �ɂȂ�
            curScene = nextScene;
        }
            // ���͏���
            input::update();

        // ���݂̃V�[���ɉ������X�V�E�`�揈��
        switch (curScene)
        {
        case SCENE_TITLE:
            title_update();
            title_render();
            break;

        case SCENE_TUTO:
            tuto_update();
            tuto_render();
            break;

        case SCENE_GAME:
            game_update();
            game_render();
            break;

        case SCENE_RESULT:
            result_update();
            result_render();
            break;
        }
//#ifdef USE_IMGUI
//        ImGui_ImplDX11_NewFrame();
//        ImGui_ImplWin32_NewFrame();
//        ImGui::NewFrame();
//#endif

       

//#ifdef USE_IMGUI
//        ImGui::Render();
//        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//#endif

        // �f�o�b�O�������`��
        debug::display(1.0f, 1.0f, 1.0f, 1, 1);

        //debug::setString("GP1_01 SAMPLE");

        // �o�b�N�o�b�t�@�̓��e��\��
        GameLib::present(1, 0);
    }


    // �Q�[���V�[���̏I��
    // ���݂̃V�[���ɉ������I���������s��
    switch (curScene)
    {
    case SCENE_TITLE:
        title_deinit();
        break;

    case SCENE_TUTO:
        tuto_deinit();
        break;

    case SCENE_GAME:
        game_deinit();
        break;

    case SCENE_RESULT:
        result_deinit();
        break;
    
    }

    // �Q�[�����C�u�����̏I������
    GameLib::uninit();

    return 0;
}
