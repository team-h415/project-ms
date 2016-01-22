//=========================================================
// config.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Config_H__
#define __Config_H__

//-------------------------------------
// include
//-------------------------------------
#include "../math/vector.h"
#include <d3dx9.h>

class Config
{
public:
	static void SetupConfig(void);

	static int GAME_TIME;
	static float CHAR_ROT_SPEED_LR;
	static float CHAR_ROT_SPEED_UD;
	static float GRANDFATHER_MOVESPEED;
	static float GRANDFATHER_RECOVER_WATERGAUGE;
	static float GRANDFATHER_SUB_BULLET_WATERGAUGE;
	static float GRANDFATHER_SUB_BOMB_WATERGAUGE;
	static float GRANDFATHER_DAMAGE;
	static float CHILD_MOVESPEED;
	static float CHILD_RECOVER_WATERGAUGE;
	static float CHILD_SUB_WATERGAUGE;
	static float CHILD_DAMAGE;
	static float GRANDFATHER_RECOVER_HP;
	static int CHILD_RESPAWN_WAITTIME;
	static float CHILD_RECOVER_HP;
	static int CHILD_DAMAGE_SHUT_OUT;
	static int MAX_CHILD_REMAINING_LIVE;
	static float FORT_DAMAGE;
	static float SHIELD_DAMAGE_ATTENUATION;
	static float SHIELD_OFF_LIFE;
	static float SHIELD_ON_LIFE;
};


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Matching
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
static const Vector3 MATCHING_POSITION[5]
{
	{5.75f, 0.00f, -24.69f},
	{5.75f, 0.00f, -28.13f},
	{5.75f, 0.00f, -32.13f},
	{5.75f, 0.00f, -36.13f},
	{5.75f, 0.00f, -40.13f},
};

static const float MATCHING_ROTATION[5]
{
	1.69f,
	1.69f,
	1.69f,
	1.69f,
	1.69f,
};

#define SEA_HEIGHT -0.8f

//-------------------------------------
// �|�[�^��
#define PORTAL_POSITION { 28.63f, 0.3f, -34.24f }
#define PORTAL_DISTANCE 3.0f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Game
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//


//-------------------------------------
// �J����
#define CAMERA_FOCUS_OFFSET_Y 1.0f					// �L�����N�^�[�̍��W��������Ȃ̂ŏ��
#define CAMERA_FOCUS_OFFSET 0.5f					// �L�����N�^�[�̒��S����̃I�t�Z�b�g
#define CAMERA_POS_LEN 5.5f							// �J���������_����J�������W�܂ł̋���
#define CAMERA_ROT_X_LIMIT (D3DX_PI * 0.4f)			// �J����X��]�̌��E�l
#define CAMERA_ROT_SPEED (D3DX_PI * 0.005f)			// �J������]���x
#define CAMARA_LEN_SPEED (CAMERA_POS_LEN * 0.01f)	// �J���������_�����W�����̉��Z�X�s�[�h

#define CAMERA_SUB_ROT_SPEED (D3DX_PI * 0.007f)		// �T�u�J������]���x

//-------------------------------------
// �e
#define BULLET_OFFSET_ROT (D3DX_PI * 0.1f)			// �Q�[������󂯎������]�𒲐�����l
#define BULLET_DEF_SPEED_XZ 0.4f					// �o���b�g�̏������xXZ
#define BULLET_DEF_SPEED_Y 0.05f					// �o���b�g�̏������xY
#define BULLET_ADD_SPEED_Y 0.3f						// ���ˊp����o���b�g�ɉ��Z����鑬�x�̒����l
#define BULLET_GRAVITY 0.003f						// �o���b�g�ɖ��t���[��������d�ʒl

//-------------------------------------
// �{��
#define BOMB_OFFSET_ROT (D3DX_PI * 0.1f)			// �Q�[������󂯎������]�𒲐�����l
#define BOMB_DEF_SPEED_XZ 0.2f						// �{���̏������xXZ
#define BOMB_DEF_SPEED_Y 0.05f						// �{���̏������xY
#define BOMB_ADD_SPEED_Y 0.3f						// ���ˊp����{���ɉ��Z����鑬�x�̒����l
#define BOMB_GRAVITY 0.01f
#define BOMB_TIMER 120


