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
	static void StartCommunication(SceneManager *set);		// �ʐM���J�n���܂�
	static void SendTo(NETWORK_DATA network_data);			// �f�[�^���M
	static void CloseCommunication();						// �ʐM���I�����܂�

private:
	static unsigned __stdcall Communication();				// �z�X�g����̎�M���s���܂��A��{�I�ɃT�u�X���b�h�ŉғ����܂�

	static SOCKET		socket_data_;						// �\�P�b�g
	static ULONG		host_addr_;							// �z�X�g�A�h���X
	static MyThread		*thread_;							// �X���b�h
	static SceneManager	*scene_manager_;					// �V�[���}�l�[�W���[

	NetworkGuest(){}
	~NetworkGuest(){}
};


#endif //__NetworkGuest_H__


//-------------------------------------
// end of file
//-------------------------------------