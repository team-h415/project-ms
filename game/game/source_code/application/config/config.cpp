//=========================================================
// config.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "config.h"
#include <stdio.h>


//-------------------------------------
// static
//-------------------------------------
int Config::GAME_TIME(180);
float Config::CHAR_ROT_SPEED_LR(0.023f);
float Config::CHAR_ROT_SPEED_UD(-0.023f);
float Config::GRANDFATHER_MOVESPEED(0.12f);
float Config::GRANDFATHER_RECOVER_WATERGAUGE(0.005f);
float Config::GRANDFATHER_SUB_BULLET_WATERGAUGE(0.01f);
float Config::GRANDFATHER_SUB_BOMB_WATERGAUGE(0.25f);
float Config::GRANDFATHER_DAMAGE(0.05f);
float Config::GRANDFATHER_RECOVER_HP(0.0005f);
float Config::CHILD_MOVESPEED(0.12f);
float Config::CHILD_RECOVER_WATERGAUGE(0.01f);
float Config::CHILD_SUB_WATERGAUGE(0.01f);
float Config::CHILD_DAMAGE(0.2f);
float Config::CHILD_RECOVER_HP(0.0001f);
int Config::CHILD_RESPAWN_WAITTIME(120);
int Config::CHILD_DAMAGE_SHUT_OUT(120);
int Config::MAX_CHILD_REMAINING_LIVE(20);
float Config::FORT_DAMAGE(0.02f);
float Config::SHIELD_DAMAGE_ATTENUATION(0.25f);
float Config::SHIELD_OFF_LIFE(0.5f);
float Config::SHIELD_ON_LIFE(0.6f);


void Config::SetupConfig(void)
{
	FILE* file;
	fopen_s(&file,"./resource/config.txt", "rt");
	if(file == nullptr)
	{
		return;
	}

	fscanf_s(file, "// �Q�[������\n");
	fscanf_s(file, "%d\n", &GAME_TIME);
	fscanf_s(file, "// �J�������E��]\n");
	fscanf_s(file, "%f\n", &CHAR_ROT_SPEED_LR);
	fscanf_s(file, "// �J�����㉺��]\n");
	fscanf_s(file, "%f\n", &CHAR_ROT_SPEED_UD);
	fscanf_s(file, "// �����ړ����x\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_MOVESPEED);
	fscanf_s(file, "// ��������\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_RECOVER_WATERGAUGE);
	fscanf_s(file, "// �����V���b�g�������l\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_SUB_BULLET_WATERGAUGE);
	fscanf_s(file, "// �����{���������l\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_SUB_BOMB_WATERGAUGE);
	fscanf_s(file, "// �������炤�_���[�W\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_DAMAGE);
	fscanf_s(file, "// �����P�t���[��HP�񕜗�\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_RECOVER_HP);
	fscanf_s(file, "// �q���ړ����x\n");
	fscanf_s(file, "%f\n", &CHILD_MOVESPEED);
	fscanf_s(file, "// �q������\n");
	fscanf_s(file, "%f\n", &CHILD_RECOVER_WATERGAUGE);
	fscanf_s(file, "// �q���V���b�g�������l\n");
	fscanf_s(file, "%f\n", &CHILD_SUB_WATERGAUGE);
	fscanf_s(file, "// �q�����炤�_���[�W\n");
	fscanf_s(file, "%f\n", &CHILD_DAMAGE);
	fscanf_s(file, "// �q���P�t���[��HP�񕜗�\n");
	fscanf_s(file, "%f\n", &CHILD_RECOVER_HP);
	fscanf_s(file, "// ���X�|�[���҂�����\n");
	fscanf_s(file, "%d\n", &CHILD_RESPAWN_WAITTIME);
	fscanf_s(file, "// �q���U�������t���[��\n");
	fscanf_s(file, "%d\n", &CHILD_DAMAGE_SHUT_OUT);
	fscanf_s(file, "// �c�@\n");
	fscanf_s(file, "%d\n", &MAX_CHILD_REMAINING_LIVE);
	fscanf_s(file, "// �e�P���̃_���[�W\n");
	fscanf_s(file, "%f\n", &FORT_DAMAGE);
	fscanf_s(file, "// �_���[�W������\n");
	fscanf_s(file, "%f\n", &SHIELD_DAMAGE_ATTENUATION);
	fscanf_s(file, "// �V�[���hOFF\n");
	fscanf_s(file, "%f\n", &SHIELD_OFF_LIFE);
	fscanf_s(file, "// �V�[���hON\n");
	fscanf_s(file, "%f\n", &SHIELD_ON_LIFE);

	fclose(file);
}


//-------------------------------------
// end of file
//-------------------------------------
