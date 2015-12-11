//=========================================================
// fps.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_FPS_H__
#define __APP_FPS_H__


//-------------------------------------
// class
//-------------------------------------
class Fps
{
public:
	Fps();
	virtual ~Fps();
	bool CheckExecute();
private:
	void Update();
	DWORD execute_lasttime_;
	DWORD fps_lasttime_;
	DWORD current_time_;
	DWORD frame_count_;
	static const int fps_ = 60;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
