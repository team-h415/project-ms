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

	static bool isPress(int pad){
		return press_[pad];
	}
	static bool isTrigger(int pad){
		return trigger_[pad];
	}
	static bool isRelease(int pad){
		return release_[pad];
	}
	static bool isRepeat(int pad){
		return repeat_[pad];
	}
	static STICK_PARAMETER_DESC isStick(){
		STICK_PARAMETER_DESC stick;
		stick.lsx_ = static_cast<float>(joy_state_.lX) / 1000.0f;
		stick.lsy_ = static_cast<float>(joy_state_.lY) / 1000.0f;
		stick.rsx_ = static_cast<float>(joy_state_.lZ) / 1000.0f;
		stick.rsy_ = static_cast<float>(joy_state_.lRz) / 1000.0f;
		return stick;
	}


private:
	static BOOL CALLBACK CallBackPad(
		const DIDEVICEINSTANCE *instance,
		VOID *reference);
	static BOOL CALLBACK CallBackAces(
		const LPCDIDEVICEOBJECTINSTANCE instance,
		LPVOID reference);

	static LPDIRECTINPUTDEVICE8 device_;
	static DIJOYSTATE2 joy_state_;
	static bool state_[PAD_MAX];
	static bool press_[PAD_MAX];
	static bool trigger_[PAD_MAX];
	static bool repeat_[PAD_MAX];
	static bool release_[PAD_MAX];
	static int repeat_count_[PAD_MAX];
};


#endif //__GamePad_H__


//-------------------------------------
// end of file
//-------------------------------------