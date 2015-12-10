//=========================================================
// result.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Result_H__
#define __Result_H__


//-------------------------------------
// class
//-------------------------------------
class ObjectManager;
class CameraManager;
class DebugFont;
class Result : public Scene
{
public:
	Result();
	virtual ~Result();
	void Update();
	void Draw();
private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	DebugFont *font_;
};


#endif //__Result_H__


//-------------------------------------
// end of file
//-------------------------------------
