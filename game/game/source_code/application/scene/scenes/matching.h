//=========================================================
// matching.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Matching_H__
#define __Matching_H__


//-------------------------------------
// class
//-------------------------------------
using namespace Effekseer;
using namespace EffekseerRenderer;
class ObjectManager;
class CameraManager;
class EffectManager;
class DebugFont;
class Matching : public Scene
{
public:
	Matching();
	virtual ~Matching();
	void Update();
	void Draw();

	ObjectManager* GetObjectManager(){ return object_manager_; }
	CameraManager* GetCameraManager(){ return camera_manager_; }
	EffectManager* GetEffectManager(){ return effect_manager_; }

private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	EffectManager *effect_manager_;
	DebugFont *font_;
};


#endif //__Matching_H__


//-------------------------------------
// end of file
//-------------------------------------
