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

enum SCENE_STATE
{
	STATE_MATCH_WAITING,
	STATE_GO_GAME_WAITING,
	STATE_GUEST_WAITING,
	STATE_COUNTDOWN,
	STATE_RUN,
	STATE_FORT_OUT,
	STATE_FORT_IN,
	STATE_GAME_END,
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
class Sound;
class FbxGrandfather;
class FbxChild;
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

private:
	void Matching();
	void Game();
	void Result();

	void MatchingGrandfather();
	void MatchingChild();

	void GameGrandfather();
	void GameChild();

	void ChangeServerState(SERVER_STATE next);

	SERVER_STATE		server_state_;
	int					scene_state_;
	ObjectManager		*object_manager_;
	CameraManager		*camera_manager_;
	CollisionManager	*collision_manager_;
	DebugFont			*font_;
	bool				guest_scene_change_[MAX_GUEST];
	float				camera_pos_len_[MAX_GUEST];
	int					time_;
	// BGM
	Sound				*matching_bgm_;
	Sound				*game_bgm_;
	Sound				*result_bgm_;

	//-------------------------------------
	// ゲームルール用パラメータ
	//-------------------------------------
	int shot_late_[MAX_GUEST];
	int walk_timer_[MAX_GUEST];
	// ステージ(砦が壊されるごとに進行/1からスタート)
	int now_target_fort_;
	// 砦高さ補正
	float fort_y_[3];
	// おじデバフフラグ
	bool grandfather_debuff_;
	// 子供死亡フラグ
	bool child_death_[MAX_GUEST - 1];
	// 子供リスポーンタイマー
	int child_respawn_waittime_[MAX_GUEST - 1];
	// じじいダッシュ
	int dash_effect_timer_;
	// 砦アナウンスステート
	int fort_announce_state_;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
