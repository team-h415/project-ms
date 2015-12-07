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

    unsigned int figure = (unsigned int)pow((float)kFigureDefine, figure_);
	
	

	figure_offset_ = 0.0f;
	figure_ = 0;
}


//-------------------------------------
// ~Timer()
//-------------------------------------
Timer::~Timer()
{
    for (int num = 0; num < figure_; num++){
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
    ++count_;
    if ((count_ % 60) == 0)
    {
        --value_;
        // �I������
        if (value_ < 0)
            value_ = kTimerCount;
        count_ = 0;
    }
    //unsigned int figure = (unsigned int)pow((float)kFigureDefine, figure_);
    //for (int num = 0; num < figure_; num++)
    //{
    //    // ����̌��̒l������
    //    int value = (value_ % figure) / (figure / kFigureDefine);
    //    // �l���Z�b�g
    //    p_number_[num]->SetValue(value);
    //    // �����炵
    //    figure /= kFigureDefine;
    //}


	int value = value_;
	for (int num = 0; num < figure_; num++)
	{
		// ����̌��̒l������
		int figure_value = value % 10;
		// �l���Z�b�g
		p_number_[num]->SetValue(figure_value);
		// �����炵
		value /= 10;
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Timer::Draw()
{
	if (p_number_ == NULL){ return; }

    for (int num = 0; num < figure_; num++)
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

	unsigned int figure = (unsigned int)pow((float)kFigureDefine, figure_);
	for (int num = 0; num < figure_; num++)
	{
		// ����̌��̒l������
		int value = (value_ % figure) / (figure / kFigureDefine);

		// �l���Z�b�g
		p_number_[num]->SetValue(value);
		// �����炵
		figure /= kFigureDefine;
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

	for (int num = 0; num < figure_; num++)
	{
		// �e�N�X�`�����Z�b�g
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

	float right_center_offset = static_cast<float>(figure_)
		* 0.5f
		* parameter_.scaling_.x_
		+ parameter_.scaling_.x_
		* 0.5f
		+ static_cast<float>(figure_)
		* 0.5f
		* figure_offset_;

	for (int num = 0; num < figure_; num++)
	{
		Vector3 pos = parameter_.position_;
		
		// ���W�C��
		pos.x_ += right_center_offset - parameter_.scaling_.x_ * num
			+ static_cast<float>(figure_)* 0.5f * figure_offset_ * num;

		// ���W���Z�b�g
		p_number_[num]->SetPosition(pos);
	}

}

//-------------------------------------
// GenerateNumber()
//-------------------------------------
void Timer::GenerateNumber(void)
{
	// �����v�Z
	figure_ = static_cast<unsigned int>(log10(static_cast<double>(value_))+1);
	
	if (figure_ == 0){ figure_ = 1; }

	if (p_number_){
		for (int num = 0; num < figure_; num++){
			SAFE_DELETE(p_number_[num]);
		}
		SAFE_DELETE_ARRAY(p_number_);
	}

	p_number_ = new Number *[figure_];

	/*float left_center = static_cast<float>(figure_)
		* 0.5f 
		* parameter_.scaling_.x_ 
		- parameter_.scaling_.x_
		* 0.5f
		- static_cast<float>(figure_)
		* 0.5f
		* figure_offset_;
*/

	float right_center_offset = 
		static_cast<float>(figure_) * 0.5f * parameter_.scaling_.x_
		- parameter_.scaling_.x_ * 0.5f
		+ static_cast<float>(figure_) * 0.5f * figure_offset_;

	int value = value_;
	for (int num = 0; num < figure_; num++)
	{
		OBJECT_PARAMETER_DESC param = parameter_;
		// ����̌��̒l������
		int figure_value = value % 10;
		// ���W�C��
		param.position_.x_ += right_center_offset 
								- parameter_.scaling_.x_ * num
								+ figure_offset_ * num;

		// �l���Z�b�g
		p_number_[num] = new Number(param, value);
		p_number_[num]->SetValue(figure_value);
		if (texture_ != NULL){
			p_number_[num]->SetTexture(texture_);
		}

		value /= 10;
	}

}

//-------------------------------------
// end of file
//-------------------------------------
