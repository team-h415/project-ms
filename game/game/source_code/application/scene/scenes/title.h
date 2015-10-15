//=========================================================
// title.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENES_TITLE__
#define __APP_SCENE_SCENES_TITLE__


//-------------------------------------
// class
//-------------------------------------
class ObjectManager;
class CameraManager;
class DebugFont;
class Title : public Scene
{
public:
	Title();
	virtual ~Title();
	void Update();
	void Draw();
private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	DebugFont *font_;
};


#endif //__APP_SCENE_SCENES_TITLE__


//-------------------------------------
// end of file
//-------------------------------------
