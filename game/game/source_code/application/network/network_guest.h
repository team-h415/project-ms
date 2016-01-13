//=========================================================
// network_guest.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __NetworkGuest_H__
#define __NetworkGuest_H__


//-------------------------------------
// include
//-------------------------------------
#include <winsock2.h>
#include <ws2tcpip.h>
#include "network.h"


//-------------------------------------
// pragma
//-------------------------------------
#pragma comment(lib, "ws2_32.lib")
#pragma warning (disable : 4996 )


//-------------------------------------
// class
//-------------------------------------
class MyThread;
class SceneManager;
class ObjectManager;
class CameraManager;
class EffectManager;
class NetworkGuest
{
public:
	static void StartCommunication(SceneManager *set);		// 通信を開始します
	static void TrySearchHost(void);						// ホストを探します
	static void SendTo(NETWORK_DATA network_data);			// データ送信
	static void CloseCommunication();						// 通信を終了します

	//-------------------------------------
	// パラメータ
	//-------------------------------------
	static int id(){ return id_; }
	static bool disco_host(){ return disco_host_; }
	static int winner(){ return winner_; }
	static bool data_process(){ return data_process_; }
	static bool deta_stock(){ return deta_stock_; }
	static bool data_neglect(){ return data_neglect_; }

	static void deta_stock(bool set){ deta_stock_ = set; }
	static void data_neglect(bool set){ data_neglect_ = set; }

private:
	static unsigned __stdcall Communication();				// ホストからの受信を行います、基本的にサブスレッドで稼働します
	static void ObjDataAdaptation(
		ObjectManager *object_manager, CameraManager *camera_manager, EffectManager *effect_manager, NETWORK_DATA rec_data);

	static SOCKET		socket_data_;						// ソケット
	static ULONG		host_addr_;							// ホストアドレス
	static int			id_;								// 自分のID
	static MyThread		*thread_;							// スレッド
	static SceneManager	*scene_manager_;					// シーンマネージャー
	static int			winner_;							// 勝者
	static bool			disco_host_;						// ホスト発見フラグ
	static bool			data_process_;						// 受信データ処理中フラグ
	static bool			deta_stock_;						// 受信データ処理を一度止め、ストックするフラグ
	static bool			data_neglect_;						// 受信データを処理せず、破棄するフラグ

	NetworkGuest(){}
	~NetworkGuest(){}
};


#endif //__NetworkGuest_H__


//-------------------------------------
// end of file
//-------------------------------------