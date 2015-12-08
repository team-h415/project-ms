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
#include "../../../resource/texture_manager.h"
#include "../../object.h"
#include "Timer.h"



//-------------------------------------
// Timer()
//-------------------------------------
Timer::Timer(
    const OBJECT_PARAMETER_DESC &parameter)
{
    parameter_ = parameter;
	parameter_.scaling_ = parameter.scaling_;
    value_ = 0;
    count_ = 0;
	texture_ = NULL;
	p_number_ = NULL;

	figure_offset_ = 0.0f;
	figure_ = 0;
	state_ = TIMER_COUNT;
}


//-------------------------------------
// ~Timer()
//-------------------------------------
Timer::~Timer()
{
    for (unsigned int num = 0; num < figure_; num++)
	{
        SAFE_DELETE(p_number_[num]);
    }
    SAFE_DELETE_ARRAY(p_number_);
	texture_ = NULL;
}


//-------------------------------------
// Update
//-------------------------------------
void Timer::Update()
{
	
	if (state_ == TIMER_COUNT)
	{
		++count_;
		if ((count_ % 60) == 0)
		{
			--value_;
			// 終了条件
			if (value_ < 0)
			{
				value_ = 0;
				count_ = 0;
				state_ = TIMER_END;
			}
			// 値更新
			this->SetValue(value_);
		}
	}

	this->UpdateNumber();
	
}


//-------------------------------------
// Draw()
//-------------------------------------
void Timer::Draw()
{
	if (p_number_ == NULL){ return; }

    for (unsigned int num = 0; num < figure_; num++)
    {
        p_number_[num]->Draw();
    }
}

//-------------------------------------
// SetValue()
//-------------------------------------
void Timer::SetValue(int value)
{
	value_ = value;

	if (p_number_ == NULL){ return; }
	
	int sub_value = value;
	for (unsigned int num = 0; num < figure_; num++)
	{
		// 特定の桁の値を入れる
		int figure_value = sub_value % 10;
		
		// 値をセット;
		p_number_[num]->SetValue(figure_value);

		sub_value /= 10;
	}
}

//-------------------------------------
// SetTexture()
//-------------------------------------
void Timer::SetTexture(
	const std::string &path)
{
	texture_ = TextureManager::GetTexture(path.c_str());

	if (p_number_ == NULL){ return; }

	for (unsigned int num = 0; num < figure_; num++)
	{
		// テクスチャをセット
		p_number_[num]->SetTexture(texture_);
	}
}
//-------------------------------------
// SetFigureOffset()
//-------------------------------------
void Timer::SetFigureOffset(float Offset)
{
	figure_offset_ = Offset;

	if (p_number_ == NULL){ return; }

	float right_center_offset =
		parameter_.scaling_.x_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f)
			+ figure_offset_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f);


	for (unsigned int num = 0; num < figure_; num++)
	{
		Vector3 pos = parameter_.position_;
		
		// 座標修正
		pos.x_ += right_center_offset - num * (parameter_.scaling_.x_ + figure_offset_);

		// 座標をセット
		p_number_[num]->SetPosition(pos);
	}

}

//-------------------------------------
// GenerateNumber()
//-------------------------------------
void Timer::GenerateNumber(void)
{
	// 桁数計算
	figure_ = static_cast<unsigned int>(log10(static_cast<double>(value_))+1);
	
	if (figure_ == 0){ figure_ = 1; }

	if (p_number_){
		for (unsigned int num = 0; num < figure_; num++){
			SAFE_DELETE(p_number_[num]);
		}
		SAFE_DELETE_ARRAY(p_number_);
	}

	p_number_ = new Number *[figure_];

	float right_center_offset = 
		parameter_.scaling_.x_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f)
			+ figure_offset_ * (static_cast<float>(figure_ / 2) - static_cast<float>((figure_ + 1) % 2) * 0.5f);

	int value = value_;
	for (unsigned int num = 0; num < figure_; num++)
	{
		OBJECT_PARAMETER_DESC param = parameter_;
		// 特定の桁の値を入れる
		int figure_value = value % 10;
		// 座標修正
		param.position_.x_ += right_center_offset - num * (parameter_.scaling_.x_ + figure_offset_);

		// 値をセット
		p_number_[num] = new Number(param, value);
		p_number_[num]->SetValue(figure_value);
		if (texture_ != NULL){
			p_number_[num]->SetTexture(texture_);
		}

		value /= 10;
	}

}

//-------------------------------------
// GetNumberPointer()
//-------------------------------------
Number* Timer::GetNumberPointer(unsigned int  num)
{
	if (num >= figure_ || p_number_ == NULL){ return NULL; }

	return p_number_[num];

}

//-------------------------------------
// UpdateNumber()
//-------------------------------------
void Timer::UpdateNumber(void)
{
	if (p_number_ == NULL){ return; }
	
	for (unsigned int num = 0; num < figure_; num++)
	{
		p_number_[num]->Update();
	}

}


//-------------------------------------
// end of file
//-------------------------------------
