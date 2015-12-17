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


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Game / Matching ����
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
#define CHARANCTER_MOVESPEED 0.05f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Matching
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
#define GRANDFATHER_POSITION { 59.75f, 0.00f, -39.26f }
#define GRANDFATHER_ROTATION { 0.00f, 4.68f, 0.00f }

#define CHILD1_POSITION { 39.42f, 0.00f, -55.70f }
#define CHILD1_ROTATION { 0.00f, -0.03f, 0.00f }

#define CHILD2_POSITION { 42.95f, 0.00f, -55.60f }
#define CHILD2_ROTATION { 0.00f, -0.03f, 0.00f }

#define CHILD3_POSITION { 47.24f, 0.00f, -55.67f }
#define CHILD3_ROTATION { 0.00f, -0.03f, 0.00f }

#define CHILD4_POSITION { 51.98f, 0.00f, -55.83f }
#define CHILD4_ROTATION { 0.00f, -0.03f, 0.00f }

#define PORTAL_POSITION { 40.00f, 0.00f, -40.00f }
#define PORTAL_DISTANCE 3.0f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Game
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//

#define CHAR_ROT_SPEED (D3DX_PI * 0.01f)			// �L�����N�^�[��]���x

#define CAMERA_FOCUS_OFFSET_Y 1.0f					// �L�����N�^�[�̍��W��������Ȃ̂ŏ��
#define CAMERA_FOCUS_OFFSET 0.5f					// �L�����N�^�[�̒��S����̃I�t�Z�b�g
#define CAMERA_POS_LEN 5.5f							// �J���������_����J�������W�܂ł̋���
#define CAMERA_ROT_X_LIMIT (D3DX_PI * 0.4f)			// �J����X��]�̌��E�l
#define CAMERA_ROT_SPEED (D3DX_PI * 0.01f)			// �J������]���x
#define CAMARA_LEN_SPEED (CAMERA_POS_LEN * 0.01f)	// �J���������_�����W�����̉��Z�X�s�[�h

#define CAMERA_SUB_ROT_SPEED (D3DX_PI * 0.007f)		// �T�u�J������]���x

#define BULLET_OFFSET_ROT (D3DX_PI * 0.1f)			// �Q�[������󂯎������]�𒲐�����l
#define BULLET_DEF_SPEED_XZ 0.2f					// �o���b�g�̏������xXZ
#define BULLET_DEF_SPEED_Y 0.05f					// �o���b�g�̏������xY
#define BULLET_ADD_SPEED_Y 0.3f						// ���ˊp����o���b�g�ɉ��Z����鑬�x�̒����l
#define BULLET_GRAVITY 0.003f						// �o���b�g�ɖ��t���[��������d�ʒl

#define GAME_TIMER (100)							// �Q�[���̎���

#define MAX_BULLET (100)							// �o���b�g�ő吔
#define LAKE_RANGE	(35.0f)							// �΂̔��a
#define LAKE_COLLISION_RANGE (24.5f)				// �΂̓����蔼�a



//-------------------------------------
// ����

// �̗�
#define GRANDFATHER_LIFE 1.0f
// ���Q�[�W
#define GRANDFATHER_WATERGAUGE 1.0f
#define GRANDFATHER_SUB_WATERGAUGE 0.01f
// ���炤�_���[�W��
#define GRANDFATHER_DAMAGE 0.01f
// ���W
static const Vector3 GRANDFATHER_POSITION_STAGE[3]
{
	{  40.0f, 0.0f, -40.0f },
	{ -60.0f, 0.0f, 55.0f },
	{  40.0f, 0.0f, 65.0f },
};
// �p�x
static const float GRANDFATHER_ROTATION_STAGE[3]
{
	-0.82f,
	3.14f,
	4.30f,
};

// HP�����񕜑҂�����
#define GRANDFATHER_RECOVER_WAITE_TIME 180
// HP�񕜗�
#define GRANDFATHER_RECOVER_HP 0.003f


//-------------------------------------
// �q��

// �̗�
#define CHILD_LIFE 1.0f
// ���Q�[�W
#define CHILD_WATERGAUGE 1.0f
#define CHILD_SUB_WATERGAUGE 0.01f
// ���炤�_���[�W��
#define CHILD_DAMAGE 0.02f
// ���W
static const Vector3 CHILD_POSITION[4]
{
	{-55.0f, 0.0f, -50.0f},
	{-50.0f, 0.0f, -50.0f},
	{-55.0f, 0.0f, -55.0f},
	{-50.0f, 0.0f, -55.0f},
};

// �p�x
static const float CHILD_ROTATION[4]
{
	0.79f,
	0.79f,
	0.79f,
	0.79f,
};

// ���X�|�[���҂�����
#define CHILD_RESPAWN_WAITTIME 120
// HP�����񕜑҂�����
#define CHILD_RECOVER_WAITE_TIME 180
// HP�񕜗�
#define CHILD_RECOVER_HP 0.003f


#define GAME_TIME 180							// �Q�[�����ԁi�b�j

//-------------------------------------
// ��

// ���W
static const Vector3 FORT_POSITION[3]
{
	{  50.0f, 0.0f, -50.0f },
	{ -56.0f, 0.0f, 63.0f },
	{  53.0f, 0.0f, 62.0f },
};
// �̗�
static const float FORT_LIFE[3]{
	1.0f,
	1.0f,
	1.0f,
};
// ���炤�_���[�W��
#define FORT_DAMAGE 0.01f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
