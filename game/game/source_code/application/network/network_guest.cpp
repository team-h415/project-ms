//=========================================================
// network_guest.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "network_guest.h"
#include "../thread/thread.h"
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../math/vector.h"
#include "../input/input.h"
#include "../input/inputs/keyboard.h"
#include "../input/inputs/gamepad.h"
#include "../debug/debug_font.h"
#include "../object/object.h"
#include "../object/object_manager.h"
#include "../object/objects/mesh/field.h"
#include "../effect/effect.h"
#include "../effect/effect_manager.h"
#include "../camera/camera.h"
#include "../camera/camera_manager.h"
#include "../scene/scene.h"
#include "../scene/scene_manager.h"
#include "../scene/scenes/matching.h"
#include "../scene/scenes/game.h"
#include "../scene/scenes/result.h"


//-------------------------------------
// static
//-------------------------------------
SOCKET			NetworkGuest::socket_data_(INVALID_SOCKET);					// �\�P�b�g
ULONG			NetworkGuest::host_addr_(0);								// �z�X�g�A�h���X
MyThread		*NetworkGuest::thread_(nullptr);							// �X���b�h
SceneManager	*NetworkGuest::scene_manager_(nullptr);						// �V�[���}�l�[�W���[


//-------------------------------------
// StartCommunication()
//-------------------------------------
void NetworkGuest::StartCommunication(SceneManager *set)
{
	// �V�[���}�l�[�W���[�|�C���^�Z�b�g
	scene_manager_ = set;

	// �X���b�h�X�^�[�g
	if(thread_ == nullptr)
	{
		thread_ = new MyThread();

		// WinSock������
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		// ��M�X���b�h�N��
		thread_->Create(&NetworkGuest::Communication);

		//printf("�Q�X�g�X���b�h�ғ�\n");
	}
}


//-------------------------------------
// EndCommunication()
//-------------------------------------
void NetworkGuest::CloseCommunication()
{
	// �X���b�h�N���[�Y
	if(thread_ != nullptr)
	{
		thread_->CloseThread(false);
		delete thread_;
		thread_ = nullptr;

		// WinSock�I������
		WSACleanup();

		//printf("�Q�X�g�X���b�h�I��\n");
	}
}


//-------------------------------------
// SendTo(NETWORK_DATA network_data)
//-------------------------------------
void NetworkGuest::SendTo(NETWORK_DATA network_data)
{
	// ���M�ݒ�
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_1);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = host_addr_;
	// ���M
	sendto(socket_data_, (char*)&network_data, sizeof(network_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));
}


