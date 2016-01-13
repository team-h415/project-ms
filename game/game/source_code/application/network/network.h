//=========================================================
// network.h
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Network_H__
#define __Network_H__


//-------------------------------------
// define
//-------------------------------------
//#define NETWORK_HOST_MODE						// exeがホストなのかゲストなのかの分岐 通常はホスト、コメントアウトでゲストに

#define ID_NONE -1								// IDの無い状態
#define PORT_NUMBER_0 20000						// ゲストからホストへの送信時のポート
#define PORT_NUMBER_1 20001						// ホストからゲストへの送信時のポート
#define MULTICAST_ADDRESS "239.0.0.25"			// マルチキャスト用のアドレス
#define MAX_GUEST 5								// 接続ゲスト最大数
#define MAX_NAME_LEN 20						// 最大文字数

//-------------------------------------
// enum
//-------------------------------------
enum DATA_TYPE
{
	DATA_REQUEST_ADDR,			// ゲストが使用 ホストにIPアドレスを聞く
	DATA_GIVE_ADDR,				// ホストが使用 ゲストにIPアドレスを教える

	DATA_SCENE_CHANGE_GAME,		// ホストが使用 マッチング画面からゲーム画面への遷移命令
	DATA_SCENE_CHANGE_RESULT,	// ホストが使用 ゲーム画面からリザルト画面への遷移命令
	DATA_SCENE_CHANGE_MATCHING,	// ホストが使用 リザルト画面からマッチング画面への遷移命令

	DATA_COMPLETE_SCENE_CHANGE,	// ゲストが使用 シーンチェンジが完了したことを伝える

	DATA_GAME_WINNER,			// ホストが使用 ゲームの勝者を送る
	DATA_OBJ_PARAM,				// ホストが使用 オブジェクトのパラメータ情報を送信
	DATA_UI_PARAM,				// ホストが使用 UIのパラメータ情報を送信

	DATA_SPRITE2D_TEX,			// スプライト2Dのテクスチャ変更メッセージ
};


enum OBJ_TYPE
{
	OBJ_PLAYER,					// プレイヤーオブジェクト
	OBJ_EFFECT,					// エフェクト
	OBJ_FORT,					// 砦
	OBJ_BULLET,					// 球
	OBJ_UI,						// UI
	OBJ_ARROW,					// ARROW
	OBJ_BOMB,					// ボム
	OBJ_BLIND,					// 目隠し

	OBJ_CAMERA,					// めんどいのでカメラもオブジェクト扱い
};


enum UI_TYPE
{
	UI_TIME,					// おじいちゃん
	UI_LIFE,					// 子供
	UI_WATER,					// エフェクト
	UI_FORT,					// 砦
};


//-------------------------------------
// struct
//-------------------------------------
struct VEC3
{
	float x_, y_, z_;
};

struct OBJ_PARAM
{
	OBJ_TYPE	type_;			// オブジェクトタイプ
	VEC3		position_;		// 座標
	VEC3		rotation_;		// 回転
	int			ex_id_;			// EX項目 FBXのアニメーションIDなどで
};

struct UI_PARAM
{
	UI_TYPE		type_;			// UIタイプ
	int			value_i_;		// 値
	float		value_f_;		// 値
};

struct NETWORK_DATA
{
	int			id_;
	DATA_TYPE	type_;
	char		name_[MAX_NAME_LEN];

	union
	{
		OBJ_PARAM	object_param_;
		UI_PARAM	ui_param_;
	};
};


#endif //__Network_H__


//-------------------------------------
// end of file
//-------------------------------------