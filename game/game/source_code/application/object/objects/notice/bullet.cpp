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
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../mesh/field.h"
#include "bullet.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
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
	speed_ = { 0.2f, 0.05f, 0.2f };

	speed_.y += sinf(parameter_.rotation_.x_) * 0.3f;

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();
	if (str != "Game"){
		ASSERT_ERROR("�e�����������ׂ��V�[���ł͂���܂���");
		return;
	}
	Game *game = dynamic_cast<Game*>(scene);
	collision_ = game->collision_manager()->Create(this, param);
	
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
	static float gravity = 0.003f;
	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= gravity;

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
	//-------------------------------------
	// ����X���f���Ɠ���������
	if (target->parameter().layer_ == LAYER_MODEL_X){
		//-------------------------------------
		// �V�[���擾
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if (str == "Game"){
			Game *game = dynamic_cast<Game*>(scene);
			//-------------------------------------
			// �V�[������G�t�F�N�g�擾
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = game->effect_manager()->Get("damage");
			effect_param = effect->parameter();
			effect_param.position_ = parameter_.position_;
			effect_param.position_.y_ += 0.5f;
			effect_param.rotation_ = parameter_.rotation_;
			effect->SetParameter(effect_param);
			//-------------------------------------
			// �G�t�F�N�g�Đ�
			game->effect_manager()->Play("damage");
		}
		this_delete_ = true;
		collision_->SetThisDelete(true);
	}
}


//-------------------------------------
// end of file
//-------------------------------------
