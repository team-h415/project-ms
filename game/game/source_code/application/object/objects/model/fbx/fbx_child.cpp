//=========================================================
// fbx_child.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../network/network.h"
#include "../../../../network/network_host.h"
#include "../../../../../common/common.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../input/input.h"
#include "../../../../input/inputs/gamepad.h"
#include "../../../../math/vector.h"
#include "../../../../config/config.h"
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
#ifndef NETWORK_HOST_MODE
	return;
#endif

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
	if (target->parameter().layer_ == LAYER_SPRITE_LAKE)
	{
		char name[10];
		char temp;
		strcpy_s(name, 10, parameter_.name_.c_str());
		temp = name[6];
		int my_pad = atoi(&temp);

		if(GamePad::isPress(my_pad, PAD_BUTTON_6)
			&& !GamePad::isPress(my_pad, PAD_BUTTON_8)
			&& water_gauge_ < 1.0f){
			if(water_supply_enable_){
				// 水補給
				water_gauge_ += GRANDFATHER_SUB_WATERGAUGE;
				water_gauge_ = std::min<float>(water_gauge_, 1.0f);

				// 重複防止
				water_supply_enable_ = false;

				NETWORK_DATA send_data;
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};

				if(water_supply_effect_timer_ % 45 == 0)
				{
					// 補給エフェクト
					send_data.object_param_.position_.x_ = parameter_.position_.x_;
					send_data.object_param_.position_.y_ = parameter_.position_.y_;
					send_data.object_param_.position_.z_ = parameter_.position_.z_;

					send_data.object_param_.position_.x_ += sinf(parameter_.rotation_.y_)*0.2f;
					send_data.object_param_.position_.z_ += cosf(parameter_.rotation_.y_)*0.2f;
					send_data.object_param_.position_.y_ += 0.5f;

					strcpy_s(send_data.name, MAX_NAME_LEN, "watersupply");
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}

				// 補給泡エフェクト
				send_data.object_param_.position_.x_ = parameter_.position_.x_;
				send_data.object_param_.position_.y_ = parameter_.position_.y_;
				send_data.object_param_.position_.z_ = parameter_.position_.z_;
				send_data.object_param_.position_.y_ += 0.2f;

				send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
				strcpy_s(send_data.name, MAX_NAME_LEN, "watersupplybubble");
				NetworkHost::SendTo(DELI_MULTI, send_data);

				// タイムインクリメント
				water_supply_effect_timer_++;
			}
		}
		else{
			water_supply_effect_timer_ = 0;
		}
	}
}


//-------------------------------------
// end of file
//-------------------------------------