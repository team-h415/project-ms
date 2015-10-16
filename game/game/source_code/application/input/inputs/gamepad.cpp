//=========================================================
// gamepad.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../window/window_holder.h"
#include "../input.h"
#include "gamepad.h"


//-------------------------------------
// variable
//-------------------------------------
LPDIRECTINPUTDEVICE8 GamePad::device_;
DIJOYSTATE2 GamePad::joy_state_;
bool GamePad::state_[PAD_MAX];
bool GamePad::press_[PAD_MAX];
bool GamePad::trigger_[PAD_MAX];
bool GamePad::repeat_[PAD_MAX];
bool GamePad::release_[PAD_MAX];
int GamePad::repeat_count_[PAD_MAX];


//-------------------------------------
// GamePad()
//-------------------------------------
GamePad::GamePad()
{
	if (FAILED(input_->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		CallBackPad,
		NULL,
		DIEDFL_ATTACHEDONLY))){
		ASSERT_ERROR("オブジェクト生成に失敗");
		return;
	}

	if (!device_) return;

	if (FAILED(device_->SetDataFormat(
		&c_dfDIJoystick2))){
		ASSERT_ERROR("デバイス生成に失敗");
		return;
	}

	if (FAILED(device_->SetCooperativeLevel(
		WindowHolder::handle_,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))){
		ASSERT_ERROR("協調レベル設定に失敗");
		return;
	}

	if (FAILED(device_->EnumObjects(
		CallBackAces, NULL, DIDFT_AXIS))){
		ASSERT_ERROR("使用可能デバイスがありませんでした");
		return;
	}

	device_->Acquire();

	memset(&joy_state_, 0, sizeof(joy_state_));
	memset(&state_, 0, sizeof(state_));
	memset(&press_, 0, sizeof(press_));
	memset(&trigger_, 0, sizeof(trigger_));
	memset(&repeat_, 0, sizeof(repeat_));
	memset(&release_, 0, sizeof(release_));
}


//-------------------------------------
// ~GamePad()
//-------------------------------------
GamePad::~GamePad()
{
	if (device_) device_->Unacquire();
	SAFE_RELEASE(device_);
}


//-------------------------------------
// Update()
//-------------------------------------
void GamePad::Update()
{
	bool state[PAD_MAX];

	if (!device_) return;

	if (SUCCEEDED(device_->GetDeviceState(
		sizeof(joy_state_), &joy_state_))){

		for (int i = 0, j = 0; i + j < PAD_MAX; i++){
			if (i == 0){
				state[PAD_LS_LEFT] = 
					joy_state_.lX < -PAD_STICK_MARGIN || (joy_state_.rgdwPOV[0]
					>= 225 * 100 && joy_state_.rgdwPOV[0] <= 315 * 100);
				state[PAD_LS_RIGHT] = joy_state_.lX > PAD_STICK_MARGIN ||
					(joy_state_.rgdwPOV[0] >= 45 * 100 && joy_state_.rgdwPOV[0]
					<= 135 * 100);
				state[PAD_LS_UP] = joy_state_.lY < -PAD_STICK_MARGIN ||
					((joy_state_.rgdwPOV[0] >= 315 * 100 && joy_state_.rgdwPOV[0]
					<= 360 * 100) || (joy_state_.rgdwPOV[0] >= 0 * 100 && 
					joy_state_.rgdwPOV[0] <= 45 * 100));
				state[PAD_LS_DOWN] = joy_state_.lY > PAD_STICK_MARGIN || 
					(joy_state_.rgdwPOV[0] >= 135 * 100 && joy_state_.rgdwPOV[0]
					<= 225 * 100);

				state[PAD_RS_LEFT] = joy_state_.lZ < -PAD_STICK_MARGIN;
				state[PAD_RS_RIGHT] = joy_state_.lZ > PAD_STICK_MARGIN;
				state[PAD_RS_UP] = joy_state_.lRz < -PAD_STICK_MARGIN;
				state[PAD_RS_DOWN] = joy_state_.lRz > PAD_STICK_MARGIN;

				j = 8;
			}
			state[i + j] = joy_state_.rgbButtons[i] == 128;
		}

		for (int i = 0; i < PAD_MAX; i++){
			trigger_[i] = (state[i] ^ state_[i]) & state[i];
			release_[i] = (state[i] ^ state_[i]) & state_[i];
			press_[i] = state[i];

			state_[i] = state[i];
		}
	}
	else{
		device_->Acquire();
	}
}


//-------------------------------------
// CallBackPad()
//-------------------------------------
BOOL CALLBACK GamePad::CallBackPad(
	const DIDEVICEINSTANCE *lpddi,
	VOID *pvRef)
{
	DIDEVCAPS	DevCaps;
	HRESULT		Result;

	Result = input_->CreateDevice(lpddi->guidInstance, &device_, NULL);

	if (FAILED(Result))
	{
		return DIENUM_CONTINUE;
	}

	DevCaps.dwSize = sizeof(DIDEVCAPS);
	Result = device_->GetCapabilities(&DevCaps);

	if (FAILED(Result))
	{
		SAFE_RELEASE(device_);
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}


//-------------------------------------
// CallBackAces()
//-------------------------------------
BOOL CALLBACK GamePad::CallBackAces(
	const LPCDIDEVICEOBJECTINSTANCE lpddoi,
	LPVOID pvRef)
{
	HRESULT Result;

	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	Result = device_->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(Result))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;

}


//-------------------------------------
// end of file
//-------------------------------------
