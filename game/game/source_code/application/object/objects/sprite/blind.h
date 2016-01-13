//=========================================================
// sprite2d.h
// author:fuka takahashi
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Blind_H__
#define __Blind_H__


//-------------------------------------
// class
//-------------------------------------
class Blind : public Object
{
public:
	Blind(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Blind();
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
	void SetBlind(
		const OBJECT_PARAMETER_DESC &parameter);

protected:
	virtual void CalculateVertex();
	void AttenuateAlpha();
	void MagnifyScaling();
	Vertex2D *vertex_;
	LPDIRECT3DTEXTURE9 texture_;
	D3DXCOLOR color_;
	float scaling_max_;
};


#endif //__Blind_H__


//-------------------------------------
// end of file
//-------------------------------------
