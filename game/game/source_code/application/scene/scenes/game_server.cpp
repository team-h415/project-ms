//=========================================================
// game_server.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../network/network.h"
#include "../../network/network_host.h"
#include "../../network/network_guest.h"
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../../math/my_math.h"
#include "../../math/vector.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../input/inputs/gamepad.h"
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/mesh/field.h"
#include "../../object/objects/model/x_model.h"
#include "../../object/objects/model/x/x_fort.h"
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_player.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../object/objects/model/fbx/fbx_child.h"
#include "../../object/objects/bullet/bullet.h"
#include "../../object/objects/bullet/bomb.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../collision/collision.h"
#include "../../collision/collision_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "../../config/config.h"
#include "game_server.h"
#include "../fade/fade.h"
#include "../../fps/fps.h"
#include "../../sound/sound.h"

//-------------------------------------
// warning
//-------------------------------------
#pragma warning (disable:4996)


//-------------------------------------
// GameServer()
//-------------------------------------
GameServer::GameServer()
{
	//-------------------------------------
	// �e�}�l�[�W���E�f�o�b�O�V�X�e��������
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	collision_manager_ = new CollisionManager;
	font_ = new DebugFont;
	scene_state_ = 0;
}


//-------------------------------------
// Initialize()
//-------------------------------------
void GameServer::Initialize()
{
	//-------------------------------------
	// BGM�SE��s�ǂݍ���
	//-------------------------------------
	matching_bgm_ = Sound::LoadSound("resource/sound/bgm/matching/matching_bgm.wav");
	game_bgm_ = Sound::LoadSound("resource/sound/bgm/game/game_bgm.wav");
	result_bgm_ = Sound::LoadSound("resource/sound/bgm/game/ms-bgm.wav");
	Sound::StockSE("resource/sound/se/game/countdown.wav");
	Sound::StockSE("resource/sound/se/game/start.wav");
	for(int i = 0; i < 7; i++)
	{
		Sound::StockSE("resource/sound/se/game/shootGrandfather.wav");

		for(int j = 0; j < 4; j++)
		{
			Sound::StockSE("resource/sound/se/game/waterBreak.wav");
			Sound::StockSE("resource/sound/se/game/shootChild.wav");
		}
	}
	for(int i = 0; i < MAX_GUEST; i++)
	{
		Sound::StockSE("resource/sound/se/game/footstep.wav");
	}

	//-------------------------------------
	// �J����
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = {0.0f, 0.0f, -10.0f};
	camera_param.focus_ = {0.0f, 0.0f, 0.0f};
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = {0.0f, 1.0f, 0.0f};
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;

	for(int i = 0; i < MAX_GUEST; i++)
	{
		camera_manager_->Create(
			"Perspective", "camera" + std::to_string(i), camera_param);
	}

	//-------------------------------------
	// �T�u�J����
	//-------------------------------------
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = {0.0f, 0.0f, -10.0f};
	camera_param.focus_ = {0.0f, 0.0f, 0.0f};
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = {0.0f, 1.0f, 0.0f};
	camera_param.near_ = 0.1f;
	camera_param.far_ = 800.0f;

	camera_manager_->Create(
		"Perspective", "SubCamera", camera_param);

	//-------------------------------------
	// �n�`
	//-------------------------------------
	OBJECT_PARAMETER_DESC param;
	param.name_ = "field";
	param.position_ = {0.0f, 0.0f, 0.0f};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {100.0f, 1.0f, 200.0f};
	param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		param,
		"resource/mesh/map.heightmap");

	//-------------------------------------
	// �C
	//-------------------------------------
	OBJECT_PARAMETER_DESC sea_param;
	sea_param.name_ = "sea";
	sea_param.position_ = {0.0f, SEA_HEIGHT, 0.0f};
	sea_param.rotation_ = {0.0f, 0.0f, 0.0f};
	sea_param.scaling_ = {300.0f, 1.0f, 300.0f};
	sea_param.layer_ = LAYER_SPRITE_LAKE;

	object_manager_->Create(
		sea_param);

	//-------------------------------------
	// ��
	//-------------------------------------
	OBJECT_PARAMETER_DESC fort_param;
	COLLISION_PARAMETER_DESC fort_collision_param;
	Object *fort_obj;
	XFort *fort;
	Vector3 fort_pos;
	std::string name;
	for(int i = 0; i < 3; i++)
	{
		name = "fort" + std::to_string(i);
		fort_param.name_ = name;
		fort_param.layer_ = LAYER_MODEL_FORT;
		fort_param.position_ = FORT_POSITION[i];
		fort_param.rotation_ = {0.0f, 0.0f, 0.0f};
		fort_param.scaling_ = {1.0f, 1.0f, 1.0f};

		object_manager_->Create(
			fort_param,
			"resource/model/x/test.x");

		fort_obj = object_manager_->Get(name);
		fort = dynamic_cast<XFort*>(fort_obj);

		fort_pos = fort_obj->parameter().position_;
		fort_collision_param.position_ = {
			fort_pos.x_,
			fort_pos.y_ + 0.5f,
			fort_pos.z_};
		fort_collision_param.range_ = 1.0f;
		fort_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

		collision_manager_->Create(fort_obj,
			fort_collision_param);
		fort->SetLife(FORT_LIFE[i]);
	}

	//-------------------------------------
	// FBX����
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "player0";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = {1.0f, 0.0f, 0.0f};
	grandfather_param.rotation_ = {0.0f, 0.0f, 0.0f};
	grandfather_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(
		grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("player0");

	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_};
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(object_manager_->Get("player0"), fbx_collision_param);

	//-------------------------------------
	// FBX�q��
	//-------------------------------------
	OBJECT_PARAMETER_DESC child_param;
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.position_ = {-1.0f, 0.0f, 0.0f};
	child_param.rotation_ = {0.0f, 0.0f, 0.0f};
	child_param.scaling_ = {1.0f, 1.0f, 1.0f};

	for(int i = 1; i < MAX_GUEST; i++)
	{
		name = "player" + std::to_string(i);
		child_param.name_ = name;
		object_manager_->Create(
			child_param);

		COLLISION_PARAMETER_DESC child_collision_param;
		Object *obj3 = object_manager_->Get(name);
		child_collision_param.position_ = {
			obj3->parameter().position_.x_,
			obj3->parameter().position_.y_,
			obj3->parameter().position_.z_};
		child_collision_param.range_ = 1.0f;
		child_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

		collision_manager_->Create(object_manager_->Get(name),
			child_collision_param);
	}

	//-------------------------------------
	// �o���b�g�������Ă�����
	//-------------------------------------
	OBJECT_PARAMETER_DESC bullet_param;
	bullet_param.layer_ = LAYER_BULLET;
	for(int i = 0; i < MAX_BULLET; i++)
	{
		bullet_param.name_ = "bullet" + std::to_string(i);
		object_manager_->Create(bullet_param);
	}

	//-------------------------------------
	// �X�e�[�g�Z�b�g
	//-------------------------------------
	ChangeServerState(STATE_MATCHING);

	//-------------------------------------
	// �Z�b�g�A�b�v����
	//-------------------------------------
	setup_ = true;
}


