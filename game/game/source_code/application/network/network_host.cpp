//=========================================================
// network_host.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "network.h"
#include "network_host.h"
#include "../thread/thread.h"
#include "../../common/common.h"
#include "../scene/scene.h"
#include "../scene/scene_manager.h"
#include "../scene/scenes/game_server.h"


//-------------------------------------
// static
//-------------------------------------
MyThread*		NetworkHost::thread_(nullptr);
SOCKET			NetworkHost::socket_data_(INVALID_SOCKET);
unsigned long	NetworkHost::guest_addr_[MAX_GUEST];
SceneManager*	NetworkHost::scene_manager_(nullptr);					// シーンマネージャー
int				NetworkHost::access_guest_(0);							// アクセスしてきたゲスト数


//-------------------------------------
// StartCommunication()
//-------------------------------------
void NetworkHost::StartCommunication(SceneManager *set)
{
	scene_manager_ = set;

	// スレッドスタート
	if(thread_ == nullptr)
	{
		thread_ = new MyThread();

		for(int i = 0; i < MAX_GUEST; i++)
		{
			guest_addr_[i] = 0;
		}

		// WinSock初期化
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		// 受信スレッド起動
		access_guest_ = 0;
		thread_->Create(&NetworkHost::Communication);

		printf("ホストスレッド稼働\n");
	}
}


//-------------------------------------
// SendTo(DELI_TYPE deli_type, NETWORK_DATA network_data)
//-------------------------------------
void NetworkHost::SendTo(DELI_TYPE deli_type, NETWORK_DATA network_data)
{
	// 送信設定
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_1);
	send_addr.sin_family = AF_INET;
	switch(deli_type)
	{
		case DELI_GRANDFATHER:
			send_addr.sin_addr.s_addr = guest_addr_[0];
			break;
		case DELI_CHILD1:
			send_addr.sin_addr.s_addr = guest_addr_[1];
			break;
		case DELI_CHILD2:
			send_addr.sin_addr.s_addr = guest_addr_[2];
			break;
		case DELI_CHILD3:
			send_addr.sin_addr.s_addr = guest_addr_[3];
			break;
		case DELI_CHILD4:
			send_addr.sin_addr.s_addr = guest_addr_[4];
			break;
		case DELI_MULTI:
			send_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);
			break;
	}
	// 送信
	sendto(socket_data_, (char*)&network_data, sizeof(network_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));
}


//-------------------------------------
// CloseCommunication()
//-------------------------------------
void NetworkHost::CloseCommunication()
{
	// スレッドクローズ
	if(thread_ != nullptr)
	{
		thread_->CloseThread(false);
		delete thread_;
		thread_ = nullptr;

		// WinSock終了処理
		WSACleanup();
		access_guest_ = 0;

		printf("ホストスレッド終了\n");
	}
}


//-------------------------------------
// Communication()
//-------------------------------------
unsigned __stdcall NetworkHost::Communication()
{
	// ソケット生成
	socket_data_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ソケット設定
	int value(1);
	setsockopt(socket_data_, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

	// 受信アドレス
	sockaddr_in rec_addr;
	ZeroMemory(&rec_addr, sizeof(rec_addr));
	rec_addr.sin_port = htons(PORT_NUMBER_0);
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = INADDR_ANY;

	// バインド
	bind(socket_data_, (sockaddr*)&rec_addr, sizeof(rec_addr));

	// 送信設定
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_1);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);

	// 送信
	NETWORK_DATA send_data;
	ZeroMemory(&send_data, sizeof(send_data));

	// 受信準備
	NETWORK_DATA rec_data;
	int erc;
	sockaddr_in from_addr;
	int from_len(sizeof(from_addr));

	printf("ホスト起動\n");
	while(true)
	{
		erc = recvfrom(socket_data_, (char*)&rec_data, sizeof(rec_data), 0, (sockaddr*)&from_addr, &from_len);
		if(erc == SOCKET_ERROR)
		{
			// エラーデータ受信
		}
		else
		{
			// ホストからアドレスの通知
			switch(rec_data.type_)
			{
				case DATA_REQUEST_ADDR:
					{
						printf("アドレスリクエストを受信\n");
						from_addr.sin_port = htons(PORT_NUMBER_1);
						send_data.type_ = DATA_GIVE_ADDR;
						// IPアドレス登録
						bool no_ip(true);
						for(int i = 0; i < MAX_GUEST; i++)
						{
							// IPアドレス重複チェック
							if(guest_addr_[i] == from_addr.sin_addr.s_addr)
							{
								no_ip = false;
								break;
							}
						}
						// IPアドレスの記録
						if(no_ip)
						{
							guest_addr_[access_guest_] = from_addr.sin_addr.s_addr;
							access_guest_++;
						}
						sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&from_addr, sizeof(from_addr));
						printf("アドレスを返信\n");
					}
					break;

				case DATA_COMPLETE_SCENE_CHANGE:
					// メインループに通知
					{
						GameServer* game_server = dynamic_cast<GameServer*>(scene_manager_->GetCurrentScene());
						game_server->SetGuestSceneChange(rec_data.id_, true);
					}
					break;

				case DATA_TEST:
					//printf("テストを受信\n");
					//send_data.type_ = DATA_TEST;
					//sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));
					//printf("テストを送信\n");
					break;

				default:
					printf("デフォルト\n");
					break;
			}
		}
	}

	// ソケットクローズ
	closesocket(socket_data_);
	socket_data_ = INVALID_SOCKET;

	return 1;
}


//-------------------------------------
// end of file
//-------------------------------------