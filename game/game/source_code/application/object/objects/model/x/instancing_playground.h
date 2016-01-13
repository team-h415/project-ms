//=========================================================
// instancing_playground.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __InstancingPlaygound_H__
#define __InstancingPlaygound_H__


//-------------------------------------
// class
//-------------------------------------
class InstancingPlaygound : public Object
{
public:
	InstancingPlaygound(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~InstancingPlaygound();
	void Update();
	void Draw();
private:
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DINDEXBUFFER9 index_buffer_;
	LPDIRECT3DVERTEXBUFFER9 world_buffer_;
	int object_count_;
	int count_vertex_;
	int count_face_;
};


#endif //__InstancingBench_H__


//-------------------------------------
// end of file
//-------------------------------------
