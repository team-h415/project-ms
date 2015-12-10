//=========================================================
// fade.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "fade.h"


//-------------------------------------
// variable
//-------------------------------------
FADE_PARAMETER Fade::param_ = FADE_NONE;
FADE_PARAMETER Fade::request_ = FADE_NONE;
Vertex2D Fade::vertex_[4];


//-------------------------------------
// Fade()
//-------------------------------------
Fade::Fade()
{
	alpha_ = 1.0f;

	vertex_[0].position_ = { 0.0f, 0.0f, 0.0f };
	vertex_[1].position_ = { SCREEN_WIDTH, 0.0f, 0.0f };
	vertex_[2].position_ = { 0.0f, SCREEN_HEIGHT, 0.0f };
	vertex_[3].position_ = { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f };

	vertex_[0].rhw_ = 1.0f;
	vertex_[1].rhw_ = 1.0f;
	vertex_[2].rhw_ = 1.0f;
	vertex_[3].rhw_ = 1.0f;

	vertex_[0].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[1].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[2].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[3].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);

	vertex_[0].texture_ = { 0.0f, 0.0f };
	vertex_[1].texture_ = { 1.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 1.0f };
	vertex_[3].texture_ = { 1.0f, 1.0f };
}


//-------------------------------------
// ~Fade()
//-------------------------------------
Fade::~Fade()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Fade::Update()
{
	// リクエストに沿って値を変更
	switch (request_)
	{
	case FADE_IN:
		alpha_ += 1.0f / 60.0f;
		alpha_ = min(alpha_, 1.0f);
		break;

	case FADE_OUT:
		alpha_ -= 1.0f / 60.0f;
		alpha_ = max(alpha_, 0.0f);
		break;
	}


	// 値を設定
	vertex_[0].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[1].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[2].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);
	vertex_[3].diffuse_ = D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha_);


	// 現在の状態を更新
	if (alpha_ == 1.0f)
	{
		param_ = FADE_IN;
	}
	else if (alpha_ == 0.0f)
	{
		param_ = FADE_OUT;
	}
	else
	{
		param_ = FADE_NONE;
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Fade::Draw()
{
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_2d_);

	DirectX9Holder::device_->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertex_,
		sizeof(Vertex2D));
}


//-------------------------------------
// end of file
//-------------------------------------