//-------------------------------------
// Communication()
//-------------------------------------
unsigned __stdcall NetworkGuest::Communication()
{
	//===================================================================
	// �z�X�g�T��
	//===================================================================

	// �\�P�b�g����
	socket_data_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// �\�P�b�g�ݒ�
	int value(1);
	setsockopt(socket_data_, SOL_SOCKET, SO_BROADCAST, (char*)&value, sizeof(value));

	// ��M�A�h���X�ݒ�
	sockaddr_in rec_addr;
	ZeroMemory(&rec_addr, sizeof(rec_addr));
	rec_addr.sin_port = htons(PORT_NUMBER_1);
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = INADDR_ANY;

	// �o�C���h
	bind(socket_data_, (sockaddr*)&rec_addr, sizeof(rec_addr));

	// ���M�A�h���X�ݒ�
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_0);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

	// �z�X�g�փ��b�Z�[�W���M
	//printf("�z�X�g�փA�h���X���N�G�X�g\n");
	NETWORK_DATA send_data;
	ZeroMemory(&send_data, sizeof(send_data));
	send_data.type_ = DATA_REQUEST_ADDR;
	sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));

	// ��M����
	NETWORK_DATA rec_data;
	int erc;
	sockaddr_in from_addr;
	int from_len(sizeof(from_addr));
	int id(ID_NONE);

	// ��M���[�v
	//printf("�z�X�g����̕Ԏ��ҋ@\n");
	while(true)
	{
		ZeroMemory(&rec_data, sizeof(rec_data));
		erc = recvfrom(socket_data_, (char*)&rec_data, sizeof(rec_data), 0, (sockaddr*)&from_addr, &from_len);
		if(erc == SOCKET_ERROR)
		{
			// �G���[�f�[�^��M
		}
		else
		{
			// �z�X�g����A�h���X�̒ʒm
			if(rec_data.type_ == DATA_GIVE_ADDR)
			{
				host_addr_ = from_addr.sin_addr.s_addr;
				id = rec_data.id_;
				//printf("�z�X�g����A�h���X�EID���擾\n");
				break;
			}
		}
	}

	// �\�P�b�g�N���[�Y
	closesocket(socket_data_);
	socket_data_ = INVALID_SOCKET;

	//===================================================================
	// �󑗐M
	//===================================================================

	// �\�P�b�g����
	socket_data_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// �\�P�b�g�ݒ�
	value = 1;
	setsockopt(socket_data_, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

	// ��M�A�h���X�ݒ�
	ZeroMemory(&rec_addr, sizeof(rec_addr));
	rec_addr.sin_port = htons(PORT_NUMBER_1);
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = INADDR_ANY;

	// �o�C���h
	bind(socket_data_, (sockaddr*)&rec_addr, sizeof(rec_addr));

	// �}���`�L���X�g�ݒ�
	ip_mreq mreq;
	ZeroMemory(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
	mreq.imr_interface.s_addr = INADDR_ANY;
	setsockopt(socket_data_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	// ���M�A�h���X�ݒ�
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_0);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = host_addr_;

	// �z�X�g�փ��b�Z�[�W���M
	//printf("�z�X�g�փe�X�g���M\n");
	//ZeroMemory(&send_data, sizeof(send_data));
	//send_data.type_ = DATA_TEST;
	//send_data.id_ = id;
	//sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));

	// ��M����
	std::string scene_name;

	// ��M���[�v
	//printf("��M�J�n\n");
	while(true)
	{
		ZeroMemory(&rec_data, sizeof(rec_data));
		erc = recvfrom(socket_data_, (char*)&rec_data, sizeof(rec_data), 0, (sockaddr*)&from_addr, &from_len);
		if(erc == SOCKET_ERROR)
		{
			// �G���[�f�[�^��M
		}
		else
		{
			// ���ߕ���
			switch(rec_data.type_)
			{
				case DATA_SCENE_CHANGE_GAME:		// �}�b�`���O��ʂ���Q�[����ʂւ̑J�ږ���
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Matching" == scene_name || "Loading" == scene_name)
					{
						SceneManager::RequestScene("Game");
					}
					break;

				case DATA_SCENE_CHANGE_RESULT:		// �Q�[����ʂ��烊�U���g��ʂւ̑J�ږ���
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Game" == scene_name)
					{
						SceneManager::RequestScene("Result");
					}
					break;

				case DATA_SCENE_CHANGE_MATCHING:	// �^�C�g����ʂ���}�b�`���O��ʂւ̑J�ږ���
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Result" == scene_name)
					{
						SceneManager::RequestScene("Matching");
					}
					break;

				case DATA_GAME_START:				// �Q�[���̊J�n����
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Game" == scene_name)
					{
					}
					break;

				case DATA_GAME_END:					// �Q�[���̏I������
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Game" == scene_name)
					{
						
					}
					break;

				case DATA_OBJ_PARAM:				// �I�u�W�F�N�g�̃p�����[�^���𑗐M
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Matching" == scene_name)
					{
					}
					else if("Game" == scene_name)
					{
						// �Q�[���N���X�擾
						Game *game = reinterpret_cast<Game*>(scene_manager_->GetCurrentScene());
						if(game == nullptr)
						{
							continue;
						}
						switch(rec_data.object_param_.type_)
						{
							case OBJ_GRANDFATHER:			// �����������
							case OBJ_CHILD:					// �q��
								{
									ObjectManager *object_manager = game->GetObjectManager();
									if(object_manager == nullptr)
									{
										continue;
									}
									Object *object;
									switch(rec_data.id_)
									{
										case 0:
											object = object_manager->Get("player1");
											break;
										case 1:
											object = object_manager->Get("player2");
											break;
										case 2:
											object = object_manager->Get("player3");
											break;
										case 3:
											object = object_manager->Get("player4");
											break;
										case 4:
											object = object_manager->Get("player5");
											break;
										default:
											object = object_manager->Get("player1");
											break;
									}
									if(object == nullptr)
									{
										continue;
									}
									Vector3 set_param;
									// ���W
									set_param.x_ = rec_data.object_param_.position_.x_;
									set_param.y_ = rec_data.object_param_.position_.y_;
									set_param.z_ = rec_data.object_param_.position_.z_;
									object->SetPosition(set_param);
									// ��]
									set_param.x_ = rec_data.object_param_.rotation_.x_;
									set_param.y_ = rec_data.object_param_.rotation_.y_;
									set_param.z_ = rec_data.object_param_.rotation_.z_;
									object->SetRotation(set_param);
								}
								break;

							case OBJ_EFFECT:				// �G�t�F�N�g
								{
									EffectManager *effect_manager_ = game->GetEffectManager();
									if(effect_manager_ == nullptr)
									{
										continue;
									}
									EFFECT_PARAMETER_DESC effect_param;
									MyEffect *effect = effect_manager_->Get("water");
									if(effect == nullptr)
									{
										continue;
									}
									effect_param = effect->parameter();
									effect_param.position_.x_ = rec_data.object_param_.position_.x_;
									effect_param.position_.y_ = rec_data.object_param_.position_.y_;
									effect_param.position_.z_ = rec_data.object_param_.position_.z_;
									effect->SetParameter(effect_param);
									effect_manager_->Play("water");
								}
								break;

							case OBJ_UI:					// UI
								{
									
								}
								break;

							case OBJ_CAMERA:
								{
									CameraManager *camera_manager = game->GetCameraManager();
									if(camera_manager == nullptr)
									{
										continue;
									}
									Camera *main_camera = camera_manager->Get("camera1");
									if(main_camera == nullptr)
									{
										continue;
									}
									D3DXVECTOR3 camera_position;
									D3DXVECTOR3 camera_focus;

									camera_position.x = rec_data.object_param_.position_.x_;
									camera_position.y = rec_data.object_param_.position_.y_;
									camera_position.z = rec_data.object_param_.position_.z_;

									camera_focus.x = rec_data.object_param_.rotation_.x_;
									camera_focus.y = rec_data.object_param_.rotation_.y_;
									camera_focus.z = rec_data.object_param_.rotation_.z_;

									main_camera->SetPosition(camera_position);
									main_camera->SetFocus(camera_focus);

								}
								break;

							default:
								break;
						}
					}
					break;

				case DATA_TEST:						// �e�X�g
					//printf("�e�X�g����M\n");
					break;

				default:
					break;
			}
		}
	}

	// �\�P�b�g�N���[�Y
	closesocket(socket_data_);
	socket_data_ = INVALID_SOCKET;

	return 1;
}


//-------------------------------------
// end of file
//-------------------------------------