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
	// 全要素を更新
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
	// 球形同士のあたり判定を実施
	for (auto it = collision_.begin(); it != collision_.end(); ++it)
	{
		// 使用フラグ確認
		if((*it)->GetUse() == false)
		{
			continue;
		}
		for (auto it2 = collision_.begin(); it2 != collision_.end(); ++it2)
		{
			// 同一オブジェクトや同一レイヤーに当たり判定が必要なオブジェクトは存在しない
			if(it == it2)
			{
				continue;
			}
			// 使用フラグ確認
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
				// 当たった時の処理
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
