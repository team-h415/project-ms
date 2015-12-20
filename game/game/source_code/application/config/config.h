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
#define GRANDFATHER_POSITION { 30.75f, 0.00f, -39.26f }
#define GRANDFATHER_ROTATION { 0.00f, 4.68f, 0.00f }




#define PORTAL_POSITION { 20.0f, 0.30f, -30.00f }
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

#define BOMB_OFFSET_ROT (D3DX_PI * 0.1f)			// �Q�[������󂯎������]�𒲐�����l
#define BOMB_DEF_SPEED_XZ 0.2f						// �{���̏������xXZ
#define BOMB_DEF_SPEED_Y 0.05f						// �{���̏������xY
#define BOMB_ADD_SPEED_Y 0.3f						// ���ˊp����{���ɉ��Z����鑬�x�̒����l
#define BOMB_GRAVITY 0.01f
#define BOMB_TIMER 120

#define GAME_TIMER (100)							// �Q�[���̎���

#define MAX_BULLET (100)							// �o���b�g�ő吔

#define MESSAGE_DISP_POSITION_Y 500.0f				// ���b�Z�[�W�\�����W


//-------------------------------------
// ����

// �̗�
#define GRANDFATHER_LIFE 1.0f
// ���Q�[�W
#define GRANDFATHER_WATERGAUGE 1.0f
#define GRANDFATHER_SUB_BULLET_WATERGAUGE 0.01f
#define GRANDFATHER_SUB_BOMB_WATERGAUGE 0.7f

// ���炤�_���[�W��
#define GRANDFATHER_DAMAGE 0.01f
// ���W
#define GRANDFATHER_POSITION_STAGE1 { -28.76f, 0.30f, 69.61f }
#define GRANDFATHER_POSITION_STAGE2 { 27.30f, 0.00f, 5.67f }
#define GRANDFATHER_POSITION_STAGE3 { -2.36f, 0.00f, -78.30f }
// �p�x
#define GRANDFATHER_ROTATION_STAGE1 1.70f
#define GRANDFATHER_ROTATION_STAGE2  2.21f
#define GRANDFATHER_ROTATION_STAGE3  -0.23f
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
// ���W�A�p�x
#define CHILD1_POSITION1 { 37.58f, 0.00f, 75.41f }
#define CHILD1_ROTATION1 { 0.00f, -1.49f, 0.00f }
#define CHILD2_POSITION1 { 34.39f, 0.00f, 77.99f }
#define CHILD2_ROTATION1 { 0.00f, -1.49f, 0.00f }
#define CHILD3_POSITION1 { 34.49f, 0.00f, 82.64f }
#define CHILD3_ROTATION1 { 0.00f, -1.49f, 0.00f }
#define CHILD4_POSITION1 { 36.32f, 0.00f, 85.72f }
#define CHILD4_ROTATION1 { 0.00f, -1.49f, 0.00f }

#define CHILD1_POSITION2 { -36.52f, 0.30f, 61.88f }
#define CHILD1_ROTATION2 { 0.00f, 2.29f, 0.00f }
#define CHILD2_POSITION2 { -32.16f, 0.30f, 64.62f }
#define CHILD2_ROTATION2 { 0.00f, 2.29f, 0.00f }
#define CHILD3_POSITION2 { -30.39f, 0.30f, 69.10f }
#define CHILD3_ROTATION2 { 0.00f, 2.29f, 0.00f }
#define CHILD4_POSITION2 { -31.84f, 0.30f, 73.71f }
#define CHILD4_ROTATION2 { 0.00f, 2.29f, 0.00f }

#define CHILD1_POSITION3 { 27.03f, 0.00f, -12.50f }
#define CHILD1_ROTATION3 { 0.00f, -2.47f, 0.00f }
#define CHILD2_POSITION3 { 28.01f, 0.00f, -16.77f }
#define CHILD2_ROTATION3 { 0.00f, -2.47f, 0.00f }
#define CHILD3_POSITION3 { 30.53f, 0.00f, -19.73f }
#define CHILD3_ROTATION3 { 0.00f, -2.47f, 0.00f }
#define CHILD4_POSITION3 { 35.23f, 0.00f, -20.31f }
#define CHILD4_ROTATION3 { 0.00f, -2.47f, 0.00f }
// ���X�|�[���҂�����
#define CHILD_RESPAWN_WAITTIME 120
// HP�����񕜑҂�����
#define CHILD_RECOVER_WAITE_TIME 180
// HP�񕜗�
#define CHILD_RECOVER_HP 0.003f


//-------------------------------------
// ��

// ���W
#define FORT1_POSITION { -37.86f, 0.30f, 68.30f }
#define FORT2_POSITION { 35.51f, 0.30f, -1.87f }
#define FORT3_POSITION { 0.53f, 0.30f, -88.43f }
// �̗�
#define FORT1_LiFE 1.0f
#define FORT2_LiFE 1.0f
#define FORT3_LiFE 1.0f
// ���炤�_���[�W��
#define FORT_DAMAGE 0.01f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