//-------------------------------------
// ~GameServer()
//-------------------------------------
GameServer::~GameServer()
{
	setup_ = false;
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(collision_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void GameServer::Update()
{
	// FPS
	font_->Add("FPS:%d :", Fps::GetFps());

	// ��ԕ���
	switch(server_state_)
	{
		case STATE_MATCHING:
			Matching();
			break;

		case STATE_GAME:
			Game();
			break;

		case STATE_RESULT:
			Result();
			break;

		default:
			break;
	}

	//-------------------------------------
	// ���X�V����
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	collision_manager_->Update();

#ifdef _DEBUG
	FbxGrandfather *grandfather(dynamic_cast<FbxGrandfather*>(object_manager_->Get("player0")));
	Vector3 grandfather_position(grandfather->parameter().position_);
	Vector3 grandfather_rotation(grandfather->parameter().rotation_);
	font_->Add("POSITION(grandfather) : %3.2f %3.2f %3.2f\n", grandfather_position.x_, grandfather_position.y_, grandfather_position.z_);
	font_->Add("ROTATION(grandfather) : %3.2f \n", grandfather_rotation.y_);
#endif
}


//-------------------------------------
// Draw()
//-------------------------------------
void GameServer::Draw()
{
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(0.0f, 1.0f, 1.0f, 1.0f);
	MaterialColor color(255, 255, 255, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);

#ifdef _DEBUG
	camera_manager_->Set("camera0");

	switch(server_state_)
	{
		case STATE_GAME:
			switch(scene_state_)
			{
				case STATE_GUEST_WAITING:
				case STATE_COUNTDOWN:
				case STATE_FORT_IN:
				case STATE_FORT_OUT:
					camera_manager_->Set("SubCamera");
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}

	object_manager_->Draw();
	collision_manager_->Draw();
#endif

	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// Matching()
//-------------------------------------
void GameServer::Matching()
{
	NETWORK_DATA send_data;

	font_->Add("�V�[����:");
	font_->Add("Matching\n");

	// �f�o�b�O�p�J��
	if(KeyBoard::isTrigger(DIK_RETURN))
	{
		ChangeServerState(STATE_GAME);
		// �V�[���`�F���W���ߑ��M
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_SCENE_CHANGE_GAME;
		NetworkHost::SendTo(DELI_MULTI, send_data);
		return;
	}

	// �v���C���[�X�V
	MatchingGrandfather();
	MatchingChild();

	// ��ԕ���
	switch(scene_state_)
	{
		case STATE_MATCH_WAITING:
			{
				//-------------------------------------
				// �S���W��������V�[���J��
				//-------------------------------------
				int access_guest(NetworkHost::access_guest());
				if(access_guest <= 0)
				{
					return;
				}
				D3DXVECTOR3 portal_posiiton = PORTAL_POSITION;
				D3DXVECTOR3 player_pos;
				for(int i = 0; i < access_guest; i++)
				{
					std::string player_str = "player" + std::to_string(i);
					Object *player_obj = object_manager_->Get(player_str);
					Vector3 temp = player_obj->parameter().position_;
					player_pos.x = temp.x_;
					player_pos.y = temp.y_;
					player_pos.z = temp.z_;
					D3DXVECTOR3 vec(player_pos - portal_posiiton);
					float distance = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
					if (!(distance < PORTAL_DISTANCE)){
						return;
					}
				}
				scene_state_ = STATE_GO_GAME_WAITING;
				time_ = 60 * 5;
				// �e�N�X�`���ύX���b�Z�[�W���M
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_SPRITE2D_TEX;
				NetworkHost::SendTo(DELI_MULTI, send_data);
			}
			break;
		case STATE_GO_GAME_WAITING:
			{
				time_--;
				if(time_ == 0)
				{
					// �V�[���`�F���W���ߑ��M
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_SCENE_CHANGE_GAME;
					NetworkHost::SendTo(DELI_MULTI, send_data);
					// �T�[�o�[�X�e�[�g�ύX
					ChangeServerState(STATE_GAME);
				}
			}
			break;

		default:
			break;
	}
}


//-------------------------------------
// Game()
//-------------------------------------
void GameServer::Game()
{
	font_->Add("�V�[����:");
	font_->Add("Game\n");
	font_->Add("Time:%d\n", time_);

	NETWORK_DATA send_data;
	ZeroMemory(&send_data, sizeof(send_data));

	// �f�o�b�O�p�J�ڏ��҂�����
	if(KeyBoard::isTrigger(DIK_9))
	{
		// �V�[���`�F���W���ߑ��M
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_SCENE_CHANGE_RESULT;
		send_data.id_ = 0;
		NetworkHost::SendTo(DELI_MULTI, send_data);
		// �T�[�o�[�X�e�[�g�ύX
		ChangeServerState(STATE_RESULT);
		return;
	}
	// �f�o�b�O�p�J�ڏ��҃K�L
	else if(KeyBoard::isTrigger(DIK_0))
	{
		// �V�[���`�F���W���ߑ��M
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_SCENE_CHANGE_RESULT;
		send_data.id_ = 1;
		NetworkHost::SendTo(DELI_MULTI, send_data);
		// �T�[�o�[�X�e�[�g�ύX
		ChangeServerState(STATE_RESULT);
		return;
	}

	// �t�B�[���h�擾
	Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));

	switch(scene_state_)
	{
		case STATE_GUEST_WAITING:
			{
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_UI_PARAM;
				send_data.object_param_.type_ = OBJ_UI;
				strcpy_s(send_data.name_, MAX_NAME_LEN, "time");
				send_data.ui_param_.value_i_ = 180;
				NetworkHost::SendTo(DELI_MULTI, send_data);

				for(int i = 0; i < MAX_GUEST; i++)
				{
					//------------------------------------------------
					// �v���C���[�f�[�^�]��
					//------------------------------------------------
					FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(i)));
					Vector3 player_position = player->parameter().position_;
					Vector3 player_rotation = player->parameter().rotation_;

					send_data.type_ = DATA_OBJ_PARAM;
					send_data.id_ = i;
					send_data.object_param_.ex_id_ = 0;
					send_data.object_param_.type_ = OBJ_PLAYER;
					send_data.object_param_.position_.x_ = player_position.x_;
					send_data.object_param_.position_.y_ = player_position.y_;
					send_data.object_param_.position_.z_ = player_position.z_;

					send_data.object_param_.rotation_.x_ = player_rotation.x_;
					send_data.object_param_.rotation_.y_ = player_rotation.y_;
					send_data.object_param_.rotation_.z_ = player_rotation.z_;

					NetworkHost::SendTo(DELI_MULTI, send_data);

					//------------------------------------------------
					// �J�����f�[�^�]��
					//------------------------------------------------
					Camera* main_camera = camera_manager_->Get("camera" + std::to_string(i));
					D3DXVECTOR3 camera_position = main_camera->position();
					D3DXVECTOR3 camera_focus = main_camera->focus();
					D3DXVECTOR3 camera_rotation = main_camera->rotation();

					send_data.type_ = DATA_OBJ_PARAM;
					send_data.object_param_.type_ = OBJ_CAMERA;
					send_data.object_param_.ex_id_ = 0;

					send_data.object_param_.position_.x_ = camera_position.x;
					send_data.object_param_.position_.y_ = camera_position.y;
					send_data.object_param_.position_.z_ = camera_position.z;

					send_data.object_param_.rotation_.x_ = camera_focus.x;
					send_data.object_param_.rotation_.y_ = camera_focus.y;
					send_data.object_param_.rotation_.z_ = camera_focus.z;
					NetworkHost::SendTo((DELI_TYPE)i, send_data);

					// ��]
					send_data.object_param_.ex_id_ = 1;
					send_data.object_param_.rotation_.x_ = camera_rotation.x;
					send_data.object_param_.rotation_.y_ = camera_rotation.y;
					send_data.object_param_.rotation_.z_ = camera_rotation.z;
					NetworkHost::SendTo((DELI_TYPE)i, send_data);
				}

				int access_guest = NetworkHost::access_guest();
				for(int i = 0; i < access_guest; i++)
				{
					if(!guest_scene_change_[i])
					{
						return;
					}
				}
				scene_state_ = STATE_COUNTDOWN;
				time_ = 60 * (5 + 4);
				return;
			}
			break;

		case STATE_COUNTDOWN:
			{
				time_--;
				if(time_ == 0)
				{
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_UI_PARAM;
					send_data.object_param_.type_ = OBJ_UI;
					strcpy_s(send_data.name_, MAX_NAME_LEN, "countdown");
					send_data.ui_param_.value_i_ = 4;
					NetworkHost::SendTo(DELI_MULTI, send_data);

					scene_state_ = STATE_RUN;
					time_ = 60 * GAME_TIME;

					Sound::LoadAndPlaySE("resource/sound/se/game/start.wav");
				}
				else if(time_ <= (60 * 4))
				{
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_UI_PARAM;
					send_data.object_param_.type_ = OBJ_UI;
					strcpy_s(send_data.name_, MAX_NAME_LEN, "countdown");
					send_data.ui_param_.value_i_ = time_ / 60;
					NetworkHost::SendTo(DELI_MULTI, send_data);

					if(time_ % 60 == 0)
					{
						Sound::LoadAndPlaySE("resource/sound/se/game/countdown.wav");
					}
				}

				//------------------------------------------------
				// �v���C���[�f�[�^�]��
				//------------------------------------------------
				for(int i = 0; i < MAX_GUEST; i++)
				{
					FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(i)));
					Vector3 player_position = player->parameter().position_;
					Vector3 player_rotation = player->parameter().rotation_;

					send_data.type_ = DATA_OBJ_PARAM;
					send_data.id_ = i;
					send_data.object_param_.ex_id_ = 0;
					send_data.object_param_.type_ = OBJ_PLAYER;
					send_data.object_param_.position_.x_ = player_position.x_;
					send_data.object_param_.position_.y_ = player_position.y_;
					send_data.object_param_.position_.z_ = player_position.z_;

					send_data.object_param_.rotation_.x_ = player_rotation.x_;
					send_data.object_param_.rotation_.y_ = player_rotation.y_;
					send_data.object_param_.rotation_.z_ = player_rotation.z_;

					NetworkHost::SendTo(DELI_MULTI, send_data);
				}

				int real_time = time_ / 60;
				switch(real_time)
				{
					case 3:
						{
							//------------------------------------------------
							// �J�����f�[�^�]��
							//------------------------------------------------
							Camera* sub_camera = camera_manager_->Get("SubCamera");
							D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
							sub_camera_rotation = sub_camera->rotation();
							sub_camera_position = sub_camera->position();
							sub_camera_focus = sub_camera->focus();

							FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(1)));
							Vector3 focus = player->parameter().position_;
							// �t�H�[�J�X�ݒ�
							sub_camera_focus.x = focus.x_;
							sub_camera_focus.y = focus.y_;
							sub_camera_focus.z = focus.z_;
							sub_camera_focus.y = 1.5f;
							// �����_����ɃJ�������W��ݒ�
							sub_camera_position = sub_camera_focus;
							sub_camera_position.x -=
								sinf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
							sub_camera_position.z -=
								cosf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
							sub_camera_position.y -=
								sinf(sub_camera_rotation.x) * CAMERA_POS_LEN;

							// �T�u�J�����֒l���U
							sub_camera->SetPosition(sub_camera_position);
							sub_camera->SetFocus(sub_camera_focus);
							sub_camera->SetRotation(sub_camera_rotation);

							D3DXVECTOR3 camera_position = sub_camera->position();
							D3DXVECTOR3 camera_focus = sub_camera->focus();
							D3DXVECTOR3 camera_rotation = sub_camera->rotation();
							for(int i = 0; i < MAX_GUEST; i++)
							{
								send_data.type_ = DATA_OBJ_PARAM;
								send_data.object_param_.type_ = OBJ_CAMERA;
								send_data.object_param_.ex_id_ = 0;

								send_data.object_param_.position_.x_ = camera_position.x;
								send_data.object_param_.position_.y_ = camera_position.y;
								send_data.object_param_.position_.z_ = camera_position.z;

								send_data.object_param_.rotation_.x_ = camera_focus.x;
								send_data.object_param_.rotation_.y_ = camera_focus.y;
								send_data.object_param_.rotation_.z_ = camera_focus.z;
								NetworkHost::SendTo((DELI_TYPE)i, send_data);
							}
						}
						break;
					case 2:
						{
							//------------------------------------------------
							// �J�����f�[�^�]��
							//------------------------------------------------
							Camera* sub_camera = camera_manager_->Get("SubCamera");
							D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
							sub_camera_rotation = sub_camera->rotation();
							sub_camera_position = sub_camera->position();
							sub_camera_focus = sub_camera->focus();

							FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(0)));
							Vector3 focus = player->parameter().position_;
							// �t�H�[�J�X�ݒ�
							sub_camera_focus.x = focus.x_;
							sub_camera_focus.y = focus.y_;
							sub_camera_focus.z = focus.z_;
							sub_camera_focus.y = 1.5f;
							// �����_����ɃJ�������W��ݒ�
							sub_camera_position = sub_camera_focus;
							sub_camera_position.x -=
								sinf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
							sub_camera_position.z -=
								cosf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
							sub_camera_position.y -=
								sinf(sub_camera_rotation.x) * CAMERA_POS_LEN;

							// �T�u�J�����֒l���U
							sub_camera->SetPosition(sub_camera_position);
							sub_camera->SetFocus(sub_camera_focus);
							sub_camera->SetRotation(sub_camera_rotation);

							D3DXVECTOR3 camera_position = sub_camera->position();
							D3DXVECTOR3 camera_focus = sub_camera->focus();
							D3DXVECTOR3 camera_rotation = sub_camera->rotation();
							for(int i = 0; i < MAX_GUEST; i++)
							{
								send_data.type_ = DATA_OBJ_PARAM;
								send_data.object_param_.type_ = OBJ_CAMERA;
								send_data.object_param_.ex_id_ = 0;

								send_data.object_param_.position_.x_ = camera_position.x;
								send_data.object_param_.position_.y_ = camera_position.y;
								send_data.object_param_.position_.z_ = camera_position.z;

								send_data.object_param_.rotation_.x_ = camera_focus.x;
								send_data.object_param_.rotation_.y_ = camera_focus.y;
								send_data.object_param_.rotation_.z_ = camera_focus.z;
								NetworkHost::SendTo((DELI_TYPE)i, send_data);
							}
						}
						break;

					case 1:
					default:
						// �v���C���[���Ă�
						for(int i = 0; i < MAX_GUEST; i++)
						{
							//------------------------------------------------
							// �J�����f�[�^�]��
							//------------------------------------------------
							FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(i)));
							Vector3 player_position = player->parameter().position_;
							Vector3 player_rotation = player->parameter().rotation_;

							Camera* main_camera = camera_manager_->Get("camera" + std::to_string(i));
							D3DXVECTOR3 camera_position = main_camera->position();
							D3DXVECTOR3 camera_focus = main_camera->focus();
							D3DXVECTOR3 camera_rotation = main_camera->rotation();

							send_data.type_ = DATA_OBJ_PARAM;
							send_data.object_param_.type_ = OBJ_CAMERA;
							send_data.object_param_.ex_id_ = 0;

							send_data.object_param_.position_.x_ = camera_position.x;
							send_data.object_param_.position_.y_ = camera_position.y;
							send_data.object_param_.position_.z_ = camera_position.z;

							send_data.object_param_.rotation_.x_ = camera_focus.x;
							send_data.object_param_.rotation_.y_ = camera_focus.y;
							send_data.object_param_.rotation_.z_ = camera_focus.z;
							NetworkHost::SendTo((DELI_TYPE)i, send_data);

							// ��]
							send_data.object_param_.ex_id_ = 1;
							send_data.object_param_.rotation_.x_ = camera_rotation.x;
							send_data.object_param_.rotation_.y_ = camera_rotation.y;
							send_data.object_param_.rotation_.z_ = camera_rotation.z;
							NetworkHost::SendTo((DELI_TYPE)i, send_data);

							#ifdef _DEBUG
								if(i == 0)
								{
									Camera* sub_camera = camera_manager_->Get("SubCamera");
									sub_camera->SetPosition(camera_position);
									sub_camera->SetFocus(camera_focus);
								}
							#endif
						}
						break;
				}
			
				return;
			}
			break;

		case STATE_RUN:
			{
				// ���Ԍo��
				time_--;
				if(time_ > 0)
				{
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_UI_PARAM;
					send_data.object_param_.type_ = OBJ_UI;
					strcpy_s(send_data.name_, MAX_NAME_LEN, "time");
					send_data.ui_param_.value_i_ = time_ / 60;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
				else
				{
					// �Q�[���G���h�����P �^�C���A�b�v
					// ����������
					// ���s���b�Z�[�W
					time_ = 60 * 5;
					scene_state_ = STATE_GAME_END;

					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_GAME_WINNER;
					send_data.id_ = 0;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}

				// �ԃ��C�t�Ǘ�
				std::string fort_str = "fort" + std::to_string(now_target_fort_);
				Object *fort_object = object_manager_->Get(fort_str);
				XFort *fort = dynamic_cast<XFort*>(fort_object);
				send_data.type_ = DATA_UI_PARAM;
				send_data.id_ = now_target_fort_;
				float life(fort->GetLife());
				send_data.ui_param_.value_f_ = life;
				strcpy_s(send_data.name_, MAX_NAME_LEN, "fort_gauge_manager");
				NetworkHost::SendTo(DELI_MULTI, send_data);

				// �A�i�E���X�`�F�b�N
				if(fort_announce_state_ * 0.01f > life)
				{
					// �Q�X�g�փA�i�E���X���m����
					std::string name = "message_fort_" + std::to_string(100 - fort_announce_state_);
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_OBJ_PARAM;
					send_data.object_param_.type_ = OBJ_UI;
					send_data.object_param_.position_.x_ = SCREEN_WIDTH + 200.0f;
					send_data.object_param_.position_.y_ = SCREEN_HEIGHT - 200.0f;
					send_data.object_param_.position_.z_ = 0.0f;
					strcpy_s(send_data.name_, MAX_NAME_LEN, name.c_str());
					NetworkHost::SendTo(DELI_MULTI, send_data);

					// �A�i�E���X�p�l����
					fort_announce_state_ -= 25;
				}

				if(life <= 0.0f)
				{
					fort_announce_state_ = 75;
					now_target_fort_++;
					scene_state_ = STATE_FORT_OUT;
				}
			}
			break;

		case STATE_FORT_OUT:
			{
				// �Ԃ̕ύX
				D3DXVECTOR3 fort_pos;
				Vector3 fort_position;
				Vector3 fort_rotation;

				int i = now_target_fort_ - 1;
				std::string fort_str = "fort" + std::to_string(i);
				Object *fort_object = object_manager_->Get(fort_str);
				XFort *fort = dynamic_cast<XFort*>(fort_object);
				fort_position = fort_object->parameter().position_;
				fort_rotation = fort_object->parameter().rotation_;

				fort_pos.x = fort_position.x_;
				fort_pos.y = fort_position.y_;
				fort_pos.z = fort_position.z_;

				float field_height = field->GetHeight(fort_pos);

				fort_y_[i] -= 0.01f;
				if(fort_y_[i] < -3.0f)
				{
					fort_y_[i] = -3.0f;
					if(now_target_fort_ >= 3)
					{
						// �K�L�̏���
						// ���s���b�Z�[�W
						time_ = 60 * 5;
						scene_state_ = STATE_GAME_END;

						ZeroMemory(&send_data, sizeof(send_data));
						send_data.type_ = DATA_GAME_WINNER;
						send_data.id_ = 1;
						NetworkHost::SendTo(DELI_MULTI, send_data);
					}
					else
					{
						scene_state_ = STATE_FORT_IN;
					}
				}
				// �ԃG�t�F�N�g
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.position_.x_ = fort_position.x_;
				send_data.object_param_.position_.y_ = field_height;
				send_data.object_param_.position_.z_ = fort_position.z_;
				strcpy_s(send_data.name_, MAX_NAME_LEN, "smoke2");
				NetworkHost::SendTo(DELI_MULTI, send_data);

				fort_position.y_ = field_height + fort_y_[i];

				fort->SetPosition(fort_position);

				// �I�u�W�F�N�g�f�[�^�]��
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_FORT;
				send_data.object_param_.position_.x_ = fort_position.x_;
				send_data.object_param_.position_.y_ = fort_position.y_;
				send_data.object_param_.position_.z_ = fort_position.z_;
				send_data.object_param_.rotation_.x_ = fort_rotation.x_;
				send_data.object_param_.rotation_.y_ = fort_rotation.y_;
				send_data.object_param_.rotation_.z_ = fort_rotation.z_;
				strcpy_s(send_data.name_, MAX_NAME_LEN, fort_str.c_str());
				NetworkHost::SendTo(DELI_MULTI, send_data);

				Camera* sub_camera = camera_manager_->Get("SubCamera");
				D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
				sub_camera_rotation = sub_camera->rotation();
				sub_camera_position = sub_camera->position();
				sub_camera_focus = sub_camera->focus();

				Vector3 focus = fort_object->parameter().position_;
				// �t�H�[�J�X�ݒ�
				sub_camera_focus.x = focus.x_;
				sub_camera_focus.y = focus.y_;
				sub_camera_focus.z = focus.z_;
				sub_camera_focus.y = 1.5f;
				// �����_����ɃJ�������W��ݒ�
				sub_camera_position = sub_camera_focus;
				sub_camera_position.x -=
					sinf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
				sub_camera_position.z -=
					cosf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
				sub_camera_position.y -=
					sinf(sub_camera_rotation.x) * CAMERA_POS_LEN;

				// �T�u�J�����֒l���U
				sub_camera->SetPosition(sub_camera_position);
				sub_camera->SetFocus(sub_camera_focus);
				sub_camera->SetRotation(sub_camera_rotation);
			}
			break;

		case STATE_FORT_IN:
			{
				// �Ԃ̕ύX
				D3DXVECTOR3 fort_pos;
				Vector3 fort_position;
				Vector3 fort_rotation;
				int i = now_target_fort_;

				std::string fort_str = "fort" + std::to_string(i);
				Object *fort_object = object_manager_->Get(fort_str);
				XFort *fort = dynamic_cast<XFort*>(fort_object);
				fort_position = fort_object->parameter().position_;
				fort_rotation = fort_object->parameter().rotation_;

				fort_pos.x = fort_position.x_;
				fort_pos.y = fort_position.y_;
				fort_pos.z = fort_position.z_;

				float field_height = field->GetHeight(fort_pos);
				fort_y_[i] += 0.01f;
				if(fort_y_[i] > 0.0f)
				{
					fort_y_[i] = 0.0f;
					scene_state_ = STATE_RUN;
					Camera* sub_camera = camera_manager_->Get("SubCamera");
					sub_camera->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
				// �ԃG�t�F�N�g
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.position_.x_ = fort_position.x_;
				send_data.object_param_.position_.y_ = field_height;
				send_data.object_param_.position_.z_ = fort_position.z_;
				strcpy_s(send_data.name_, MAX_NAME_LEN, "smoke2");
				NetworkHost::SendTo(DELI_MULTI, send_data);

				fort_position.y_ = field_height + fort_y_[i];
				fort->SetPosition(fort_position);

				// �I�u�W�F�N�g�f�[�^�]��
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_FORT;
				send_data.object_param_.position_.x_ = fort_position.x_;
				send_data.object_param_.position_.y_ = fort_position.y_;
				send_data.object_param_.position_.z_ = fort_position.z_;
				send_data.object_param_.rotation_.x_ = fort_rotation.x_;
				send_data.object_param_.rotation_.y_ = fort_rotation.y_;
				send_data.object_param_.rotation_.z_ = fort_rotation.z_;
				strcpy_s(send_data.name_, MAX_NAME_LEN, fort_str.c_str());
				NetworkHost::SendTo(DELI_MULTI, send_data);

				Camera* sub_camera = camera_manager_->Get("SubCamera");
				D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
				sub_camera_rotation = sub_camera->rotation();
				sub_camera_position = sub_camera->position();
				sub_camera_focus = sub_camera->focus();

				Vector3 focus = fort_object->parameter().position_;
				// �t�H�[�J�X�ݒ�
				sub_camera_focus.x = focus.x_;
				sub_camera_focus.y = focus.y_;
				sub_camera_focus.z = focus.z_;
				sub_camera_focus.y = 1.5f;
				// �����_����ɃJ�������W��ݒ�
				sub_camera_position = sub_camera_focus;
				sub_camera_position.x -=
					sinf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
				sub_camera_position.z -=
					cosf(sub_camera_rotation.y) * CAMERA_POS_LEN * cosf(sub_camera_rotation.x);
				sub_camera_position.y -=
					sinf(sub_camera_rotation.x) * CAMERA_POS_LEN;

				// �T�u�J�����֒l���U
				sub_camera->SetPosition(sub_camera_position);
				sub_camera->SetFocus(sub_camera_focus);
				sub_camera->SetRotation(sub_camera_rotation);
			}
			break;

		case STATE_GAME_END:
			{
				time_--;
				if(time_ < 0)
				{
					// �V�[���`�F���W���ߑ��M
					ZeroMemory(&send_data, sizeof(send_data));
					send_data.type_ = DATA_SCENE_CHANGE_RESULT;
					NetworkHost::SendTo(DELI_MULTI, send_data);
					ChangeServerState(STATE_RESULT);
				}
			}
			break;

		case STATE_END:
			{
			}
			break;

		default:
			break;
	}

	// �v���C���[�X�V
	GameGrandfather();
	GameChild();
}


