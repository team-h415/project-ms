//=========================================================
// blind.cpp
// author:fuka takahashi
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../common/common.h"
#include "../../../config/config.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../object.h"
#include "../sprite/blind.h"
#include "blind.h"
#include "../../../resource/texture_manager.h"



//-------------------------------------
// Blind()
//-------------------------------------
Blind::Blind(
	const OBJECT_PARAMETER_DESC &parameter)
{
	scaling_max_ = 1.0f;
	parameter_ = parameter;
	vertex_ = new Vertex2D[4];
	color_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	CalculateVertex();
	texture_ = NULL;
	use_ = false;
}


//-------------------------------------
// ~Blind()
//-------------------------------------
Blind::~Blind()
{
	SAFE_DELETE_ARRAY(vertex_);
	texture_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void Blind::Update()
{
	if (use_ == false)
		return;

	this->AttenuateAlpha();
	this->MagnifyScaling();
	this->CalculateVertex();
	
}


//-------------------------------------
// Draw()
//-------------------------------------
void Blind::Draw()
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
void Blind::CalculateVertex()
{
	if (!vertex_) return;

	vertex_[0].position_ = {
		-parameter_.scaling_.x_ * 0.5f,
		-parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[1].position_ = {
		parameter_.scaling_.x_ * 0.5f,
		-parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[2].position_ = {
		-parameter_.scaling_.x_ * 0.5f,
		parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[3].position_ = {
		parameter_.scaling_.x_ * 0.5f,
		parameter_.scaling_.y_ * 0.5f,
		0.0f
	};

	vertex_[0].rhw_ = 1.0f;
	vertex_[1].rhw_ = 1.0f;
	vertex_[2].rhw_ = 1.0f;
	vertex_[3].rhw_ = 1.0f;

	vertex_[0].diffuse_ = color_;
	vertex_[1].diffuse_ = color_;
	vertex_[2].diffuse_ = color_;
	vertex_[3].diffuse_ = color_;

	vertex_[0].texture_ = { 0.0f, 0.0f };
	vertex_[1].texture_ = { 1.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 1.0f };
	vertex_[3].texture_ = { 1.0f, 1.0f };


	D3DXMATRIX mtxTransration;			// 回転・移動後のマトリクス
	D3DXMATRIX posMatrix, rotMatrix;
	D3DXMatrixIdentity(&mtxTransration); // 単位行列生成
	D3DXMatrixIdentity(&rotMatrix);

	// 回転角を格納
	D3DXMatrixRotationYawPitchRoll(&rotMatrix, 0.0f, 0.0f, parameter_.rotation_.z_);

	// 現在の移動座標を格納
	D3DXMatrixTranslation(&posMatrix, parameter_.position_.x_, parameter_.position_.y_, 0.0f);

	// 回転
	D3DXMatrixMultiply(&mtxTransration, &mtxTransration, &rotMatrix);
	// 移動
	D3DXMatrixMultiply(&mtxTransration, &mtxTransration, &posMatrix);


	// 移動と回転をかける
	for (int i = 0; i < 4; i++)
	{
		D3DXMATRIX mtxTransPos;			// 回転・移動後のマトリクス
		D3DXMatrixTranslation(&mtxTransPos, vertex_[i].position_.x, vertex_[i].position_.y, 0.0f);
		// 回転
		D3DXMatrixMultiply(&mtxTransPos, &mtxTransPos, &rotMatrix);
		// 移動
		D3DXMatrixMultiply(&mtxTransPos, &mtxTransPos, &posMatrix);

		// 適応後の座標をしまう
		vertex_[i].position_.x = mtxTransPos._41;
		vertex_[i].position_.y = mtxTransPos._42;
	}
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void Blind::SetTexture(
	const std::string &path)
{
	texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// AttenuateAlpha()
//-------------------------------------
void Blind::AttenuateAlpha(void)
{
	color_.a -= BLIND_ALPHA_ATTENUATION_SPEED;

	if (color_.a < 0.08f){
		color_.a = 0.0f;
		use_ = false;
	}

}

//-------------------------------------
// MagnifyScaling()
//-------------------------------------
void Blind::MagnifyScaling(void)
{
	float scaling = (scaling_max_ - parameter_.scaling_.x_)*BLIND_DEST_SCALING_SPEED;
	parameter_.scaling_.x_ += scaling;
	parameter_.scaling_.y_ += scaling;
}

//-------------------------------------
// SetBlind()
//-------------------------------------
void Blind::SetBlind(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;

	scaling_max_ = parameter.scaling_.x_;

	parameter_.scaling_.x_ *= 0.5f;
	parameter_.scaling_.y_ *= 0.5f;

	color_ = { 1.0f, 1.0, 1.0f, 1.0f };
	CalculateVertex();


	// 使用フラグON
	use_ = true;
}
//-------------------------------------
// end of file
//-------------------------------------
