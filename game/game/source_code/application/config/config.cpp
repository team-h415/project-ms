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

	fscanf_s(file, "// ゲーム時間\n");
	fscanf_s(file, "%d\n", &GAME_TIME);
	fscanf_s(file, "// カメラ左右回転\n");
	fscanf_s(file, "%f\n", &CHAR_ROT_SPEED_LR);
	fscanf_s(file, "// カメラ上下回転\n");
	fscanf_s(file, "%f\n", &CHAR_ROT_SPEED_UD);
	fscanf_s(file, "// じじ移動速度\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_MOVESPEED);
	fscanf_s(file, "// じじ水回復\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_RECOVER_WATERGAUGE);
	fscanf_s(file, "// じじショット水減少値\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_SUB_BULLET_WATERGAUGE);
	fscanf_s(file, "// じじボム水減少値\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_SUB_BOMB_WATERGAUGE);
	fscanf_s(file, "// じじくらうダメージ\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_DAMAGE);
	fscanf_s(file, "// じじ１フレームHP回復力\n");
	fscanf_s(file, "%f\n", &GRANDFATHER_RECOVER_HP);
	fscanf_s(file, "// 子供移動速度\n");
	fscanf_s(file, "%f\n", &CHILD_MOVESPEED);
	fscanf_s(file, "// 子供水回復\n");
	fscanf_s(file, "%f\n", &CHILD_RECOVER_WATERGAUGE);
	fscanf_s(file, "// 子供ショット水減少値\n");
	fscanf_s(file, "%f\n", &CHILD_SUB_WATERGAUGE);
	fscanf_s(file, "// 子供くらうダメージ\n");
	fscanf_s(file, "%f\n", &CHILD_DAMAGE);
	fscanf_s(file, "// 子供１フレームHP回復力\n");
	fscanf_s(file, "%f\n", &CHILD_RECOVER_HP);
	fscanf_s(file, "// リスポーン待ち時間\n");
	fscanf_s(file, "%d\n", &CHILD_RESPAWN_WAITTIME);
	fscanf_s(file, "// 子供攻撃無効フレーム\n");
	fscanf_s(file, "%d\n", &CHILD_DAMAGE_SHUT_OUT);
	fscanf_s(file, "// 残機\n");
	fscanf_s(file, "%d\n", &MAX_CHILD_REMAINING_LIVE);
	fscanf_s(file, "// 弾１発のダメージ\n");
	fscanf_s(file, "%f\n", &FORT_DAMAGE);
	fscanf_s(file, "// ダメージ減衰率\n");
	fscanf_s(file, "%f\n", &SHIELD_DAMAGE_ATTENUATION);
	fscanf_s(file, "// シールドOFF\n");
	fscanf_s(file, "%f\n", &SHIELD_OFF_LIFE);
	fscanf_s(file, "// シールドON\n");
	fscanf_s(file, "%f\n", &SHIELD_ON_LIFE);

	fclose(file);
}


//-------------------------------------
// end of file
//-------------------------------------
