//=========================================================
// InstancingBench.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __InstancingBench_H__
#define __InstancingBench_H__


//-------------------------------------
// class
//-------------------------------------
class InstancingBench : public Object
{
public:
	InstancingBench(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~InstancingBench();
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
