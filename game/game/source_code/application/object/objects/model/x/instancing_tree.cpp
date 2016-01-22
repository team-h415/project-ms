//=========================================================
// instancing_tree.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../../common/common.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../math/vector.h"
#include "../../../../shader/shader.h"
#include "../../../../shader/shader_manager.h"
#include "../../../object.h"
#include "../../../../resource/x_container.h"
#include "../../../../resource/x_container_manager.h"
#include "../../../../resource/texture_manager.h"
#include "instancing_tree.h"


//-------------------------------------
// variable
//-------------------------------------
	// 0 - 25 / / -60 - -80
const D3DXVECTOR3 instance_position1[] = {
	{ -20.04f, 0.50f, 76.80f },
	{ -13.87f, 0.50f, 77.66f },
	{ -1.66f, 0.50f, 66.59f },
	{ 3.33f, 0.50f, 72.77f },
	{ 2.65f, 0.50f, 67.71f },
	{ -1.27f, 0.50f, 63.36f },
	{ -1.91f, 0.50f, 57.76f },
	{ 4.56f, 0.50f, 6.09f },
	{ 4.26f, 0.50f, -0.52f },
	{ 13.40f, 0.50f, 3.96f },
	{ 8.30f, 0.50f, -2.61f },
	{ 3.54f, 0.50f, -8.14f },
	{ 12.63f, 0.50f, -5.41f },
	{ 17.85f, 0.50f, -6.61f },
	{ 12.22f, 0.50f, -11.31f },
	{ 6.39f, 0.50f, -16.76f },
	{ 16.34f, 0.50f, 29.79f },
	{ 20.79f, 0.50f, 26.20f },
	{ 22.20f, 0.50f, 23.92f },
	{ 26.26f, 0.50f, 20.11f },
	{ -21.54f, 0.50f, -43.96f },
	{ -25.23f, 0.50f, -37.29f },
	{ -22.93f, 0.50f, -60.25f },
	{ -18.56f, 0.50f, -62.08f },
	{ -23.87f, 0.50f, -68.85f },
	{ -15.83f, 0.50f, -64.64f },
	{ -20.29f, 0.50f, -71.69f },
	{ -14.06f, 0.50f, -73.44f },
	
};
const D3DXVECTOR3 instance_position2[] = {
	{ -21.66f, 0.50f, 82.52f },
	{ -18.29f, 0.50f, 80.28f },
	{ -15.95f, 0.50f, 74.57f },
	{ -1.58f, 0.50f, 69.47f },
	{ 6.24f, 0.50f, 71.37f },
	{ 2.39f, 0.50f, 64.59f },
	{ -1.82f, 0.50f, 65.49f },
	{ -23.26f, 0.50f, 57.36f },
	{ 2.58f, 0.50f, 3.00f },
	{ 6.30f, 0.50f, 1.45f },
	{ 14.48f, 0.50f, 1.06f },
	{ 6.22f, 0.50f, -5.10f },
	{ 6.43f, 0.50f, -8.93f },
	{ 15.46f, 0.50f, -4.94f },
	{ 16.43f, 0.50f, -8.97f },
	{ 11.26f, 0.50f, -14.92f },
	{ 7.43f, 0.50f, -13.41f },
	{ 20.33f, 0.50f, 30.25f },
	{ 16.76f, 0.50f, 26.24f },
	{ 26.32f, 0.50f, 24.73f },
	{ 22.53f, 0.50f, 18.12f },
	{ -25.26f, 0.50f, -44.90f },
	{ -24.96f, 0.50f, -34.35f },
	{ -20.88f, 0.50f, -60.43f },
	{ -20.98f, 0.50f, -65.16f },
	{ -19.66f, 0.50f, -69.20f },
	{ -14.42f, 0.50f, -69.60f },
	{ -22.04f, 0.50f, -73.53f },
};

