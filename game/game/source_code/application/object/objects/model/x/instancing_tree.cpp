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
#include "../../../object.h"
#include "../../../../resource/x_container.h"
#include "../../../../resource/x_container_manager.h"
#include "../../../../resource/texture_manager.h"
#include "instancing_tree.h"


//-------------------------------------
// variable
//-------------------------------------
const D3DXVECTOR3 instance_position[] = {

	{ 27.35f, 0.50f, -43.90f },
	{ 22.75f, 0.50f, -46.71f },
	{ 22.29f, 0.50f, -39.76f },
	{ 19.05f, 0.50f, -35.65f },
	{ 12.47f, 0.50f, -34.16f },
	{ 9.93f, 0.50f, -38.03f },
	{ 10.02f, 0.50f, -44.29f },
	{ 4.65f, 0.50f, -41.70f },
	{ 2.29f, 0.50f, -36.69f },
	{ -0.55f, 0.50f, -31.73f },
	{ -5.62f, 0.40f, -29.58f },
	{ -6.80f, 0.41f, -34.07f },
	{ -9.25f, 0.50f, -40.01f },
	{ -13.82f, 0.50f, -43.15f },
	{ -17.31f, 0.50f, -39.56f },
	{ -23.46f, 0.50f, -35.93f },
	{ -28.18f, 0.50f, -38.06f },
	{ -28.69f, 0.65f, -44.05f },
	{ -35.03f, 0.50f, -47.16f },
	{ 49.68f, 0.50f, -0.31f },
	{ 48.44f, 0.50f, 7.15f },
	{ 42.71f, 0.50f, 9.60f },
	{ 45.15f, 0.50f, 15.03f },
	{ 50.16f, 0.50f, 18.03f },
	{ 44.90f, 0.50f, 24.27f },
	{ 45.28f, 0.50f, 30.63f },
	{ 44.22f, 0.50f, 36.80f },
	{ 42.37f, 0.50f, 42.29f },
	{ 39.56f, 0.50f, 49.04f },
	{ 34.34f, 0.50f, 51.50f },
	{ 32.61f, 0.50f, 47.47f },
	{ 35.05f, 0.50f, 42.46f },
	{ 36.71f, 0.50f, 35.71f },
	{ 37.18f, 0.50f, 28.70f },
	{ 35.68f, 0.50f, 20.43f },
	{ 28.90f, 0.50f, 23.21f },
	{ 27.57f, 0.50f, 29.68f },
	{ 24.61f, 0.50f, 34.76f },
	{ 23.06f, 0.50f, 40.95f },
	{ 18.52f, 0.50f, 45.85f },
	{ 12.81f, 0.50f, 44.40f },
	{ 9.46f, 0.50f, 47.87f },
	{ 2.89f, 0.50f, 44.80f },
	{ -2.13f, 0.50f, 47.12f },
	{ -6.46f, 0.50f, 44.92f },
	{ -9.46f, 0.50f, 49.56f },
	{ -13.01f, 0.50f, 52.37f },
	{ -16.09f, 0.50f, 56.50f },
	{ -18.72f, 0.50f, 59.97f },
	{ -19.05f, 0.50f, 54.71f },
	{ -19.66f, 0.50f, 49.37f },
	{ -21.40f, 0.50f, 43.79f },
	{ -22.49f, 0.50f, 37.71f },
	{ 30.05f, 0.50f, 38.50f },
	{ -34.33f, 0.50f, 59.57f },
	{ -39.63f, 0.50f, 56.83f },
	{ -33.70f, 0.50f, 53.89f },
	{ -31.12f, 0.50f, 57.90f },
	{ -40.80f, 0.50f, 51.57f },
	{ -45.58f, 0.50f, 51.00f },
	{ -47.94f, 0.50f, 48.07f },
	{ -50.53f, 0.50f, 43.56f },
	{ -49.42f, 0.50f, 39.76f },
	{ -46.83f, 0.50f, 35.76f },
	{ -44.61f, 0.50f, 32.32f },
	{ -42.72f, 0.50f, 29.11f },
	{ -40.50f, 0.50f, 25.68f },
	{ -38.88f, 0.50f, 22.19f },
	{ -35.88f, 0.50f, 23.62f },
	{ -35.73f, 0.50f, 26.83f },
	{ -36.25f, 0.50f, 29.68f },
	{ -37.10f, 0.50f, 33.32f },
	{ -37.87f, 0.50f, 36.94f },
	{ -36.80f, 0.50f, 44.05f },
	{ -39.30f, 0.47f, 45.63f },
	{ -42.51f, 0.50f, 43.52f },
	{ -44.08f, 0.50f, 40.24f },
	{ -35.72f, 0.50f, 50.10f },
	{ -37.15f, 0.50f, 51.40f },
	{ -34.71f, 0.50f, 53.80f },
	{ -32.43f, 0.50f, 58.19f },
	{ -39.04f, 0.50f, 21.63f },
	{ -35.85f, 0.50f, 24.99f },
	{ -33.82f, 0.50f, 28.99f },
	{ -37.93f, 0.50f, 27.30f },
	{ -41.80f, 0.50f, 26.15f },
	{ -45.11f, 0.50f, 29.17f },
	{ -40.81f, 0.50f, 30.17f },
	{ -37.18f, 0.50f, 30.81f },
	{ -34.72f, 0.50f, 33.41f },
	{ -38.54f, 0.50f, 35.65f },
	{ -42.89f, 0.50f, 36.48f },
	{ -46.63f, 0.50f, 35.49f },
	{ -49.62f, 0.50f, 36.90f },
	{ -50.24f, 0.50f, 40.03f },
	{ -50.47f, 0.50f, 44.40f },
	{ -47.41f, 0.48f, 43.22f },
	{ -44.78f, 0.50f, 42.56f },
	{ -41.54f, 0.50f, 42.20f },
	{ -37.80f, 0.50f, 43.24f },
	{ -36.34f, 0.50f, 46.39f },
	{ -35.68f, 0.50f, 49.67f },
	{ -39.59f, 0.50f, 51.14f },
	{ -42.64f, 0.40f, 50.25f },
	{ -45.53f, 0.50f, 51.68f },
	{ -42.88f, 0.50f, 54.46f },
	{ -40.06f, 0.50f, 57.47f },
	{ -38.16f, 0.50f, 59.27f },
	{ -35.45f, 0.50f, 59.74f },
	{ -32.10f, 0.50f, 58.62f },
	{ -33.42f, 0.50f, 55.45f },
	{ -36.97f, 0.50f, 56.13f },
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
	shader_ = new Shader("resource/shader/instancing.hlsl");
	texture_ = TextureManager::GetTexture("resource/texture/game/tree01.png");

	XContainer* container = XContainerManager::GetContainer("./resource/model/x/tree03.x");

	LPD3DXMESH mesh = container->mesh_;
	mesh->GetVertexBuffer(&vertex_buffer_);	// 頂点バッファオブジェクトへのポインタをゲット
	mesh->GetIndexBuffer(&index_buffer_);	// インデックスバッファオブジェクトへのポインタをゲット
	count_vertex_ = mesh->GetNumVertices();	// 頂点数をゲット
	count_face_ = mesh->GetNumFaces();		// 面数をゲット

	// テスト用オブジェクト数
	object_count_ = sizeof(instance_position) / sizeof(D3DXVECTOR3);
	DirectX9Holder::device_->CreateVertexBuffer(
		sizeof(D3DXVECTOR3) * object_count_,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&world_buffer_,
		NULL);

	// テスト用座標設定
	D3DXVECTOR3 *world;
	world_buffer_->Lock(0, 0, (void**)&world, 0);
	for(int i = 0; i < object_count_; i++)
	{
		world[i] = instance_position[i];
	}
	world_buffer_->Unlock();
}


//-------------------------------------
// ~InstancingTree()
//-------------------------------------
InstancingTree::~InstancingTree()
{
	SAFE_RELEASE(world_buffer_);
	SAFE_RELEASE(index_buffer_);
	SAFE_RELEASE(vertex_buffer_);
	SAFE_DELETE(shader_);
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

	DirectX9Holder::device_->SetStreamSource(
		1,
		world_buffer_,
		0,
		sizeof(D3DXVECTOR3));

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
// end of file
//-------------------------------------
