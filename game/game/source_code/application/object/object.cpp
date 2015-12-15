//=========================================================
// object.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
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
	const float range)
{
}


//-------------------------------------
// end of file
//-------------------------------------
