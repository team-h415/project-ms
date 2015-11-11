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
#define NETWORK_HOST_MODE						// exeがホストなのかゲストなのかの分岐 通常はホスト、コメントアウトでゲストに

#define ID_NONE -1								// IDの無い状態
#define PORT_NUMBER_0 20000						// ゲストからホストへの送信時のポート
#define PORT_NUMBER_1 20001						// ホストからゲストへの送信時のポート
#define MULTICAST_ADDRESS "239.0.0.25"			// マルチキャスト用のアドレス


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

	DATA_GAME_START,			// ホストが使用 ゲームの開始命令
	DATA_GAME_END,				// ホストが使用 ゲームの終了命令

	DATA_OBJ_PARAM,				// ホストが使用 オブジェクトのパラメータ情報を送信

	DATA_TEST,			// テスト
};


enum OBJ_TYPE
{
	OBJ_GRANDFATHER,			// おじいちゃん
	OBJ_CHILD,					// 子供
	OBJ_EFFECT,					// エフェクト
	OBJ_UI,						// UI

	OBJ_CAMERA,					// めんどいのでカメラもオブジェクト扱い
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
	OBJ_TYPE	type_;
	VEC3		position_;
	VEC3		rotation_;
	VEC3		scaling_;
};

struct NETWORK_DATA
{
	int			id_;
	DATA_TYPE	type_;

	union
	{
		OBJ_PARAM	object_param_;
	};
};


#endif //__Network_H__


//-------------------------------------
// end of file
//-------------------------------------