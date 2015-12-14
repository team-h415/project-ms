//=========================================================
// lake.cpp
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
#include "../../../shader/shader.h"
#include "../../object.h"
#include "../../object_manager.h"
#include "lake.h"
#include "../../../resource/texture_manager.h"


//-------------------------------------
// Lake()
//-------------------------------------
Lake::Lake(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	shader_ = new Shader("resource/shader/halflambert_lighting.hlsl");
	texture_ = TextureManager::GetTexture("resource/texture/game/lake_upper.jpg");

	vertex_[0].position_ = { -1.0f, 0.0f, 1.0f };
	vertex_[1].position_ = { 1.0f, 0.0f, 1.0f };
	vertex_[2].position_ = { -1.0f, 0.0f, -1.0f };
	vertex_[3].position_ = { 1.0f, 0.0f, -1.0f };

	vertex_[0].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[1].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[2].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex_[3].diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vertex_[0].normal_ = { 0.0f, 1.0f, 0.0f };
	vertex_[1].normal_ = { 0.0f, 1.0f, 0.0f };
	vertex_[2].normal_ = { 0.0f, 1.0f, 0.0f };
	vertex_[3].normal_ = { 0.0f, 1.0f, 0.0f };

	vertex_[0].texture_ = { 0.0f, 0.0f };
	vertex_[1].texture_ = { 5.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 5.0f };
	vertex_[3].texture_ = { 5.0f, 5.0f };
}


//-------------------------------------
// ~Lake()
//-------------------------------------
Lake::~Lake()
{
	SAFE_DELETE(shader_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Lake::Update()
{
	const D3DXVECTOR2 scroll_speed = { 0.0005f, 0.001f };
	D3DXMATRIX translate, rotate, scaling;
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&world_);

	vertex_[0].texture_ += scroll_speed;
	vertex_[1].texture_ += scroll_speed;
	vertex_[2].texture_ += scroll_speed;
	vertex_[3].texture_ += scroll_speed;

	D3DXMatrixScaling(
		&scaling, parameter_.scaling_.x_, parameter_.scaling_.y_, parameter_.scaling_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &scaling);
	D3DXMatrixRotationYawPitchRoll(
		&rotate, parameter_.rotation_.y_, parameter_.rotation_.x_, parameter_.rotation_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &rotate);
	D3DXMatrixTranslation(
		&translate, parameter_.position_.x_, parameter_.position_.y_, parameter_.position_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &translate);
}


//-------------------------------------
// Draw()
//-------------------------------------
void Lake::Draw()
{
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	DirectX9Holder::device_->SetTransform(D3DTS_WORLD, &world_);
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_3d_);

	D3DXMATRIX view, projection, wvp;
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &projection);
	wvp = world_ * view * projection;

	D3DXVECTOR3 light_vec(0.5f, -0.5f, 0.5f);
	D3DXVec3Normalize(&light_vec, &light_vec);
	D3DXCOLOR light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);

	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_wvp", &wvp);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_w", &world_);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_direction",
		reinterpret_cast<float*>(&light_vec),
		3);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_diffuse",
		reinterpret_cast<float*>(&light_diffuse),
		4);

	DirectX9Holder::device_->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_0"), texture_);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	DirectX9Holder::device_->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP, 2, vertex_, sizeof(Vertex3D));

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// end of file
//-------------------------------------
