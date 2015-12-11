//=========================================================
// collision.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../math/vector.h"
#include "../object/object.h"
#include "collision.h"


//-------------------------------------
// Collision()
//-------------------------------------
Collision::Collision(
	Object *parent,
	const COLLISION_PARAMETER_DESC &parameter)
{
	parent_ = parent;
	parameter_ = parameter;
	D3DXMatrixIdentity(&world_);
	CalculateVertex();
	this_delete_ = false;
}


//-------------------------------------
// ~Collision()
//-------------------------------------
Collision::~Collision()
{
	parent_ = nullptr;
}


//-------------------------------------
// Update()
//-------------------------------------
void Collision::Update()
{
	Vector3 v = parent_->parameter().position_;
	parameter_.position_.x = v.x_;
	parameter_.position_.y = v.y_;
	parameter_.position_.z = v.z_;
	parameter_.position_ += parameter_.offset_;
	D3DXMatrixIdentity(&world_);
	D3DXMATRIX translate;
	D3DXMatrixTranslation(
		&translate,
		parameter_.position_.x,
		parameter_.position_.y,
		parameter_.position_.z);
	D3DXMatrixMultiply(&world_, &world_, &translate);
}


//-------------------------------------
// Draw()
//-------------------------------------
void Collision::Draw()
{
	DirectX9Holder::device_->SetRenderState(D3DRS_LIGHTING, FALSE);
	DirectX9Holder::device_->SetTransform(D3DTS_WORLD, &world_);
	
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_3d_);

	DirectX9Holder::device_->DrawPrimitiveUP(D3DPT_LINESTRIP, 10, vertex_[0], sizeof(Vertex3D));
	DirectX9Holder::device_->DrawPrimitiveUP(D3DPT_LINESTRIP, 10, vertex_[1], sizeof(Vertex3D));
	DirectX9Holder::device_->DrawPrimitiveUP(D3DPT_LINESTRIP, 10, vertex_[2], sizeof(Vertex3D));

	DirectX9Holder::device_->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//-------------------------------------
// CalculateVertex()
//-------------------------------------
void Collision::CalculateVertex()
{
	float arc = 0.0f;
	for (int i = 0; i < 11; i++, arc += D3DX_PI * 2.0f * 0.1f){
		vertex_[0][i].position_ = { 
			sinf(arc) * parameter_.range_,
			cosf(arc) * parameter_.range_,
			0.0f };
		vertex_[0][i].diffuse_ = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		vertex_[0][i].normal_ = { 0.0f, 1.0f, 0.0f };
		vertex_[0][i].texture_ = { 0.0f, 0.0f };
	}
	arc = 0.0f;

	for (int i = 0; i < 11; i++, arc += D3DX_PI * 2.0f * 0.1f){
		vertex_[1][i].position_ = { 
			0.0f,
			cosf(arc) * parameter_.range_,
			sinf(arc) * parameter_.range_ };
		vertex_[1][i].diffuse_ = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		vertex_[1][i].normal_ = { 0.0f, 1.0f, 0.0f };
		vertex_[1][i].texture_ = { 0.0f, 0.0f };
	}
	arc = 0.0f;

	for (int i = 0; i < 11; i++, arc += D3DX_PI * 2.0f * 0.1f){
		vertex_[2][i].position_ = { 
			sinf(arc) * parameter_.range_,
			0.0f,
			cosf(arc) * parameter_.range_ };
		vertex_[2][i].diffuse_ = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		vertex_[2][i].normal_ = { 0.0f, 1.0f, 0.0f };
		vertex_[2][i].texture_ = { 0.0f, 0.0f };
	}
}


//-------------------------------------
// end of file
//-------------------------------------
