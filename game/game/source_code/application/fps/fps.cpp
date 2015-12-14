//=========================================================
// fps.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "fps.h"


//-------------------------------------
// variable
//-------------------------------------
int Fps::fps_ = 0;


//-------------------------------------
// Fps()
//-------------------------------------
Fps::Fps()
{
	timeBeginPeriod(1);
	execute_lasttime_ = timeGetTime();
	fps_lasttime_ = timeGetTime();
	current_time_ = 0;
	frame_count_ = 0;
}


//-------------------------------------
// ~Fps()
//-------------------------------------
Fps::~Fps()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Fps::Update()
{
	current_time_ = timeGetTime();
	DWORD deltatime = (current_time_ - fps_lasttime_);
	if (deltatime < 500) return;
	fps_ = frame_count_ * 1000 / deltatime;
	fps_lasttime_ = current_time_;
	frame_count_ = 0;
}


//-------------------------------------
// CheckExecute()
//-------------------------------------
bool Fps::CheckExecute()
{
	Update();
	DWORD deltatime = (current_time_ - execute_lasttime_);
	if (deltatime < (1000 / frame_late_)) return false;
	execute_lasttime_ = current_time_;
	frame_count_++;
	return true;
}


//-------------------------------------
// end of file
//-------------------------------------
