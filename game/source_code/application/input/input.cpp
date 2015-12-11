//=========================================================
// input.cpp
// author:ryuya nakamura
//=========================================================


//=========================================================
// include
//=========================================================
#include "../../common/common.h"
#include "../window/window_holder.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "input.h"


//=========================================================
// macro definition
//=========================================================
#pragma comment(lib,"dinput8.lib")


//=========================================================
// variable definition
//=========================================================
LPDIRECTINPUT8 Input::input_ = NULL;
int Input::device_count_ = 0;
bool Input::aquire_ = false;

//-------------------------------------
// Input()
//-------------------------------------
Input::Input()
{
	aquire_ = true; 
	if (input_ == NULL)
	{
		DirectInput8Create(
			WindowHolder::instance_,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&input_,
			NULL);
	}
	device_count_++;
}


//-------------------------------------
// ~Input()
//-------------------------------------
Input::~Input()
{
	device_count_--;
	if (!device_count_) SAFE_RELEASE(input_);
}

//-------------------------------------
// end of file
//-------------------------------------
