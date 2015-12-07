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
    // 1分
    static const int kOneMinute = 60;
    // 何分か
    static const int kMinuteNum = 3;
    // 何秒か
    static const int kSecondNum = 0;
    // 何進数か
    static const int kFigureDefine = 10;
    // 何桁か
    static const int kMaxFigure = 3;
    // ゲームタイム
    static const int kTimerCount = kMinuteNum * kOneMinute + kSecondNum;
    // 文字の間隔
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
	void SetValue(int value);
	void SetTexture(const std::string &path);
	void SetFigureOffset(float Offset);
	float SetFigureOffset(void){ return figure_offset_; }
	void GenerateNumber(void);

private:
    int count_;
    int value_;
    Number **p_number_;
	LPDIRECT3DTEXTURE9 texture_;

	float figure_offset_;
	unsigned int figure_;


};


#endif //__Timer_H__


//-------------------------------------
// end of file
//-------------------------------------
