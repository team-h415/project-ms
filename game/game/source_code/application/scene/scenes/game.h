//=========================================================
// game.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENES_GAME_H__
#define __APP_SCENE_SCENES_GAME_H__


//-------------------------------------
// class
//-------------------------------------
using namespace Effekseer;
using namespace EffekseerRenderer;
class ObjectManager;
class CameraManager;
class EffectManager;
class DebugFont;
class Game : public Scene
{
public:
	Game();
	virtual ~Game();
	void Update();
	void Draw();

	ObjectManager* GetObjectManager(){return object_manager_;}
	CameraManager* GetCameraManager(){return camera_manager_;}
	EffectManager* GetEffectManager(){return effect_manager_;}

private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	EffectManager *effect_manager_;
	DebugFont *font_;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
