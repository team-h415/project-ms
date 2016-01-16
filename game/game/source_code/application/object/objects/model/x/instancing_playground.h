//=========================================================
// instancing_playground.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __InstancingPlayground_H__
#define __InstancingPlayground_H__


//-------------------------------------
// class
//-------------------------------------
class XContainer;
class InstancingPlayground : public Object
{
public:
	InstancingPlayground(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~InstancingPlayground();
	void Update();
	void Draw();
	void SetTexture(
		const std::string &path);
	void SetMesh(
		const std::string &path);
	void SetPositionPatern(
		int patern);
	void Action(
		Object *target,
		const float range,
		D3DXVECTOR3 my_position);

private:
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DINDEXBUFFER9 index_buffer_;
	LPDIRECT3DVERTEXBUFFER9 world_buffer_[3];
	XContainer* container_;
	int object_count_[3];
	int count_vertex_;
	int count_face_;
	int position_patern_;
};


#endif //__InstancingPlayground_H__


//-------------------------------------
// end of file
//-------------------------------------
