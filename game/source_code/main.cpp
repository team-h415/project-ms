//=========================================================
// main.cpp
// author:ryuya nakamura
// エントリポイント
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
	// Windows環境下のメモリリークチェック(DEBUGのみ) 
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG


	// アプリケーションの実行
	Application::Run(
		instance,
		command_show);

}


//-------------------------------------
// end of file
//-------------------------------------
