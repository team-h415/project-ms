//=========================================================
// camera.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Camera_H__
#define __Camera_H__


//-------------------------------------
// struct
//-------------------------------------
struct CAMERA_PARAMETER_DESC
{
	D3DXVECTOR3 position_, rotation_, focus_, up_;
	float near_, far_;
	// perspective_parameter
	float fovy_, acpect_;
	// oltho_parameter
	float w_, h_;
};


//-------------------------------------
// class
//-------------------------------------
class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void Update() = 0;
	virtual void Set() = 0;

	// パラメータ取得
	const D3DXMATRIX &view(){
		return view_;
	}
	const D3DXMATRIX &projection(){
		return projection_;
	}
	const D3DXVECTOR3 &position(){
		return parameter_.position_;
	}
	const D3DXVECTOR3 &rotation(){
		return parameter_.rotation_;
	}
	const D3DXVECTOR3 &focus(){
		return parameter_.focus_;
	}

	// パラメータ設定
	void SetView(const D3DXMATRIX &view){
		view_ = view;
	}
	void SetProjection(const D3DXMATRIX &projection){
		projection_ = projection;
	}
	void SetPosition(const D3DXVECTOR3 &position){
		parameter_.position_ = position;
	}
	void SetRotation(const D3DXVECTOR3 &rotation){
		parameter_.rotation_ = rotation;
	}
	void SetFocus(const D3DXVECTOR3 &focus){
		parameter_.focus_ = focus;
	}


protected:
	CAMERA_PARAMETER_DESC parameter_;
	D3DXMATRIX view_;
	D3DXMATRIX projection_;
};


#endif //__Camera_H__


//-------------------------------------
// end of file
//-------------------------------------
