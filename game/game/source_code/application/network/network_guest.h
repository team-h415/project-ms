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
	static void StartCommunication(SceneManager *set);		// �ʐM���J�n���܂�
	static void TrySearchHost(void);						// �z�X�g��T���܂�
	static void SendTo(NETWORK_DATA network_data);			// �f�[�^���M
	static void CloseCommunication();						// �ʐM���I�����܂�

	//-------------------------------------
	// �p�����[�^
	//-------------------------------------
	static int id(){ return id_; }
	static bool disco_host(){ return disco_host_; }
	static int winner(){ return winner_; }
	static bool rec_data_flag(){ return rec_data_flag_; }

	static bool			deta_stop_;							// �f�[�^�������X�g�b�v����t���O

private:
	static unsigned __stdcall Communication();				// �z�X�g����̎�M���s���܂��A��{�I�ɃT�u�X���b�h�ŉғ����܂�
	static void ObjDataAdaptation(
		ObjectManager *object_manager, CameraManager *camera_manager, EffectManager *effect_manager, NETWORK_DATA rec_data);

	static SOCKET		socket_data_;						// �\�P�b�g
	static ULONG		host_addr_;							// �z�X�g�A�h���X
	static int			id_;								// ������ID
	static MyThread		*thread_;							// �X���b�h
	static SceneManager	*scene_manager_;					// �V�[���}�l�[�W���[
	static int			winner_;							// ����
	static bool			disco_host_;						// �z�X�g�����t���O
	static bool			rec_data_flag_;						// �f�[�^�K�����t���O

	NetworkGuest(){}
	~NetworkGuest(){}
};


#endif //__NetworkGuest_H__


//-------------------------------------
// end of file
//-------------------------------------