//-------------------------------------
// Result()
//-------------------------------------
void GameServer::Result()
{
	NETWORK_DATA send_data;

	font_->Add("�V�[����:");
	font_->Add("Result\n");
	if(KeyBoard::isTrigger(DIK_RETURN))
	{
		ChangeServerState(STATE_MATCHING);
		// �V�[���`�F���W���ߑ��M
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_SCENE_CHANGE_MATCHING;
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}

	for(int i = 0; i < MAX_GUEST; i++)
	{
		if(GamePad::isTrigger(i, PAD_BUTTON_8))
		{
			ChangeServerState(STATE_MATCHING);
			// �V�[���`�F���W���ߑ��M
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_SCENE_CHANGE_MATCHING;
			NetworkHost::SendTo(DELI_MULTI, send_data);
			break;
		}
	}
}


//-------------------------------------
// MatchingGrandfather()
//-------------------------------------
void GameServer::MatchingGrandfather()
{
	//-------------------------------------
	// ���p�ϐ���`
	//-------------------------------------
	FbxGrandfather *grandfather(dynamic_cast<FbxGrandfather*>(object_manager_->Get("player0")));
	Field *field(dynamic_cast<Field*>(object_manager_->Get("field")));
	Vector3 grandfather_position(grandfather->parameter().position_);
	Vector3 grandfather_rotation(grandfather->parameter().rotation_);
	NETWORK_DATA send_data{0};


	// ���x�{���ݒ�
	float grandfather_speed(CHARANCTER_MOVESPEED);
	// �_�b�V��
	if(GamePad::isPress(0, PAD_BUTTON_11))
	{
		grandfather_speed = CHARANCTER_MOVESPEED * 2.0f;
		// �_�b�V���G�t�F�N�g
		if(dash_effect_timer_ % 10 == 0)
		{
			// �G�t�F�N�g�Đ�
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = grandfather_position.x_;
			send_data.object_param_.position_.y_ = grandfather_position.y_;
			send_data.object_param_.position_.z_ = grandfather_position.z_;
			send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
			strcpy_s(send_data.name_, MAX_NAME_LEN, "dash");
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
		dash_effect_timer_++;
	}
	// �����ύX
	grandfather_rotation.y_ += GamePad::isStick(0).rsx_ * CHAR_ROT_SPEED;
	if(grandfather_rotation.y_ > D3DX_PI)
	{
		grandfather_rotation.y_ -= D3DX_PI * 2.0f;
	}
	else if(grandfather_rotation.y_ < -D3DX_PI)
	{
		grandfather_rotation.y_ += D3DX_PI * 2.0f;
	}
	// �����W�ۑ�
	Vector3 grandfather_position_old(grandfather_position);
	// ���W�ړ�
	grandfather_position.x_ += (
		cosf(grandfather_rotation.y_) * GamePad::isStick(0).lsx_ +
		sinf(-grandfather_rotation.y_) * GamePad::isStick(0).lsy_) * grandfather_speed;
	grandfather_position.z_ -= (
		sinf(grandfather_rotation.y_) * GamePad::isStick(0).lsx_ +
		cosf(-grandfather_rotation.y_) * GamePad::isStick(0).lsy_) * grandfather_speed;
	// �t�B�[���h�̍����ɍ��킹��
	D3DXVECTOR3 grandfather_pos(
		grandfather_position.x_,
		grandfather_position.y_,
		grandfather_position.z_);
	grandfather_position.y_ = field->GetHeight(grandfather_pos);
	// �N���s�̈�`�F�b�N
	if(grandfather_position.y_ > 0.4f || grandfather_position.y_ < -0.4f)
	{
		grandfather_position = grandfather_position_old;
	}
	//grandfather_position.x_ = std::max<float>(grandfather_position.x_, 32.0f);
	//grandfather_position.z_ = std::min<float>(grandfather_position.z_, -35.0f);
	// �p�����[�^�K��(�J�����p��D3DXVECTOR3�ɂ�
	grandfather->SetPosition(grandfather_position);
	grandfather->SetRotation(grandfather_rotation);
	grandfather_pos.x = grandfather_position.x_;
	grandfather_pos.y = grandfather_position.y_;
	grandfather_pos.z = grandfather_position.z_;


	// �J�����N���X�擾
	Camera *main_camera = camera_manager_->Get("camera0");
	// �J���������X�V
	D3DXVECTOR3 camera_rotation(main_camera->rotation());
	camera_rotation.x += GamePad::isStick(0).rsy_ * CHAR_ROT_SPEED;
	if(camera_rotation.x > CAMERA_ROT_X_LIMIT)
	{
		camera_rotation.x = CAMERA_ROT_X_LIMIT;
	}
	else if(camera_rotation.x < -CAMERA_ROT_X_LIMIT)
	{
		camera_rotation.x = -CAMERA_ROT_X_LIMIT;
	}
	// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
	camera_rotation.y = grandfather_rotation.y_;
	// ��U���f���𒍎��_��
	D3DXVECTOR3 camera_focus(grandfather_pos);
	// ���������̂̒��S�ӂ�����
	camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	// ���f���̏����������悤�ɒ���
	camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;
	// �����_����ɃJ�������W��ݒ�
	D3DXVECTOR3 camera_position(camera_focus);
	camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
	camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
	camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[0];

	// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
	D3DXVECTOR3 vec_camera_to_focus(camera_focus - camera_position);
	bool camera_re_calculate(false);
	for(int j = 0; j < 10; j++)
	{
		// ���Ԓn�_���v�Z
		D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
		float pos_y = field->GetHeight(lay_point);
		// �������
		if(lay_point.y < pos_y + 0.1f)
		{
			camera_re_calculate = true;
			camera_pos_len_[0] -= CAMARA_LEN_SPEED;
		}
	}

	//�J�������W�Čv�Z
	if(camera_re_calculate == true)
	{
		camera_position = camera_focus;
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[0];
		camera_position.y = field->GetHeight(camera_position) + 0.1f;
	}

	camera_pos_len_[0] += CAMARA_LEN_SPEED;
	if(camera_pos_len_[0] > CAMERA_POS_LEN)
	{
		camera_pos_len_[0] = CAMERA_POS_LEN;
	}

	// �J�����Ƀp�����[�^���ăZ�b�g
	main_camera->SetPosition(camera_position);
	main_camera->SetFocus(camera_focus);
	main_camera->SetRotation(camera_rotation);

	//-------------------------------------
	// �o���b�g����
	//-------------------------------------
	shot_late_[0]--;
	shot_late_[0] = std::max<int>(shot_late_[0], 0);

	if(GamePad::isPress(0, PAD_BUTTON_8) && shot_late_[0] == 0)
	{
		shot_late_[0] = 10;

		// �o���b�g�p�����[�^�ݒ�
		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bullet_param.position_ = grandfather_position;
		bullet_param.position_.x_ += sinf(grandfather_rotation.y_) * 0.8f;
		bullet_param.position_.z_ += cosf(grandfather_rotation.y_) * 0.8f;
		bullet_param.position_.y_ += 0.7f;
		bullet_param.rotation_ = grandfather_rotation;
		bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

		// �G�t�F�N�g�Đ�
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = bullet_param.position_.x_;
		send_data.object_param_.position_.y_ = bullet_param.position_.y_;
		send_data.object_param_.position_.z_ = bullet_param.position_.z_;
		send_data.object_param_.rotation_ = {0.0f, grandfather_rotation.y_, 0.0f};
		strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		// ���˃��[�v
		for(int i = 0; i < 3; i++)
		{
			// �J�����̉�]X�𗘗p
			bullet_param.rotation_.x_ = camera_rotation.x;
			float range = MyMath::Random_Range(-5, 5) * 0.01f;
			bullet_param.rotation_.y_ += range;

			// �o���b�g���C
			Bullet* bullet = object_manager_->GetNoUseBullet();
			bullet->Fire(bullet_param);
		}

		//-------------------------------------
		// �e����SE�Đ�
		//-------------------------------------
		Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
	}

	//-------------------------------------
	// �{������
	//-------------------------------------
	if(GamePad::isTrigger(0, PAD_BUTTON_6))
	{

		OBJECT_PARAMETER_DESC bomb_param;
		bomb_param.layer_ = LAYER_BOMB;
		bomb_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bomb_param.position_ = grandfather_position;
		bomb_param.position_.y_ += 0.6f;
		bomb_param.rotation_ = grandfather_rotation;

		// �J�����̉�]X�𗘗p
		bomb_param.rotation_.x_ = camera_rotation.x;
		float range = MyMath::Random_Range(-5, 5) * 0.01f;
		bomb_param.rotation_.y_ += range;

		// �G�t�F�N�g�Đ�
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = bomb_param.position_.x_;
		send_data.object_param_.position_.y_ = bomb_param.position_.y_;
		send_data.object_param_.position_.z_ = bomb_param.position_.z_;
		send_data.object_param_.rotation_ = {0.0f, grandfather_rotation.y_, 0.0f};
		strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		Bomb* bomb = object_manager_->GetNoUseBomb();
		bomb->Fire(bomb_param);

		//-------------------------------------
		// �����̒e����SE�Đ�
		Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
	}

	//------------------------------------------------
	// �v���C���[�f�[�^�]��
	//------------------------------------------------
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_PLAYER;
	send_data.id_ = 0;
	send_data.object_param_.ex_id_ = 0;
	strcpy_s(send_data.name_, MAX_NAME_LEN, "player0");
	if(GamePad::isPress(0, PAD_LS_DOWN) ||
		GamePad::isPress(0, PAD_LS_UP) ||
		GamePad::isPress(0, PAD_LS_LEFT) ||
		GamePad::isPress(0, PAD_LS_RIGHT))
	{
		send_data.object_param_.ex_id_ = 1;
	}

	send_data.object_param_.position_.x_ = grandfather_position.x_;
	send_data.object_param_.position_.y_ = grandfather_position.y_;
	send_data.object_param_.position_.z_ = grandfather_position.z_;

	send_data.object_param_.rotation_.x_ = grandfather_rotation.x_;
	send_data.object_param_.rotation_.y_ = grandfather_rotation.y_;
	send_data.object_param_.rotation_.z_ = grandfather_rotation.z_;

	NetworkHost::SendTo(DELI_MULTI, send_data);

	//------------------------------------------------
	// �J�����f�[�^�]��
	//------------------------------------------------
	camera_position = main_camera->position();
	camera_focus = main_camera->focus();
	camera_rotation = main_camera->rotation();

	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_CAMERA;
	send_data.object_param_.ex_id_ = 0;

	send_data.object_param_.position_.x_ = camera_position.x;
	send_data.object_param_.position_.y_ = camera_position.y;
	send_data.object_param_.position_.z_ = camera_position.z;

	send_data.object_param_.rotation_.x_ = camera_focus.x;
	send_data.object_param_.rotation_.y_ = camera_focus.y;
	send_data.object_param_.rotation_.z_ = camera_focus.z;
	NetworkHost::SendTo((DELI_TYPE)0, send_data);

	// ��]
	send_data.object_param_.ex_id_ = 1;
	send_data.object_param_.rotation_.x_ = camera_rotation.x;
	send_data.object_param_.rotation_.y_ = camera_rotation.y;
	send_data.object_param_.rotation_.z_ = camera_rotation.z;
	NetworkHost::SendTo((DELI_TYPE)0, send_data);
}


//-------------------------------------
// MatchingChild()
//-------------------------------------
void GameServer::MatchingChild()
{
	//-------------------------------------
	// ���p�ϐ���`
	//-------------------------------------
	Field *field(dynamic_cast<Field*>(object_manager_->Get("field")));
	std::string name;
	FbxChild *child;
	Vector3 child_position;
	Vector3 child_rotation;
	NETWORK_DATA send_data;

	for(int i = 1; i < MAX_GUEST; i++)
	{
		name = "player" + std::to_string(i);
		child = dynamic_cast<FbxChild*>(object_manager_->Get(name));
		child_position = child->parameter().position_;
		child_rotation = child->parameter().rotation_;

		//-------------------------------------
		// �v���C���[��n�`�ɉ����Ĉړ�������
		//-------------------------------------
		Vector3 child_position_old(child_position);
		float child_speed(CHARANCTER_MOVESPEED);
		bool input(true);
		if(child_death_[i - 1])
		{
			input = false;
		}
		if(input)
		{
			// ����
			child_rotation.y_ += GamePad::isStick(i).rsx_ * CHAR_ROT_SPEED;
			if(child_rotation.y_ > D3DX_PI)
			{
				child_rotation.y_ -= D3DX_PI * 2.0f;
			}
			else if(child_rotation.y_ < -D3DX_PI)
			{
				child_rotation.y_ += D3DX_PI * 2.0f;
			}
			// �ړ�
			child_position.x_ += (
				cosf(child_rotation.y_) * GamePad::isStick(i).lsx_ +
				sinf(-child_rotation.y_) * GamePad::isStick(i).lsy_) * child_speed;
			child_position.z_ -= (
				sinf(child_rotation.y_) * GamePad::isStick(i).lsx_ +
				cosf(-child_rotation.y_) * GamePad::isStick(i).lsy_) * child_speed;
		}

		D3DXVECTOR3 child_pos(
			child_position.x_,
			child_position.y_,
			child_position.z_);
		child_position.y_ = field->GetHeight(child_pos);

		//-------------------------------------
		// �N���s�̈�`�F�b�N
		//-------------------------------------
		if(child_position.y_ > 0.4f || child_position.y_ < -0.4f){
			child_position = child_position_old;
		}
		//child_position.x_ = std::max<float>(child_position.x_, 32.0f);
		//child_position.z_ = std::min<float>(child_position.z_, -35.0f);

		//-------------------------------------
		// �p�����[�^�K��
		//-------------------------------------
		child_pos.x = child_position.x_;
		child_pos.y = child_position.y_;
		child_pos.z = child_position.z_;
		child->SetPosition(child_position);
		child->SetRotation(child_rotation);

		//-------------------------------------
		// �J�����Ǐ]
		//-------------------------------------
		std::string camera_str = "camera" + std::to_string(i);
		Camera *main_camera = camera_manager_->Get(camera_str);
		D3DXVECTOR3 camera_position, camera_focus;
		D3DXVECTOR3 camera_rotation(main_camera->rotation());

		// ����
		camera_rotation.x += GamePad::isStick(i).rsy_ * CHAR_ROT_SPEED;
		if(camera_rotation.x > CAMERA_ROT_X_LIMIT)
		{
			camera_rotation.x = CAMERA_ROT_X_LIMIT;
		}
		else if(camera_rotation.x < -CAMERA_ROT_X_LIMIT)
		{
			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
		}

		// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
		camera_rotation.y = child_rotation.y_;
		// ��U���f���𒍎��_��
		camera_focus = child_pos;
		// ���������̂̒��S�ӂ�����
		camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
		// ���f���̏����������悤�ɒ���
		camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

		// �����_����ɃJ�������W��ݒ�
		camera_position = camera_focus;
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];

		// �J�����̒n�ʂ߂荞�݉������
		D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

		// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
		bool camera_re_calculate = false;
		for(int j = 0; j < 10; ++j){
			// ���Ԓn�_���v�Z
			D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
			float pos_y = field->GetHeight(lay_point);
			// �������
			if(lay_point.y < pos_y + 0.1f){
				camera_re_calculate = true;
				camera_pos_len_[i] -= CAMARA_LEN_SPEED;
			}
		}

		//�J�������W�Čv�Z
		if(camera_re_calculate == true){
			camera_position = camera_focus;
			camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];
			camera_position.y = field->GetHeight(camera_position) + 0.1f;
		}

		camera_pos_len_[i] += CAMARA_LEN_SPEED;
		if(camera_pos_len_[i] > CAMERA_POS_LEN){
			camera_pos_len_[i] = CAMERA_POS_LEN;
		}

		// �J�����Ƀp�����[�^���ăZ�b�g
		main_camera->SetPosition(camera_position);
		main_camera->SetFocus(camera_focus);
		main_camera->SetRotation(camera_rotation);

		//-------------------------------------
		// �o���b�g����
		//-------------------------------------
		shot_late_[i]--;
		shot_late_[i] = std::max<int>(shot_late_[i], 0);

		if(GamePad::isPress(i, PAD_BUTTON_8) && shot_late_[i] == 0)
		{
			shot_late_[i] = 10;

			OBJECT_PARAMETER_DESC bullet_param;
			bullet_param.layer_ = LAYER_BULLET;
			bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
			bullet_param.position_ = child_position;
			bullet_param.position_.x_ += sinf(child_rotation.y_) * 0.8f;
			bullet_param.position_.z_ += cosf(child_rotation.y_) * 0.8f;
			bullet_param.position_.y_ += 0.7f;
			bullet_param.rotation_ = child_rotation;
			bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

			// �J�����̉�]X�𗘗p
			bullet_param.rotation_.x_ = camera_rotation.x;

			// �o���b�g���C
			Bullet* bullet = object_manager_->GetNoUseBullet();
			bullet->Fire(bullet_param);

			// �G�t�F�N�g�Đ�
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = bullet_param.position_.x_;
			send_data.object_param_.position_.y_ = bullet_param.position_.y_;
			send_data.object_param_.position_.z_ = bullet_param.position_.z_;
			send_data.object_param_.rotation_ = {0.0f, child_rotation.y_, 0.0f};
			strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
			NetworkHost::SendTo(DELI_MULTI, send_data);

			//-------------------------------------
			// �e����SE�Đ�
			//-------------------------------------
			Sound::LoadAndPlaySE("resource/sound/se/game/shootChild.wav");
		}

		//------------------------------------------------
		// �v���C���[�f�[�^�]��
		//------------------------------------------------
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.id_ = i;
		send_data.object_param_.ex_id_ = 0;
		send_data.object_param_.type_ = OBJ_PLAYER;
		strcpy_s(send_data.name_, MAX_NAME_LEN, name.c_str());
		if(input)
		{
			if(GamePad::isPress(i, PAD_LS_DOWN) ||
				GamePad::isPress(i, PAD_LS_UP) ||
				GamePad::isPress(i, PAD_LS_LEFT) ||
				GamePad::isPress(i, PAD_LS_RIGHT)){
				send_data.object_param_.ex_id_ = 1;
			}
		}
		else
		{
			send_data.object_param_.ex_id_ = 2;
		}

		send_data.object_param_.position_.x_ = child_position.x_;
		send_data.object_param_.position_.y_ = child_position.y_;
		send_data.object_param_.position_.z_ = child_position.z_;

		send_data.object_param_.rotation_.x_ = child_rotation.x_;
		send_data.object_param_.rotation_.y_ = child_rotation.y_;
		send_data.object_param_.rotation_.z_ = child_rotation.z_;

		NetworkHost::SendTo(DELI_MULTI, send_data);

		//------------------------------------------------
		// �J�����f�[�^�]��
		//------------------------------------------------
		camera_position = main_camera->position();
		camera_focus = main_camera->focus();
		camera_rotation = main_camera->rotation();

		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_CAMERA;
		send_data.object_param_.ex_id_ = 0;

		send_data.object_param_.position_.x_ = camera_position.x;
		send_data.object_param_.position_.y_ = camera_position.y;
		send_data.object_param_.position_.z_ = camera_position.z;

		send_data.object_param_.rotation_.x_ = camera_focus.x;
		send_data.object_param_.rotation_.y_ = camera_focus.y;
		send_data.object_param_.rotation_.z_ = camera_focus.z;
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		// ��]
		send_data.object_param_.ex_id_ = 1;
		send_data.object_param_.rotation_.x_ = camera_rotation.x;
		send_data.object_param_.rotation_.y_ = camera_rotation.y;
		send_data.object_param_.rotation_.z_ = camera_rotation.z;
		NetworkHost::SendTo((DELI_TYPE)i, send_data);
	}
}


