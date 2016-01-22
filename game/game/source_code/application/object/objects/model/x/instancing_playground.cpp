//=========================================================
// instancing_playground.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../network/network.h"
#include "../../../../../common/common.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../scene/scene.h"
#include "../../../../scene/scene_manager.h"
#include "../../../../scene/scenes/game.h"
#include "../../../../scene/scenes/matching.h"
#include "../../../../math/vector.h"
#include "../../../../shader/shader.h"
#include "../../../../shader/shader_manager.h"
#include "../../../object.h"
#include "../../../../resource/x_container.h"
#include "../../../../resource/x_container_manager.h"
#include "../../../../resource/texture_manager.h"
#include "instancing_playground.h"
#include "../../../../collision/collision.h"
#include "../../../../collision/collision_manager.h"


//-------------------------------------
// variable
//-------------------------------------
// 1 = 滑り台
const D3DXVECTOR3 instance_position1[] = {
	{ -11.72f, 0.10f, 22.18f },
};

const D3DXVECTOR3 instance_rotation1[] = {
	{ 0.00f, 2.25f, 0.00f },
};

// 2 = ブランコ
const D3DXVECTOR3 instance_position2[] = {
	{ -23.00f, 0.10f, 20.00f },
	{ 4.05f, 0.10f, -66.11f },

};

const D3DXVECTOR3 instance_rotation2[] = {
	{ 0.00f, 2.25f, 0.00f },
	{ 0.00f, 0.30f, 0.00f },
};

// 3 = タイヤ
const D3DXVECTOR3 instance_position3[] = {
	{ 10.35f, 0.10f, -68.65f },
	{ 11.28f, 0.10f, -69.13f },
	{ 12.42f, 0.10f, -69.71f },
	{ 13.35f, 0.10f, -70.19f },
	{ 14.50f, 0.10f, -70.78f },
	{ 15.63f, 0.10f, -71.36f },
	{ 16.69f, 0.10f, -71.91f },
	{ 17.92f, 0.10f, -72.53f },
	{ 19.12f, 0.10f, -73.15f },
	{ 20.44f, 0.10f, -73.83f },
	{ 21.64f, 0.10f, -74.44f },

};

const D3DXVECTOR3 instance_rotation3[] = {
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
	{ 0.00f, 2.04f, 0.00f },
};