const D3DXVECTOR3 instance_position3[] = {
	// -0 - -30 / / 15 - 30
	{ -14.29f, 0.50f, 80.74f },
	{ -15.76f, 0.50f, 72.04f },
	{ -0.57f, 0.50f, 72.81f },
	{ 5.53f, 0.50f, 68.90f },
	{ 1.49f, 0.50f, 61.74f },
	{ -3.62f, 0.50f, 59.89f },
	{ -26.04f, 0.50f, 55.26f },
	{ 7.41f, 0.50f, 8.09f },
	{ 1.39f, 0.50f, -0.56f },
	{ 9.41f, 0.50f, 3.83f },
	{ 11.74f, 0.50f, -1.50f },
	{ 3.42f, 0.50f, -5.98f },
	{ 9.22f, 0.50f, -8.01f },
	{ 16.47f, 0.50f, -2.38f },
	{ 15.17f, 0.50f, -12.79f },
	{ 9.76f, 0.50f, -17.80f },
	{ 5.08f, 0.50f, -12.54f },
	{ 24.42f, 0.50f, 29.29f },
	{ 18.44f, 0.50f, 22.98f },
	{ 28.89f, 0.50f, 23.13f },
	{ -19.38f, 0.38f, -46.17f },
	{ -25.55f, 0.50f, -41.14f },
	{ -26.16f, 0.50f, -32.03f },
	{ -22.70f, 0.50f, -62.20f },
	{ -24.36f, 0.50f, -66.39f },
	{ -16.85f, 0.50f, -67.31f },
	{ -16.58f, 0.19f, -71.69f },
	{ -17.82f, 0.50f, -74.94f },
};

//-------------------------------------
// InstancingTree()
//-------------------------------------
InstancingTree::InstancingTree(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;

	count_vertex_ = 0;
	count_face_ = 0;
	object_count_ = 0;
	position_patern_ = 0;
	shader_ = ShaderManager::Get("resource/shader/instancing.hlsl");
	texture_ = NULL;
	container_ = nullptr;


	for (int i = 0; i < 3; i++)
	{
		// オブジェクト数
		object_count_ = 28;
		DirectX9Holder::device_->CreateVertexBuffer(
			sizeof(D3DXVECTOR3) * object_count_,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&world_buffer_[i],
			NULL);

		// 座標設定
		D3DXVECTOR3 *world;
		world_buffer_[i]->Lock(0, 0, (void**)&world, 0);
		for (int j = position_patern_ * object_count_; j < position_patern_ * object_count_ + object_count_; j++)
		{
			switch (i)
			{
			case 0:
				world[j] = instance_position1[j];
				break;
			case 1:
				world[j] = instance_position2[j];
				break;
			case 2:
				world[j] = instance_position3[j];
				break;
			}
		}

		world_buffer_[i]->Unlock();
	}
}



//-------------------------------------
// ~InstancingTree()
//-------------------------------------
InstancingTree::~InstancingTree()
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
void InstancingTree::Update()
{
}


//-------------------------------------
// Draw()
//-------------------------------------
void InstancingTree::Draw()
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
		DirectX9Holder::vertex_declaration_instancing_);

	DirectX9Holder::device_->SetStreamSourceFreq(
		0,
		D3DSTREAMSOURCE_INDEXEDDATA | object_count_);
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
			sizeof(D3DXVECTOR3));
		break;
	case 1:
		DirectX9Holder::device_->SetStreamSource(
			1,
			world_buffer_[1],
			0,
			sizeof(D3DXVECTOR3));
		break;
	case 2:
		DirectX9Holder::device_->SetStreamSource(
			1,
			world_buffer_[2],
			0,
			sizeof(D3DXVECTOR3));
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
void InstancingTree::SetTexture(
	const std::string &path)
{
	texture_ = TextureManager::GetTexture(path.c_str());
}

//-------------------------------------
// SetMesh()
//-------------------------------------
void InstancingTree::SetMesh(
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
void InstancingTree::SetPositionPatern(
	int patern){
	position_patern_ = patern;
}


//-------------------------------------
// end of file
//-------------------------------------
