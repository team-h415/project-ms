//=========================================================
// x_model.cpp
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
#include "../../../resource/x_container.h"
#include "../../../resource/x_container_manager.h"
#include "../../../resource/texture_manager.h"
#include "x_model.h"


//-------------------------------------
// XModel()
//-------------------------------------
XModel::XModel(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	mesh_ = NULL;
	material_buffer_ = NULL;
	shader_ = nullptr;
	shader_ = new Shader("resource/shader/halflambert_lighting.hlsl");
	texture_ = TextureManager::GetTexture("resource/texture/red.png");
}


//-------------------------------------
// ~XModel()
//-------------------------------------
XModel::~XModel()
{
	SAFE_DELETE(shader_);
}


//-------------------------------------
// Update()
//-------------------------------------
void XModel::Update()
{
	D3DXMATRIX translate, rotate, scaling;
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&world_);

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
void XModel::Draw()
{
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

	D3DMATERIAL9 default_material;
	D3DXMATERIAL *material;
	DirectX9Holder::device_->GetMaterial(&default_material);
	material = (D3DXMATERIAL*)material_buffer_->GetBufferPointer();

	for (DWORD i = 0; i < material_count_; i++)
	{
		mesh_->DrawSubset(i);
	}

	DirectX9Holder::device_->SetMaterial(&default_material);


	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void XModel::LoadMesh(
	const std::string &path)
{
	XContainer* container = XContainerManager::GetContainer(path);
	mesh_ = container->mesh_;
	material_buffer_ = container->material_buffer_;
	material_count_ = container->material_count_;
}


//-------------------------------------
// end of file
//-------------------------------------
