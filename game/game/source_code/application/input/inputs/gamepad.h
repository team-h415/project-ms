//=========================================================
// GamePad.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __GamePad_H__
#define __GamePad_H__


//-------------------------------------
// define
//-------------------------------------
#define PAD_STICK_MARGIN 32


//-------------------------------------
// enum
//-------------------------------------
enum INPUT_PAD
{
	PAD_LS_LEFT = 0,
	PAD_LS_RIGHT,
	PAD_LS_UP,
	PAD_LS_DOWN,
	PAD_RS_LEFT,
	PAD_RS_RIGHT,
	PAD_RS_UP,
	PAD_RS_DOWN,
	PAD_BUTTON_1,
	PAD_BUTTON_2,
	PAD_BUTTON_3,
	PAD_BUTTON_4,
	PAD_BUTTON_5,
	PAD_BUTTON_6,
	PAD_BUTTON_7,
	PAD_BUTTON_8,
	PAD_BUTTON_9,
	PAD_BUTTON_10,
	PAD_BUTTON_11,
	PAD_BUTTON_12,
	PAD_BUTTON_13,
	PAD_MAX
};

enum GAMEPAD_ID
{
	GAMEPAD_GRANDFATHER = 0,
	GAMEPAD_CHILD1,
	GAMEPAD_CHILD2,
	GAMEPAD_CHILD3,
	GAMEPAD_CHILD4,
	GAMEPAD_MAX
};


//-------------------------------------
// struct
//-------------------------------------
struct STICK_PARAMETER_DESC
{
	float lsx_, lsy_, rsx_, rsy_;
};


//-------------------------------------
// class
//-------------------------------------
class GamePad : public Input
{
public:
	GamePad();
	virtual ~GamePad();
	void Update();

	static bool isPress(int id, int pad){
		return press_[id][pad];
	}
	static bool isTrigger(int id, int pad){
		return trigger_[id][pad];
	}
	static bool isRelease(int id, int pad){
		return release_[id][pad];
	}
	static bool isRepeat(int id, int pad){
		return repeat_[id][pad];
	}
	static STICK_PARAMETER_DESC isStick(int id){
		STICK_PARAMETER_DESC stick;
		stick.lsx_ = static_cast<float>(joy_state_[id].lX) / 1000.0f;
		stick.lsy_ = static_cast<float>(joy_state_[id].lY) / 1000.0f;
		stick.rsx_ = static_cast<float>(joy_state_[id].lZ) / 1000.0f;
		stick.rsy_ = static_cast<float>(joy_state_[id].lRz) / 1000.0f;
		return stick;
	}


private:
	static BOOL CALLBACK CallBackPad(
		const DIDEVICEINSTANCE *instance,
		VOID *reference);
	static BOOL CALLBACK CallBackAces(
		const LPCDIDEVICEOBJECTINSTANCE instance,
		LPVOID reference);

	static LPDIRECTINPUTDEVICE8 device_[GAMEPAD_MAX];
	static DIJOYSTATE2 joy_state_[GAMEPAD_MAX];
	static bool state_[GAMEPAD_MAX][PAD_MAX];
	static bool press_[GAMEPAD_MAX][PAD_MAX];
	static bool trigger_[GAMEPAD_MAX][PAD_MAX];
	static bool repeat_[GAMEPAD_MAX][PAD_MAX];
	static bool release_[GAMEPAD_MAX][PAD_MAX];
	static int repeat_count_[GAMEPAD_MAX][PAD_MAX];
	static bool gamepad_aquire[GAMEPAD_MAX];
};


#endif //__GamePad_H__


//-------------------------------------
// end of file
//-------------------------------------