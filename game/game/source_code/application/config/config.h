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

#define CHAR_ROT_SPEED (D3DX_PI * 0.01f)			// キャラクター回転速度

#define CAMERA_FOCUS_OFFSET_Y 1.0f					// キャラクターの座標が足元基準なので上へ
#define CAMERA_FOCUS_OFFSET 0.5f					// キャラクターの中心からのオフセット
#define CAMERA_POS_LEN 5.5f							// カメラ注視点からカメラ座標までの距離
#define CAMERA_ROT_X_LIMIT (D3DX_PI * 0.4f)			// カメラX回転の限界値
#define CAMERA_ROT_SPEED (D3DX_PI * 0.01f)			// カメラ回転速度
#define CAMARA_LEN_SPEED (CAMERA_POS_LEN * 0.01f)	// カメラ注視点→座標距離の加算スピード

#define BULLET_OFFSET_ROT (D3DX_PI * 0.1f)			// ゲームから受け取った回転を調整する値
#define BULLET_DEF_SPEED_XZ 0.2f					// バレットの初期速度XZ
#define BULLET_DEF_SPEED_Y 0.05f					// バレットの初期速度Y
#define BULLET_ADD_SPEED_Y 0.3f						// 発射角からバレットに加算される速度の調整値
#define BULLET_GRAVITY 0.003f						// バレットに毎フレームかかる重量値


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
#define CHILD_POSITION1 { -55.0f,  0.0f, -50.0f }
#define CHILD_POSITION2 { -50.0f,  0.0f, -50.0f }
#define CHILD_POSITION3 { -55.0f,  0.0f, -55.0f }
#define CHILD_POSITION4 { -50.0f,  0.0f, -55.0f }
// 角度
#define CHILD_ROTATION1 0.79f
#define CHILD_ROTATION2 0.79f
#define CHILD_ROTATION3 0.79f
#define CHILD_ROTATION4 0.79f
// リスポーン待ち時間
#define CHILD_RESPAWN_WAITTIME 120


#define GAME_TIME 180							// ゲーム時間（秒）

//-------------------------------------
// 砦

// 座標
#define FORT1_POSITION {  50.0f,  0.0f, -50.0f }
#define FORT2_POSITION { -56.0f,  0.0f,  63.0f }
#define FORT3_POSITION {  53.0f,  0.0f,  62.0f }
// 体力
#define FORT1_LiFE 0.5f
#define FORT2_LiFE 0.5f
#define FORT3_LiFE 0.5f
// くらうダメージ量
#define FORT_DAMAGE 0.01f;


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
