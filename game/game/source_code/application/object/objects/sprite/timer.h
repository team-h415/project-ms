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
    // 1•ª
    static const int kOneMinute = 60;
    // ‰½•ª‚©
    static const int kMinuteNum = 3;
    // ‰½•b‚©
    static const int kSecondNum = 0;
    // ‰½i”‚©
    static const int kFigureDefine = 10;
    // ‰½Œ…‚©
    static const int kMaxFigure = 3;
    // ƒQ[ƒ€ƒ^ƒCƒ€
    static const int kTimerCount = kMinuteNum * kOneMinute + kSecondNum;
    // •¶š‚ÌŠÔŠu
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
private:
    int count_;
    int value_;
    Number **p_number_;
};


#endif //__Timer_H__


//-------------------------------------
// end of file
//-------------------------------------
