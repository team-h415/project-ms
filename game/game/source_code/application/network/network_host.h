//=========================================================
// network_host.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __NetworkHost_H__
#define __NetworkHost_H__


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
// enum
//-------------------------------------
enum DELI_TYPE
{
	DELI_GRANDFATHER,
	DELI_CHILD1,
	DELI_CHILD2,
	DELI_CHILD3,
	DELI_CHILD4,
	DELI_MULTI,
};

//-------------------------------------
// class
//-------------------------------------
class MyThread;
class SceneManager;
class NetworkHost
{
public:
	static void StartCommunication(SceneManager *set);						// 通信を開始します
	static void SendTo(DELI_TYPE deli_type, NETWORK_DATA network_data);		// データ送信
	static void CloseCommunication();										// 通信を終了します

private:
	static unsigned __stdcall Communication();								// ホストからの受信を行います、基本的にサブスレッドで稼働します

	static MyThread			*thread_;										// スレッド
	static SOCKET			socket_data_;									// ソケット
	static unsigned long	guest_addr_[MAX_GUEST];							// ゲストのIPアドレス
	static SceneManager		*scene_manager_;								// シーンマネージャー

	NetworkHost();
	virtual ~NetworkHost();
};


#endif //__NetworkHost_H__


//-------------------------------------
// end of file
//-------------------------------------