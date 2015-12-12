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
	DebugFont *font1_;
	DebugFont *font2_;
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

};


#endif //__ADD_YOUR_HANDS__


//-------------------------------------
// end of file
//-------------------------------------
