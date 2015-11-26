//=========================================================
// collision.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Collision_H__
#define __Collision_H__


//-------------------------------------
// struct 
//-------------------------------------
struct COLLISION_PARAMETER_DESC
{
	D3DXVECTOR3 position_;
	D3DXVECTOR3 offset_;
	float range_;
};


//-------------------------------------
// class
//-------------------------------------
class Collision
{
public:
	Collision(
		Object *parent,
		const COLLISION_PARAMETER_DESC &parameter);
	virtual ~Collision();
	
	//-------------------------------------
	// パラメータ
	const COLLISION_PARAMETER_DESC &parameter(){
		return parameter_;
	}
	Object *parent(){
		return parent_;
	}

	//-------------------------------------
	// 描画用
	void CalculateVertex();
	void Update();
	void Draw();

private:
	COLLISION_PARAMETER_DESC parameter_;
	Object *parent_;
	Vertex3D vertex_[3][11];
	D3DXMATRIX world_;
};


#endif //__Collision_H__


//-------------------------------------
// end of file
//-------------------------------------
