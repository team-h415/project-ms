//=========================================================
// fbx_child.cpp
// author:ryuya nakamura
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
}


//-------------------------------------
// end of file
//-------------------------------------