//=========================================================
// timer.cpp
// author:shohei matsumoto
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../object.h"
#include "Timer.h"


//-------------------------------------
// Timer()
//-------------------------------------
Timer::Timer(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
    parameter_.scaling_ = { 80.0f, 80.0f, 0.0f };
    value_ = kTimerCount;
    count_ = 0;

    p_number_ = new Number *[kMaxFigure];

    unsigned int figure = (unsigned int)pow((float)kFigureDefine, kMaxFigure);
    for (int num = 0; num < kMaxFigure; num++)
    {
        // 特定の桁の値を入れる
        int value = (value_ % figure) / (figure / kFigureDefine);

        parameter_.position_.x_ = -kSpace + parameter.position_.x_ + kSpace * num;
        // 値をセット
        p_number_[num] = new Number(parameter_, value);
        p_number_[num]->SetTexture("resource/texture/figure_all.png");
        p_number_[num]->SetValue(value);
        // 桁ずらし
        figure /= kFigureDefine;
    }
}


//-------------------------------------
// ~Timer()
//-------------------------------------
Timer::~Timer()
{
    for (int num = 0; num < kMaxFigure; num++){
        SAFE_DELETE(p_number_[num]);
    }
    SAFE_DELETE_ARRAY(p_number_);
}


//-------------------------------------
// Update
//-------------------------------------
void Timer::Update()
{
    ++count_;
    if ((count_ % 60) == 0)
    {
        --value_;
        // 終了条件
        if (value_ < 0)
            value_ = kTimerCount;
        count_ = 0;
    }
    unsigned int figure = (unsigned int)pow((float)kFigureDefine, kMaxFigure);
    for (int num = 0; num < kMaxFigure; num++)
    {
        // 特定の桁の値を入れる
        int value = (value_ % figure) / (figure / kFigureDefine);
        // 値をセット
        p_number_[num]->SetValue(value);
        // 桁ずらし
        figure /= kFigureDefine;
    }

}


//-------------------------------------
// Draw()
//-------------------------------------
void Timer::Draw()
{
    for (int num = 0; num < kMaxFigure; num++)
    {
        p_number_[num]->Draw();
    }
}


//-------------------------------------
// end of file
//-------------------------------------
