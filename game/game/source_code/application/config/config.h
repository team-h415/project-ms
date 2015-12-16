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
// Game / Matching 共通
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
#define CHARANCTER_MOVESPEED 0.05f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Matching
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
#define GRANDFATHER_POSITION { 59.75f, 0.00f, -39.26f };
#define GRANDFATHER_ROTATION { 0.00f, 4.68f, 0.00f };

#define CHILD1_POSITION { 39.42f, 0.00f, -55.70f };
#define CHILD1_ROTATION { 0.00f, -0.03f, 0.00f };

#define CHILD2_POSITION { 42.95f, 0.00f, -55.60f };
#define CHILD2_ROTATION { 0.00f, -0.03f, 0.00f };

#define CHILD3_POSITION { 47.24f, 0.00f, -55.67f };
#define CHILD3_ROTATION { 0.00f, -0.03f, 0.00f };

#define CHILD4_POSITION { 51.98f, 0.00f, -55.83f };
#define CHILD4_ROTATION { 0.00f, -0.03f, 0.00f };


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Game
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//

#define CHAR_ROT_SPEED (D3DX_PI * 0.01f)			// キャラクター回転速度

#define CAMERA_FOCUS_OFFSET_Y 1.0f					// キャラクターの座標が足元基準なので上へ
#define CAMERA_FOCUS_OFFSET 0.5f					// キャラクターの中心からのオフセット
#define CAMERA_POS_LEN 5.5f							// カメラ注視点からカメラ座標までの距離
#define CAMERA_ROT_X_LIMIT (D3DX_PI * 0.4f)			// カメラX回転の限界値
#define CAMERA_ROT_SPEED (D3DX_PI * 0.01f)			// カメラ回転速度
#define CAMARA_LEN_SPEED (CAMERA_POS_LEN * 0.01f)	// カメラ注視点→座標距離の加算スピード

#define CAMERA_SUB_ROT_SPEED (D3DX_PI * 0.007f)		// サブカメラ回転速度

#define BULLET_OFFSET_ROT (D3DX_PI * 0.1f)			// ゲームから受け取った回転を調整する値
#define BULLET_DEF_SPEED_XZ 0.2f					// バレットの初期速度XZ
#define BULLET_DEF_SPEED_Y 0.05f					// バレットの初期速度Y
#define BULLET_ADD_SPEED_Y 0.3f						// 発射角からバレットに加算される速度の調整値
#define BULLET_GRAVITY 0.003f						// バレットに毎フレームかかる重量値

#define GAME_TIMER (100)							// ゲームの時間

#define MAX_BULLET (100)							// バレット最大数
#define LAKE_RANGE	(35.0f)							// 湖の半径
#define LAKE_COLLISION_RANGE (24.5f)				// 湖の当たり半径



//-------------------------------------
// おじ

// 体力
#define GRANDFATHER_LIFE 1.0f
// 水ゲージ
#define GRANDFATHER_WATERGAUGE 1.0f
#define GRANDFATHER_SUB_WATERGAUGE 0.01f
// くらうダメージ量
#define GRANDFATHER_DAMAGE 0.01f
// 座標
#define GRANDFATHER_POSITION_STAGE1 {  40.0f,  0.0f, -40.0f }
#define GRANDFATHER_POSITION_STAGE2 { -60.0f,  0.0f,  55.0f }
#define GRANDFATHER_POSITION_STAGE3 {  40.0f,  0.0f,  65.0f }
// 角度
#define GRANDFATHER_ROTATION_STAGE1 -0.82f
#define GRANDFATHER_ROTATION_STAGE2  3.14f
#define GRANDFATHER_ROTATION_STAGE3  4.30f
// HP自動回復待ち時間
#define GRANDFATHER_RECOVER_WAITE_TIME 180
// HP回復量
#define GRANDFATHER_RECOVER_HP 0.003f


//-------------------------------------
// 子供

// 体力
#define CHILD_LIFE 1.0f
// 水ゲージ
#define CHILD_WATERGAUGE 1.0f
#define CHILD_SUB_WATERGAUGE 0.01f
// くらうダメージ量
#define CHILD_DAMAGE 0.02f
// 座標
static const Vector3 CHILD_POSITION[4]
{
	{-55.0f, 0.0f, -50.0f},
	{-50.0f, 0.0f, -50.0f},
	{-55.0f, 0.0f, -55.0f},
	{-50.0f, 0.0f, -55.0f},
};

// 角度
static const Vector3 CHILD_ROTATION[4]
{
	{0.0f, 0.79f, 0.0f},
	{0.0f, 0.79f, 0.0f},
	{0.0f, 0.79f, 0.0f},
	{0.0f, 0.79f, 0.0f},
};

// リスポーン待ち時間
#define CHILD_RESPAWN_WAITTIME 120
// HP自動回復待ち時間
#define CHILD_RECOVER_WAITE_TIME 180
// HP回復量
#define CHILD_RECOVER_HP 0.003f


#define GAME_TIME 180							// ゲーム時間（秒）

//-------------------------------------
// 砦

// 座標
#define FORT1_POSITION {  50.0f,  0.0f, -50.0f }
#define FORT2_POSITION { -56.0f,  0.0f,  63.0f }
#define FORT3_POSITION {  53.0f,  0.0f,  62.0f }
// 体力
#define FORT1_LiFE 1.0f
#define FORT2_LiFE 1.0f
#define FORT3_LiFE 1.0f
// くらうダメージ量
#define FORT_DAMAGE 0.01f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
