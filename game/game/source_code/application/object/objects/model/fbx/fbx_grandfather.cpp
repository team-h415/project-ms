//=========================================================
// fbx_grandfather.cpp
// author:takeshi iwasawa
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
#include "../../../../resource/texture_manager.h"
#include "../../../../scene/scene.h"
#include "../../../../scene/scene_manager.h"
#include "../../../../scene/scenes/game.h"
#include "../../../object.h"
#include "../../../object_manager.h"
#include "../../sprite/water_gage.h"
#include "../fbx_model.h"
#include "fbx_player.h"
#include "fbx_grandfather.h"




//-------------------------------------
// FbxGrandfather()
//-------------------------------------
FbxGrandfather::FbxGrandfather(const OBJECT_PARAMETER_DESC &parameter) :
	FbxPlayer(parameter)
{
	animation_count_ = MAX_TYPE;

	// モデル読み込み
	Load("./resource/model/fbx/ogchan.fbx");
	texture_ = TextureManager::GetTexture("resource/texture/game/ogchan.jpg");
	life_ = 1.0f;
	water_gauge_ = 1.0f;

#ifdef _DEBUG
	int x;
	float time;
	// ボーンのパラメータチェク用
	for(int i = 0; i < bone_count_; i++)
	{
		x = bone_[i].key_max_;
		int st = 0;
		for(int l = 0; l < x; l++)
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
// ~FbxGrandfather()
//-------------------------------------
FbxGrandfather::~FbxGrandfather()
{
	SAFE_DELETE_ARRAY(animation_);
}


//-------------------------------------
// Action()
//-------------------------------------
void FbxGrandfather::Action(
	Object *target,
	const float range)
{
	//-------------------------------------
	// Xモデルと当たったら
	if (target->parameter().layer_ == LAYER_MODEL_FORT ||
		target->parameter().layer_ == LAYER_MODEL_CHILD){
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
		if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_6) && water_gauge_ < 1.0f){
			//-------------------------------------
			// シーン取得
			Scene *scene = SceneManager::GetCurrentScene();
			std::string str = SceneManager::GetCurrentSceneName();
			if (str == "Game"){
				Game *game = dynamic_cast<Game*>(scene);

				// 水補給
				water_gauge_ += GRANDFATHER_SUB_WATERGAUGE;
				water_gauge_ = std::min<float>(water_gauge_, 1.0f);

				Object *obj = game->object_manager()->Get("water_gage");
				WaterGage *water_gage_obj = static_cast<WaterGage*>(obj);
				water_gage_obj->SetChangeValue(water_gauge_);
			}
		}
	}

}


//-------------------------------------
// end of file
//-------------------------------------