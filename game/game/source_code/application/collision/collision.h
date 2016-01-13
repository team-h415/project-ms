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

	void Update();
	void Draw();
	
	//-------------------------------------
	// ÉpÉâÉÅÅ[É^
	const COLLISION_PARAMETER_DESC &parameter(){
		return parameter_;
	}
	Object *parent(){
		return parent_;
	}
	bool this_delete(){
		return this_delete_;
	}

	bool GetThisDelete(void){ return this_delete_; }
	void SetThisDelete(bool flag){
		this_delete_ = flag;
	}
	bool GetUse(void){ return use_; }
	void SetUse(bool flag){
		use_ = flag;
	}

private:
	void CalculateVertex();

	COLLISION_PARAMETER_DESC parameter_;
	Object *parent_;
	bool this_delete_;
	bool use_;
#ifdef _DEBUG
	Vertex3D vertex_[3][11];
	D3DXMATRIX world_;
#endif
};


#endif //__Collision_H__


//-------------------------------------
// end of file
//-------------------------------------
