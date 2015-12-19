//=========================================================
// matching.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Matching_H__
#define __Matching_H__


//-------------------------------------
// class
//-------------------------------------
class ObjectManager;
class CameraManager;
class DebugFont;
class CollisionManager;
class Matching : public Scene
{
public:
	Matching();
	virtual ~Matching();
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
	float camera_pos_len_;
	// 経過フレーム数
	int frame_;
	// 経過時間
	int timer_;
	// シーン終了までのフレーム計測(マッチング完了時)
	int movescene_waitframe_;
};


#endif //__Matching_H__


//-------------------------------------
// end of file
//-------------------------------------
