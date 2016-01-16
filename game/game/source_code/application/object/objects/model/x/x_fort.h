//=========================================================
// x_fort.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __XFort_H__
#define __XFort_H__

#include "../x_model.h"

//-------------------------------------
// class
//-------------------------------------
class XFort : public XModel
{
public:
	XFort(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~XFort();
	void Update();
	void Action(
		Object *target,
		const float range,
		D3DXVECTOR3 my_position);

	//-------------------------------------
	// ÉpÉâÉÅÅ[É^
	//-------------------------------------
	// ëÃóÕ
	const float GetLife(){
		return life_;
	}
	void SetLife(const float life){
		life_ = life;
	}
private:
	// ëÃóÕ
	float life_;

};


#endif //__XFort_H__


//-------------------------------------
// end of file
//-------------------------------------
