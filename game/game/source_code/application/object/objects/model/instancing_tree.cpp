//=========================================================
// instancing_tree.cpp
// author:takeshi iwasawa
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
#include "instancing_tree.h"


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
	texture_ = TextureManager::GetTexture("resource/texture/game/tree.png");

	XContainer* container = XContainerManager::GetContainer("./resource/model/x/tree.x");

	LPD3DXMESH mesh = container->mesh_;
	mesh->GetVertexBuffer(&vertex_buffer_);	// ���_�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
	mesh->GetIndexBuffer(&index_buffer_);	// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
	count_vertex_ = mesh->GetNumVertices();	// ���_�����Q�b�g
	count_face_ = mesh->GetNumFaces();		// �ʐ����Q�b�g

	// �e�X�g�p�I�u�W�F�N�g��
	object_count_ = 50;
	DirectX9Holder::device_->CreateVertexBuffer(
		sizeof(D3DXVECTOR3) * object_count_,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&world_buffer_,
		NULL);

	// �e�X�g�p���W�ݒ�
	D3DXVECTOR3 *world;
	world_buffer_->Lock(0, 0, (void**)&world, 0);
	for(int i = 0; i < object_count_; i++)
	{
		world[i].x = i * 3.0f;
		world[i].y = 0.0f;
		world[i].z = 0.0f;
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
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	D3DXMATRIX world, view, projection;
	D3DXMatrixIdentity(&world);
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &projection);

	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_w", &world);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_v", &view);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_p", &projection);

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

	DirectX9Holder::device_->SetStreamSourceFreq(0, 1);
	DirectX9Holder::device_->SetStreamSourceFreq(1, 1);
	DirectX9Holder::device_->SetStreamSourceFreq(2, 1);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// end of file
//-------------------------------------