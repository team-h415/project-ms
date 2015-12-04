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
namespace{
    // 1ï™
    static const int kOneMinute = 60;
    // âΩï™Ç©
    static const int kMinuteNum = 3;
    // âΩïbÇ©
    static const int kSecondNum = 0;
    // âΩêiêîÇ©
    static const int kFigureDefine = 10;
    // âΩåÖÇ©
    static const int kMaxFigure = 3;
    // ÉQÅ[ÉÄÉ^ÉCÉÄ
    static const int kTimerCount = kMinuteNum * kOneMinute + kSecondNum;
    // ï∂éöÇÃä‘äu
    static const float kSpace = 40.0f;
}


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
	void SetValue(int value_);
	void SetTexture(const std::string &path);

private:
    int count_;
    int value_;
    Number **p_number_;
};


#endif //__Timer_H__


//-------------------------------------
// end of file
//-------------------------------------
