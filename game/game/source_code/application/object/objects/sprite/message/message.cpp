//=========================================================
// message.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../../common/common.h"
#include "../../../../math/vector.h"
#include "../../../../config/config.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../object.h"
#include "../sprite2d.h"
#include "message.h"
#include "../../../../resource/texture_manager.h"


//-------------------------------------
// Message()
//-------------------------------------
Message::Message(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	vertex_ = new Vertex2D[4];
	CalculateVertex();
	frame_texture_ = TextureManager::GetTexture(
		"resource/texture/game/message/message_frame.png");
	play_ = false;
	frame_count_ = 0;
	dest_position_ = parameter_.position_.y_;
}


//-------------------------------------
// ~Message()
//-------------------------------------
Message::~Message()
{
	SAFE_DELETE_ARRAY(vertex_);
	frame_texture_ = NULL;
	message_texture_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void Message::Update()
{
	if (play_){
		frame_count_++;
		if (frame_count_ > 120){
			play_ = false;;
		}
		float height = frame_count_ * (100.0f / 30.0f);
		height = std::min<float>(height, 100.0f);
		parameter_.position_.y_ +=
			(dest_position_ - parameter_.position_.y_) * 0.1f;

		vertex_[0].position_.y = parameter_.position_.y_ - height * 0.5f;
		vertex_[1].position_.y = parameter_.position_.y_ - height * 0.5f;
		vertex_[2].position_.y = parameter_.position_.y_ + height * 0.5f;
		vertex_[3].position_.y = parameter_.position_.y_ + height * 0.5f;
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Message::Draw()
{
	if (play_){
		DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
		DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		DirectX9Holder::device_->SetVertexDeclaration(
			DirectX9Holder::vertex_declaration_2d_);

		DirectX9Holder::device_->SetTexture(0, frame_texture_);

		DirectX9Holder::device_->DrawPrimitiveUP(
			D3DPT_TRIANGLESTRIP,
			2,
			vertex_,
			sizeof(Vertex2D));

		DirectX9Holder::device_->SetTexture(0, message_texture_);

		DirectX9Holder::device_->DrawPrimitiveUP(
			D3DPT_TRIANGLESTRIP,
			2,
			vertex_,
			sizeof(Vertex2D));

		DirectX9Holder::device_->SetTexture(0, NULL);

		DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}


//-------------------------------------
// calculateVertex()
//-------------------------------------
void Message::CalculateVertex()
{
	vertex_[0].position_ = {
		0.0f,
		parameter_.position_.y_,
		0.0f
	};

	vertex_[1].position_ = {
		SCREEN_WIDTH,
		parameter_.position_.y_,
		0.0f
	};

	vertex_[2].position_ = {
		0.0f,
		parameter_.position_.y_,
		0.0f
	};

	vertex_[3].position_ = {
		SCREEN_WIDTH,
		parameter_.position_.y_,
		0.0f
	};

	vertex_[0].rhw_ = 1.0f;
	vertex_[1].rhw_ = 1.0f;
	vertex_[2].rhw_ = 1.0f;
	vertex_[3].rhw_ = 1.0f;

	vertex_[0].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[1].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[2].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[3].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vertex_[0].texture_ = { 0.0f, 0.0f };
	vertex_[1].texture_ = { 1.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 1.0f };
	vertex_[3].texture_ = { 1.0f, 1.0f };
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void Message::SetTexture(
	const std::string &path)
{
	message_texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// Play()
//-------------------------------------
void Message::Play()
{
	play_ = true;
	frame_count_ = 0;
	parameter_.position_.y_ = MESSAGE_DISP_POSITION_Y;
	dest_position_ = parameter_.position_.y_;
}


//-------------------------------------
// end of file
//-------------------------------------
