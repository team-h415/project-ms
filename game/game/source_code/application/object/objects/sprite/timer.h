//=========================================================
// timer.h
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Timer_H__
#define __Timer_H__


//-------------------------------------
// include 
//-------------------------------------
#include "number.h"


//-------------------------------------
// macro
//-------------------------------------
typedef enum TIMER_STATE
{
	TIMER_COUNT = 0,
	TIMER_STOP,
	TIMER_END,
	TIMER_MAX
};

//-------------------------------------
// class
//-------------------------------------
class Timer : public Object
{
public:
    Timer(
		const OBJECT_PARAMETER_DESC &parameter);
    virtual ~Timer();
    void Update();
    void Draw();
    int GetValue(void){ return value_; }
	void SetValue(int value);
	void SetTexture(const std::string &path);
	void SetFigureOffset(float Offset);
	float SetFigureOffset(void){ return figure_offset_; }
	void GenerateNumber(void);
	void SetState(TIMER_STATE state){ state_ = state; }
	TIMER_STATE GetState(void){ return state_; }

private:
    int count_;
    int value_;
    Number **p_number_;
	LPDIRECT3DTEXTURE9 texture_;

	float figure_offset_;
	unsigned int figure_;
	TIMER_STATE state_;
};


#endif //__Timer_H__


//-------------------------------------
// end of file
//-------------------------------------
