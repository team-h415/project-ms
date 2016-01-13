//=========================================================
// network.h
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Network_H__
#define __Network_H__


//-------------------------------------
// define
//-------------------------------------
//#define NETWORK_HOST_MODE						// exe���z�X�g�Ȃ̂��Q�X�g�Ȃ̂��̕��� �ʏ�̓z�X�g�A�R�����g�A�E�g�ŃQ�X�g��

#define ID_NONE -1								// ID�̖������
#define PORT_NUMBER_0 20000						// �Q�X�g����z�X�g�ւ̑��M���̃|�[�g
#define PORT_NUMBER_1 20001						// �z�X�g����Q�X�g�ւ̑��M���̃|�[�g
#define MULTICAST_ADDRESS "239.0.0.25"			// �}���`�L���X�g�p�̃A�h���X
#define MAX_GUEST 5								// �ڑ��Q�X�g�ő吔
#define MAX_NAME_LEN 20						// �ő啶����

//-------------------------------------
// enum
//-------------------------------------
enum DATA_TYPE
{
	DATA_REQUEST_ADDR,			// �Q�X�g���g�p �z�X�g��IP�A�h���X�𕷂�
	DATA_GIVE_ADDR,				// �z�X�g���g�p �Q�X�g��IP�A�h���X��������

	DATA_SCENE_CHANGE_GAME,		// �z�X�g���g�p �}�b�`���O��ʂ���Q�[����ʂւ̑J�ږ���
	DATA_SCENE_CHANGE_RESULT,	// �z�X�g���g�p �Q�[����ʂ��烊�U���g��ʂւ̑J�ږ���
	DATA_SCENE_CHANGE_MATCHING,	// �z�X�g���g�p ���U���g��ʂ���}�b�`���O��ʂւ̑J�ږ���

	DATA_COMPLETE_SCENE_CHANGE,	// �Q�X�g���g�p �V�[���`�F���W�������������Ƃ�`����

	DATA_GAME_WINNER,			// �z�X�g���g�p �Q�[���̏��҂𑗂�
	DATA_OBJ_PARAM,				// �z�X�g���g�p �I�u�W�F�N�g�̃p�����[�^���𑗐M
	DATA_UI_PARAM,				// �z�X�g���g�p UI�̃p�����[�^���𑗐M

	DATA_SPRITE2D_TEX,			// �X�v���C�g2D�̃e�N�X�`���ύX���b�Z�[�W
};


enum OBJ_TYPE
{
	OBJ_PLAYER,					// �v���C���[�I�u�W�F�N�g
	OBJ_EFFECT,					// �G�t�F�N�g
	OBJ_FORT,					// ��
	OBJ_BULLET,					// ��
	OBJ_UI,						// UI
	OBJ_ARROW,					// ARROW
	OBJ_BOMB,					// �{��
	OBJ_BLIND,					// �ډB��

	OBJ_CAMERA,					// �߂�ǂ��̂ŃJ�������I�u�W�F�N�g����
};


enum UI_TYPE
{
	UI_TIME,					// �����������
	UI_LIFE,					// �q��
	UI_WATER,					// �G�t�F�N�g
	UI_FORT,					// ��
};


//-------------------------------------
// struct
//-------------------------------------
struct VEC3
{
	float x_, y_, z_;
};

struct OBJ_PARAM
{
	OBJ_TYPE	type_;			// �I�u�W�F�N�g�^�C�v
	VEC3		position_;		// ���W
	VEC3		rotation_;		// ��]
	int			ex_id_;			// EX���� FBX�̃A�j���[�V����ID�Ȃǂ�
};

struct UI_PARAM
{
	UI_TYPE		type_;			// UI�^�C�v
	int			value_i_;		// �l
	float		value_f_;		// �l
};

struct NETWORK_DATA
{
	int			id_;
	DATA_TYPE	type_;
	char		name_[MAX_NAME_LEN];

	union
	{
		OBJ_PARAM	object_param_;
		UI_PARAM	ui_param_;
	};
};


#endif //__Network_H__


//-------------------------------------
// end of file
//-------------------------------------