//=========================================================
// Input.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_INPUT_INPUT_H__
#define __APP_INPUT_INPUT_H__


//-------------------------------------
// define 
//-------------------------------------
#define DIRECTINPUT_VERSION (0x0800)


//-------------------------------------
// include
//-------------------------------------
#include <dinput.h>


//-------------------------------------
// class
//-------------------------------------
class Input
{
public:
	Input();
	virtual ~Input();

	virtual void Update() = 0;
	virtual void Draw() = 0;
	static bool isAquire(){ return aquire_; }
	static void SetAquire(bool aquire){
		aquire_ = aquire;
	}

protected:
	static LPDIRECTINPUT8 input_;
	static int device_count_;
	static bool aquire_;
};


#endif //__APP_INPUT_INPUT_H__


//-------------------------------------
// end of file
//-------------------------------------
