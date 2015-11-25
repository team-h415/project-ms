//=========================================================
// collision_factory.cpp
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


//-------------------------------------
// Create()
//-------------------------------------
Collision *CollisionFactory::Create(
	Object *parent,
	const COLLISION_PARAMETER_DESC &parameter)
{
	Collision *collision = nullptr;
	collision = new Collision(parent, parameter);
	return collision;
}


//-------------------------------------
// end of file
//-------------------------------------