#define MAX_BULLET (100)							// �o���b�g�ő吔
#define MAX_BOMB (50)								// �o���b�g�ő吔

#define MESSAGE_DISP_POSITION_Y 500.0f				// ���b�Z�[�W�\�����W


//-------------------------------------
// ����

// �̗�
#define GRANDFATHER_LIFE 1.0f
// ���Q�[�W
#define GRANDFATHER_WATERGAUGE 1.0f

// ���W
static const Vector3 GRANDFATHER_POSITION_STAGE[]
{
	{-31.72f, 0.30f, 68.73f},
	{29.9f, 0.00f, 5.67f},
	{-1.20f, 0.04f, -80.61f},
};
// �p�x
static const float GRANDFATHER_ROTATION_STAGE[]
{
	1.41f,
	-0.80f,
	-0.23f,
};

//-------------------------------------
// �q��

// �̗�
#define CHILD_LIFE 1.0f
// ���Q�[�W
#define CHILD_WATERGAUGE 1.0f

// ���W
static const Vector3 CHILD_POSITION[]
{
	// �X�e�[�W�P
	{37.58f, 0.00f, 75.41f},
	{34.39f, 0.00f, 77.99f},
	{34.49f, 0.00f, 82.64f},
	{36.32f, 0.00f, 85.72f},
	// �X�e�[�W�Q
	{-28.26f, 0.30f, 71.80f},
	{-32.50f, 0.30f, 65.13f},
	{-34.04f, 0.30f, 75.66f},
	{-38.54f, 0.30f, 68.88f},
	// �X�e�[�W�R
	{35.08f, 0.30f, -10.82f},
	{28.88f, 0.00f, -7.34f},
	{35.21f, 0.30f, -3.25f},
	{28.57f, 0.00f, -0.54f},
};

// �p�x
static const float CHILD_ROTATION[]
{
	// �X�e�[�W�P
	-1.49f,
	-1.49f,
	-1.49f,
	-1.49f,
	// �X�e�[�W�Q
	2.43f,
	2.04f,
	2.42f,
	2.28f,
	// �X�e�[�W�R
	-2.72f,
	-2.81f,
	-2.47f,
	-2.85f,
};

//-------------------------------------
// ��

// ���W
static const Vector3 FORT_POSITION[]
{
	{-37.86f, 0.30f, 68.30f},
	{35.51f, 0.30f, -1.87f},
	{0.53f, 0.30f, -88.43f},
};

// �̗�
static const float FORT_LIFE[]
{
	1.0f,
	1.0f,
	1.0f,
};


//-------------------------------------
// �V�[���h

// ���W
#define SHIELD_POSITION_Y 1.5f


//-------------------------------------
// �u���C���h

// �A���t�@���ޑ��x
#define BLIND_ALPHA_ATTENUATION_SPEED 0.02f

// �ړ��l
#define BLIND_LEN_MIN 150.0f	// ��ʒ�������̍ŏ��ړ��l
#define BLIND_LEN_MAX 450.0f	// ��ʒ�������̍ő�ړ��l

// �X�P�[��
#define BLIND_SCALING_MIN 150	// �ŏ��T�C�Y(int)
#define BLIND_SCALING_MAX 300	// �ő�T�C�Y(int)

// �e�N�X�`��
#define BLIND_TEXTURE_MAX 4		// �g�p��

// �{��
#define BLIND_BOMB_NUM 10				// �{���ɓ����������̃u���C���h�o����
#define BLIND_BOMB_MAGNIFICATION 1.3f	// �{���̎�����������Ƒ傫�߂ɂ���

#define BLIND_DEST_SCALING_SPEED 0.1f	// �g�呬�x

#define MAX_BLIND 300 // �ő吔


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
