//=========================================================
// message.cpp
// author:ryuya nakamura
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
#include "sprite2d.h"
#include "message.h"
#include "../../../resource/texture_manager.h"


//-------------------------------------
// Message()
//-------------------------------------
Message::Message(
	const OBJECT_PARAMETER_DESC &parameter) :
	Sprite2D(parameter)
{
	timer_ = 120;
	alpha_ = 1.0f;
}


//-------------------------------------
// ~Message()
//-------------------------------------
Message::~Message()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Message::Update()
{
	static float left_border =
		parameter_.position_.x_ - parameter_.scaling_.x_;

	timer_--;
	timer_ = std::max<int>(timer_, 0);
	parameter_.position_.x_ -= 30.0f;
	parameter_.position_.x_ = std::max<float>(parameter_.position_.x_, left_border);
	if (!timer_){
		parameter_.position_.y_ -= 1.0f;
		alpha_ -= 0.01f;
	}
	CalculateVertex();
}


//-------------------------------------
// calculateVertex()
//-------------------------------------
void Message::CalculateVertex()
{
	if (!vertex_) return;

	vertex_[0].position_ = {
		parameter_.position_.x_ - parameter_.scaling_.x_ * 0.5f,
		parameter_.position_.y_ - parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[1].position_ = {
		parameter_.position_.x_ + parameter_.scaling_.x_ * 0.5f,
		parameter_.position_.y_ - parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[2].position_ = {
		parameter_.position_.x_ - parameter_.scaling_.x_ * 0.5f,
		parameter_.position_.y_ + parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[3].position_ = {
		parameter_.position_.x_ + parameter_.scaling_.x_ * 0.5f,
		parameter_.position_.y_ + parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[0].rhw_ = 1.0f;
	vertex_[1].rhw_ = 1.0f;
	vertex_[2].rhw_ = 1.0f;
	vertex_[3].rhw_ = 1.0f;

	vertex_[0].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha_);
	vertex_[1].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha_);
	vertex_[2].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha_);
	vertex_[3].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha_);

	vertex_[0].texture_ = { 0.0f, 0.0f };
	vertex_[1].texture_ = { 1.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 1.0f };
	vertex_[3].texture_ = { 1.0f, 1.0f };
}


//-------------------------------------
// end of file
//-------------------------------------
