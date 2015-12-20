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
#include "../font/debug_font.h"
#include "../object/object.h"
#include "../object/object_manager.h"
#include "../object/objects/mesh/field.h"
#include "../object/objects/sprite/sprite2d.h"
#include "../object/objects/sprite/number.h"
#include "../object/objects/sprite/timer.h"
#include "../object/objects/sprite/fort_gauge_manager.h"
#include "../object/objects/sprite/water_gage.h"
#include "../object/objects/sprite/damage_effect.h"
#include "../object/objects/sprite/damage_effect.h"
#include "../object/objects/sprite/water_gage.h"
#include "../object/objects/sprite/countdown.h"
#include "../object/objects/model/x_model.h"
#include "../object/objects/model/fbx_model.h"
#include "../object/objects/bullet/bullet.h"
#include "../object/objects/bullet/bomb.h"
#include "../object/objects/model/fbx/fbx_player.h"
#include "../object/objects/model/fbx/fbx_grandfather.h"
#include "../object/objects/model/fbx/fbx_child.h"
#include "../object/objects/sprite/message/message.h"
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
SOCKET			NetworkGuest::socket_data_(INVALID_SOCKET);					// ソケット
ULONG			NetworkGuest::host_addr_(0);								// ホストアドレス
MyThread		*NetworkGuest::thread_(nullptr);							// スレッド
int				NetworkGuest::id_(ID_NONE);									// ID
SceneManager	*NetworkGuest::scene_manager_(nullptr);						// シーンマネージャー
int				NetworkGuest::winner_(0);									// 勝者
bool			NetworkGuest::disco_host_(false);							// ホスト発見フラグ

//-------------------------------------
// StartCommunication()
//-------------------------------------
void NetworkGuest::StartCommunication(SceneManager *set)
{
	// シーンマネージャーポインタセット
	scene_manager_ = set;

	// スレッドスタート
	if(thread_ == nullptr)
	{
		thread_ = new MyThread();

		// WinSock初期化
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		// 受信スレッド起動
		disco_host_ = false;
		thread_->Create(&NetworkGuest::Communication);

		//printf("ゲストスレッド稼働\n");
	}
}


//-------------------------------------
// RetrySearchHost()
//-------------------------------------
void NetworkGuest::TrySearchHost(void)
{
	// 送信アドレス設定
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_0);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

	NETWORK_DATA send_data;
	ZeroMemory(&send_data, sizeof(send_data));
	send_data.type_ = DATA_REQUEST_ADDR;
	sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));
}

//-------------------------------------
// EndCommunication()
//-------------------------------------
void NetworkGuest::CloseCommunication()
{
	// スレッドクローズ
	if(thread_ != nullptr)
	{
		thread_->CloseThread(false);
		delete thread_;
		thread_ = nullptr;

		// WinSock終了処理
		WSACleanup();

		//printf("ゲストスレッド終了\n");
	}
}


//-------------------------------------
// SendTo(NETWORK_DATA network_data)
//-------------------------------------
void NetworkGuest::SendTo(NETWORK_DATA network_data)
{
	// ID設定
	network_data.id_ = id_;
	// 送信設定
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_0);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = host_addr_;
	// 送信
	sendto(socket_data_, (char*)&network_data, sizeof(network_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));
}


