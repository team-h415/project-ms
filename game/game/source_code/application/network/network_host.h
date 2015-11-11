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
// define
//-------------------------------------
#define MAX_GUEST 5

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
class NetworkHost
{
public:
	static void StartCommunication();										// �ʐM���J�n���܂�
	static void SendTo(DELI_TYPE deli_type, NETWORK_DATA network_data);		// �f�[�^���M
	static void CloseCommunication();										// �ʐM���I�����܂�

private:
	static unsigned __stdcall Communication();								// �z�X�g����̎�M���s���܂��A��{�I�ɃT�u�X���b�h�ŉғ����܂�

	static MyThread*		thread_;										// �X���b�h
	static SOCKET			socket_data_;									// �\�P�b�g
	static unsigned long	guest_addr_[MAX_GUEST];							// �Q�X�g��IP�A�h���X

	NetworkHost();
	virtual ~NetworkHost();
};


#endif //__NetworkHost_H__


//-------------------------------------
// end of file
//-------------------------------------