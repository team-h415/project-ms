//=========================================================
// arrow.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Arrow_H__
#define __Arrow_H__


//-------------------------------------
// class
//-------------------------------------
class Arrow : public Object
{
public:
	Arrow(
		const OBJECT_PARAMETER_DESC &parameter);
	~Arrow();
	void Update();
	void Draw();

protected:
	LPD3DXMESH mesh_;
	LPD3DXBUFFER material_buffer_;
	DWORD material_count_;
	D3DXMATRIX world_;
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
};


#endif //__Arrow_H__


//-------------------------------------
// end of file
//-------------------------------------
