//=========================================================
// shadow.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Shadow_H__
#define __Shadow_H__


//-------------------------------------
// class
//-------------------------------------
class Collision;
class Shadow : public Object
{
public:
	Shadow(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Shadow();
	void Update();
	void Draw();

private:
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
};


#endif //__Shadow_H__


//-------------------------------------
// end of file
//-------------------------------------
