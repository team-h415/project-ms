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
// Game
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//

#define CHAR_ROT_SPEED (D3DX_PI * 0.01f)			// �L�����N�^�[��]���x

#define CAMERA_FOCUS_OFFSET_Y 1.0f					// �L�����N�^�[�̍��W��������Ȃ̂ŏ��
#define CAMERA_FOCUS_OFFSET 0.5f					// �L�����N�^�[�̒��S����̃I�t�Z�b�g
#define CAMERA_POS_LEN 5.5f							// �J���������_����J�������W�܂ł̋���
#define CAMERA_ROT_X_LIMIT (D3DX_PI * 0.4f)			// �J����X��]�̌��E�l
#define CAMERA_ROT_SPEED (D3DX_PI * 0.01f)			// �J������]���x
#define CAMARA_LEN_SPEED (CAMERA_POS_LEN * 0.01f)	// �J���������_�����W�����̉��Z�X�s�[�h

#define BULLET_OFFSET_ROT (D3DX_PI * 0.1f)			// �Q�[������󂯎������]�𒲐�����l
#define BULLET_DEF_SPEED_XZ 0.2f					// �o���b�g�̏������xXZ
#define BULLET_DEF_SPEED_Y 0.05f					// �o���b�g�̏������xY
#define BULLET_ADD_SPEED_Y 0.3f						// ���ˊp����o���b�g�ɉ��Z����鑬�x�̒����l
#define BULLET_GRAVITY 0.003f						// �o���b�g�ɖ��t���[��������d�ʒl


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
#define GRANDFATHER_POSITION_STAGE1 {  40.0f,  0.0f, -40.0f }
#define GRANDFATHER_POSITION_STAGE2 { -60.0f,  0.0f,  55.0f }
#define GRANDFATHER_POSITION_STAGE3 {  40.0f,  0.0f,  65.0f }
// �p�x
#define GRANDFATHER_ROTATION_STAGE1 -0.82f
#define GRANDFATHER_ROTATION_STAGE2  3.14f
#define GRANDFATHER_ROTATION_STAGE3  4.30f



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
#define CHILD_POSITION1 { -55.0f,  0.0f, -50.0f }
#define CHILD_POSITION2 { -50.0f,  0.0f, -50.0f }
#define CHILD_POSITION3 { -55.0f,  0.0f, -55.0f }
#define CHILD_POSITION4 { -50.0f,  0.0f, -55.0f }
// �p�x
#define CHILD_ROTATION1 0.79f
#define CHILD_ROTATION2 0.79f
#define CHILD_ROTATION3 0.79f
#define CHILD_ROTATION4 0.79f
// ���X�|�[���҂�����
#define CHILD_RESPAWN_WAITTIME 120


#define GAME_TIME 180							// �Q�[�����ԁi�b�j

//-------------------------------------
// ��

// ���W
#define FORT1_POSITION {  50.0f,  0.0f, -50.0f }
#define FORT2_POSITION { -56.0f,  0.0f,  63.0f }
#define FORT3_POSITION {  53.0f,  0.0f,  62.0f }
// �̗�
#define FORT1_LiFE 0.5f
#define FORT2_LiFE 0.5f
#define FORT3_LiFE 0.5f
// ���炤�_���[�W��
#define FORT_DAMAGE 0.01f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
