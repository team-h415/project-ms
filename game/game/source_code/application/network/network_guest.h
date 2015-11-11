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
class NetworkGuest
{
public:
	static void StartCommunication(SceneManager *set);		// 通信を開始します
	static void SendTo(NETWORK_DATA network_data);			// データ送信
	static void CloseCommunication();						// 通信を終了します

private:
	static unsigned __stdcall Communication();				// ホストからの受信を行います、基本的にサブスレッドで稼働します

	static SOCKET		socket_data_;						// ソケット
	static ULONG		host_addr_;							// ホストアドレス
	static MyThread		*thread_;							// スレッド
	static SceneManager	*scene_manager_;					// シーンマネージャー

	NetworkGuest(){}
	~NetworkGuest(){}
};


#endif //__NetworkGuest_H__


//-------------------------------------
// end of file
//-------------------------------------