//-------------------------------------
// GameGrandfather()
//-------------------------------------
void GameServer::GameGrandfather()
{
	//-------------------------------------
	// �ϐ��錾
	//-------------------------------------
	Field* field(dynamic_cast<Field*>(object_manager_->Get("field")));
	std::string grandfather_str = "player" + std::to_string(0);
	FbxGrandfather *grandfather(dynamic_cast<FbxGrandfather*>(object_manager_->Get(grandfather_str)));
	Vector3 grandfather_position(grandfather->parameter().position_);
	Vector3 grandfather_rotation(grandfather->parameter().rotation_);
	Vector3 grandfather_position_old(grandfather_position);
	NETWORK_DATA send_data;

	//-------------------------------------
	// �v���C���[��n�`�ɉ����Ĉړ�������
	//-------------------------------------
	// ����������񃏁[�v
	if(GamePad::isTrigger(0, PAD_BUTTON_7) && now_target_fort_ <= 2)
	{
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = grandfather_position.x_;
		send_data.object_param_.position_.y_ = grandfather_position.y_;
		send_data.object_param_.position_.z_ = grandfather_position.z_;
		strcpy_s(send_data.name_, MAX_NAME_LEN, "smoke");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		grandfather_position = GRANDFATHER_POSITION_STAGE[now_target_fort_];
		grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE[now_target_fort_];

		send_data.object_param_.position_.x_ = grandfather_position.x_;
		send_data.object_param_.position_.y_ = grandfather_position.y_;
		send_data.object_param_.position_.z_ = grandfather_position.z_;
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}

	// �������_�b�V��
	float grandfather_speed(CHARANCTER_MOVESPEED);
	if(GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_11))
	{
		grandfather_speed = CHARANCTER_MOVESPEED * 2.0f;
		// �_�b�V���G�t�F�N�g
		if(dash_effect_timer_ % 10 == 0){

			// �G�t�F�N�g�Đ�
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = grandfather_position.x_;
			send_data.object_param_.position_.y_ = grandfather_position.y_;
			send_data.object_param_.position_.z_ = grandfather_position.z_;
			send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
			strcpy_s(send_data.name_, MAX_NAME_LEN, "dash");
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
		dash_effect_timer_++;
	}
	// �������f�o�t
	grandfather_speed -= grandfather_speed * grandfather->GetDebuffPower();

	// ����
	grandfather_rotation.y_ += GamePad::isStick(0).rsx_ * CHAR_ROT_SPEED;
	if(grandfather_rotation.y_ > D3DX_PI)
	{
		grandfather_rotation.y_ -= D3DX_PI * 2.0f;
	}
	else if(grandfather_rotation.y_ < -D3DX_PI)
	{
		grandfather_rotation.y_ += D3DX_PI * 2.0f;
	}
	// �ړ�
	grandfather_position.x_ += (
		cosf(grandfather_rotation.y_) * GamePad::isStick(0).lsx_ +
		sinf(-grandfather_rotation.y_) * GamePad::isStick(0).lsy_) * grandfather_speed;
	grandfather_position.z_ -= (
		sinf(grandfather_rotation.y_) * GamePad::isStick(0).lsx_ +
		cosf(-grandfather_rotation.y_) * GamePad::isStick(0).lsy_) * grandfather_speed;

	D3DXVECTOR3 grandfather_pos(
		grandfather_position.x_,
		grandfather_position.y_,
		grandfather_position.z_);
	grandfather_position.y_ = field->GetHeight(grandfather_pos);
	// �N���s�̈�`�F�b�N
	if(grandfather_position.y_ > 0.4f || grandfather_position.y_ < -0.4f){
		grandfather_position = grandfather_position_old;
	}

	grandfather_pos.x = grandfather_position.x_;
	grandfather_pos.y = grandfather_position.y_;
	grandfather_pos.z = grandfather_position.z_;
	grandfather->SetPosition(grandfather_position);
	grandfather->SetRotation(grandfather_rotation);

	//-------------------------------------
	// �J�����Ǐ]
	//-------------------------------------
	std::string camera_str = "camera" + std::to_string(0);
	Camera *main_camera = camera_manager_->Get(camera_str);
	D3DXVECTOR3 camera_position, camera_focus;
	D3DXVECTOR3 camera_rotation(main_camera->rotation());

	// ����
	camera_rotation.x += GamePad::isStick(0).rsy_ * CHAR_ROT_SPEED;
	if(camera_rotation.x > CAMERA_ROT_X_LIMIT)
	{
		camera_rotation.x = CAMERA_ROT_X_LIMIT;
	}
	else if(camera_rotation.x < -CAMERA_ROT_X_LIMIT)
	{
		camera_rotation.x = -CAMERA_ROT_X_LIMIT;
	}

	// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
	camera_rotation.y = grandfather_rotation.y_;
	// ��U���f���𒍎��_��
	camera_focus = grandfather_pos;
	// ���������̂̒��S�ӂ�����
	camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	// ���f���̏����������悤�ɒ���
	camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	// �����_����ɃJ�������W��ݒ�
	camera_position = camera_focus;
	camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
	camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
	camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[0];

	// �J�����̒n�ʂ߂荞�݉������
	D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

	// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
	bool camera_re_calculate = false;
	for(int j = 0; j < 10; ++j)
	{
		// ���Ԓn�_���v�Z
		D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
		float pos_y = field->GetHeight(lay_point);
		// �������
		if(lay_point.y < pos_y + 0.1f)
		{
			camera_re_calculate = true;
			camera_pos_len_[0] -= CAMARA_LEN_SPEED;
		}
	}

	//�J�������W�Čv�Z
	if(camera_re_calculate == true){
		camera_position = camera_focus;
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[0] * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[0];
		camera_position.y = field->GetHeight(camera_position) + 0.1f;
	}

	camera_pos_len_[0] += CAMARA_LEN_SPEED;
	if(camera_pos_len_[0] > CAMERA_POS_LEN)
	{
		camera_pos_len_[0] = CAMERA_POS_LEN;
	}

	// �J�����Ƀp�����[�^���ăZ�b�g
	main_camera->SetPosition(camera_position);
	main_camera->SetFocus(camera_focus);
	main_camera->SetRotation(camera_rotation);

	//-------------------------------------
	// �o���b�g����
	//-------------------------------------
	float watergauge = grandfather->GetWaterGauge();
	shot_late_[0]--;
	shot_late_[0] = std::max<int>(shot_late_[0], 0);

	if(GamePad::isPress(0, PAD_BUTTON_8) && watergauge >= GRANDFATHER_SUB_BULLET_WATERGAUGE && shot_late_[0] == 0)
	{
		shot_late_[0] = 10;

		// �o���b�g�p�����[�^�ݒ�
		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bullet_param.position_ = grandfather_position;
		bullet_param.position_.x_ += sinf(grandfather_rotation.y_) * 0.8f;
		bullet_param.position_.z_ += cosf(grandfather_rotation.y_) * 0.8f;
		bullet_param.position_.y_ += 0.7f;
		bullet_param.rotation_ = grandfather_rotation;
		bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

		// �G�t�F�N�g�Đ�
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = bullet_param.position_.x_;
		send_data.object_param_.position_.y_ = bullet_param.position_.y_;
		send_data.object_param_.position_.z_ = bullet_param.position_.z_;
		send_data.object_param_.rotation_ = {0.0f, grandfather_rotation.y_, 0.0f};
		strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		// ���˃��[�v
		for (int i = 0; i < 3; i++)
		{
			// �J�����̉�]X�𗘗p
			bullet_param.rotation_.x_ = camera_rotation.x;
			float range = MyMath::Random_Range(-5, 5) * 0.01f;
			bullet_param.rotation_.y_ += range;

			// �o���b�g���C
			Bullet* bullet = object_manager_->GetNoUseBullet();
			bullet->Fire(bullet_param);
		}

		//-------------------------------------
		// ���Q�[�W������������
		//-------------------------------------
		watergauge -= GRANDFATHER_SUB_BULLET_WATERGAUGE;
		watergauge = std::max<float>(watergauge, 0.0f);
		grandfather->SetWaterGauge(watergauge);

		//-------------------------------------
		// �e����SE�Đ�
		//-------------------------------------
		Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
	}

	//-------------------------------------
	// �{������
	//-------------------------------------
	if(GamePad::isTrigger(0, PAD_BUTTON_6) && watergauge >= GRANDFATHER_SUB_BOMB_WATERGAUGE)
	{

		OBJECT_PARAMETER_DESC bomb_param;
		bomb_param.layer_ = LAYER_BOMB;
		bomb_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bomb_param.position_ = grandfather_position;
		bomb_param.position_.y_ += 0.6f;
		bomb_param.rotation_ = grandfather_rotation;

		// �J�����̉�]X�𗘗p
		bomb_param.rotation_.x_ = camera_rotation.x;
		float range = MyMath::Random_Range(-5, 5) * 0.01f;
		bomb_param.rotation_.y_ += range;

		// �G�t�F�N�g�Đ�
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = bomb_param.position_.x_;
		send_data.object_param_.position_.y_ = bomb_param.position_.y_;
		send_data.object_param_.position_.z_ = bomb_param.position_.z_;
		send_data.object_param_.rotation_ = {0.0f, grandfather_rotation.y_, 0.0f};
		strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		Bomb* bomb = object_manager_->GetNoUseBomb();
		bomb->Fire(bomb_param);

		//-------------------------------------
		// ���Q�[�W������������
		//-------------------------------------
		watergauge -= GRANDFATHER_SUB_BOMB_WATERGAUGE;
		watergauge = std::max<float>(watergauge, 0.0f);
		grandfather->SetWaterGauge(watergauge);

		//-------------------------------------
		// �����̒e����SE�Đ�
		Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
	}

	//-------------------------------------
	// �����̃f�o�t�p���[���Z�b�g
	//-------------------------------------
	float life = grandfather->GetLife();
	// HP��
	life = std::max<float>(life, 0.0f);
	life += CHILD_RECOVER_HP;
	life = std::min<float>(life, 1.0f);
	grandfather->SetLife(life);
	if(life < 0.5f)
	{
		float debuff_power = life - 0.75f;
		debuff_power = abs(debuff_power * 0.5f);
		grandfather->SetDebuffPower(debuff_power);

		// �G�t�F�N�g�Đ�
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = grandfather_position.x_;
		send_data.object_param_.position_.y_ = grandfather_position.y_;
		send_data.object_param_.position_.z_ = grandfather_position.z_;
		strcpy_s(send_data.name_, MAX_NAME_LEN, "SpeedDown");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}
	else
	{
		// �G�t�F�N�g��~
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.y_ = 50000.0f;
		strcpy_s(send_data.name_, MAX_NAME_LEN, "SpeedDown");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}

	//------------------------------------------------
	// UI�f�[�^�]��
	//------------------------------------------------
	// ���Q�[�W
	send_data.type_ = DATA_UI_PARAM;
	send_data.object_param_.type_ = OBJ_UI;
	send_data.ui_param_.value_f_ = grandfather->GetWaterGauge();
	strcpy_s(send_data.name_, MAX_NAME_LEN, "water_gage");
	NetworkHost::SendTo((DELI_TYPE)0, send_data);

	// �_���[�W�G�t�F�N�g
	send_data.ui_param_.value_f_ = grandfather->GetLife();
	strcpy_s(send_data.name_, MAX_NAME_LEN, "damage_effect");
	NetworkHost::SendTo((DELI_TYPE)0, send_data);

	//------------------------------------------------
	// �v���C���[�f�[�^�]��
	//------------------------------------------------
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.id_ = 0;
	send_data.object_param_.ex_id_ = 0;
	send_data.object_param_.type_ = OBJ_PLAYER;
	strcpy_s(send_data.name_, MAX_NAME_LEN, grandfather_str.c_str());
	if(GamePad::isPress(0, PAD_LS_DOWN) ||
		GamePad::isPress(0, PAD_LS_UP) ||
		GamePad::isPress(0, PAD_LS_LEFT) ||
		GamePad::isPress(0, PAD_LS_RIGHT)){
		send_data.object_param_.ex_id_ = 1;
		walk_timer_[0]++;
		if((walk_timer_[0] >= 30))
		{
			Sound::LoadAndPlaySE("resource/sound/se/game/footstep.wav");
			walk_timer_[0] = 0;
		}
	}

	send_data.object_param_.position_.x_ = grandfather_position.x_;
	send_data.object_param_.position_.y_ = grandfather_position.y_;
	send_data.object_param_.position_.z_ = grandfather_position.z_;

	send_data.object_param_.rotation_.x_ = grandfather_rotation.x_;
	send_data.object_param_.rotation_.y_ = grandfather_rotation.y_;
	send_data.object_param_.rotation_.z_ = grandfather_rotation.z_;

	NetworkHost::SendTo(DELI_MULTI, send_data);

	//------------------------------------------------
	// �J�����f�[�^�]��
	//------------------------------------------------
	camera_position = main_camera->position();
	camera_focus = main_camera->focus();
	camera_rotation = main_camera->rotation();

	ZeroMemory(&send_data, sizeof(send_data));
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_CAMERA;
	send_data.object_param_.ex_id_ = 0;

	switch(scene_state_)
	{
		case STATE_COUNTDOWN:
		case STATE_FORT_OUT:
		case STATE_FORT_IN:
			{
				Camera* sub_camera = camera_manager_->Get("SubCamera");
				D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
				sub_camera_rotation = sub_camera->rotation();
				sub_camera_position = sub_camera->position();
				sub_camera_focus = sub_camera->focus();

				// �f�[�^�Z�b�g
				send_data.object_param_.position_.x_ = sub_camera_position.x;
				send_data.object_param_.position_.y_ = sub_camera_position.y;
				send_data.object_param_.position_.z_ = sub_camera_position.z;

				send_data.object_param_.rotation_.x_ = sub_camera_focus.x;
				send_data.object_param_.rotation_.y_ = sub_camera_focus.y;
				send_data.object_param_.rotation_.z_ = sub_camera_focus.z;
			}
			break;

		default:
			send_data.object_param_.position_.x_ = camera_position.x;
			send_data.object_param_.position_.y_ = camera_position.y;
			send_data.object_param_.position_.z_ = camera_position.z;

			send_data.object_param_.rotation_.x_ = camera_focus.x;
			send_data.object_param_.rotation_.y_ = camera_focus.y;
			send_data.object_param_.rotation_.z_ = camera_focus.z;

			break;

	}
	NetworkHost::SendTo((DELI_TYPE)0, send_data);

	// ��]
	send_data.object_param_.ex_id_ = 1;
	send_data.object_param_.rotation_.x_ = camera_rotation.x;
	send_data.object_param_.rotation_.y_ = camera_rotation.y;
	send_data.object_param_.rotation_.z_ = camera_rotation.z;
	NetworkHost::SendTo((DELI_TYPE)0, send_data);

	//------------------------------------------------
	// �A���[�f�[�^�]��
	//------------------------------------------------
	//ZeroMemory(&send_data, sizeof(send_data));
	//send_data.type_ = DATA_OBJ_PARAM;
	//send_data.object_param_.type_ = OBJ_ARROW;

	//if(now_target_fort_ <= 2)
	//{
	//	send_data.object_param_.position_.x_ = grandfather_position.x_;
	//	send_data.object_param_.position_.y_ = grandfather_position.y_ + 1.5f;
	//	send_data.object_param_.position_.z_ = grandfather_position.z_;

	//	std::string name = "fort" + std::to_string(now_target_fort_);
	//	Object *fort_object = object_manager_->Get(name);
	//	Vector3 fpos = fort_object->parameter().position_;
	//	Vector3 temp = fpos - grandfather_position;
	//	send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
	//	send_data.object_param_.rotation_.y_ = atan2(temp.x_, temp.z_);
	//}
	//else
	//{
	//	send_data.object_param_.position_.y_ = 5000.0f;
	//}

	//NetworkHost::SendTo((DELI_TYPE)i, send_data);
}


