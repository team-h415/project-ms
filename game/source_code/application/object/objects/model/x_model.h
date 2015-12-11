//=========================================================
// x_model.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __XModel_H__
#define __XModel_H__


//-------------------------------------
// class
//-------------------------------------
class XModel : public Object
{
public:
	XModel(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~XModel();
	virtual void Update();
	void Draw();
	void LoadMesh(
		const std::string &path);
	virtual void Action(
		Object *target,
		const float range);
protected:
	LPD3DXMESH mesh_;
	LPD3DXBUFFER material_buffer_;
	DWORD material_count_;
	D3DXMATRIX world_;
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
};


#endif //__XModel_H__


//-------------------------------------
// end of file
//-------------------------------------
