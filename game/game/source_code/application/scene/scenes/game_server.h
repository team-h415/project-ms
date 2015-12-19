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
	// �Q�[�����[���p�p�����[�^
	//-------------------------------------
	int shot_late_[MAX_GUEST];
	int walk_timer_[MAX_GUEST];
	// �X�e�[�W(�Ԃ��󂳂�邲�Ƃɐi�s/1����X�^�[�g)
	int now_target_fort_;
	// �ԍ����␳
	float fort_y_[3];
	// �����f�o�t�t���O
	bool grandfather_debuff_;
	// �q�����S�t���O
	bool child_death_[MAX_GUEST - 1];
	// �q�����X�|�[���^�C�}�[
	int child_respawn_waittime_[MAX_GUEST - 1];
	// �������_�b�V��
	int dash_effect_timer_;
	// �ԃA�i�E���X�X�e�[�g
	int fort_announce_state_;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
