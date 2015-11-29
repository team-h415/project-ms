//=========================================================
// skydome.h
// author:fuka takahashi
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __SkyDome_H__
#define __SkyDome_H__


//-------------------------------------
// class
//-------------------------------------
class Shader;
class SkyDome : public Object
{
public:
	SkyDome(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~SkyDome();
	void Update();
	void Draw();
	void LoadMesh(
		const std::string &path);
private:
	void CalculateVertex(
		D3DXVECTOR3 *source_buffer);
	void CalculateIndex();
	void CalculateNormal();
	void CalculateVertexNormal();
	void CalculateTextureUV();
	void NomalizeNormal();

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DINDEXBUFFER9 index_buffer_;
	D3DXVECTOR3 *normal_buffer_;
	unsigned int index_count_;
	float mesh_radius_;
	D3DXVECTOR2 mesh_division_;
	D3DXMATRIX world_;
	Shader *shader_;
	LPDIRECT3DTEXTURE9 texture_;
	
};


#endif //__SkyDome_H__



//-------------------------------------
// end of file
//-------------------------------------
