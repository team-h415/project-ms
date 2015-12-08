//=========================================================
// bullet.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../network/network.h"
#ifdef NETWORK_HOST_MODE
#include "../../../network/network_host.h"
#endif
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../object.h"
#include "../../object_manager.h"
#include "../model/fbx_model.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../mesh/field.h"
#include "../../../config/config.h"
#include "bullet.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/game_server.h"
#include "../../../scene/scenes/matching.h"


//-------------------------------------
// Bullet()
//-------------------------------------
Bullet::Bullet(
	const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	
	COLLISION_PARAMETER_DESC param;
	param.position_ = {
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_ };
	param.range_ = 0.5f;
	param.offset_ = { 0.0f, 0.0f, 0.0f };
	speed_ = {BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ};

	// 回転値を少し調整
	parameter_.rotation_.x_ += BULLET_OFFSET_ROT;
	// 回転値を参照して速度を改良
	speed_.y += sinf(parameter_.rotation_.x_) * BULLET_ADD_SPEED_Y;

#ifdef NETWORK_HOST_MODE
	Scene *scene = SceneManager::GetCurrentScene();
	GameServer *game_server = dynamic_cast<GameServer*>(scene);
	collision_ = game_server->collision_manager()->Create(this, param);
#else
	//Scene *scene = SceneManager::GetCurrentScene();
	//std::string str = SceneManager::GetCurrentSceneName();
	//if (str == "Game"){
	//	ASSERT_ERROR("弾が生成されるべきシーンではありません");
	//	return;
	//}
	//Game *game = dynamic_cast<Game*>(scene);
	//collision_ = game->collision_manager()->Create(this, param);
#endif
}


//-------------------------------------
// ~Bullet()
//-------------------------------------
Bullet::~Bullet()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Bullet::Update()
{
	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BULLET_GRAVITY;

#ifndef NETWORK_HOST_MODE
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
			this_delete_ = true;
			collision_->SetThisDelete(true);
		}
	}
#endif
}


//-------------------------------------
// Draw()
//-------------------------------------
void Bullet::Draw()
{

}


//-------------------------------------
// Action()
//-------------------------------------
void Bullet::Action(
	Object *target,
	const float range)
{
#ifdef NETWORK_HOST_MODE
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
			}
			// 子供
			else if (target->parameter().layer_ == LAYER_MODEL_CHILD){
				FbxChild *child = dynamic_cast<FbxChild*>(target);
				float life = child->GetLife();
				life -= CHILD_DAMAGE;
				child->SetLife(life);
			}
			// 砦(※子供に差し替えること!)
			else if (target->parameter().layer_ == LAYER_MODEL_FORT &&
				parameter_.parent_layer_ == LAYER_MODEL_GRANDFATHER){
				XFort *fort = dynamic_cast<XFort*>(target);
				float life = fort->GetLife();
				life -= FORT_DAMAGE;
				fort->SetLife(life);
			}

			// データ転送用構造体用意
			NETWORK_DATA send_data;
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = parameter_.position_.x_;
			send_data.object_param_.position_.y_ = parameter_.position_.y_;
			send_data.object_param_.position_.z_ = parameter_.position_.z_;
			send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
			send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
			send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;
			strcpy_s(send_data.name, MAX_NAME_LEN, "damage");

			// オブジェクトデータ転送
			NetworkHost::SendTo(DELI_MULTI, send_data);

			this_delete_ = true;
			collision_->SetThisDelete(true);

			//-------------------------------------
			// シーン取得
			//Scene *scene = SceneManager::GetCurrentScene();
			//std::string str = SceneManager::GetCurrentSceneName();
			//if (str == "Game"){
			//	Game *game = dynamic_cast<Game*>(scene);

			//	//-------------------------------------
			//	// シーンからエフェクト取得
			//	EFFECT_PARAMETER_DESC effect_param;
			//	MyEffect *effect = game->effect_manager()->Get("damage");
			//	effect_param = effect->parameter();
			//	effect_param.position_ = parameter_.position_;
			//	effect_param.position_.y_ += 0.5f;
			//	effect_param.rotation_ = parameter_.rotation_;
			//	effect->SetParameter(effect_param);

			//	//-------------------------------------
			//	// エフェクト再生
			//	game->effect_manager()->Play("damage");
			//}
			//this_delete_ = true;
			//collision_->SetThisDelete(true);
		}
	}
#else
	////-------------------------------------
	//// もしXモデルと当たったら
	//if (target->parameter().layer_ == LAYER_MODEL_X){
	//	//-------------------------------------
	//	// シーン取得
	//	Scene *scene = SceneManager::GetCurrentScene();
	//	std::string str = SceneManager::GetCurrentSceneName();
	//	if (str == "Game"){
	//		Game *game = dynamic_cast<Game*>(scene);
	//		//-------------------------------------
	//		// シーンからエフェクト取得
	//		EFFECT_PARAMETER_DESC effect_param;
	//		MyEffect *effect = game->effect_manager()->Get("damage");
	//		effect_param = effect->parameter();
	//		effect_param.position_ = parameter_.position_;
	//		effect_param.position_.y_ += 0.5f;
	//		effect_param.rotation_ = parameter_.rotation_;
	//		effect->SetParameter(effect_param);
	//		//-------------------------------------
	//		// エフェクト再生
	//		game->effect_manager()->Play("damage");
	//	}
	//	this_delete_ = true;
	//	collision_->SetThisDelete(true);
	//}
#endif
}


//-------------------------------------
// end of file
//-------------------------------------
