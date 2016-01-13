//=========================================================
// loading.h
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENES_LOADING_H__
#define __APP_SCENE_SCENES_LOADING_H__


//-------------------------------------
// class
//-------------------------------------
class Sprite2D;
class Loading : public Scene
{
public:
	Loading();
	virtual ~Loading();
	void Initialize();
	void Update();
	void Draw();
private:
	Sprite2D*	background_;
};


#endif //__APP_SCENE_SCENES_LOADING_H__

//-------------------------------------
// end of file
//-------------------------------------
