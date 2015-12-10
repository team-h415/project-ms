//=========================================================
// effect.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Effect_H__
#define __Effect_H__


//-------------------------------------
// struct
//-------------------------------------
struct EFFECT_PARAMETER_DESC
{
	Vector3 position_;
	Vector3 rotation_;
	Vector3 scaling_;
	float speed_;
};


//-------------------------------------
// class
//-------------------------------------
using namespace Effekseer;
class MyEffect
{
public:
	MyEffect(
		Effekseer::Manager *manager,
		const EFFECT_PARAMETER_DESC &parameter,
		const std::string &path);
	virtual ~MyEffect();
	void Update(
		Effekseer::Manager *manager);
	void Play(
		Effekseer::Manager *manager);

	const EFFECT_PARAMETER_DESC &parameter(){
		return parameter_;
	}
	void SetParameter(
		const EFFECT_PARAMETER_DESC &parameter){
		parameter_ = parameter;
	}

private:
	Effekseer::Handle handle_;
	Effekseer::Effect *effect_;
	EFFECT_PARAMETER_DESC parameter_;
};


#endif //__Effect_H__


//-------------------------------------
// end of file
//-------------------------------------
