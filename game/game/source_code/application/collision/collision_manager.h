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
	static CollisionManager* Get();
	static void Delete();

	void Update();
	void Draw();
	Collision *Create(
		Object *parent,
		const COLLISION_PARAMETER_DESC &parameter);

private:
	CollisionManager();
	virtual ~CollisionManager();

	static CollisionManager* collision_manager_;
	std::list<Collision *>collision_;
};


#endif //__CollisionManager_H__


//-------------------------------------
// end of file
//-------------------------------------
