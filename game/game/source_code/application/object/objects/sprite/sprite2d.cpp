//=========================================================
// sprite2d.cpp
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
#include "../../../resource/texture_manager.h"


//-------------------------------------
// Sprite2D()
//-------------------------------------
Sprite2D::Sprite2D(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	vertex_ = new Vertex2D[4];
	CalculateVertex();
	texture_ = NULL;
}


//-------------------------------------
// ~Sprite2D()
//-------------------------------------
Sprite2D::~Sprite2D()
{
	SAFE_DELETE_ARRAY(vertex_);
	texture_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void Sprite2D::Update()
{
	CalculateVertex();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Sprite2D::Draw()
{
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_2d_);

	DirectX9Holder::device_->SetTexture(0, texture_);

	DirectX9Holder::device_->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertex_,
		sizeof(Vertex2D));

	DirectX9Holder::device_->SetTexture(0, NULL);

	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// CreateVertex()
//-------------------------------------
void Sprite2D::CalculateVertex()
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
void Sprite2D::SetTexture(
	const std::string &path)
{
	texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// end of file
//-------------------------------------
