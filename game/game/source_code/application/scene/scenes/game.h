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
	// �Q�[�����[���p�p�����[�^
	//-------------------------------------
	// �X�e�[�W(�Ԃ��󂳂�邲�Ƃɐi�s/1����X�^�[�g)
	int stage_;
	// �����f�o�t�t���O
	bool grandfather_debuff_;
	// �q�����S�t���O
	bool child_death_;
	// �q�����X�|�[���^�C�}�[
	int child_respawn_waittime_;
	// �_�b�V���G�t�F�N�g�^�C�}�[
	int dash_effect_timer_;
	// ���ݕ\���Ɏg���Ă�J����
	std::string use_camera_name_;
	// �o�߃t���[����
	int frame_;
	// �o�ߎ���
	int timer_;
    // �T�E���hSE���A���ōĐ������Ȃ����߂̃t���O
    bool sound_se_flg_;
    // �Q�[���J�n��������SE�t���O
    bool start_only_se_flg_;
    // �v���C���[�ړ����̌o�ߎ���
    int walk_timer_;
    // ����SE�t���O�i�m�F�p�j
    bool walk_flg_;
    // �f�o�t�G�t�F�N�g�t���O
    bool debuff_effect_flg;
};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
