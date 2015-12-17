//=========================================================
// sprite2d.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Sprite2D_H__
#define __Sprite2D_H__


//-------------------------------------
// class
//-------------------------------------
class Sprite2D : public Object
{
public:
	Sprite2D(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Sprite2D();
	virtual void Update();
	void Draw();
	void SetTexture(
		const std::string &path);
	void SetColor(
		const D3DXCOLOR &color){
		color_ = color;
	}
	const D3DXCOLOR GetColor(){
		return color_;
	}

protected:
	virtual void CalculateVertex();
	Vertex2D *vertex_;
	LPDIRECT3DTEXTURE9 texture_;
	D3DXCOLOR color_;
};


#endif //__Sprite2D_H__


//-------------------------------------
// end of file
//-------------------------------------
