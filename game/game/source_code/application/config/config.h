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
#define CHARANCTER_MOVESPEED 0.2f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Matching
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
static const Vector3 MATCHING_POSITION[5]
{
	{59.75f, 0.00f, -39.26f},
	{39.42f, 0.00f, -55.70f},
	{42.95f, 0.00f, -55.60f},
	{47.24f, 0.00f, -55.67f},
	{51.98f, 0.00f, -55.83f},
};

static const float MATCHING_ROTATION[5]
{
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
};

#define PORTAL_POSITION { 40.00f, 0.00f, -40.00f }
#define PORTAL_DISTANCE 3.0f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Game
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//

#define CHAR_ROT_SPEED (D3DX_PI * 0.015f)			// キャラクター回転速度

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
static const Vector3 GRANDFATHER_POSITION_STAGE[3]
{
	{28.76f, 0.30f, 69.61f},
	{27.30f, 0.00f, 5.67f},
	{-2.36f, 0.00f, -78.30f},
};
// 角度
static const float GRANDFATHER_ROTATION_STAGE[3]
{
	1.70f,
	2.21f,
	-0.23f,
};

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
static const Vector3 CHILD_POSITION[]
{
	// ステージ１
	{37.58f, 0.00f, 75.41f},
	{34.39f, 0.00f, 77.99f},
	{34.49f, 0.00f, 82.64f},
	{36.32f, 0.00f, 85.72f},
	// ステージ２
	{-36.52f, 0.30f, 61.88f},
	{-32.16f, 0.30f, 64.62f},
	{-30.39f, 0.30f, 69.10f},
	{-31.84f, 0.30f, 73.71f},
	// ステージ３
	{27.03f, 0.00f, -12.50f},
	{28.01f, 0.00f, -16.77f},
	{30.53f, 0.00f, -19.73f},
	{35.23f, 0.00f, -20.31f},
};

// 角度
static const float CHILD_ROTATION[]
{
	// ステージ１
	-1.49f,
	-1.49f,
	-1.49f,
	-1.49f,
	// ステージ２
	0.30f,
	0.30f,
	0.30f,
	0.30f,
	// ステージ３
	-2.47f,
	-2.47f,
	-2.47f,
	-2.47f,
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
static const Vector3 FORT_POSITION[3]
{
	{-37.86f, 0.30f, 68.30f},
	{35.51f, 0.30f, -1.87f},
	{0.53f, 0.30f, -88.43f},
};

// 体力
static const float FORT_LIFE[3]{
	1.0f,
	1.0f,
	1.0f,
};
// くらうダメージ量
#define FORT_DAMAGE 0.05f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
