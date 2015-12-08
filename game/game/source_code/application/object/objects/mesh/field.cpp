//=========================================================
// field.cpp
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
#include "field.h"
#include "../../../resource/texture_manager.h"


//-------------------------------------
// warninig
//-------------------------------------
#pragma warning(disable:4996)


//-------------------------------------
// Field()
//-------------------------------------
Field::Field(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	vertex_buffer_ = NULL;
	index_buffer_ = NULL;
	normal_buffer_ = nullptr;
	index_count_ = 0;
	mesh_division_ = { 0, 0 };
	D3DXMatrixIdentity(&world_);
	shader_ = nullptr;
	shader_ = new Shader("resource/shader/field.hlsl");

	texture_[0] = TextureManager::GetTexture("resource/texture/game/field_1.png");
	texture_[1] = TextureManager::GetTexture("resource/texture/game/field_2.png");
	texture_[2] = TextureManager::GetTexture("resource/texture/game/field_3.png");
	texture_[3] = TextureManager::GetTexture("resource/texture/game/field_4.png");
}


//-------------------------------------
// ~Field()
//-------------------------------------
Field::~Field()
{
	SAFE_RELEASE(vertex_buffer_);
	SAFE_RELEASE(index_buffer_);
	SAFE_DELETE_ARRAY(normal_buffer_);
	SAFE_DELETE(shader_);
	for (int i = 0; i < 4; i++){
		texture_[i] = NULL;
	}
}


//-------------------------------------
// Update()
//-------------------------------------
void Field::Update()
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
void Field::Draw()
{
//	DirectX9Holder::device_->SetRenderState(
//		D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_3d_);

	D3DXMATRIX view,projection,wvp;
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
		shader_->pixel_table()->GetSamplerIndex("texture_0"), texture_[0]);
	DirectX9Holder::device_->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_1"), texture_[1]);
	DirectX9Holder::device_->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_2"), texture_[2]);
	DirectX9Holder::device_->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_3"), texture_[3]);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	DirectX9Holder::device_->SetStreamSource(
		0,
		vertex_buffer_,
		0,
		sizeof(Vertex3DField));
	DirectX9Holder::device_->SetIndices(index_buffer_);
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_field_);
	DirectX9Holder::device_->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		index_count_,
		0,
		index_count_ - 2);

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);