//-------------------------------------
// GameChild()
//-------------------------------------
void GameServer::GameChild()
{
	//-------------------------------------
	// ���p�ϐ���`
	//-------------------------------------
	Field *field(dynamic_cast<Field*>(object_manager_->Get("field")));
	std::string child_str;
	FbxChild *child;
	Vector3 child_position;
	Vector3 child_rotation;
	Vector3 child_position_old;
	NETWORK_DATA send_data;
	int my_id;

	for(int i = 1; i < 5; i++)
	{
		//-------------------------------------
		// �ϐ��錾
		//-------------------------------------
		child_str = "player" + std::to_string(i);
		child = dynamic_cast<FbxChild*>(object_manager_->Get(child_str));
		child_position = child->parameter().position_;
		child_rotation = child->parameter().rotation_;
		child_position_old = child_position;
		my_id = i - 1;

		//-------------------------------------
		// �v���C���[��n�`�ɉ����Ĉړ�������
		//-------------------------------------
		bool input(true);
		if(child_death_[my_id])
		{
			input = false;
		}
		if(input)
		{
			// ����
			child_rotation.y_ += GamePad::isStick(i).rsx_ * CHAR_ROT_SPEED;
			if(child_rotation.y_ > D3DX_PI)
			{
				child_rotation.y_ -= D3DX_PI * 2.0f;
			}
			else if(child_rotation.y_ < -D3DX_PI)
			{
				child_rotation.y_ += D3DX_PI * 2.0f;
			}
			// �ړ�
			float child_speed = CHARANCTER_MOVESPEED;
			child_position.x_ += (
				cosf(child_rotation.y_) * GamePad::isStick(i).lsx_ +
				sinf(-child_rotation.y_) * GamePad::isStick(i).lsy_) * child_speed;
			child_position.z_ -= (
				sinf(child_rotation.y_) * GamePad::isStick(i).lsx_ +
				cosf(-child_rotation.y_) * GamePad::isStick(i).lsy_) * child_speed;
		}

		D3DXVECTOR3 child_pos(
			child_position.x_,
			child_position.y_,
			child_position.z_);
		child_position.y_ = field->GetHeight(child_pos);
		// �N���s�̈�`�F�b�N
		if(child_position.y_ > 0.4f || child_position.y_ < -0.4f){
			child_position = child_position_old;
		}

		child_pos.x = child_position.x_;
		child_pos.y = child_position.y_;
		child_pos.z = child_position.z_;
		child->SetPosition(child_position);
		child->SetRotation(child_rotation);

		//-------------------------------------
		// �J�����Ǐ]
		//-------------------------------------
		std::string camera_str = "camera" + std::to_string(i);
		Camera *main_camera = camera_manager_->Get(camera_str);
		D3DXVECTOR3 camera_position, camera_focus;
		D3DXVECTOR3 camera_rotation(main_camera->rotation());

		// ����
		camera_rotation.x += GamePad::isStick(i).rsy_ * CHAR_ROT_SPEED;
		if(camera_rotation.x > CAMERA_ROT_X_LIMIT)
		{
			camera_rotation.x = CAMERA_ROT_X_LIMIT;
		}
		else if(camera_rotation.x < -CAMERA_ROT_X_LIMIT)
		{
			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
		}

		// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
		camera_rotation.y = child_rotation.y_;
		// ��U���f���𒍎��_��
		camera_focus = child_pos;
		// ���������̂̒��S�ӂ�����
		camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
		// ���f���̏����������悤�ɒ���
		camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

		// �����_����ɃJ�������W��ݒ�
		camera_position = camera_focus;
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];

		// �J�����̒n�ʂ߂荞�݉������
		D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

		// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
		bool camera_re_calculate = false;
		for(int j = 0; j < 10; ++j){
			// ���Ԓn�_���v�Z
			D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
			float pos_y = field->GetHeight(lay_point);
			// �������
			if(lay_point.y < pos_y + 0.1f){
				camera_re_calculate = true;
				camera_pos_len_[i] -= CAMARA_LEN_SPEED;
			}
		}

		//�J�������W�Čv�Z
		if(camera_re_calculate == true){
			camera_position = camera_focus;
			camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];
			camera_position.y = field->GetHeight(camera_position) + 0.1f;
		}

		camera_pos_len_[i] += CAMARA_LEN_SPEED;
		if(camera_pos_len_[i] > CAMERA_POS_LEN){
			camera_pos_len_[i] = CAMERA_POS_LEN;
		}

		// �J�����Ƀp�����[�^���ăZ�b�g
		main_camera->SetPosition(camera_position);
		main_camera->SetFocus(camera_focus);
		main_camera->SetRotation(camera_rotation);

		//-------------------------------------
		// �o���b�g����
		//-------------------------------------
		float watergauge = child->GetWaterGauge();
		shot_late_[i]--;
		shot_late_[i] = std::max<int>(shot_late_[i], 0);

		if(GamePad::isPress(i, PAD_BUTTON_8) && watergauge > 0.0f && shot_late_[i] == 0)
		{
			shot_late_[i] = 10;

			OBJECT_PARAMETER_DESC bullet_param;
			bullet_param.layer_ = LAYER_BULLET;
			bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
			bullet_param.position_ = child_position;
			bullet_param.position_.x_ += sinf(child_rotation.y_) * 0.8f;
			bullet_param.position_.z_ += cosf(child_rotation.y_) * 0.8f;
			bullet_param.position_.y_ += 0.7f;
			bullet_param.rotation_ = child_rotation;
			bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

			// �J�����̉�]X�𗘗p
			bullet_param.rotation_.x_ = camera_rotation.x;

			// �o���b�g���C
			Bullet* bullet = object_manager_->GetNoUseBullet();
			bullet->Fire(bullet_param);

			// �G�t�F�N�g�Đ�
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = bullet_param.position_.x_;
			send_data.object_param_.position_.y_ = bullet_param.position_.y_;
			send_data.object_param_.position_.z_ = bullet_param.position_.z_;
			send_data.object_param_.rotation_ = {0.0f, child_rotation.y_, 0.0f};
			strcpy_s(send_data.name_, MAX_NAME_LEN, "water");
			NetworkHost::SendTo(DELI_MULTI, send_data);

			//-------------------------------------
			// ���Q�[�W������������
			//-------------------------------------
			watergauge -= GRANDFATHER_SUB_BULLET_WATERGAUGE;
			watergauge = std::max<float>(watergauge, 0.0f);
			child->SetWaterGauge(watergauge);

			//-------------------------------------
			// �e����SE�Đ�
			//-------------------------------------
			Sound::LoadAndPlaySE("resource/sound/se/game/shootChild.wav");
		}

		//-------------------------------------
		// �q�����S������
		//-------------------------------------
		float child_life = child->GetLife();
		if(child_life < 0 && !child_death_[my_id])
		{
			child->PlayAnimation(FbxChild::DOWN);
			child_death_[my_id] = true;
			child_respawn_waittime_[my_id] = CHILD_RESPAWN_WAITTIME;

			Vector3 child_position = child->parameter().position_;

			// �G�t�F�N�g�Đ�
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = child_position.x_;
			send_data.object_param_.position_.y_ = child_position.y_;
			send_data.object_param_.position_.z_ = child_position.z_;
			send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
			strcpy_s(send_data.name_, MAX_NAME_LEN, "dead");
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
		else if(child_death_[my_id] && !child_respawn_waittime_[my_id])
		{
			child->PlayAnimation(FbxChild::IDLE);
			child_death_[my_id] = false;
			child_life = CHILD_LIFE;
			child->SetLife(child_life);
			if(now_target_fort_ <= 2)
			{
				child->SetPosition(CHILD_POSITION[my_id + (now_target_fort_ * 4)]);
				child->SetRotationY(CHILD_ROTATION[my_id + (now_target_fort_ * 4)]);
			}
			else
			{
				child->SetPosition(CHILD_POSITION[my_id + (2 * 4)]);
				child->SetRotationY(CHILD_ROTATION[my_id + (2 * 4)]);
			}
		}

		child_respawn_waittime_[my_id]--;
		child_respawn_waittime_[my_id] = std::max<int>(child_respawn_waittime_[my_id], 0);

		//-------------------------------------
		// �q���̗͎����񕜐���
		//-------------------------------------
		if(child_life < 1.0f && !child_death_[my_id]){
			int child_recover_wait_timer = child->GetRecoverWaitTimer();

			if(child_recover_wait_timer > CHILD_RECOVER_WAITE_TIME){
				float child_life = child->GetLife();
				child_life += CHILD_RECOVER_HP;
				std::min<float>(child_life, 1.0f);
				child->SetLife(child_life);
			}
			child_recover_wait_timer++;
			child->SetRecoverWaitTimer(child_recover_wait_timer);
		}

		//------------------------------------------------
		// UI�f�[�^�]��
		//------------------------------------------------
		// ���Q�[�W
		send_data.type_ = DATA_UI_PARAM;
		send_data.object_param_.type_ = OBJ_UI;
		send_data.ui_param_.value_f_ = child->GetWaterGauge();
		strcpy_s(send_data.name_, MAX_NAME_LEN, "water_gage");
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		// �_���[�W�G�t�F�N�g
		send_data.ui_param_.value_f_ = child->GetLife();
		strcpy_s(send_data.name_, MAX_NAME_LEN, "damage_effect");
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		//------------------------------------------------
		// �v���C���[�f�[�^�]��
		//------------------------------------------------
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.id_ = i;
		send_data.object_param_.ex_id_ = 0;
		send_data.object_param_.type_ = OBJ_PLAYER;
		strcpy_s(send_data.name_, MAX_NAME_LEN, child_str.c_str());
		if(input)
		{
			if(GamePad::isPress(i, PAD_LS_DOWN) ||
				GamePad::isPress(i, PAD_LS_UP) ||
				GamePad::isPress(i, PAD_LS_LEFT) ||
				GamePad::isPress(i, PAD_LS_RIGHT)){
				send_data.object_param_.ex_id_ = 1;
				walk_timer_[i]++;
				if((walk_timer_[i] >= 30))
				{
					Sound::LoadAndPlaySE("resource/sound/se/game/footstep.wav");
					walk_timer_[i] = 0;
				}

			}
		}
		else
		{
			send_data.object_param_.ex_id_ = 2;
		}

		send_data.object_param_.position_.x_ = child_position.x_;
		send_data.object_param_.position_.y_ = child_position.y_;
		send_data.object_param_.position_.z_ = child_position.z_;

		send_data.object_param_.rotation_.x_ = child_rotation.x_;
		send_data.object_param_.rotation_.y_ = child_rotation.y_;
		send_data.object_param_.rotation_.z_ = child_rotation.z_;

		NetworkHost::SendTo(DELI_MULTI, send_data);

		//------------------------------------------------
		// �J�����f�[�^�]��
		//------------------------------------------------
		camera_position = main_camera->position();
		camera_focus = main_camera->focus();
		camera_rotation = main_camera->rotation();

		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_CAMERA;
		send_data.object_param_.ex_id_ = 0;

		switch(scene_state_)
		{
			case STATE_COUNTDOWN:
			case STATE_FORT_OUT:
			case STATE_FORT_IN:
				{
					Camera* sub_camera = camera_manager_->Get("SubCamera");
					D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
					sub_camera_rotation = sub_camera->rotation();
					sub_camera_position = sub_camera->position();
					sub_camera_focus = sub_camera->focus();

					// �f�[�^�Z�b�g
					send_data.object_param_.position_.x_ = sub_camera_position.x;
					send_data.object_param_.position_.y_ = sub_camera_position.y;
					send_data.object_param_.position_.z_ = sub_camera_position.z;

					send_data.object_param_.rotation_.x_ = sub_camera_focus.x;
					send_data.object_param_.rotation_.y_ = sub_camera_focus.y;
					send_data.object_param_.rotation_.z_ = sub_camera_focus.z;
				}
				break;

			default:
				send_data.object_param_.position_.x_ = camera_position.x;
				send_data.object_param_.position_.y_ = camera_position.y;
				send_data.object_param_.position_.z_ = camera_position.z;

				send_data.object_param_.rotation_.x_ = camera_focus.x;
				send_data.object_param_.rotation_.y_ = camera_focus.y;
				send_data.object_param_.rotation_.z_ = camera_focus.z;

				break;

		}
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		// ��]
		send_data.object_param_.ex_id_ = 1;
		send_data.object_param_.rotation_.x_ = camera_rotation.x;
		send_data.object_param_.rotation_.y_ = camera_rotation.y;
		send_data.object_param_.rotation_.z_ = camera_rotation.z;
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		//------------------------------------------------
		// �A���[�f�[�^�]��
		//------------------------------------------------
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_ARROW;

		if(now_target_fort_ <= 2)
		{
			send_data.object_param_.position_.x_ = child_position.x_;
			send_data.object_param_.position_.y_ = child_position.y_ + 1.5f;
			send_data.object_param_.position_.z_ = child_position.z_;

			std::string name = "fort" + std::to_string(now_target_fort_);
			Object *fort_object = object_manager_->Get(name);
			Vector3 fpos = fort_object->parameter().position_;
			Vector3 temp = fpos - child_position;
			send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
			send_data.object_param_.rotation_.y_ = atan2(temp.x_, temp.z_);
		}
		else
		{
			send_data.object_param_.position_.y_ = 5000.0f;
		}

		NetworkHost::SendTo((DELI_TYPE)i, send_data);
	}
}


