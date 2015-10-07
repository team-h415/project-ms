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
class ObjectManager;
class CameraManager;
class LightManager;
class ShaderManager;
class SurfaceManager;
class Game : public Scene
{
public:
	Game();
	virtual ~Game();
	void Update();
	void Draw();
private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	LightManager *light_manager_;
	ShaderManager *shader_manager_;
	SurfaceManager *sueface_manager;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
