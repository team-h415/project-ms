//=========================================================
// scene.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENE_H__
#define __APP_SCENE_SCENE_H__


class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	bool setup(){ return setup_; }
	void SetSetup(bool set){ setup_ = set; }	// ”O‚Ì‚½‚ß

protected:
	bool setup_;
};


#endif //__APP_SCENE_SCENE_H__


//-------------------------------------
// end of file
//-------------------------------------
