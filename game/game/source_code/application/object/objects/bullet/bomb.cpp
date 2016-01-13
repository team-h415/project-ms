//=========================================================
// bomb.cpp
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
#include "../../../resource/x_container.h"
#include "../../../resource/x_container_manager.h"
#include "../../../resource/texture_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../../../sound/sound.h"
#include "../../../config/config.h"
#include "../../../shader/shader.h"
#include "../../../shader/shader_manager.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/matching.h"
#include "../../object.h"
#include "../../object_manager.h"
#include "../model/fbx_model.h"
#include "../model/fbx/fbx_player.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../mesh/field.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "bomb.h"


//-------------------------------------
// Bomb()
//-------------------------------------
Bomb::Bomb(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// 弾実体生成
	parameter_ = parameter;
	mesh_ = NULL;
	material_buffer_ = NULL;
	shader_ = nullptr;
	shader_ = ShaderManager::Get("resource/shader/bullet.hlsl");
	texture_ = NULL;
	LoadMesh("resource/model/x/bomb.x");
	SetTexture("resource/texture/game/bomb.png");

	// 使用フラグOFF
	collision_ = nullptr;
	use_ = false;

	if (collision_ == nullptr)
	{
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if (str != "Game" && str != "Matching"){
			ASSERT_ERROR("弾が生成されるべきシーンではありません");
			return;
		}
		COLLISION_PARAMETER_DESC param;
		param.position_ = {
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_ };
		param.range_ = 3.0f;
		param.offset_ = { 0.0f, 0.0f, 0.0f };

		if (str == "Game"){
			Game *game = dynamic_cast<Game*>(scene);
			collision_ = game->collision_manager()->Create(this, param);
		}
		if (str == "Matching"){
			Matching *matching = dynamic_cast<Matching*>(scene);
			collision_ = matching->collision_manager()->Create(this, param);
		}
	}
	collision_->SetUse(false);
}


//-------------------------------------
// ~Bomb()
//-------------------------------------
Bomb::~Bomb()
{
	if (collision_){
		collision_->SetThisDelete(true);
	}
	shader_ = nullptr;
}


//-------------------------------------
// Fire()
//-------------------------------------
void Bomb::Fire(OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	frame_count_ = 0;

	speed_ = { BOMB_DEF_SPEED_XZ, BOMB_DEF_SPEED_Y, BOMB_DEF_SPEED_XZ };

	// 回転値を少し調整
	parameter_.rotation_.x_ += BOMB_OFFSET_ROT;
	// 回転値を参照して速度を改良
	speed_.y += sinf(parameter_.rotation_.x_) * BOMB_ADD_SPEED_Y;


	// 使用フラグOFF
	use_ = true;
	collision_->SetUse(false);
}


