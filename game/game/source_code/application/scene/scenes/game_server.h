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

enum GAME_STATE
{
	STATE_GUEST_WAITING = 0,
	STATE_COUNTDOWN,
	STATE_RUN,
	STATE_CHANGE_FORT,
	STATE_END,
};

//-------------------------------------
// class
//-------------------------------------
class ObjectManager;
class CameraManager;
class EffectManager;
class DebugFont;
class CollisionManager;
class Object;
class Field;
class GameServer : public Scene
{
public:
	GameServer();
	virtual ~GameServer();
	void Update();
	void Draw();

	void SetGuestSceneChange(int i, bool set){ guest_scene_change_[i] = set; }

	//-------------------------------------
	// パラメータ
	//-------------------------------------
	CollisionManager *collision_manager(){
		return collision_manager_;
	}
	ObjectManager *object_manager(){
		return object_manager_;
	}
	CameraManager *camera_manager(){
		return camera_manager_;
	}
	Object* field();

private:
	void Matching();
	void Game();
	void Result();

	void ChangeServerState(SERVER_STATE next);

	SERVER_STATE		server_state_;
	int					scene_state_;
	ObjectManager		*object_manager_;
	CameraManager		*camera_manager_;
	CollisionManager	*collision_manager_;
	DebugFont			*font_;
	bool				guest_scene_change_[MAX_GUEST];
	float				camera_pos_len_[MAX_GUEST];
	int					bullet_count_;
	int					time_;

	//-------------------------------------
	// ゲームルール用パラメータ
	//-------------------------------------
	int shot_late[MAX_GUEST];
	// ステージ(砦が壊されるごとに進行/1からスタート)
	int stage_;
	// 砦高さ補正
	float fort_y[3];
	// おじデバフフラグ
	bool grandfather_debuff_;
	// 子供死亡フラグ
	bool child_death_[MAX_GUEST - 1];
	// 子供リスポーンタイマー
	int child_respawn_waittime_[MAX_GUEST - 1];
	// じじいダッシュ
	int dash_effect_timer_;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
