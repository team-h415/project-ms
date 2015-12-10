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
LPDIRECTINPUTDEVICE8 GamePad::device_[GAMEPAD_MAX];
DIJOYSTATE2 GamePad::joy_state_[GAMEPAD_MAX];
bool GamePad::state_[GAMEPAD_MAX][PAD_MAX];
bool GamePad::press_[GAMEPAD_MAX][PAD_MAX];
bool GamePad::trigger_[GAMEPAD_MAX][PAD_MAX];
bool GamePad::repeat_[GAMEPAD_MAX][PAD_MAX];
bool GamePad::release_[GAMEPAD_MAX][PAD_MAX];
int GamePad::repeat_count_[GAMEPAD_MAX][PAD_MAX];
bool GamePad::gamepad_aquire[GAMEPAD_MAX] = {false};
int current_;

//-------------------------------------
// GamePad()
//-------------------------------------
GamePad::GamePad()
{
	for (int i = 0; i < GAMEPAD_MAX; i++){
		current_ = i;

		if (FAILED(input_->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			CallBackPad,
			NULL,
			DIEDFL_ATTACHEDONLY))){
			ASSERT_ERROR("オブジェクト生成に失敗");
			return;
		}

		if (!device_[i]) return;

		if (FAILED(device_[i]->SetDataFormat(
			&c_dfDIJoystick2))){
			ASSERT_ERROR("デバイス生成に失敗");
			return;
		}

		if (FAILED(device_[i]->SetCooperativeLevel(
			WindowHolder::handle_,
			(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))){
			ASSERT_ERROR("協調レベル設定に失敗");
			return;
		}

		if (FAILED(device_[i]->EnumObjects(
			CallBackAces, NULL, DIDFT_AXIS))){
			ASSERT_ERROR("使用可能デバイスがありませんでした");
			return;
		}

		device_[i]->Acquire();

		gamepad_aquire[i] = true;
	}

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
	for (int i = 0; i < GAMEPAD_MAX; i++){
		if (device_[i]) device_[i]->Unacquire();
		SAFE_RELEASE(device_[i]);
	}
}


//-------------------------------------
// Update()
//-------------------------------------
void GamePad::Update()
{
	for (int k = 0; k < GAMEPAD_MAX; k++){
		bool state[PAD_MAX];

		if (!device_[k]) continue;
		if (!gamepad_aquire[k]) continue;

		if (SUCCEEDED(device_[k]->GetDeviceState(
			sizeof(joy_state_[k]), &joy_state_[k]))){

			for (int i = 0, j = 0; i + j < PAD_MAX; i++){
				if (i == 0){
					state[PAD_LS_LEFT] =
						joy_state_[k].lX < -PAD_STICK_MARGIN || (joy_state_[k].rgdwPOV[0]
						>= 225 * 100 && joy_state_[k].rgdwPOV[0] <= 315 * 100);
					state[PAD_LS_RIGHT] = joy_state_[k].lX > PAD_STICK_MARGIN ||
						(joy_state_[k].rgdwPOV[0] >= 45 * 100 && joy_state_[k].rgdwPOV[0]
						<= 135 * 100);
					state[PAD_LS_UP] = joy_state_[k].lY < -PAD_STICK_MARGIN ||
						((joy_state_[k].rgdwPOV[0] >= 315 * 100 && joy_state_[k].rgdwPOV[0]
						<= 360 * 100) || (joy_state_[k].rgdwPOV[0] >= 0 * 100 &&
						joy_state_[k].rgdwPOV[0] <= 45 * 100));
					state[PAD_LS_DOWN] = joy_state_[k].lY > PAD_STICK_MARGIN ||
						(joy_state_[k].rgdwPOV[0] >= 135 * 100 && joy_state_[k].rgdwPOV[0]
						<= 225 * 100);

					state[PAD_RS_LEFT] = joy_state_[k].lZ < -PAD_STICK_MARGIN;
					state[PAD_RS_RIGHT] = joy_state_[k].lZ > PAD_STICK_MARGIN;
					state[PAD_RS_UP] = joy_state_[k].lRz < -PAD_STICK_MARGIN;
					state[PAD_RS_DOWN] = joy_state_[k].lRz > PAD_STICK_MARGIN;

					j = 8;
				}
				state[i + j] = joy_state_[k].rgbButtons[i] == 128;
			}

			for (int i = 0; i < PAD_MAX; i++){
				trigger_[k][i] = (state[i] ^ state_[k][i]) & state[i];
				release_[k][i] = (state[i] ^ state_[k][i]) & state_[k][i];
				press_[k][i] = state[i];

				state_[k][i] = state[i];
			}
		}
		else{
			device_[k]->Acquire();
		}
	}
}


//-------------------------------------
// CallBackPad()
//-------------------------------------
BOOL CALLBACK GamePad::CallBackPad(
	const DIDEVICEINSTANCE *lpddi,
	VOID *pvRef)
{
	static GUID gamepad_discrimination[GAMEPAD_MAX];
	DIDEVCAPS	DevCaps;
	HRESULT		Result;

	for (int i = 0; i < current_; i++){
		if (gamepad_discrimination[i] == lpddi->guidInstance){
			return DIENUM_CONTINUE;
		}
	}
	Result = input_->CreateDevice(lpddi->guidInstance, &device_[current_], NULL);

	if (FAILED(Result))
	{
		return DIENUM_CONTINUE;
	}

	DevCaps.dwSize = sizeof(DIDEVCAPS);
	Result = device_[current_]->GetCapabilities(&DevCaps);

	if (FAILED(Result))
	{
		SAFE_RELEASE(device_[current_]);
		return DIENUM_CONTINUE;
	}

	gamepad_discrimination[current_] = lpddi->guidInstance;

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

	Result = device_[current_]->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(Result))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;

}


//-------------------------------------
// end of file
//-------------------------------------
