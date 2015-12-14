//=========================================================
// lake.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Lake_H__
#define __Lake_H__


//-------------------------------------
// class
//-------------------------------------
class Lake : public Object
{
public:
	Lake(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Lake();
	void Update();
	void Draw();

private:
	Vertex3D vertex_[4];
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
	D3DXMATRIX world_;
};


#endif //__Lake_H__


//-------------------------------------
// end of file
//-------------------------------------
