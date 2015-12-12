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
class CollisionManager;
class Game : public Scene
{
public:
	Game();
	virtual ~Game();
	void Update();
	void Draw();

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
	EffectManager *effect_manager(){
		return effect_manager_;
	}

private:
	ObjectManager *object_manager_;
	CameraManager *camera_manager_;
	EffectManager *effect_manager_;
	CollisionManager *collision_manager_;
	DebugFont *font1_;
	DebugFont *font2_;
	int current_id_;
	float camera_pos_len_;

	//-------------------------------------
	// ゲームルール用パラメータ
	//-------------------------------------
	// ステージ(砦が壊されるごとに進行/1からスタート)
	int stage_;
	// おじデバフフラグ
	bool grandfather_debuff_;
	// 子供死亡フラグ
	bool child_death_;
	// 子供リスポーンタイマー
	int child_respawn_waittime_;
	// ダッシュエフェクトタイマー
	int dash_effect_timer_;
	// 現在表示に使ってるカメラ
	std::string use_camera_name_;

};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
