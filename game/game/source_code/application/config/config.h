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
// Game / Matching
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
#define POSITION_STAGE1 { 1.0f,1.0f,1.0f }


//-------------------------------------
// 子供

// 体力
#define CHILD_LIFE 1.0f
// 水ゲージ
#define CHILD_WATERGAUGE 1.0f
#define CHILD_SUB_wATERGAUGE 0.01f
// くらうダメージ量
#define CHILD_DAMAGE 0.02f


//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//
// Result
//-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-//



#endif //__Config_H__


//-------------------------------------
// end of file
//-------------------------------------
