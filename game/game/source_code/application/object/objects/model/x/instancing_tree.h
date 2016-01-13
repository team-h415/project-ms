//=========================================================
// instancing_tree.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __InstancingTree_H__
#define __InstancingTree_H__


//-------------------------------------
// class
//-------------------------------------
class XContainer;
class Shader;
class InstancingTree : public Object
{
public:
	InstancingTree(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~InstancingTree();
	void Update();
	void Draw();
	void SetTexture(
		const std::string &path);
	void SetMesh(
		const std::string &path);
	void SetPositionPatern(
		int patern);

private:
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_;
	LPDIRECT3DINDEXBUFFER9 index_buffer_;
	LPDIRECT3DVERTEXBUFFER9 world_buffer_[3];
	XContainer* container_;
	int object_count_;
	int count_vertex_;
	int count_face_;
	int position_patern_;
};


#endif //__InstancingTree_H__


//-------------------------------------
// end of file
//-------------------------------------
