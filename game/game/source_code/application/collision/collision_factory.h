//=========================================================
// collision_factory.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __CollisionFactory_H__
#define __CollisionFactory_H__


//-------------------------------------
// class
//-------------------------------------
class CollisionFactory
{
public:
	static Collision *Create(
		Object *parent,
		const COLLISION_PARAMETER_DESC &parameter);
};


#endif //__CollisionFactory_H__


//-------------------------------------
// end of file
//-------------------------------------
