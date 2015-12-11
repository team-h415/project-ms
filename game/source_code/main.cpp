//=========================================================
// main.cpp
// author:ryuya nakamura
// �G���g���|�C���g
//=========================================================


//-------------------------------------
// include 
//-------------------------------------
#include "common\common.h"
#include "application\application.h"
#include "main.h"

//-------------------------------------
// WinMain
//-------------------------------------
int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR command_line,
	int command_show)
{
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(command_line);

#ifdef _DEBUG
	// Windows�����̃��������[�N�`�F�b�N(DEBUG�̂�) 
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG


	// �A�v���P�[�V�����̎��s
	Application::Run(
		instance,
		command_show);

}


//-------------------------------------
// end of file
//-------------------------------------
