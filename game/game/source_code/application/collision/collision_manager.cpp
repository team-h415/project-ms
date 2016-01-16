//=========================================================
// collision_manager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../math/vector.h"
#include "../object/object.h"
#include "collision.h"
#include "collision_factory.h"
#include "collision_manager.h"


//-------------------------------------
// variable
//-------------------------------------
CollisionManager* CollisionManager::collision_manager_(nullptr);


//-------------------------------------
// Get()
//-------------------------------------
CollisionManager* CollisionManager::Get()
{
	if(collision_manager_ == nullptr)
	{
		collision_manager_ = new CollisionManager();
	}

	return collision_manager_;
}


//-------------------------------------
// Delete()
//-------------------------------------
void CollisionManager::Delete()
{
	SAFE_DELETE(collision_manager_);
}


//-------------------------------------
// CollisionManager()
//-------------------------------------
CollisionManager::CollisionManager()
{
	collision_.clear();
}


//-------------------------------------
// ~CollisionManager()
//-------------------------------------
CollisionManager::~CollisionManager()
{
	for (auto it = collision_.begin(); it != collision_.end(); ++it){
		SAFE_DELETE((*it));
	}
	collision_.clear();
}


//-------------------------------------
// Update()
//-------------------------------------
void CollisionManager::Update()
{
	//-------------------------------------
	// �S�v�f���X�V
	for (auto it = collision_.begin(); it != collision_.end(); ++it){
		if ((*it)->this_delete()){
			SAFE_DELETE((*it));
		}
	}
	collision_.remove(nullptr);

	for(auto it = collision_.begin(); it != collision_.end(); ++it){
		(*it)->Update();
	}


	//-------------------------------------
	// ���`���m�̂����蔻������{
	for (auto it = collision_.begin(); it != collision_.end(); ++it)
	{
		// �g�p�t���O�m�F
		if((*it)->GetUse() == false)
		{
			continue;
		}
		for (auto it2 = collision_.begin(); it2 != collision_.end(); ++it2)
		{
			// ����I�u�W�F�N�g�⓯�ꃌ�C���[�ɓ����蔻�肪�K�v�ȃI�u�W�F�N�g�͑��݂��Ȃ�
			if(it == it2)
			{
				continue;
			}
			// �g�p�t���O�m�F
			if((*it2)->GetUse() == false)
			{
				continue;
			}
			D3DXVECTOR3 distance =
				(*it)->parameter().position_ - (*it2)->parameter().position_;
			float range = (*it)->parameter().range_ + (*it2)->parameter().range_;
			float d = sqrt(
				(distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
			if (range > d){
				//-------------------------------------
				// �����������̏���
				(*it)->parent()->Action((*it2)->parent(), range, (*it)->parameter().position_);
				(*it2)->parent()->Action((*it)->parent(), range, (*it2)->parameter().position_);
			}
		}
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void CollisionManager::Draw()
{
#ifdef _DEBUG
	for (auto it = collision_.begin(); it != collision_.end(); ++it){
		(*it)->Draw();
	}
#endif //_DEBUG
}


//-------------------------------------
// Create()
//-------------------------------------
Collision *CollisionManager::Create(
	Object *parent,
	const COLLISION_PARAMETER_DESC &parameter)
{
	Collision *collision = nullptr;
	collision = CollisionFactory::Create(parent, parameter);
	collision_.push_back(collision);
	return collision;
}


//-------------------------------------
// end of file
//-------------------------------------
