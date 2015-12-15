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
class ObjectManager;
class CameraManager;
class EffectManager;
class DebugFont;
class CollisionManager;
class GameServer : public Scene
{
public:
	GameServer();
	virtual ~GameServer();
	void Update();
	void Draw();

	void SetGuestSceneChange(int i, bool set){ guest_scene_change_[i] = set; }

	//-------------------------------------
	// �p�����[�^
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
	//EffectManager *effect_manager(){
	//	return effect_manager_;
	//}

private:
	void MatchingAndGame();
	void ChangeState(SERVER_STATE next);

	SERVER_STATE		state;
	ObjectManager		*object_manager_;
	CameraManager		*camera_manager_;
	CollisionManager	*collision_manager_;
	DebugFont			*font_;
	bool				guest_scene_change_[MAX_GUEST];
	float				camera_pos_len_[MAX_GUEST];
	int					bullet_count_;
	int					time_;

	//-------------------------------------
	// �Q�[�����[���p�p�����[�^
	//-------------------------------------
	int shot_late[MAX_GUEST];
	// �X�e�[�W(�Ԃ��󂳂�邲�Ƃɐi�s/1����X�^�[�g)
	int stage_;
	// �����f�o�t�t���O
	bool grandfather_debuff_;
	// �q�����S�t���O
	bool child_death_[MAX_GUEST - 1];
	// �q�����X�|�[���^�C�}�[
	int child_respawn_waittime_[MAX_GUEST - 1];
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