//	DirectX9Holder::device_->SetRenderState(
//		D3DRS_FILLMODE, D3DFILL_SOLID);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void Field::LoadMesh(
	const std::string &path)
{
	FILE *file = fopen(path.c_str(), "rb");

	if (file == nullptr)
	{
		std::string warning;
		warning = path;
		warning += ": ファイルの読み込みに失敗しました";
		ASSERT_ERROR(warning.c_str());
	}


	int div_x(0), div_z(0), vertex_count(0);
	D3DXVECTOR3 *vertex_buffer;
	D3DXVECTOR4 *texture_alpha;
	D3DXVECTOR2 scale_(0.0f, 0.0f);
	fread(&div_x, sizeof(int), 1, file);
	fread(&div_z, sizeof(int), 1, file);
	fread(&scale_, sizeof(D3DXVECTOR2), 1, file);
	fread(&vertex_count, sizeof(unsigned int), 1, file);

	vertex_buffer = new D3DXVECTOR3[vertex_count];
	texture_alpha = new D3DXVECTOR4[vertex_count];
	fread(vertex_buffer, sizeof(D3DXVECTOR3), vertex_count, file);
	fread(texture_alpha, sizeof(D3DXVECTOR4), vertex_count, file);

	fclose(file);

	index_count_ = ((div_x + 1) * 2 + 2) * div_z - 2;
	mesh_division_ = {
		static_cast<float>(div_x),
		static_cast<float>(div_z)
	};

	if (FAILED(DirectX9Holder::device_->CreateVertexBuffer(
		sizeof(Vertex3DField)* vertex_count,
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

	normal_buffer_ = new D3DXVECTOR3[(div_x * 2) * div_z];

	CalculateVertex(vertex_buffer, texture_alpha);
	CalculateNormal();
	CalculateIndex();
	SAFE_DELETE_ARRAY(vertex_buffer);
	SAFE_DELETE_ARRAY(texture_alpha);
}


//-------------------------------------
// CalculateVertex()
//-------------------------------------
void Field::CalculateVertex(
	D3DXVECTOR3 *source_buffer,
	D3DXVECTOR4 *texture_alpha)
{
	Vertex3DField *vertex;
	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);
	for (int z = 0; z < static_cast<int>(mesh_division_.y + 1); z++)
	{
		for (int x = 0; x < static_cast<int>(mesh_division_.x + 1); x++)
		{
			int num = static_cast<int>(z * (mesh_division_.x + 1) + x);
			vertex[num].position_ = {
				(-parameter_.scaling_.x_ / 2.0f) + (x * (parameter_.scaling_.x_ / mesh_division_.x)),
				source_buffer[num].y * parameter_.scaling_.y_,
				(parameter_.scaling_.z_ / 2.0f) - (z * (parameter_.scaling_.z_ / mesh_division_.y))
			};
			vertex[num].color_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[num].texture_ = {
				static_cast<float>(x) / 5.0f,
				static_cast<float>(z) / 5.0f
			};
			vertex[num].texturealpha_ = texture_alpha[num];
		}
	}
	vertex_buffer_->Unlock();
}


//-------------------------------------
// CalculateIndex()
//-------------------------------------
void Field::CalculateIndex()
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
// CalculateNormal()
//-------------------------------------
void Field::CalculateNormal()
{
	CalculatePanelNormal();
	CalculateVertexNormal();
}


//-------------------------------------
// CalculatePanelNormal()
//-------------------------------------
void Field::CalculatePanelNormal()
{
	Vertex3DField *vertex;
	D3DXVECTOR3 *normal = normal_buffer_;

	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);

	for (int z = 0; z < static_cast<int>(mesh_division_.y); z++)
	{
		for (int x = 0; x < static_cast<int>(mesh_division_.x); x++)
		{
			D3DXVECTOR3 vec1, vec2, vec_cross;
			int num1, num2;

			num1 = static_cast<int>(z * (mesh_division_.x + 1) + x);
			num2 = static_cast<int>((z + 1) * (mesh_division_.x + 1) + x);
			vec1 = vertex[num1].position_ - vertex[num2].position_;
			num1 = static_cast<int>((z + 1) * (mesh_division_.x + 1) + x + 1);
			num2 = static_cast<int>((z + 1) * (mesh_division_.x + 1) + x);
			vec2 = vertex[num1].position_ - vertex[num2].position_;
			D3DXVec3Cross(&vec_cross, &vec1, &vec2);
			D3DXVec3Normalize(&vec_cross, &vec_cross);
			*normal = vec_cross;
			normal++;

			num1 = static_cast<int>(z * (mesh_division_.x + 1) + x);
			num2 = static_cast<int>(z * (mesh_division_.x + 1) + x + 1);
			vec1 = vertex[num1].position_ - vertex[num2].position_;
			num1 = static_cast<int>((z + 1) * (mesh_division_.x + 1) + x + 1);
			num2 = static_cast<int>(z * (mesh_division_.x + 1) + x + 1);
			vec2 = vertex[num1].position_ - vertex[num2].position_;
			D3DXVec3Cross(&vec_cross, &vec2, &vec1);
			D3DXVec3Normalize(&vec_cross, &vec_cross);
			*normal = vec_cross;
			normal++;
		}
	}

	vertex_buffer_->Unlock();
}


//-------------------------------------
// CalculateVertexNormal()
//-------------------------------------
void Field::CalculateVertexNormal()
{
	Vertex3DField *vertex;
	D3DXVECTOR3 *normal = normal_buffer_;

	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);

	for (int z = 0, num = 0; z < static_cast<int>(mesh_division_.y + 1); z++)
	{
		for (int x = 0; x < static_cast<int>(mesh_division_.x + 1); x++, num++)
		{
			if (z == 0)
			{
				if (x == 0)
				{
					int num1, num2;
					num1 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2));
					num2 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) + 1);
					vertex[num].normal_ = (normal[num1] + normal[num2]) / 2.0f;
				}
				else if (x == static_cast<int>(mesh_division_.x))
				{
					int num1 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) + 1);
					vertex[num].normal_ = normal[num1];
				}
				else
				{
					int num1, num2, num3;
					num1 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) - 1);
					num2 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2));
					num3 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) + 1);
					vertex[num].normal_ = (normal[num1] + normal[num2] + normal[num3]) / 3.0f;
				}
			}
			else if (z == static_cast<int>(mesh_division_.y))
			{
				if (x == 0)
				{
					int num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2));
					vertex[num].normal_ = normal[num1];
				}
				else if (x == static_cast<int>(mesh_division_.x))
				{
					int num1, num2;
					num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 2);
					num2 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 1);
					vertex[num].normal_ = (normal[num1] + normal[num2]) / 2.0f;
				}
				else
				{
					int num1, num2, num3;
					num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 2);
					num2 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 1);
					num3 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2));
					vertex[num].normal_ = (normal[num1] + normal[num2] + normal[num3]) / 3.0f;
				}
			}
			else
			{
				if (x == 0)
				{
					int num1, num2, num3;
					num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2));
					num2 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2));
					num3 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) + 1);
					vertex[num].normal_ = (normal[num1] + normal[num2] + normal[num3]) / 3.0f;

				}
				else if (x == static_cast<int>(mesh_division_.x))
				{
					int num1, num2, num3;
					num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 2);
					num2 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 1);
					num3 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) - 1);
					vertex[num].normal_ = (normal[num1] + normal[num2] + normal[num3]) / 3.0f;

				}
				else
				{
					int num1, num2, num3, num4, num5, num6;
					num1 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 2);
					num2 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2) - 1);
					num3 = static_cast<int>((z - 1) * (mesh_division_.x * 2) + (x * 2));
					num4 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) - 1);
					num5 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2));
					num6 = static_cast<int>(z * (mesh_division_.x * 2) + (x * 2) + 1);
					vertex[num].normal_ = (normal[num1] + normal[num2] + normal[num3] + normal[num4] + normal[num5] + normal[num6]) / 6.0f;
				}
			}
			D3DXVec3Normalize(&vertex[num].normal_, &vertex[num].normal_);
		}
	}

	vertex_buffer_->Unlock();
}


