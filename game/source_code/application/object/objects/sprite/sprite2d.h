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
	void Update();
	void Draw();
	void SetTexture(
		const std::string &path);

private:
	void CalculateVertex();
	Vertex2D *vertex_;
	LPDIRECT3DTEXTURE9 texture_;
};


#endif //__Sprite2D_H__


//-------------------------------------
// end of file
//-------------------------------------
