//=========================================================
// game_server.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_SCENE_SCENES_GAME_SERVER_H__
#define __APP_SCENE_SCENES_GAME_SERVER_H__


//-------------------------------------
// enum
//-------------------------------------
enum SERVER_STATE
{
	STATE_MATCHING,
	STATE_GAME,
	STATE_RESULT,
};


//-------------------------------------
// class
//-------------------------------------
using namespace Effekseer;
using namespace EffekseerRenderer;
class ObjectManager;
class CameraManager;
class EffectManager;
class DebugFont;
class GameServer : public Scene
{
public:
	GameServer();
	virtual ~GameServer();
	void Update();
	void Draw();

private:
	void ChangeState(SERVER_STATE next);

	SERVER_STATE	state;
	ObjectManager	*object_manager_;
	CameraManager	*camera_manager_;
	DebugFont		*font_;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
