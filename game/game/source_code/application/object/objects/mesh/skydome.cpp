//=========================================================
// SkyDome.cpp
// author:fuka takahashi
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
#include "skydome.h"
#include "../../../resource/texture_manager.h"

//-------------------------------------
// warninig
//-------------------------------------
#pragma warning(disable:4996)


//-------------------------------------
// SkyDome()
//-------------------------------------
SkyDome::SkyDome(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	vertex_buffer_ = NULL;
	index_buffer_ = NULL;
	normal_buffer_ = nullptr;
	index_count_ = 0;
	mesh_radius_ = 0.0f;
	mesh_division_ = { 0, 0 };
	D3DXMatrixIdentity(&world_);
	shader_ = nullptr;
	shader_ = new Shader("resource/shader/no_lighting.hlsl");
	texture_ = NULL;
	mesh_diffuse_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}


//-------------------------------------
// ~SkyDome()
//-------------------------------------
SkyDome::~SkyDome()
{
	SAFE_RELEASE(vertex_buffer_);
	SAFE_RELEASE(index_buffer_);
	SAFE_DELETE_ARRAY(normal_buffer_);
	SAFE_DELETE(shader_);
	texture_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void SkyDome::Update()
{
	D3DXMATRIX translate, rotate, scaling;
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&world_);

	D3DXMatrixScaling(
		&scaling, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(
		&world_, &world_, &scaling);
	D3DXMatrixRotationYawPitchRoll(
		&rotate,
		parameter_.rotation_.y_,
		parameter_.rotation_.x_,
		parameter_.rotation_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &rotate);
	D3DXMatrixTranslation(
		&translate,
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &translate);
}


//-------------------------------------
// Draw()
//-------------------------------------
void SkyDome::Draw()
{
	//DirectX9Holder::device_->SetRenderState(
	//	D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	DirectX9Holder::device_->SetRenderState(D3DRS_LIGHTING, FALSE);
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

	DirectX9Holder::device_->SetTexture(0, texture_);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	DirectX9Holder::device_->SetStreamSource(
		0,
		vertex_buffer_,
		0,
		sizeof(Vertex3D));
	DirectX9Holder::device_->SetIndices(index_buffer_);
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_3d_);
	DirectX9Holder::device_->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		index_count_,
		0,
		index_count_ - 2);

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);


	DirectX9Holder::device_->SetRenderState(D3DRS_LIGHTING, TRUE);
	DirectX9Holder::device_->SetRenderState(
	D3DRS_FILLMODE, D3DFILL_SOLID);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void SkyDome::LoadMesh(
	const std::string &path)
{

	FILE *file = fopen(path.c_str(), "rt");
	int div_x(0), div_y(0), vertex_count(0);
	char texture_path[256];
	D3DXVECTOR3 *vertex_buffer;

	if (file == nullptr)
	{
		std::string warning;
		warning = path;
		warning += ": このファイルが見つかりません";
		ASSERT_WARNING(warning.c_str());
	}

	fscanf(file, "%d %d", &div_x, &div_y);
	fscanf(file, "%f", &mesh_radius_);
	fscanf(file, "%f %f %f %f", &mesh_diffuse_.r, &mesh_diffuse_.g, &mesh_diffuse_.b, &mesh_diffuse_.a);
	fscanf(file, "%s", &texture_path);

	// CalculateVertexNum
	vertex_count = (div_x + 1)*(div_y+1);


	vertex_buffer = new D3DXVECTOR3[vertex_count];
	fread(vertex_buffer, sizeof(D3DXVECTOR3), vertex_count, file);

	fclose(file);

	index_count_ = ((div_x + 1) * 2 + 2) * div_y - 2;
	mesh_division_ = {
		static_cast<float>(div_x),
		static_cast<float>(div_y)
	};

	if (FAILED(DirectX9Holder::device_->CreateVertexBuffer(
		sizeof(Vertex3D)* vertex_count,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&vertex_buffer_,
		NULL)))
	{
		ASSERT_ERROR("メッシュの頂点バッファ生成に失敗");
	}

	if (FAILED(DirectX9Holder::device_->CreateIndexBuffer(
		sizeof(WORD)* index_count_,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&index_buffer_,
		NULL)))
	{
		ASSERT_ERROR("メッシュのインデックスバッファ生成に失敗");
	}

	//LoadTexture
	texture_ = TextureManager::GetTexture(texture_path);

	normal_buffer_ = new D3DXVECTOR3[(div_x * 2) * div_y];

	CalculateVertex(vertex_buffer);
	CalculateIndex();
	SAFE_DELETE(vertex_buffer);
}


//-------------------------------------
// CalculateVertex()
//-------------------------------------
void SkyDome::CalculateVertex(
	D3DXVECTOR3 *source_buffer)
{
	D3DXVECTOR2 curcle_radius = D3DXVECTOR2(mesh_radius_, 0.0f);
	Vertex3D *vertex;
	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);
	float height = 0.0f;
	float angle = 0.0f;
	float lange = 0.5f;
	float height_sub = D3DX_PI * 0.5f / mesh_division_.y;
	float angle_sub = D3DX_PI * 2.0f / mesh_division_.x;
	float lange_sub = 0.5f / mesh_division_.y;

	for (int y = 0; y < static_cast<int>(mesh_division_.y + 1); y++, height += height_sub, lange -= lange_sub){
		angle = 0.0f;
		for (int x = 0; x < static_cast<int>(mesh_division_.x + 1); x++, angle += angle_sub){

			int num = static_cast<int>(y * (mesh_division_.x + 1) + x);
			
			vertex[num].position_ = {
				sinf(-angle) * mesh_radius_ * cosf(height),
				mesh_radius_ * sinf(height),
				cosf(-angle) * mesh_radius_ * cosf(height)
			};

			vertex[num].diffuse_ = mesh_diffuse_;
			vertex[num].normal_ = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			vertex[num].texture_ = {
				0.5f - sinf(angle) * lange,
				0.5f - cosf(angle) * lange };
		}
	}
	vertex_buffer_->Unlock();
}


//-------------------------------------
// CalculateIndex()
//-------------------------------------
void SkyDome::CalculateIndex()
{
	WORD *index;
	index_buffer_->Lock(0, 0, (void**)&index, 0);
	for (unsigned int i = 0, j = 0, k = 0; j < index_count_; j++)
	{
		if (j == 2 * (static_cast<int>(mesh_division_.x) + 1) + k * (2 * (static_cast<int>(mesh_division_.x) + 1) + 2))
		{
			index[j] = (i - 1) + k * (static_cast<int>(mesh_division_.x) + 1);
			index[j + 1] = i + (k + 1) * (static_cast<int>(mesh_division_.x) + 1);
			j += 2; k++; i = 0;
		}
		if (j % 2 == 0)
		{
			index[j] = i + (k + 1) * (static_cast<int>(mesh_division_.x) + 1);
		}
		else
		{
			index[j] = i + k * (static_cast<int>(mesh_division_.x) + 1);
			i++;
		}
	}
	index_buffer_->Unlock();
}


//-------------------------------------
// end of file
//-------------------------------------
