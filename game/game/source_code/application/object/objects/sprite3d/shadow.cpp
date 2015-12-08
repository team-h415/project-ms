//=========================================================
// shadow.cpp
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
#include "../../object_manager.h"
#include "shadow.h"
#include "../../../resource/texture_manager.h"


//-------------------------------------
// Shadow()
//-------------------------------------
Shadow::Shadow(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;

	shader_ = new Shader("resource/shader/shadow.hlsl");
	texture_ = TextureManager::GetTexture("resource/texture/shadow.jpg");

	if(FAILED(DirectX9Holder::device_->CreateVertexBuffer(
		sizeof(Vertex3DShadow) * 4,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&vertex_buffer_,
		NULL)))
	{
		ASSERT_ERROR("メッシュの頂点バッファ生成に失敗");
	}

	Vertex3DShadow set_vertex[4];

	set_vertex[0].position_ = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	set_vertex[1].position_ = D3DXVECTOR3(0.5f, 0.0f, 0.5f);
	set_vertex[2].position_ = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	set_vertex[3].position_ = D3DXVECTOR3(0.5f, 0.0f, -0.5f);

	set_vertex[0].texture_ = D3DXVECTOR2(0.0f, 0.0f);
	set_vertex[1].texture_ = D3DXVECTOR2(1.0f, 0.0f);
	set_vertex[2].texture_ = D3DXVECTOR2(0.0f, 1.0f);
	set_vertex[3].texture_ = D3DXVECTOR2(1.0f, 1.0f);

	Vertex3DShadow *vertex;
	vertex_buffer_->Lock(0, 0, (void**)&vertex, 0);
	memcpy(vertex, set_vertex, sizeof(set_vertex));
	vertex_buffer_->Unlock();
}


//-------------------------------------
// ~Shadow()
//-------------------------------------
Shadow::~Shadow()
{
	SAFE_RELEASE(vertex_buffer_);
	SAFE_DELETE(shader_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Shadow::Update()
{
}


//-------------------------------------
// Draw()
//-------------------------------------
void Shadow::Draw()
{
	LPDIRECT3DDEVICE9 device = DirectX9Holder::device_;

	D3DXMATRIX world, scaling;
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scaling, parameter_.scaling_.x_, 1.0f, parameter_.scaling_.z_);
	world *= scaling;
	world._41 += parameter_.position_.x_;
	world._42 += parameter_.position_.y_;
	world._43 += parameter_.position_.z_;

	D3DXMATRIX view, projection, wvp;
	device->GetTransform(D3DTS_VIEW, &view);
	device->GetTransform(D3DTS_PROJECTION, &projection);
	wvp = world * view * projection;

	shader_->vertex_table()->SetMatrix(
		device, "matrix_wvp", &wvp);
	device->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_0"), texture_);

	device->SetVertexShader(shader_->vertex_shader());
	device->SetPixelShader(shader_->pixel_shader());

	device->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_shadow_);

	device->SetStreamSource(
		0,
		vertex_buffer_,
		0,
		sizeof(Vertex3DShadow));

	// レンダーステート保存
	DWORD blendop, srcblend, destblend;//, zfunc;
	device->GetRenderState(D3DRS_BLENDOP, &blendop);
	device->GetRenderState(D3DRS_SRCBLEND, &srcblend);
	device->GetRenderState(D3DRS_DESTBLEND, &destblend);
	//device->GetRenderState(D3DRS_ZFUNC, &zfunc);

	//影用にレンダーステートを減算合成に変更する
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zバッファの設定を完全描画にする。
	//device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// 描画
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 変更したレンダースレート修正
	//device->SetRenderState(D3DRS_ZFUNC, zfunc);
	device->SetRenderState(D3DRS_BLENDOP, blendop);
	device->SetRenderState(D3DRS_SRCBLEND, srcblend);
	device->SetRenderState(D3DRS_DESTBLEND, destblend);

	device->SetVertexShader(NULL);
	device->SetPixelShader(NULL);
}


//-------------------------------------
// end of file
//-------------------------------------
