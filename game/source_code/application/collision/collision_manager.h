//=========================================================
// collision_manager.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __CollisionManager_H__
#define __CollisionManager_H__


//-------------------------------------
// class
//-------------------------------------
class CollisionManager
{
public:
	CollisionManager();
	virtual ~CollisionManager();
	void Update();
	void Draw();
	static Collision *Create(
		Object *parent,
		const COLLISION_PARAMETER_DESC &parameter);
private:
	static std::list<Collision *>collision_;
};


#endif //__CollisionManager_H__


//-------------------------------------
// end of file
//-------------------------------------
