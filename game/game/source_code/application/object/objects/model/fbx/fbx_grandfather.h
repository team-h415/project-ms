//=========================================================
// fbx_grandfather.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxGrandfather_H__
#define __FbxGrandfather_H__


//-------------------------------------
// class
//-------------------------------------
class FbxGrandfather : public FbxPlayer
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


	FbxGrandfather(
		const OBJECT_PARAMETER_DESC &parameter);
	~FbxGrandfather();

	void Action(
		Object *target,
		const float range,
		D3DXVECTOR3 my_position);
};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
