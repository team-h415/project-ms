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
class Sound;
class DebugFont;
class CollisionManager;
class Collision;
class Game : public Scene
{
public:
	Game();
	virtual ~Game();
	void Initialize();
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


	DebugFont *font_;
    Sound *sound_;
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
	// 経過フレーム数
	int frame_;
	// 経過時間
	int timer_;
    // サウンドSEが連続で再生させないためのフラグ
    bool sound_se_flg_;
    // ゲーム開始時だけのSEフラグ
    bool start_only_se_flg_;
    // プレイヤー移動時の経過時間
    int walk_timer_;
    // 歩きSEフラグ（確認用）
    bool walk_flg_;
    // デバフエフェクトフラグ
    bool debuff_effect_flg;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
