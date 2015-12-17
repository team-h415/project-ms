//=========================================================
// bullet.cpp
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
#include "../../object.h"
#include "../../object_manager.h"
#include "../model/fbx_model.h"
#include "../model/fbx/fbx_player.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../../../sound/sound.h"
#include "../mesh/field.h"
#include "../../../config/config.h"
#include "../model/x_model.h"
#include "bullet.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/matching.h"


//-------------------------------------
// Bullet()
//-------------------------------------
Bullet::Bullet(
	const OBJECT_PARAMETER_DESC &parameter) :
	XModel(parameter)
{
	// 弾実体生成
	LoadMesh("resource/model/x/ball.x");
	SetTexture("resource/texture/game/bullet.png");

	// 使用フラグOFF
	collision_ = nullptr;
	use_ = false;
}


//-------------------------------------
// ~Bullet()
//-------------------------------------
Bullet::~Bullet()
{
	if (collision_){
		collision_->SetThisDelete(true);
	}
}


//-------------------------------------
// Fire()
//-------------------------------------
void Bullet::Fire(OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;

	speed_ = {BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ};

	// 回転値を少し調整
	parameter_.rotation_.x_ += BULLET_OFFSET_ROT;
	// 回転値を参照して速度を改良
	speed_.y += sinf(parameter_.rotation_.x_) * BULLET_ADD_SPEED_Y;

	if(collision_ == nullptr)
	{
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if(str != "Game" && str != "Matching"){
			ASSERT_ERROR("弾が生成されるべきシーンではありません");
			return;
		}
		COLLISION_PARAMETER_DESC param;
		param.position_ = {
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_};
		param.range_ = 0.5f;
		param.offset_ = {0.0f, 0.0f, 0.0f};

		if (str == "Game"){
			Game *game = dynamic_cast<Game*>(scene);
			collision_ = game->collision_manager()->Create(this, param);
		}
		if (str == "Matching"){
			Matching *matching = dynamic_cast<Matching*>(scene);
			collision_ = matching->collision_manager()->Create(this, param);
		}
	}

	// 使用フラグOFF
	use_ = true;
	collision_->SetUse(true);
}


//-------------------------------------
// Update()
//-------------------------------------
void Bullet::Update()
{
	if(!use_)
	{
		return;
	}

	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BULLET_GRAVITY;

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();
	if (str == "Game"){
		Game *game = dynamic_cast<Game*>(scene);
		Object *obj = game->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));
		if (parameter_.position_.y_ < height){
			use_ = false;
			collision_->SetUse(false);
			parameter_.position_.y_ = 10000.0f;
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
			use_ = false;
			collision_->SetUse(false);
			parameter_.position_.y_ = 10000.0f;
		}
	}

	XModel::Update();
}


//-------------------------------------
// Action()
//-------------------------------------
void Bullet::Draw()
{
	if(!use_)
	{
		return;
	}
	XModel::Draw();
}


//-------------------------------------
// Action()
//-------------------------------------
void Bullet::Action(
	Object *target,
	const float range)
{
	if(!use_)
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
				life -= FORT_DAMAGE;
				fort->SetLife(life);
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
