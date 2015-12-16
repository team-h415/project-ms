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
std::list<Collision*> CollisionManager::collision_;
int CollisionManager::collision_count_ = 0;


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
	collision_count_ = 0;

	//-------------------------------------
	// ‘S—v‘f‚ðXV
	for (auto it = collision_.begin(); it != collision_.end(); ++it){
		if ((*it)->this_delete()){
			SAFE_DELETE((*it));
		}
	}
	collision_.remove(nullptr);

	for(auto it = collision_.begin(); it != collision_.end(); ++it){
		(*it)->Update();
		collision_count_++;
	}


	//-------------------------------------
	// ‹…Œ`“¯Žm‚Ì‚ ‚½‚è”»’è‚ðŽÀŽ{
	for (auto it = collision_.begin(); it != collision_.end(); ++it){
		if((*it)->GetThisDelete() == true || (*it)->GetUse() == false) continue;
		for (auto it2 = collision_.begin(); it2 != collision_.end(); ++it2){
			if((*it) == (*it2) || (*it2)->GetThisDelete() == true || (*it2)->GetUse() == false) continue;
			D3DXVECTOR3 distance =
				(*it)->parameter().position_ - (*it2)->parameter().position_;
			float range = (*it)->parameter().range_ + (*it2)->parameter().range_;
			float d = sqrt(
				(distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
			if (range > d){
				//-------------------------------------
				// “–‚½‚Á‚½Žž‚Ìˆ—
				(*it)->parent()->Action((*it2)->parent(), range);
				(*it2)->parent()->Action((*it)->parent(), range);
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