//-------------------------------------
// ChangeServerState(SERVER_STATE next)
//-------------------------------------
void GameServer::ChangeServerState(SERVER_STATE next)
{
	// �T�E���h�t�F�[�h�A�E�g
	switch(server_state_)
	{
		case STATE_MATCHING:
			matching_bgm_->Stop(60);
			break;
		case STATE_GAME:
			game_bgm_->Stop(60);
			break;
		case STATE_RESULT:
			result_bgm_->Stop(60);
			break;
		default:
			break;
	}

	// ���ʐݒ菉����
	for(int i = 0; i < MAX_GUEST; i++)
	{
		camera_pos_len_[i] = CAMERA_POS_LEN;
		shot_late_[i] = 0;
		walk_timer_[i] = 0;
	}
	collision_manager_->Update();
	now_target_fort_ = 0;
	server_state_ = next;
	switch(server_state_)
	{
		case STATE_MATCHING:
			{
				//---------------------------------------
				// �V�[���X�e�[�g������
				scene_state_ = STATE_MATCH_WAITING;
				//---------------------------------------
				// �t�B�[���h�擾
				Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
				//---------------------------------------
				// �v���C���[�֘A�p�����[�^
				std::string player_str;
				FbxPlayer *player;
				for(int i = 0; i < MAX_GUEST; i++)
				{
					player_str = "player" + std::to_string(i);
					player = dynamic_cast<FbxPlayer*>(object_manager_->Get(player_str));

					player->SetLife(1.0f);
					player->SetWaterGauge(1.0f);
					player->SetRecoverWaitTimer(0);
					player->SetWaterSupplyEnable(0);

					Vector3 pos, rot;
					pos = MATCHING_POSITION[i];
					rot = {0.0f, MATCHING_ROTATION[i], 0.0f};

					D3DXVECTOR3 poss;
					poss.x = pos.x_;
					poss.y = pos.y_;
					poss.z = pos.z_;
					pos.y_ = field->GetHeight(poss);

					player->SetPosition(pos);
					player->SetRotation(rot);

					//------------------------------------------------
					// �J��������
					//------------------------------------------------
					std::string camera_str = "camera" + std::to_string(i);
					Camera *main_camera = camera_manager_->Get(camera_str);
					D3DXVECTOR3 camera_position, camera_focus;
					D3DXVECTOR3 camera_rotation(main_camera->rotation());

					// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
					camera_rotation.y = rot.y_;
					// �J������X��]��������
					camera_rotation.x = -0.3f;
					// ��U���f���𒍎��_��
					camera_focus.x = pos.x_;
					camera_focus.y = pos.y_;
					camera_focus.z = pos.z_;
					// ���������̂̒��S�ӂ�����
					camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
					// ���f���̏����������悤�ɒ���
					camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
					camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
					camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

					// �����_����ɃJ�������W��ݒ�
					camera_position = camera_focus;
					camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
					camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
					camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];

					// �J�����̒n�ʂ߂荞�݉������
					D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

					// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
					bool camera_re_calculate = false;
					for(int j = 0; j < 10; ++j){
						// ���Ԓn�_���v�Z
						D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
						float pos_y = field->GetHeight(lay_point);
						// �������
						if(lay_point.y < pos_y + 0.1f){
							camera_re_calculate = true;
							camera_pos_len_[i] -= CAMARA_LEN_SPEED;
						}
					}

					//�J�������W�Čv�Z
					if(camera_re_calculate == true){
						camera_position = camera_focus;
						camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
						camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
						camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];
						camera_position.y = field->GetHeight(camera_position) + 0.1f;
					}

					camera_pos_len_[i] += CAMARA_LEN_SPEED;
					if(camera_pos_len_[i] > CAMERA_POS_LEN){
						camera_pos_len_[i] = CAMERA_POS_LEN;
					}

					// �J�����Ƀp�����[�^���ăZ�b�g
					main_camera->SetPosition(camera_position);
					main_camera->SetFocus(camera_focus);
					main_camera->SetRotation(camera_rotation);
				}
				matching_bgm_->Play(true, 120);
			}
			break;

		case STATE_GAME:
			{
				//---------------------------------------
				// �t�B�[���h�擾
				Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
				//---------------------------------------
				// �V�[���X�e�[�g���Z�b�g
				scene_state_ = STATE_GUEST_WAITING;
				//---------------------------------------
				// �ԃp�����[�^�̏C��
				XFort *fort;
				Vector3 fort_pos;
				D3DXVECTOR3 temp;
				for(int i = 0; i < 3; i++)
				{
					fort = dynamic_cast<XFort*>(object_manager_->Get("fort" + std::to_string(i)));
					fort->SetLife(FORT_LIFE[i]);

					fort_pos = FORT_POSITION[i];
					temp.x = fort_pos.x_;
					temp.y = fort_pos.y_;
					temp.z = fort_pos.z_;

					if(i == 0)
					{
						fort_y_[i] = 0.0f;
					}
					else
					{
						fort_y_[i] = -3.0f;
					}
					fort_pos.y_ = field->GetHeight(temp) + fort_y_[i];

					fort->SetPosition(fort_pos);
				}

				fort_announce_state_ = 75;
				now_target_fort_ = 0;
				//---------------------------------------
				// �����֘A�p�����[�^
				dash_effect_timer_ = 0;
				grandfather_debuff_ = false;
				//---------------------------------------
				// �q���֘A�p�����[�^
				for(int i = 0; i < (MAX_GUEST - 1); i++)
				{
					child_death_[i] = false;
					child_respawn_waittime_[i] = 0;
				}
				//---------------------------------------
				// �v���C���[�֘A�p�����[�^
				std::string player_str;
				FbxPlayer *player;
				for(int i = 0; i < MAX_GUEST; i++)
				{
					player_str = "player" + std::to_string(i);
					player = dynamic_cast<FbxPlayer*>(object_manager_->Get(player_str));

					player->SetLife(1.0f);
					player->SetWaterGauge(1.0f);
					player->SetRecoverWaitTimer(0);
					player->SetWaterSupplyEnable(0);

					Vector3 pos, rot;
					if(i == 0)
					{
						pos = GRANDFATHER_POSITION_STAGE[0];
						rot = {0.0f, GRANDFATHER_ROTATION_STAGE[0], 0.0f};
					}
					else
					{
						pos = CHILD_POSITION[i - 1];
						rot = {0.0f, CHILD_ROTATION[i - 1], 0.0f};
					}
					D3DXVECTOR3 poss;
					poss.x = pos.x_;
					poss.y = pos.y_;
					poss.z = pos.z_;
					pos.y_ = field->GetHeight(poss);

					player->SetPosition(pos);
					player->SetRotation(rot);

					//------------------------------------------------
					// �J��������
					//------------------------------------------------
					std::string camera_str = "camera" + std::to_string(i);
					Camera *main_camera = camera_manager_->Get(camera_str);
					D3DXVECTOR3 camera_position, camera_focus;
					D3DXVECTOR3 camera_rotation(main_camera->rotation());

					// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
					camera_rotation.y = rot.y_;
					// �J������X��]��������
					camera_rotation.x = -0.3f;
					// ��U���f���𒍎��_��
					camera_focus.x = pos.x_;
					camera_focus.y = pos.y_;
					camera_focus.z = pos.z_;
					// ���������̂̒��S�ӂ�����
					camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
					// ���f���̏����������悤�ɒ���
					camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
					camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
					camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

					// �����_����ɃJ�������W��ݒ�
					camera_position = camera_focus;
					camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
					camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
					camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];

					// �J�����̒n�ʂ߂荞�݉������
					D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

					// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
					bool camera_re_calculate = false;
					for(int j = 0; j < 10; ++j){
						// ���Ԓn�_���v�Z
						D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
						float pos_y = field->GetHeight(lay_point);
						// �������
						if(lay_point.y < pos_y + 0.1f){
							camera_re_calculate = true;
							camera_pos_len_[i] -= CAMARA_LEN_SPEED;
						}
					}

					//�J�������W�Čv�Z
					if(camera_re_calculate == true){
						camera_position = camera_focus;
						camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
						camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
						camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];
						camera_position.y = field->GetHeight(camera_position) + 0.1f;
					}

					camera_pos_len_[i] += CAMARA_LEN_SPEED;
					if(camera_pos_len_[i] > CAMERA_POS_LEN){
						camera_pos_len_[i] = CAMERA_POS_LEN;
					}

					// �J�����Ƀp�����[�^���ăZ�b�g
					main_camera->SetPosition(camera_position);
					main_camera->SetFocus(camera_focus);
					main_camera->SetRotation(camera_rotation);
				}
				game_bgm_->Play(true, 120);
			}
			break;

		case STATE_RESULT:
			result_bgm_->Play(true, 120);
			break;

		default:
			break;
	}

}


//-------------------------------------
// end of file
//-------------------------------------