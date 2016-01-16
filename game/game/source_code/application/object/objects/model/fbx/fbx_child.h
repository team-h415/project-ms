//=========================================================
// fbx_child.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxChild_H__
#define __FbxChild_H__


//-------------------------------------
// class
//-------------------------------------
class FbxChild : public FbxPlayer
{
public:
	//-------------------------------------
	// enum
	//-------------------------------------
	enum ANIMATION_TYPE
	{
		IDLE = 0,
		WALK,
		DOWN,
		MAX_TYPE
	};


	FbxChild(
		const OBJECT_PARAMETER_DESC &parameter);
	~FbxChild();

	void SetTexture(const std::string& path);
	void Action(
		Object *target,
		const float range,
		D3DXVECTOR3 my_position);
};


#endif //__FbxChild_H__


//-------------------------------------
// end of file
//-------------------------------------
