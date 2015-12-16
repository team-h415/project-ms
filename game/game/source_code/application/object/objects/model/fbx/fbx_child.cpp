//=========================================================
// fbx_child.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../config/config.h"
#include "../../../../../common/common.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../input/input.h"
#include "../../../../input/inputs/gamepad.h"
#include "../../../../math/vector.h"
#include "../../../../shader/shader.h"
#include "../../../../scene/scene.h"
#include "../../../../scene/scene_manager.h"
#include "../../../../scene/scenes/game.h"
#include "../../../object.h"
#include "../../../object_manager.h"
#include "../../sprite/water_gage.h"
#include "../../../../effect/effect.h"
#include "../../../../effect/effect_manager.h"
#include "../../../object.h"
#include "../fbx_model.h"
#include "fbx_player.h"
#include "fbx_child.h"
#include "../../../../resource/texture_manager.h"


//-------------------------------------
// define
//-------------------------------------
#define ANIMATION_SWITCHING_TIME 10


//-------------------------------------
// FbxChild()
//-------------------------------------
FbxChild::FbxChild(const OBJECT_PARAMETER_DESC &parameter) :
	FbxPlayer(parameter)
{
	animation_count_ = MAX_TYPE;

	recover_wait_timer_ = 0;

	// モデル読み込み
	Load("./resource/model/fbx/child_01.fbx");
	texture_ = TextureManager::GetTexture("resource/texture/game/child_01.jpg");
	life_ = 1.0f;
	water_gauge_ = 1.0f;

#ifdef _DEBUG
	int x;
	float time;
	// ボーンのパラメータチェク用
	for (int i = 0; i < bone_count_; i++)
	{
		x = bone_[i].key_max_;
		int st = 0;
		for (int l = 0; l < x; l++)
		{
			time = bone_[i].key_[l].time_;
			int st2 = 0;
		}
	}
#endif

	// おじいちゃん用のアニメーションを設定
	animation_ = new ANIMATION[MAX_TYPE];

	// モーション
	animation_[IDLE].begin_ = 1.0f;
	animation_[IDLE].end_ = 30.0f;
	animation_[IDLE].loop_ = true;
	animation_[IDLE].speed_ = 0.5f;
	animation_[IDLE].time_ = 0.0f;

	animation_[WALK].begin_ = 35.0f;
	animation_[WALK].end_ = 67.0f;
	animation_[WALK].loop_ = true;
	animation_[WALK].speed_ = 0.5f;
	animation_[WALK].time_ = 0.0f;

	animation_[DOWN].begin_ = 70.0f;
	animation_[DOWN].end_ = 90.0f;
	animation_[DOWN].loop_ = false;
	animation_[DOWN].speed_ = 0.5f;
	animation_[DOWN].time_ = 0.0f;

}


//-------------------------------------
// ~FbxChild()
//-------------------------------------
FbxChild::~FbxChild()
{
	SAFE_DELETE_ARRAY(animation_);
}


//-------------------------------------
// Action()
//-------------------------------------
void FbxChild::Action(
	Object *target,
	const float range)
{
	//-------------------------------------
	// モデルと当たったら
	if (target->parameter().layer_ == LAYER_MODEL_FORT ||
		target->parameter().layer_ == LAYER_MODEL_GRANDFATHER){
		Vector3 vec = target->parameter().position_ - parameter_.position_;
		Vector3 v = vec;
		Vec3Normalize(vec, vec);
		float distance = sqrtf(
			(v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_));
		float sub = range - distance;
		vec *= sub;
		parameter_.position_ -= vec;
	}

	//-------------------------------------
	// 湖と当たったら
	if (target->parameter().layer_ == LAYER_SPRITE_LAKE){
		//if (GamePad::isPress(GAMEPAD_CHILD1, PAD_BUTTON_6) && water_gauge_ < 1.0f){
		//	if (water_supply_enable_){
		//		//-------------------------------------
		//		// シーン取得
		//		Scene *scene = SceneManager::GetCurrentScene();
		//		std::string str = SceneManager::GetCurrentSceneName();
		//		if (str == "Game"){
		//			Game *game = dynamic_cast<Game*>(scene);

		//			// 水補給
		//			water_gauge_ += GRANDFATHER_SUB_WATERGAUGE;
		//			water_gauge_ = std::min<float>(water_gauge_, 1.0f);
		//			Object *obj = game->object_manager()->Get("water_gage");
		//			WaterGage *water_gage_obj = static_cast<WaterGage*>(obj);
		//			water_gage_obj->SetChangeValue(water_gauge_);
		//			// 重複防止
		//			water_supply_enable_ = false;

		//			if (water_supply_effect_timer_ % 45 == 0)
		//			{
		//				// 補給エフェクト
		//				OBJECT_PARAMETER_DESC grandfather_parameter = this->parameter();
		//				EFFECT_PARAMETER_DESC effect_param;
		//				MyEffect *effect = game->effect_manager()->Get("watersupply");
		//				effect_param = effect->parameter();
		//				effect_param.position_ = grandfather_parameter.position_;
		//				effect_param.position_.x_ += sinf(grandfather_parameter.rotation_.y_)*0.2f;
		//				effect_param.position_.z_ += cosf(grandfather_parameter.rotation_.y_)*0.2f;
		//				effect_param.position_.y_ += 0.5f;
		//				effect->SetParameter(effect_param);
		//				game->effect_manager()->Play("watersupply");
		//			}
		//			// 補給泡エフェクト
		//			OBJECT_PARAMETER_DESC grandfather_parameter = this->parameter();
		//			EFFECT_PARAMETER_DESC effect_param;
		//			MyEffect *effect = game->effect_manager()->Get("watersupplybubble");
		//			effect_param = effect->parameter();
		//			effect_param.position_ = grandfather_parameter.position_;
		//			effect_param.position_.y_ += 0.2f;
		//			effect->SetParameter(effect_param);
		//			game->effect_manager()->Play("watersupplybobble");


		//			water_supply_effect_timer_++;
		//		}
		//	}
		//}
		//else{
		//	water_supply_effect_timer_ = 0;
		//}
	}
}


//-------------------------------------
// end of file
//-------------------------------------