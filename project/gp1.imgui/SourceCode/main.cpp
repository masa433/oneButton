//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< �C���N���[�h >---------------------------------------------------------
#include "all.h"

//------< namespace >-----------------------------------------------------------
using namespace GameLib;

//------< �ϐ� >----------------------------------------------------------------

//------------------------------------------------------------------------------
//  WinMain�i�G���g���|�C���g�j
//------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// �g�p���Ȃ��ϐ��͋L�q���Ȃ�
{
    // �Q�[�����C�u�����̏����ݒ�
    GameLib::init(L"�Q�[���v���O���~���O�T", SCREEN_W, SCREEN_H, FULLSCREEN);

    game_init();

    while (GameLib::gameLoop())
    {
        // ���͏���
        input::update();

#ifdef USE_IMGUI
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
#endif

        // ���݂̃V�[�����X�V�E�`��
        game_update();
        game_render();

#ifdef USE_IMGUI
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

        // �f�o�b�O�������`��
        debug::display(1.0f, 1.0f, 1.0f, 1, 1);

        //debug::setString("GP1_01 SAMPLE");

        // �o�b�N�o�b�t�@�̓��e��\��
        GameLib::present(1, 0);
    }

    // �Q�[���V�[���̏I��
    game_deinit();

    // �Q�[�����C�u�����̏I������
    GameLib::uninit();
}
