//=========================================================
// KeyBoard.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../window/window_holder.h"
#include "../input.h"
#include "keyboard.h"


//-------------------------------------
// variable definition
//-------------------------------------
BYTE KeyBoard::state_[keymax_];
BYTE KeyBoard::press_[keymax_];
BYTE KeyBoard::trigger_[keymax_];
BYTE KeyBoard::repeat_[keymax_];
BYTE KeyBoard::release_[keymax_];
int KeyBoard::repeat_count_[keymax_];


//-------------------------------------
// KeyBoard()
//-------------------------------------
KeyBoard::KeyBoard()
{
	device_ = NULL;
	for (int i = 0; i < keymax_; i++)
	{
		state_[i] = 0;
		press_[i] = 0;
		trigger_[i] = 0;
		repeat_[i] = 0;
		release_[i] = 0;
		repeat_count_[i] = 0;
	}

	if (input_ == NULL)
	{
		ASSERT_ERROR("入力オブジェクトが生成されていません");
		return;
	}

	if (FAILED(input_->CreateDevice(
		GUID_SysKeyboard,
		&device_,
		NULL)))
	{
		ASSERT_ERROR("入力デバイス生成に失敗");
		return;
	}

	if (FAILED(device_->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		ASSERT_ERROR("データフォーマットの設定に失敗");
		return;
	}

	if (FAILED(device_->SetCooperativeLevel(
		WindowHolder::handle_,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		ASSERT_ERROR("強調モードの設定に失敗");
		return;
	}

	device_->Acquire();
}


//-------------------------------------
// ~KeyBoard()
//-------------------------------------
KeyBoard::~KeyBoard()
{
	if (device_)device_->Unacquire();
	SAFE_RELEASE(device_);
}


//-------------------------------------
// Update()
//-------------------------------------
void KeyBoard::Update()
{
	BYTE state[keymax_];
	
	if (SUCCEEDED(device_->GetDeviceState(
		sizeof(state), state)))
	{
		for (int i = 0; i < keymax_; i++)
		{
			trigger_[i] = (state[i] ^ state_[i]) & state[i];
			release_[i] = (state[i] ^ state_[i]) & state_[i];
			press_[i] = state[i];
			
			if (state[i]) repeat_count_[i]++; 
			else repeat_count_[i] = 0;
			
			if (repeat_count_[i] >= 20 &&
				!(repeat_[i] % 5) ||
				trigger_[i])
			{
				repeat_[i] = 1;
			}
			else 
			{
				repeat_[i] = 0;
			}

			state_[i] = state[i];
		}
	}
	else
	{
		device_->Acquire();
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void KeyBoard::Draw()
{

}


//-------------------------------------
// end of file
//-------------------------------------