//-------------------------------------
// Communication()
//-------------------------------------
unsigned __stdcall NetworkGuest::Communication()
{
	//===================================================================
	// ホスト探索
	//===================================================================

	// ソケット生成
	socket_data_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ソケット設定
	int value(1);
	setsockopt(socket_data_, SOL_SOCKET, SO_BROADCAST, (char*)&value, sizeof(value));

	// 受信アドレス設定
	sockaddr_in rec_addr;
	ZeroMemory(&rec_addr, sizeof(rec_addr));
	rec_addr.sin_port = htons(PORT_NUMBER_1);
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = INADDR_ANY;

	// バインド
	bind(socket_data_, (sockaddr*)&rec_addr, sizeof(rec_addr));

	// 親の探索
	TrySearchHost();

	// 受信準備
	NETWORK_DATA rec_data;
	int erc;
	sockaddr_in from_addr;
	int from_len(sizeof(from_addr));
	int id(ID_NONE);

	// 受信ループ
	//printf("ホストからの返事待機\n");
	while(true)
	{
		ZeroMemory(&rec_data, sizeof(rec_data));
		erc = recvfrom(socket_data_, (char*)&rec_data, sizeof(rec_data), 0, (sockaddr*)&from_addr, &from_len);
		if(erc == SOCKET_ERROR)
		{
			// エラーデータ受信
		}
		else
		{
			// ホストからアドレスの通知
			if(rec_data.type_ == DATA_GIVE_ADDR)
			{
				host_addr_ = from_addr.sin_addr.s_addr;
				id_ = rec_data.id_;
				disco_host_ = true;
				//printf("ホストからアドレス・IDを取得\n");
				break;
			}
		}
	}

	// ソケットクローズ
	closesocket(socket_data_);
	socket_data_ = INVALID_SOCKET;

	//===================================================================
	// 受送信
	//===================================================================

	// ソケット生成
	socket_data_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ソケット設定
	value = 1;
	setsockopt(socket_data_, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

	// 受信アドレス設定
	ZeroMemory(&rec_addr, sizeof(rec_addr));
	rec_addr.sin_port = htons(PORT_NUMBER_1);
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = INADDR_ANY;

	// バインド
	bind(socket_data_, (sockaddr*)&rec_addr, sizeof(rec_addr));

	// マルチキャスト設定
	ip_mreq mreq;
	ZeroMemory(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
	mreq.imr_interface.s_addr = INADDR_ANY;
	setsockopt(socket_data_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	// 送信アドレス設定
	sockaddr_in send_addr;
	ZeroMemory(&send_addr, sizeof(send_addr));
	send_addr.sin_port = htons(PORT_NUMBER_0);
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = host_addr_;

	// ホストへメッセージ送信
	//printf("ホストへテスト送信\n");
	//ZeroMemory(&send_data, sizeof(send_data));
	//send_data.type_ = DATA_TEST;
	//send_data.id_ = id;
	//sendto(socket_data_, (char*)&send_data, sizeof(send_data), 0, (sockaddr*)&send_addr, sizeof(send_addr));

	// 受信準備
	std::string scene_name;

	// 受信ループ
	//printf("受信開始\n");
	while(true)
	{
		ZeroMemory(&rec_data, sizeof(rec_data));
		erc = recvfrom(socket_data_, (char*)&rec_data, sizeof(rec_data), 0, (sockaddr*)&from_addr, &from_len);
		if(erc == SOCKET_ERROR)
		{
			// エラーデータ受信
		}
		else
		{
			// 命令分岐
			switch(rec_data.type_)
			{
				case DATA_SCENE_CHANGE_GAME:		// マッチング画面からゲーム画面への遷移命令
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Matching" == scene_name || "Loading" == scene_name)
					{
						SceneManager::RequestScene("Game");
					}
					break;

				case DATA_SCENE_CHANGE_RESULT:		// ゲーム画面からリザルト画面への遷移命令
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Game" == scene_name)
					{
						SceneManager::RequestScene("Result");
					}
					break;

				case DATA_SCENE_CHANGE_MATCHING:	// リザルト画面からマッチング画面への遷移命令
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Result" == scene_name)
					{
						SceneManager::RequestScene("Matching");
					}
					break;

				case DATA_GAME_WINNER:
					{
						// 勝者保存
						winner_ = rec_data.id_;
						// ゲーム画面に勝敗表示
						scene_name = scene_manager_->GetCurrentSceneName();
						if("Game" == scene_name)
						{
							// ゲームクラス取得
							Game *game = dynamic_cast<Game*>(scene_manager_->GetCurrentScene());
							if(game == nullptr)
							{
								continue;
							}
							if(!game->setup())
							{
								continue;
							}
							Sprite2D* result_sprite = dynamic_cast<Sprite2D*>(game->object_manager()->Get("result_sprite"));
							if(result_sprite == nullptr)
							{
								continue;
							}
							if(winner_ == 0)
							{
								// じじい勝利
								if(id_ == 0)
								{
									// 自分じじい
									result_sprite->SetTexture("resource/texture/game/win.png");
								}
								else
								{
									// 自分子供
									result_sprite->SetTexture("resource/texture/game/lose.png");
								}
							}
							else
							{
								// 子供勝利
								if(id_ == 0)
								{
									// 自分じじい
									result_sprite->SetTexture("resource/texture/game/lose.png");
								}
								else
								{
									// 自分子供
									result_sprite->SetTexture("resource/texture/game/win.png");
								}
							}
							result_sprite->SetPosition({SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f});
						}
					}
					break;

				case DATA_OBJ_PARAM:				// オブジェクトのパラメータ情報を送信
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Matching" == scene_name)
					{
						// マッチングクラス取得
						Matching *matching = dynamic_cast<Matching*>(scene_manager_->GetCurrentScene());
						if(matching == nullptr)
						{
							continue;
						}
						if(!matching->setup())
						{
							continue;
						}
						ObjDataAdaptation(matching->object_manager(),
							matching->camera_manager(), matching->effect_manager(), rec_data);
					}
					else if("Game" == scene_name)
					{
						// ゲームクラス取得
						Game *game = dynamic_cast<Game*>(scene_manager_->GetCurrentScene());
						if(game == nullptr)
						{
							continue;
						}
						if(!game->setup())
						{
							continue;
						}
						ObjDataAdaptation(game->object_manager(),
							game->camera_manager(), game->effect_manager(), rec_data);
					}
					break;

				case DATA_UI_PARAM:
					scene_name = scene_manager_->GetCurrentSceneName();
					if("Game" == scene_name)
					{
						Game *game = dynamic_cast<Game*>(scene_manager_->GetCurrentScene());
						if(game == nullptr)
						{
							continue;
						}
						if(!game->setup())
						{
							continue;
						}
						ObjectManager* object_manager = game->object_manager();
						if(object_manager == nullptr)
						{
							continue;
						}
						std::string name = rec_data.name_;
						if(name == "time")
						{
							Object *object = object_manager->Get(name);
							if(object == nullptr)
							{
								continue;
							}
							Timer *timer = dynamic_cast<Timer*>(object);
							timer->SetValue(rec_data.ui_param_.value_i_);
						}

						else if(name == "water_gage")
						{
							Object *object = object_manager->Get(name);
							if(object == nullptr)
							{
								continue;
							}
							WaterGage *water_gage = dynamic_cast<WaterGage*>(object);
							water_gage->SetChangeValue(rec_data.ui_param_.value_f_);
						}

						else if(name == "damage_effect")
						{
							Object *object = object_manager->Get(name);
							if(object == nullptr)
							{
								continue;
							}
							DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(object);
							damage_effect->SetHP(rec_data.ui_param_.value_f_);
						}

						else if(name == "fort_gauge_manager")
						{
							Object *object = object_manager->Get(name);
							if(object == nullptr)
							{
								continue;
							}
							FortGaugeManager* fort_gauge_manager = dynamic_cast<FortGaugeManager*>(object);
							fort_gauge_manager->SetFortLife(rec_data.id_, rec_data.ui_param_.value_f_);
						}

						else if(name == "countdown")
						{
							Object *object = object_manager->Get(name);
							if(object == nullptr)
							{
								continue;
							}
							CountDown* countdown = dynamic_cast<CountDown*>(object);
							countdown->Play(rec_data.ui_param_.value_i_);
						}
					}
					break;

				case DATA_SPRITE2D_TEX:
					{
						scene_name = scene_manager_->GetCurrentSceneName();
						if("Matching" == scene_name)
						{
							// マッチングクラス取得
							Matching *matching = dynamic_cast<Matching*>(scene_manager_->GetCurrentScene());
							if(matching == nullptr)
							{
								continue;
							}
							if(!matching->setup())
							{
								continue;
							}
							Sprite2D* sprite = dynamic_cast<Sprite2D*>
												(matching->object_manager()->Get("standby"));
							if(sprite == nullptr)
							{
								continue;
							}
							sprite->SetTexture("resource/texture/matching/ready.png");
						}
					}
					break;

				default:
					break;
			}
		}
	}

	// ソケットクローズ
	closesocket(socket_data_);
	socket_data_ = INVALID_SOCKET;

	return 1;
}


void NetworkGuest::ObjDataAdaptation(
	ObjectManager *object_manager, CameraManager *camera_manager, EffectManager *effect_manager, NETWORK_DATA rec_data)
{
	switch(rec_data.object_param_.type_)
	{
		case OBJ_PLAYER:
			{
				if(object_manager == nullptr)
				{
					return;
				}
				std::string player_str = "player" + std::to_string(rec_data.id_);
				Object *object = object_manager->Get(player_str);
				if(object == nullptr)
				{
					return;
				}
				FbxPlayer *player = dynamic_cast<FbxPlayer*>(object);
				if(player == nullptr)
				{
					return;
				}
				Vector3 set_param;
				// 回転
				set_param.x_ = rec_data.object_param_.rotation_.x_;
				set_param.y_ = rec_data.object_param_.rotation_.y_;
				set_param.z_ = rec_data.object_param_.rotation_.z_;
				object->SetRotation(set_param);
				// 座標
				set_param.x_ = rec_data.object_param_.position_.x_;
				set_param.y_ = rec_data.object_param_.position_.y_;
				set_param.z_ = rec_data.object_param_.position_.z_;
				object->SetPosition(set_param);
				if(player->GetCurrentAnimationId() != rec_data.object_param_.ex_id_)
				{
					player->PlayAnimation(rec_data.object_param_.ex_id_);
				}
				// 影
				std::string shadow_str = "shadow" + std::to_string(rec_data.id_);
				Object *shadow = object_manager->Get(shadow_str);
				if(shadow == nullptr)
				{
					return;
				}
				set_param.y_ += 0.001f;
				shadow->SetPosition(set_param);
			}
			break;

		case OBJ_EFFECT:				// エフェクト
			{
				if(effect_manager == nullptr)
				{
					return;
				}

				EFFECT_PARAMETER_DESC effect_param;
				std::string name = rec_data.name_;
				if(name == "marker")
				{
					EFFECT_PARAMETER_DESC effect_param;
					MyEffect *effect = effect_manager->Get(name);
					effect_param = effect->parameter();
					effect_param.position_.x_ = rec_data.object_param_.position_.x_;
					effect_param.position_.y_ = rec_data.object_param_.position_.y_;
					effect_param.position_.z_ = rec_data.object_param_.position_.z_;
					effect_param.rotation_ = {0.0f, 0.0f, 0.0f};
					effect_param.rotation_.y_ = rec_data.object_param_.rotation_.y_;
					effect_param.scaling_ = {	rec_data.object_param_.rotation_.x_,
												rec_data.object_param_.rotation_.x_,
												rec_data.object_param_.rotation_.x_};
					effect->SetParameter(effect_param);
				}
				else if(name == "SpeedDown")
				{
					EFFECT_PARAMETER_DESC effect_param;
					MyEffect *effect = effect_manager->Get(name);
					effect_param = effect->parameter();
					effect_param.position_.x_ = rec_data.object_param_.position_.x_;
					effect_param.position_.y_ = rec_data.object_param_.position_.y_;
					effect_param.position_.z_ = rec_data.object_param_.position_.z_;
					effect_param.rotation_ = {0.0f, 0.0f, 0.0f};
					effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
					effect_param.speed_ = 1.0f;
					effect->SetParameter(effect_param);
				}
				else
				{
					MyEffect *effect = effect_manager->Get(name);
					if(effect == nullptr)
					{
						return;
					}
					effect_param = effect->parameter();
					effect_param.position_.x_ = rec_data.object_param_.position_.x_;
					effect_param.position_.y_ = rec_data.object_param_.position_.y_;
					effect_param.position_.z_ = rec_data.object_param_.position_.z_;

					effect_param.rotation_.x_ = rec_data.object_param_.rotation_.x_;
					effect_param.rotation_.y_ = rec_data.object_param_.rotation_.y_;
					effect_param.rotation_.z_ = rec_data.object_param_.rotation_.z_;

					effect->SetParameter(effect_param);
					effect_manager->Play(name);
				}
			}
			break;

		case OBJ_FORT:					// UI
			{
				if(object_manager == nullptr)
				{
					return;
				}
				std::string name = rec_data.name_;
				Object *object = object_manager->Get(name);
				if(object == nullptr)
				{
					return;
				}
				Vector3 set_param;
				// 回転
				set_param.x_ = rec_data.object_param_.rotation_.x_;
				set_param.y_ = rec_data.object_param_.rotation_.y_;
				set_param.z_ = rec_data.object_param_.rotation_.z_;
				object->SetRotation(set_param);
				// 座標
				set_param.x_ = rec_data.object_param_.position_.x_;
				set_param.y_ = rec_data.object_param_.position_.y_;
				set_param.z_ = rec_data.object_param_.position_.z_;
				object->SetPosition(set_param);
			}
			break;

		case OBJ_BULLET:
			{
				if(object_manager == nullptr)
				{
					return;
				}
				std::string name = rec_data.name_;
				if(rec_data.object_param_.ex_id_ == 0)
				{
					// 0なら更新
					Object *object = object_manager->Get(name);
					if(object == nullptr)
					{
						return;
					}
					Bullet *bullet = dynamic_cast<Bullet*>(object);
					Vector3 pos, rot, scl;
					pos.x_ = rec_data.object_param_.position_.x_;
					pos.y_ = rec_data.object_param_.position_.y_;
					pos.z_ = rec_data.object_param_.position_.z_;

					rot.x_ = rec_data.object_param_.rotation_.x_;
					rot.y_ = rec_data.object_param_.rotation_.y_;
					rot.z_ = rec_data.object_param_.rotation_.z_;

					scl = {1.0f, 1.0f, 1.0f};

					bullet->SetPosition(pos);
					bullet->SetRotation(rot);
					bullet->SetScaling(scl);
					bullet->SetUse(true);
				}
				else if(rec_data.object_param_.ex_id_ == 1)
				{
					// 1なら消滅
					Object *object = object_manager->Get(name);
					if(object == nullptr)
					{
						return;
					}
					Bullet *bullet = dynamic_cast<Bullet*>(object);
					bullet->SetUse(false);
				}
			}
			break;

		case OBJ_BOMB:
			{
				if(object_manager == nullptr)
				{
					return;
				}
				std::string name = rec_data.name_;
				if(rec_data.object_param_.ex_id_ == 0)
				{
					// 0なら更新
					Object *object = object_manager->Get(name);
					if(object == nullptr)
					{
						return;
					}
					Bomb *bomb = dynamic_cast<Bomb*>(object);
					Vector3 pos, rot, scl;
					pos.x_ = rec_data.object_param_.position_.x_;
					pos.y_ = rec_data.object_param_.position_.y_;
					pos.z_ = rec_data.object_param_.position_.z_;

					rot.x_ = rec_data.object_param_.rotation_.x_;
					rot.y_ = rec_data.object_param_.rotation_.y_;
					rot.z_ = rec_data.object_param_.rotation_.z_;

					bomb->SetPosition(pos);
					bomb->SetRotation(rot);
					bomb->SetUse(true);
				}
				else if(rec_data.object_param_.ex_id_ == 1)
				{
					// 1なら消滅
					Object *object = object_manager->Get(name);
					if(object == nullptr)
					{
						return;
					}
					Bomb *bomb = dynamic_cast<Bomb*>(object);
					bomb->SetUse(false);
				}
			}
			break;

		case OBJ_UI:					// UI
			{
				if(object_manager == nullptr)
				{
					return;
				}
				// アナウンス
				std::string name = rec_data.name_;
				Object *object = object_manager->Get(name);
				if(object == nullptr)
				{
					return;
				}
				Message *message = dynamic_cast<Message*>(object);
				Vector3 message_position = 
				{
					rec_data.object_param_.position_.x_,
					rec_data.object_param_.position_.y_,
					rec_data.object_param_.position_.z_
				};
				if(name == "message_fort_25")
				{
					Message *message;
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_50"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_75"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_100"));
					message->Move(-100.0f);
				}

				else if(name == "message_fort_50")
				{
					Message *message;
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_25"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_75"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_100"));
					message->Move(-100.0f);
				}

				else if(name == "message_fort_75")
				{
					Message *message;
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_25"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_50"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_100"));
					message->Move(-100.0f);
				}

				else if(name == "message_fort_100")
				{
					Message *message;
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_25"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_50"));
					message->Move(-100.0f);
					message = dynamic_cast<Message*>(object_manager->Get("message_fort_75"));
					message->Move(-100.0f);
				}

				message->SetPosition(message_position);
				message->Play();
			}
			break;

		case OBJ_ARROW:
			{
				if(object_manager == nullptr)
				{
					return;
				}
				// アロー
				Object *object = object_manager->Get("arrow");
				if(object == nullptr)
				{
					return;
				}
				Vector3 position =
				{
					rec_data.object_param_.position_.x_,
					rec_data.object_param_.position_.y_,
					rec_data.object_param_.position_.z_
				};
				Vector3 rotation =
				{
					rec_data.object_param_.rotation_.x_,
					rec_data.object_param_.rotation_.y_,
					rec_data.object_param_.rotation_.z_
				};
				object->SetPosition(position);
				object->SetRotation(rotation);
			}
			break;

		case OBJ_CAMERA:
			{
				if(camera_manager == nullptr)
				{
					return;
				}
				Camera *main_camera = camera_manager->Get("MainCamera");
				if(main_camera == nullptr)
				{
					return;
				}
				if(rec_data.object_param_.ex_id_ == 0)
				{
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
				else
				{
					D3DXVECTOR3 camera_rotation;

					camera_rotation.x = rec_data.object_param_.rotation_.x_;
					camera_rotation.y = rec_data.object_param_.rotation_.y_;
					camera_rotation.z = rec_data.object_param_.rotation_.z_;

					main_camera->SetRotation(camera_rotation);
				}
			}
			break;

		default:
			break;
	}
}

//-------------------------------------
// end of file
//-------------------------------------