//-------------------------------------
// GetHeight()
//-------------------------------------
float Field::GetHeight(const D3DXVECTOR3 &position)
{
	D3DXVECTOR3 pos1, pos2, pos3;
	D3DXVECTOR3 vec1, vec2, vec3;
	D3DXVECTOR3 vec_p1, vec_p2, vec_p3;
	float height = 0.0f;
	Vertex3DField *vertex;

	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);
	for (int z = 0; z < static_cast<int>(mesh_division_.y); z++)
	{
		for (int x = 0; x < static_cast<int>(mesh_division_.x); x++)
		{
			int num1, num2, num3;
			num1 = (z + 1) * static_cast<int>(mesh_division_.x + 1) + x + 1;
			num2 = (z + 1) * static_cast<int>(mesh_division_.x + 1) + x;
			num3 = z * static_cast<int>(mesh_division_.x + 1) + x;

			if (position.x > vertex[num1].position_.x) continue;
			if (position.z < vertex[num1].position_.z) continue;
			if (position.x < vertex[num3].position_.x) continue;
			if (position.z > vertex[num3].position_.z) continue;

			pos1 = vertex[num1].position_;
			pos2 = vertex[num2].position_;
			pos3 = vertex[num3].position_;

			vec1 = pos2 - pos1;
			vec2 = pos3 - pos2;
			vec3 = pos1 - pos3;

			vec_p1 = position - pos1;
			vec_p2 = position - pos2;
			vec_p3 = position - pos3;

			if (isStand(">", vec1, vec2, vec3, vec_p1, vec_p2, vec_p3))
			{
				int num = z * static_cast<int>(mesh_division_.x * 2) + (x * 2);
				D3DXVECTOR3 nor = normal_buffer_[num];
				height = GetHeightValue(pos1, position, nor);
				vertex_buffer_->Unlock();
				return height;
			}


			num1 = z * static_cast<int>(mesh_division_.x + 1) + x;
			num2 = z * static_cast<int>(mesh_division_.x + 1) + x + 1;
			num3 = (z + 1) * static_cast<int>(mesh_division_.x + 1) + x + 1;

			pos1 = vertex[num1].position_;
			pos2 = vertex[num2].position_;
			pos3 = vertex[num3].position_;

			vec1 = pos2 - pos1;
			vec2 = pos3 - pos2;
			vec3 = pos1 - pos3;

			vec_p1 = position - pos1;
			vec_p2 = position - pos2;
			vec_p3 = position - pos3;

			if (isStand(">=", vec1, vec2, vec3, vec_p1, vec_p2, vec_p3))
			{
				int num = z * static_cast<int>(mesh_division_.x * 2) + (x * 2) + 1;
				D3DXVECTOR3 nor = normal_buffer_[num];
				height = GetHeightValue(pos1, position, nor);
				vertex_buffer_->Unlock();
				return height;
			}
		}
	}
	vertex_buffer_->Unlock();
	return 0.0f;
}


//-------------------------------------
// isStand()
//-------------------------------------
bool Field::isStand(
	const std::string &type,
	const D3DXVECTOR3 &vec1,
	const D3DXVECTOR3 &vec2,
	const D3DXVECTOR3 &vec3,
	const D3DXVECTOR3 &vec_p1,
	const D3DXVECTOR3 &vec_p2,
	const D3DXVECTOR3 &vec_p3)
{
	if (type == ">"){
		if ((vec1.x * vec_p1.z - vec1.z * vec_p1.x) > 0) return false;
		if ((vec2.x * vec_p2.z - vec2.z * vec_p2.x) > 0) return false;
		if ((vec3.x * vec_p3.z - vec3.z * vec_p3.x) > 0) return false;
	}
	if (type == ">="){
		if ((vec1.x * vec_p1.z - vec1.z * vec_p1.x) >= 0) return false;
		if ((vec2.x * vec_p2.z - vec2.z * vec_p2.x) >= 0) return false;
		if ((vec3.x * vec_p3.z - vec3.z * vec_p3.x) >= 0) return false;
	}
	return true;
}


//-------------------------------------
// GetHeightValue()
//-------------------------------------
float Field::GetHeightValue(
	const D3DXVECTOR3 &position,
	const D3DXVECTOR3 &player_position,
	const D3DXVECTOR3 &normal)
{
	float height = 0.0f;
	height =
		position.y - (
		(player_position.x - position.x) * normal.x +
		(player_position.z - position.z) * normal.z) /
		normal.y;
	return height;
}


//-------------------------------------
// end of file
//-------------------------------------
