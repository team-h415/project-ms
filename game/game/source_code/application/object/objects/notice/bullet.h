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
class XModel;
class MyEffect;
class Bullet : public XModel
{
public:
	Bullet(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Bullet();
	void Fire(OBJECT_PARAMETER_DESC &parameter);
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