//-------------------------------------
// InstancingPlayground()
//-------------------------------------
InstancingPlayground::InstancingPlayground(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;

	count_vertex_ = 0;
	count_face_ = 0;
	object_count_[0] = sizeof(instance_position1) / sizeof(D3DXVECTOR3);
	object_count_[1] = sizeof(instance_position2) / sizeof(D3DXVECTOR3);
	object_count_[2] = sizeof(instance_position3) / sizeof(D3DXVECTOR3);
	position_patern_ = 0;
	shader_ = ShaderManager::Get("resource/shader/instancing_bench.hlsl");
	texture_ = NULL;
	container_ = nullptr;

	for (int i = 0; i < 3; i++)
	{
		// テスト用オブジェクト数
		DirectX9Holder::device_->CreateVertexBuffer(
			sizeof(D3DXMATRIX) * object_count_[i],
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&world_buffer_[i],
			NULL);

		// テスト用座標設定
		D3DXMATRIX *world;
		world_buffer_[i]->Lock(0, 0, (void**)&world, 0);
		for (int j = position_patern_ * object_count_[i], k = 0; j < position_patern_ * object_count_[i] + object_count_[i]; j++, k++)
		{
			D3DXMATRIX trans, rotate, scaling;
			D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 scl(parameter_.scaling_.x_, parameter_.scaling_.y_, parameter_.scaling_.z_);
			D3DXMatrixIdentity(&world[k]);
			D3DXMatrixIdentity(&trans);
			D3DXMatrixIdentity(&rotate);
			D3DXMatrixIdentity(&scaling);
			switch (i)
			{
			case 0:
				pos = instance_position1[k];
				rot = instance_rotation1[k];
				break;
			case 1:
				pos = instance_position2[k];
				rot = instance_rotation2[k];
				break;
			case 2:
				pos = instance_position3[k];
				rot = instance_rotation3[k];
				break;
			}
			D3DXMatrixRotationYawPitchRoll(
				&rotate, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(
				&world[k], &world[k], &rotate);
			D3DXMatrixScaling(
				&scaling, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(
				&world[k], &scaling, &scaling);
			D3DXMatrixTranslation(
				&trans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(
				&world[k], &world[k], &trans);
		}

		world_buffer_[i]->Unlock();
	}

	// 変数宣言
	COLLISION_PARAMETER_DESC collision_param;
	collision_param.position_ = { 0.0f, 0.0f, 0.0f };

	// 定数バッファの数だけあたり判定を生成
	int collision_count = object_count_[0];
	for (int i = 0; i < collision_count; i++)
	{
		collision_param.offset_ = instance_position1[i];
		collision_param.range_ = 0.5f;

#ifdef NETWORK_HOST_MODE
		// 当たり判定作成
		CollisionManager::Get()->Create(this, collision_param);
#endif
	}

	collision_count = object_count_[1];
	for (int i = 0; i < collision_count; i++)
	{
		collision_param.offset_ = instance_position2[i];
		collision_param.range_ = 1.5f;
#ifdef NETWORK_HOST_MODE
		// 当たり判定作成
		CollisionManager::Get()->Create(this, collision_param);
#endif
	}

	collision_count = object_count_[2];
	for(int i = 0; i < collision_count; i++)
	{
		collision_param.offset_ = instance_position3[i];
		collision_param.range_ = 0.2f;
#ifdef NETWORK_HOST_MODE
		// 当たり判定作成
		CollisionManager::Get()->Create(this, collision_param);
#endif
	}
}



//-------------------------------------
// ~InstancingPlayground()
//-------------------------------------
InstancingPlayground::~InstancingPlayground()
{
	SAFE_RELEASE(world_buffer_[0]);
	SAFE_RELEASE(world_buffer_[1]);
	SAFE_RELEASE(world_buffer_[2]);
	SAFE_RELEASE(index_buffer_);
	SAFE_RELEASE(vertex_buffer_);
	container_ = nullptr;
	shader_ = nullptr;
}


//-------------------------------------
// Update()
//-------------------------------------
void InstancingPlayground::Update()
{
}


//-------------------------------------
// Draw()
//-------------------------------------
void InstancingPlayground::Draw()
{
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x66);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	D3DXMATRIX world, view, projection;
	D3DXMatrixIdentity(&world);
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &projection);


	D3DXVECTOR3 light_vec(0.5f, -0.5f, 0.5f);
	D3DXVec3Normalize(&light_vec, &light_vec);
	D3DXCOLOR light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);

	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_w", &world);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_v", &view);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_p", &projection);

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

	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_instancing_bench_);

	DirectX9Holder::device_->SetStreamSourceFreq(
		0,
		D3DSTREAMSOURCE_INDEXEDDATA | object_count_[position_patern_]);
	DirectX9Holder::device_->SetStreamSourceFreq(
		1,
		D3DSTREAMSOURCE_INSTANCEDATA | 1);

	DirectX9Holder::device_->SetStreamSource(
		0,
		vertex_buffer_,
		0,
		sizeof(VertexInstancing3D));

	switch (position_patern_)
	{
	case 0:
		DirectX9Holder::device_->SetStreamSource(
			1,
			world_buffer_[0],
			0,
			sizeof(D3DXMATRIX));
		break;
	case 1:
		DirectX9Holder::device_->SetStreamSource(
			1,
			world_buffer_[1],
			0,
			sizeof(D3DXMATRIX));
		break;
	case 2:
		DirectX9Holder::device_->SetStreamSource(
			1,
			world_buffer_[2],
			0,
			sizeof(D3DXMATRIX));
		break;
	}

	DirectX9Holder::device_->SetIndices(index_buffer_);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	DirectX9Holder::device_->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		count_vertex_,
		0,
		count_face_);

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);

	DirectX9Holder::device_->SetTexture(0, NULL);

	DirectX9Holder::device_->SetStreamSourceFreq(0, 1);
	DirectX9Holder::device_->SetStreamSourceFreq(1, 1);
	DirectX9Holder::device_->SetStreamSourceFreq(2, 1);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void InstancingPlayground::SetTexture(
	const std::string &path)
{
	texture_ = TextureManager::GetTexture(path.c_str());
}

//-------------------------------------
// SetMesh()
//-------------------------------------
void InstancingPlayground::SetMesh(
	const std::string &path)
{
	container_ = XContainerManager::GetContainer(path.c_str());

	LPD3DXMESH mesh = container_->mesh_;
	mesh->GetVertexBuffer(&vertex_buffer_);	// 頂点バッファオブジェクトへのポインタをゲット
	mesh->GetIndexBuffer(&index_buffer_);	// インデックスバッファオブジェクトへのポインタをゲット
	count_vertex_ = mesh->GetNumVertices();	// 頂点数をゲット
	count_face_ = mesh->GetNumFaces();		// 面数をゲット
}


//-------------------------------------
// SetPositionPatern()
//-------------------------------------
void InstancingPlayground::SetPositionPatern(
	int patern){
	position_patern_ = patern;
}


//-------------------------------------
// Action()
//-------------------------------------
void InstancingPlayground::Action(
	Object *target,
	const float range,
	D3DXVECTOR3 my_position)
{
	//-------------------------------------
	// Xモデルと当たったら
	if (target->parameter().layer_ == LAYER_MODEL_GRANDFATHER ||
		target->parameter().layer_ == LAYER_MODEL_CHILD){
		Vector3 my_pos(my_position.x, my_position.y, my_position.z);
		Vector3 vec = target->parameter().position_ - my_pos;
		Vector3 v = vec;
		Vec3Normalize(vec, vec);
		float distance = sqrtf(
			(v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_));
		float sub = range - distance;
		vec *= sub;
		target->SetPosition(
			target->parameter().position_ + vec);
	}
}


//-------------------------------------
// end of file
//-------------------------------------
