//=========================================================
// object.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../math/vector.h"
#include "object.h"


//-------------------------------------
// Object()
//-------------------------------------
Object::Object()
{
	this_delete_ = false;
	use_ = true;
}


//-------------------------------------
// ~Object()
//-------------------------------------
Object::~Object()
{
}


//-------------------------------------
// Action()
//-------------------------------------
void Object::Action(
	Object *target,
	const float range,
	D3DXVECTOR3 my_position)
{
}


//-------------------------------------
// end of file
//-------------------------------------
