//=========================================================
// KeyBoard.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_INPUT_INPUTS_KEYBOARD_H__
#define __APP_INPUT_INPUTS_KEYBOARD_H__


//-------------------------------------
// class
//-------------------------------------
class KeyBoard : public Input
{
public:
	KeyBoard();
	virtual ~KeyBoard();

	void Update();

	//--------------------------------------
	// �L�[���͎擾(����)
	static bool isPress(int key){
		if (!isAquire()) return false;
		return press_[key] & 128 ? true : false;
	}

	//--------------------------------------
	// �L�[���͎擾(�g���K�[)
	static bool isTrigger(int key){
		if (!isAquire()) return false;
		return trigger_[key] & 128 ? true : false;
	}

	//--------------------------------------
	// �L�[���͎擾(���s�[�g)
	static bool isRepeat(int key){
		if (!isAquire()) return false;
		return repeat_[key] & 128 ? true : false;
	}

	//--------------------------------------
	// �L�[���͎擾(�����[�X)
	static bool isRelease(int key){
		if (!isAquire()) return false;
		return release_[key] & 128 ? true : false;
	}

private:
	static const int keymax_ = 256;

	LPDIRECTINPUTDEVICE8 device_;
	static BYTE state_[keymax_];
	static BYTE press_[keymax_];
	static BYTE trigger_[keymax_];
	static BYTE repeat_[keymax_];
	static BYTE release_[keymax_];
	static int repeat_count_[keymax_];
};


#endif //__APP_INPUT_INPUTS_KEYBOARD_H__


//-------------------------------------
// end of file
//-------------------------------------
