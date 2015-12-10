//=========================================================
// bullet.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Bullet_H__
#define __Bullet_H__


//-------------------------------------
// class
//-------------------------------------
class Collision;
class Bullet : public Object
{
public:
	Bullet(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Bullet();
	void Update();
	void Draw();
	void Action(
		Object *target,
		const float range);
private:
	Collision *collision_;
	D3DXVECTOR3 speed_;
};


#endif //__Bullet_H__


//-------------------------------------
// end of file
//-------------------------------------
