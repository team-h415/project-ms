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
#include "../model/fbx/fbx_player.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../mesh/field.h"
#include "../../../config/config.h"
#include "../model/x_model.h"
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
	const OBJECT_PARAMETER_DESC &parameter) :
	XModel(parameter)
{
	// �e���̐���
	LoadMesh("resource/model/x/ball.x");
	SetTexture("resource/texture/game/bullet.png");

	// �g�p�t���OOFF
	collision_ = nullptr;
	use_ = false;
}


//-------------------------------------
// ~Bullet()
//-------------------------------------
Bullet::~Bullet()
{
	if(collision_ != nullptr)
	{
		collision_->SetThisDelete(true);
		collision_ = nullptr;
	}
}


//-------------------------------------
// Fire()
//-------------------------------------
void Bullet::Fire(OBJECT_PARAMETER_DESC &parameter)
{
#ifdef NETWORK_HOST_MODE
	std::string temp = parameter_.name_;
	
	parameter_ = parameter;
	parameter_.name_ = temp;


	// ��]�l����������
	parameter_.rotation_.x_ += BULLET_OFFSET_ROT;

	// �Q�X�g�֏o����
	NETWORK_DATA send_data;
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_BULLET;
	send_data.object_param_.ex_id_ = 0;

	send_data.object_param_.position_.x_ = parameter_.position_.x_;
	send_data.object_param_.position_.y_ = parameter_.position_.y_;
	send_data.object_param_.position_.z_ = parameter_.position_.z_;

	send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
	send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
	send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;

	strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
	NetworkHost::SendTo(DELI_MULTI, send_data);

	if(collision_ == nullptr)
	{
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if(str != "GameServer"){
			ASSERT_ERROR("�e�����������ׂ��V�[���ł͂���܂���");
			return;
		}
		COLLISION_PARAMETER_DESC param;
		param.position_ = {
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_};
		param.range_ = 0.5f;
		param.offset_ = {0.0f, 0.0f, 0.0f};

		GameServer *game_server = dynamic_cast<GameServer*>(scene);
		collision_ = game_server->collision_manager()->Create(this, param);
	}
	collision_->SetUse(true);

	speed_ = {BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ};
	// ��]�l���Q�Ƃ��đ��x������
	speed_.y += sinf(parameter_.rotation_.x_) * BULLET_ADD_SPEED_Y;

	// �g�p�t���OOFF
	use_ = true;
#endif 
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
#ifdef NETWORK_HOST_MODE

	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BULLET_GRAVITY;

	// �Q�X�g�֍��W��
	NETWORK_DATA send_data;
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_BULLET;
	send_data.object_param_.ex_id_ = 0;
	send_data.object_param_.position_.x_ = parameter_.position_.x_;
	send_data.object_param_.position_.y_ = parameter_.position_.y_;
	send_data.object_param_.position_.z_ = parameter_.position_.z_;
	send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
	send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
	send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;
	strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
	NetworkHost::SendTo(DELI_MULTI, send_data);

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();
	if (str == "GameServer"){
		GameServer *game_server = dynamic_cast<GameServer*>(scene);
		Object *obj = game_server->field();
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

			// �Q�X�g�֏������
			NETWORK_DATA send_data;
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_BULLET;
			send_data.object_param_.ex_id_ = 1;
			strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
	}
#endif

#if defined(_DEBUG) || !defined(NETWORK_HOST_MODE)
	XModel::Update();
#endif
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
#ifdef NETWORK_HOST_MODE
	if(!use_)
	{
		return;
	}

	//-------------------------------------
	// ����X���f���Ɠ���������
	if (target->parameter().layer_ == LAYER_MODEL_FORT ||
		target->parameter().layer_ == LAYER_MODEL_GRANDFATHER ||
		target->parameter().layer_ == LAYER_MODEL_CHILD){

		//-------------------------------------
		// �����̐e�̃��C���[���m�F
		if (parameter_.parent_layer_ != target->parameter().layer_){

			//-------------------------------------
			// ���������ΏۂɃp�����[�^���f
			// ����
			if (target->parameter().layer_ == LAYER_MODEL_GRANDFATHER){
				FbxGrandfather *father = dynamic_cast<FbxGrandfather*>(target);
				float life = father->GetLife();
				life -= GRANDFATHER_DAMAGE;
				father->SetLife(life);
				father->SetRecoverWaitTimer(0);
			}
			// �q��
			else if (target->parameter().layer_ == LAYER_MODEL_CHILD){
				FbxChild *child = dynamic_cast<FbxChild*>(target);
				float life = child->GetLife();
				life -= CHILD_DAMAGE;
				child->SetLife(life);
				child->SetRecoverWaitTimer(0);
			}
			// ��
			else if (target->parameter().layer_ == LAYER_MODEL_FORT &&
				parameter_.parent_layer_ == LAYER_MODEL_GRANDFATHER){
				XFort *fort = dynamic_cast<XFort*>(target);
				float life = fort->GetLife();
				life -= FORT_DAMAGE;
				fort->SetLife(life);
			}

			// �f�[�^�]���p�\���̗p��
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

			// �I�u�W�F�N�g�f�[�^�]��
			NetworkHost::SendTo(DELI_MULTI, send_data);

			// �g�p�t���OOFF
			use_ = false;
			collision_->SetUse(false);

			// �Q�X�g�֏������
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_BULLET;
			send_data.object_param_.ex_id_ = 1;
			strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
	}
#endif
}


//-------------------------------------
// end of file
//-------------------------------------
