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
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/matching.h"
#include "../../../math/vector.h"
#include "../../../shader/shader.h"
#include "../../../shader/shader_manager.h"
#include "../../object.h"
#include "../../object_manager.h"
#include "lake.h"
#include "../../../resource/texture_manager.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"


//-------------------------------------
// constant
//-------------------------------------
const D3DXVECTOR4 collision_offset_and_range[] = {
	// xyz : position_offset / w : range
	{ 28.50f, 0.00f, 60.50f, 15.00f },
	{ 14.05f, 0.00f, 53.23f, 6.00f },
	{ 8.68f, 0.00f, 46.68f, 6.00f },
	{ -0.28f, 0.00f, 39.76f, 8.0f },
	{ -8.30f, 0.00f, 43.22f, 4.0f },
	{ -23.02f, 0.00f, 43.58f, 6.0f },
	{ -28.75f, 0.00f, 41.81f, 6.0f },
	{ -35.10f, 0.00f, 35.90f, 6.0f },
	{ -21.09f, 0.00f, -4.91f, 12.0f },
	{ -25.09f, 0.00f, -8.91f, 15.0f },
	{ -16.63f, 0.00f, -22.21f, 6.0f },
	{ -12.09f, 0.00f, -27.80f, 6.0f },
	{ -7.35f, 0.00f, -33.55f, 6.0f },
	{ -5.56f, 0.00f, -40.29f, 6.0f },
	{ 5.60f, 0.00f, -48.09f, 6.0f },
	{ 12.34f, 0.00f, -50.73f, 6.0f },
	{ 19.73f, 0.00f, -53.21f, 6.0f },
	{ 27.11f, 0.00f, -55.66f, 6.0f },
	{ 34.00f, 0.00f, -58.24f, 6.0f },
};


//-------------------------------------
// Lake()
//-------------------------------------
Lake::Lake(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	shader_ = ShaderManager::Get("resource/shader/halflambert_lighting.hlsl");
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
	vertex_[1].texture_ = { 50.0f, 0.0f };
	vertex_[2].texture_ = { 0.0f, 50.0f };
	vertex_[3].texture_ = { 50.0f, 50.0f };

	
	//-------------------------------------
	// マッチングかゲームの時のみ、あたり判定を生成
	//-------------------------------------

	// シーンを判定
	std::string scene_name = SceneManager::GetCurrentSceneName();
	if (scene_name != "Game" &&	scene_name != "Matching") return;

	// シーン取得
	Scene *scene = SceneManager::GetCurrentScene();
	
	// 変数宣言
	COLLISION_PARAMETER_DESC collision_param;
	collision_param.position_ = { 0.0f, 0.0f, 0.0f };

	// 定数バッファの数だけあたり判定を生成
	int collision_count = sizeof(collision_offset_and_range) / sizeof(D3DXVECTOR4);
	for (int i = 0; i < collision_count; i++){
		collision_param.offset_ = {
			collision_offset_and_range[i].x,
			collision_offset_and_range[i].y,
			collision_offset_and_range[i].z };
		collision_param.range_ = collision_offset_and_range[i].w;

		// シーンで分岐
		if (scene_name == "Game"){
			Game *game = dynamic_cast<Game*>(scene);
			game->collision_manager()->Create(
				this, collision_param);
		}
		else{
			Matching *matching = dynamic_cast<Matching*>(scene);
			matching->collision_manager()->Create(
				this, collision_param);
		}
	}
}


//-------------------------------------
// ~Lake()
//-------------------------------------
Lake::~Lake()
{
	shader_ = nullptr;
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
// Action()
//-------------------------------------
void Lake::Action(
	Object *target,
	const float range,
	D3DXVECTOR3 my_position)
{


}

//-------------------------------------
// end of file
//-------------------------------------