//-------------------------------------
// Update()
//-------------------------------------
void Bomb::Update()
{
	if (!use_)
	{
		return;
	}

	frame_count_++;
	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BOMB_GRAVITY;

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();

	// ゲームなら
	if (str == "Game"){
		Game *game = dynamic_cast<Game*>(scene);
		Object *obj = game->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));

		if (parameter_.position_.y_ < height + 0.35f){
			parameter_.position_.y_ = height + 0.35f;
			speed_ *= 0.9f;
		}

		if (frame_count_ == BOMB_TIMER){
			collision_->SetUse(true);
			
			//-------------------------------------
			// シーンからエフェクト取得
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = game->effect_manager()->Get("bombfire");
			effect_param = effect->parameter();
			effect_param.position_ = parameter_.position_;
			effect_param.position_.y_ = height;
			effect_param.rotation_ = { 0.0f, parameter_.rotation_.y_, 0.0f };
			effect->SetParameter(effect_param);

			//-------------------------------------
			// エフェクト再生
			game->effect_manager()->Play("bombfire");
		}
	}



	if (str == "Matching"){
		Matching *matching = dynamic_cast<Matching*>(scene);
		Object *obj = matching->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));
		if (parameter_.position_.y_ < height){
			parameter_.position_.y_ = height;
			speed_ *= 0.9f;
		}
		if (frame_count_ == BOMB_TIMER){
			collision_->SetUse(true);

			//-------------------------------------
			// シーンからエフェクト取得
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = matching->effect_manager()->Get("bombfire");
			effect_param = effect->parameter();
			effect_param.position_ = parameter_.position_;
			effect_param.position_.y_ = height;
			effect_param.rotation_ = { 0.0f, parameter_.rotation_.y_, 0.0f };
			effect->SetParameter(effect_param);

			//-------------------------------------
			// エフェクト再生
			matching->effect_manager()->Play("bombfire");
		}
	}

	if (frame_count_ > BOMB_TIMER){
		use_ = false;
		collision_->SetUse(false);
		parameter_.position_.y_ = 10000.0f;

	}

	// ワールド計算
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
// Action()
//-------------------------------------
void Bomb::Draw()
{
	if (!use_)
	{
		return;
	}

	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	DirectX9Holder::device_->SetTransform(D3DTS_WORLD, &world_);
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_x_);

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

	DirectX9Holder::device_->SetTexture(0, NULL);

	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void Bomb::LoadMesh(
	const std::string &path)
{
	XContainer* container = XContainerManager::GetContainer(path);
	mesh_ = container->mesh_;
	material_buffer_ = container->material_buffer_;
	material_count_ = container->material_count_;
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void Bomb::SetTexture(
	const std::string &path)
{
	//LoadTexture
	texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// Action()
//-------------------------------------
void Bomb::Action(
	Object *target,
	const float range)
{
	if (!use_)
	{
		return;
	}

	//-------------------------------------
	// もしXモデルと当たったら
	if (target->parameter().layer_ == LAYER_MODEL_FORT ||
		target->parameter().layer_ == LAYER_MODEL_GRANDFATHER ||
		target->parameter().layer_ == LAYER_MODEL_CHILD){

		//-------------------------------------
		// 自分の親のレイヤーを確認
		if (parameter_.parent_layer_ != target->parameter().layer_){

			//-------------------------------------
			// 当たった対象にパラメータ反映
			// おじ
			if (target->parameter().layer_ == LAYER_MODEL_GRANDFATHER){
				FbxGrandfather *father = dynamic_cast<FbxGrandfather*>(target);
				float life = father->GetLife();
				life -= GRANDFATHER_DAMAGE;
				father->SetLife(life);
				father->SetRecoverWaitTimer(0);
			}
			// 子供
			else if (target->parameter().layer_ == LAYER_MODEL_CHILD){
				FbxChild *child = dynamic_cast<FbxChild*>(target);
				float life = child->GetLife();
				life -= CHILD_DAMAGE;
				child->SetLife(life);
				child->SetRecoverWaitTimer(0);
			}
			// 砦(※子供に差し替えること!)
			else if (target->parameter().layer_ == LAYER_MODEL_FORT &&
				parameter_.parent_layer_ == LAYER_MODEL_GRANDFATHER){
				XFort *fort = dynamic_cast<XFort*>(target);
				float life = fort->GetLife();
				float damage = FORT_DAMAGE;
				//-------------------------------------
				// シーン取得
				Scene *scene = SceneManager::GetCurrentScene();
				std::string str = SceneManager::GetCurrentSceneName();
				if (str == "Game"){
					Game *game = dynamic_cast<Game*>(scene);
					// シールド張ってたらその分減衰する
					if (game->shield_flg() == true)
						damage *= SHIELD_DAMAGE_ATTENUATION;

					// ステージ移行中はダメージ無効
					if (game->change_stage_flg() == false){
						life -= damage;
						fort->SetLife(life);
					}
				}
			}

			//-------------------------------------
			// シーン取得
			Scene *scene = SceneManager::GetCurrentScene();
			std::string str = SceneManager::GetCurrentSceneName();
			if (str == "Game"){
				Game *game = dynamic_cast<Game*>(scene);

				//-------------------------------------
				// シーンからエフェクト取得
				EFFECT_PARAMETER_DESC effect_param;
				MyEffect *effect = game->effect_manager()->Get("damage");
				effect_param = effect->parameter();
				effect_param.position_ = parameter_.position_;
				effect_param.position_.y_ += 0.5f;
				effect_param.rotation_ = parameter_.rotation_;
				effect->SetParameter(effect_param);

				//-------------------------------------
				// エフェクト再生
				game->effect_manager()->Play("damage");
				//-------------------------------------
				// 水がはじけるSE再生
				Sound::LoadAndPlaySE("resource/sound/se/game/waterBreak.wav");
			}
			use_ = false;
			collision_->SetUse(false);
		}
	}
}

//-------------------------------------
// end of file
//-------------------------------------
