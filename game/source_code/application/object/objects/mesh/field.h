//=========================================================
// field.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Field_H__
#define __Field_H__


//-------------------------------------
// class
//-------------------------------------
class Shader;
class Field : public Object
{
public:
	Field(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Field();
	void Update();
	void Draw();
	void LoadMesh(
		const std::string &path);
	float GetHeight(
		const D3DXVECTOR3 &position);
private:
	void CalculateVertex(
		D3DXVECTOR3 *source_buffer,
		D3DXVECTOR4 *texture_alpha);
	void CalculateIndex();
	void CalculateNormal();
	void CalculatePanelNormal();
	void CalculateVertexNormal();
	bool isStand(
		const std::string &type,
		const D3DXVECTOR3 &vec1,
		const D3DXVECTOR3 &vec2,
		const D3DXVECTOR3 &vec3,
		const D3DXVECTOR3 &vec_p1,
		const D3DXVECTOR3 &vec_p2,
		const D3DXVECTOR3 &vec_p3);
	float GetHeightValue(
		const D3DXVECTOR3 &position,
		const D3DXVECTOR3 &player_position,
		const D3DXVECTOR3 &normal);

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DINDEXBUFFER9 index_buffer_;
	D3DXVECTOR3 *normal_buffer_;
	unsigned int index_count_;
	D3DXVECTOR2 mesh_division_;
	D3DXMATRIX world_;
	Shader *shader_;
	LPDIRECT3DTEXTURE9 texture_[4];
};


#endif //__Field_H__


//-------------------------------------
// end of file
//-------------------------------------
