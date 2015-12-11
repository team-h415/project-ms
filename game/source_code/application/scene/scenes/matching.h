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
class ObjectManager;
class CameraManager;
class DebugFont;
class Matching : public Scene
{
public:
	Matching();
	virtual ~Matching();
	void Update();
	void Draw();
private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	DebugFont *font_;
};


#endif //__Matching_H__


//-------------------------------------
// end of file
//-------------------------------------
