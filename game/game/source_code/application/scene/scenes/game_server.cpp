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

	//-------------------------------------
	// �J����
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;

	camera_manager_->Create(
		"Perspective", "camera1", camera_param);

	camera_manager_->Create(
		"Perspective", "camera2", camera_param);

	camera_manager_->Create(
		"Perspective", "camera3", camera_param);

	camera_manager_->Create(
		"Perspective", "camera4", camera_param);

	camera_manager_->Create(
		"Perspective", "camera5", camera_param);

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
	param.position_ = {0.0f, 0.0f, 0.0f};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {200.0f, 1.0f, 200.0f};
	param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		"field",
		param,
		"resource/mesh/map.heightmap");


	//-------------------------------------
	// ��
	//-------------------------------------
	// ��1
	OBJECT_PARAMETER_DESC fort1_param;
	fort1_param.layer_ = LAYER_MODEL_FORT;
	fort1_param.position_ = FORT1_POSITION;
	fort1_param.rotation_ = {0.0f, 0.0f, 0.0f};
	fort1_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(
		"fort1",
		fort1_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort1_collision_param;
	Object *fort1_obj = object_manager_->Get("fort1");
	fort1_collision_param.position_ = {
		fort1_obj->parameter().position_.x_,
		fort1_obj->parameter().position_.y_ + 0.5f,
		fort1_obj->parameter().position_.z_};
	fort1_collision_param.range_ = 1.0f;
	fort1_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(fort1_obj,
		fort1_collision_param);
	XFort *fort1 = dynamic_cast<XFort*>(fort1_obj);
	fort1->SetLife(FORT1_LiFE);


	// ��2
	OBJECT_PARAMETER_DESC fort2_param;
	fort2_param.layer_ = LAYER_MODEL_FORT;
	fort2_param.position_ = FORT2_POSITION;
	fort2_param.rotation_ = {0.0f, 0.0f, 0.0f};
	fort2_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(
		"fort2",
		fort2_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort2_collision_param;
	Object *fort2_obj = object_manager_->Get("fort2");
	fort2_collision_param.position_ = {
		fort2_obj->parameter().position_.x_,
		fort2_obj->parameter().position_.y_ + 0.5f,
		fort2_obj->parameter().position_.z_};
	fort2_collision_param.range_ = 1.0f;
	fort2_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(fort2_obj,
		fort2_collision_param);
	XFort *fort2 = dynamic_cast<XFort*>(fort2_obj);
	fort2->SetLife(FORT2_LiFE);


	// ��3
	OBJECT_PARAMETER_DESC fort3_param;
	fort3_param.layer_ = LAYER_MODEL_FORT;
	fort3_param.position_ = FORT3_POSITION;
	fort3_param.rotation_ = {0.0f, 0.0f, 0.0f};
	fort3_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(
		"fort3",
		fort3_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort3_collision_param;
	Object *fort3_obj = object_manager_->Get("fort3");
	fort3_collision_param.position_ = {
		fort3_obj->parameter().position_.x_,
		fort3_obj->parameter().position_.y_ + 0.5f,
		fort3_obj->parameter().position_.z_};
	fort3_collision_param.range_ = 1.0f;
	fort3_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(fort3_obj,
		fort3_collision_param);
	XFort *fort3 = dynamic_cast<XFort*>(fort3_obj);
	fort3->SetLife(FORT3_LiFE);


	//-------------------------------------
	// FBX����
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = {1.0f, 0.0f, 0.0f};
	grandfather_param.rotation_ = {0.0f, 0.0f, 0.0f};
	grandfather_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(
		"player1",
		grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("player1");

	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_};
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(object_manager_->Get("player1"), fbx_collision_param);

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
		std::string name = "player" + std::to_string(i + 1);
		object_manager_->Create(
			name,
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
	// �Q�[�����[���p�p�����[�^������
	//-------------------------------------
	for(int i = 0; i < MAX_GUEST; i++)
	{
		guest_scene_change_[i] = false;
	}
	bullet_count_ = 0;
	// �X�e�[�W
	stage_ = 1;
	// �����f�o�t�t���O
	grandfather_debuff_ = false;
	for(int i = 0; i < (MAX_GUEST - 1); i++)
	{
		// �q�����S�t���O
		child_death_[i] = false;
		// �q�����X�|�[���҂�����
		child_respawn_waittime_[i] = 0;
	}

	//-------------------------------------
	// �X�e�[�g�Z�b�g
	//-------------------------------------
	ChangeState(STATE_MATCHING);
}


//-------------------------------------
// ~GameServer()
//-------------------------------------
GameServer::~GameServer()
{
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
	// ��ԕ���
	switch(state)
	{
		case STATE_MATCHING:
			{
				MatchingAndGame();
				font_->Add("�V�[����:");
				font_->Add("Matching\n");
				if(KeyBoard::isTrigger(DIK_RETURN))
				{
					ChangeState(STATE_GAME);
					// �V�[���`�F���W���ߑ��M
					NETWORK_DATA send_data;
					send_data.type_ = DATA_SCENE_CHANGE_GAME;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
			}
			break;

		case STATE_GAME:
			{
				MatchingAndGame();

				NETWORK_DATA send_data;
				time_--;
				if(time_ > 0)
				{
					send_data.type_ = DATA_UI_PARAM;
					send_data.object_param_.type_ = OBJ_UI;
					strcpy_s(send_data.name, MAX_NAME_LEN, "time");
					send_data.ui_param_.value_i_ = time_ / 60;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}

				font_->Add("�V�[����:");
				font_->Add("Game\n");
				// ���҂�����
				if(KeyBoard::isTrigger(DIK_9))
				{
					ChangeState(STATE_RESULT);
					// �V�[���`�F���W���ߑ��M
					NETWORK_DATA send_data;
					send_data.type_ = DATA_SCENE_CHANGE_RESULT;
					send_data.object_param_.type_ = OBJ_GRANDFATHER;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
				// ���҃K�L
				else if(KeyBoard::isTrigger(DIK_0))
				{
					ChangeState(STATE_RESULT);
					// �V�[���`�F���W���ߑ��M
					NETWORK_DATA send_data;
					send_data.type_ = DATA_SCENE_CHANGE_RESULT;
					send_data.object_param_.type_ = OBJ_CHILD;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
			}
			break;

		case STATE_RESULT:
			font_->Add("�V�[����:");
			font_->Add("Result\n");
			if(KeyBoard::isTrigger(DIK_RETURN))
			{
				ChangeState(STATE_MATCHING);
				// �V�[���`�F���W���ߑ��M
				NETWORK_DATA send_data;
				send_data.type_ = DATA_SCENE_CHANGE_MATCHING;
				NetworkHost::SendTo(DELI_MULTI, send_data);
			}
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
	camera_manager_->Set("camera1");
	object_manager_->Draw();
	collision_manager_->Draw();
#endif

	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}

//-------------------------------------
// MatchingAndGame()
//-------------------------------------
void GameServer::MatchingAndGame()
{
	// �f�[�^�]���p�\���̗p��
	NETWORK_DATA send_data;

	static const float player_speed_value = 0.05f;

	//-------------------------------------
	// �Ԃ̍��W�Ǘ�
	//-------------------------------------
	static D3DXVECTOR3 fort_underground(0.0f, 0.0f, 0.0f);
	switch(stage_)
	{
	case 1:
		fort_underground.x += 0.01f;
		fort_underground.y -= 0.01f;
		fort_underground.z -= 0.01f;
		fort_underground.x = std::min<float>(fort_underground.x, 0.0f);
		fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
		fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
		break;
	case 2:
		fort_underground.x -= 0.01f;
		fort_underground.y += 0.01f;
		fort_underground.z -= 0.01f;
		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
		fort_underground.y = std::min<float>(fort_underground.y, 0.0f);
		fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
		break;
	case 3:
		fort_underground.x -= 0.01f;
		fort_underground.y -= 0.01f;
		fort_underground.z += 0.01f;
		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
		fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
		fort_underground.z = std::min<float>(fort_underground.z, 0.0f);
		break;
	}

	////-------------------------------------
	//// �T�u�J�����v�Z
	////-------------------------------------
	//Camera *sub_camera = camera_manager_->Get("SubCamera");
	//D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
	//// �K���Ȉʒu���璭�߂�
	//sub_camera_rotation.x = -D3DX_PI*0.15f;
	//// ��U�Ԃ𒍎��_��
	//switch(stage_)
	//{
	//	case 1:
	//		sub_camera_focus = fort1_pos;
	//		sub_camera_focus.y = 0.0f;
	//		if(fort_underground.x != 0.0f){
	//			use_camera_name_ = "SubCamera";
	//		}
	//		else{
	//			use_camera_name_ = "MainCamera";
	//		}
	//		break;
	//	case 2:
	//		sub_camera_focus = fort2_pos;
	//		sub_camera_focus.y = 0.0f;
	//		if(fort_underground.y != 0.0f){
	//			use_camera_name_ = "SubCamera";
	//		}
	//		else{
	//			use_camera_name_ = "MainCamera";
	//		}
	//		break;
	//	case 3:
	//		sub_camera_focus = fort3_pos;
	//		sub_camera_focus.y = 0.0f;
	//		if(fort_underground.z != 0.0f){
	//			use_camera_name_ = "SubCamera";
	//		}
	//		else{
	//			use_camera_name_ = "MainCamera";
	//		}
	//		break;
	//}

	//// ���f���̒��S�ӂ�����
	//sub_camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	//// ���f���̏����������悤�ɒ���
	//sub_camera_focus.x +=
	//	sinf(sub_camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(sub_camera_rotation.x);
	//sub_camera_focus.z +=
	//	cosf(sub_camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(sub_camera_rotation.x);
	//sub_camera_focus.y +=
	//	sinf(sub_camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	//// �����_����ɃJ�������W��ݒ�
	//sub_camera_position = sub_camera_focus;
	//sub_camera_position.x -=
	//	sinf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
	//sub_camera_position.z -=
	//	cosf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
	//sub_camera_position.y -=
	//	sinf(sub_camera_rotation.x) * camera_pos_len_;

	//// �J�����Ƀp�����[�^���ăZ�b�g
	//sub_camera->SetPosition(sub_camera_position);
	//sub_camera->SetFocus(sub_camera_focus);
	//sub_camera->SetRotation(sub_camera_rotation);

	Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
	D3DXVECTOR3 fort_pos;
	Vector3 fort_position;
	Vector3 fort_rotation;
	float fort_life[3];
	for(int i = 0; i < 3; i++)
	{
		std::string fort_str = "fort" + std::to_string(i + 1);
		Object *fort_object = object_manager_->Get(fort_str);
		XFort *fort = dynamic_cast<XFort*>(fort_object);
		fort_position = fort_object->parameter().position_;
		fort_rotation = fort_object->parameter().rotation_;
		fort_pos.x = fort_position.x_;
		fort_pos.y = fort_position.y_;
		fort_pos.z = fort_position.z_;
		switch(i)
		{
			case 0:
				fort_position.y_ = field->GetHeight(fort_pos) + fort_underground.x;
				break;
			case 1:
				fort_position.y_ = field->GetHeight(fort_pos) + fort_underground.y;
				break;
			case 2:
				fort_position.y_ = field->GetHeight(fort_pos) + fort_underground.z;
				break;
			default:
				break;
		}
		fort_object->SetPosition(fort_position);

		fort_life[i] = fort->GetLife();

		// �I�u�W�F�N�g�f�[�^�]��
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_FORT;
		send_data.object_param_.position_.x_ = fort_position.x_;
		send_data.object_param_.position_.y_ = fort_position.y_;
		send_data.object_param_.position_.z_ = fort_position.z_;
		send_data.object_param_.rotation_.x_ = fort_rotation.x_;
		send_data.object_param_.rotation_.y_ = fort_rotation.y_;
		send_data.object_param_.rotation_.z_ = fort_rotation.z_;
		strcpy_s(send_data.name, MAX_NAME_LEN, fort_str.c_str());
		NetworkHost::SendTo(DELI_MULTI, send_data);

		// �I�u�W�F�N�g�f�[�^�]��
		send_data.type_ = DATA_UI_PARAM;
		send_data.id_ = i;
		send_data.ui_param_.value_f_ = fort_life[i];
		strcpy_s(send_data.name, MAX_NAME_LEN, "fort_gauge_manager");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}

	//-------------------------------------
	// �ԂɂƂ肠�����G�t�F�N�g����
	//-------------------------------------
	int set_camera = 0;
	if(fort_underground.x != 0.0f && fort_underground.x != -3.0f){
		set_camera = 1;

		Object *fort_object = object_manager_->Get("fort1");
		XFort *fort = dynamic_cast<XFort*>(fort_object);
		fort_position = fort_object->parameter().position_;
		fort_pos.x = fort_position.x_;
		fort_pos.y = fort_position.y_;
		fort_pos.z = fort_position.z_;

		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = fort_position.x_;
		send_data.object_param_.position_.y_ = field->GetHeight(fort_pos);
		send_data.object_param_.position_.z_ = fort_position.z_;
		strcpy_s(send_data.name, MAX_NAME_LEN, "smoke2");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}
	if(fort_underground.y != 0.0f && fort_underground.y != -3.0f){
		set_camera = 2;

		Object *fort_object = object_manager_->Get("fort2");
		XFort *fort = dynamic_cast<XFort*>(fort_object);
		fort_position = fort_object->parameter().position_;
		fort_pos.x = fort_position.x_;
		fort_pos.y = fort_position.y_;
		fort_pos.z = fort_position.z_;

		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = fort_position.x_;
		send_data.object_param_.position_.y_ = field->GetHeight(fort_pos);
		send_data.object_param_.position_.z_ = fort_position.z_;
		strcpy_s(send_data.name, MAX_NAME_LEN, "smoke2");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}
	if(fort_underground.z != 0.0f && fort_underground.z != -3.0f){
		set_camera = 3;

		Object *fort_object = object_manager_->Get("fort3");
		XFort *fort = dynamic_cast<XFort*>(fort_object);
		fort_position = fort_object->parameter().position_;
		fort_pos.x = fort_position.x_;
		fort_pos.y = fort_position.y_;
		fort_pos.z = fort_position.z_;

		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = fort_position.x_;
		send_data.object_param_.position_.y_ = field->GetHeight(fort_pos);
		send_data.object_param_.position_.z_ = fort_position.z_;
		strcpy_s(send_data.name, MAX_NAME_LEN, "smoke2");
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}

	//-------------------------------------
	// �Q�[���X�e�[�W�f�o�b�O
	//-------------------------------------
	if(fort_life[0] == 0.0f){
		stage_ = 2;
		if(fort_life[1] == 0.0f){
			stage_ = 3;
		}
	}

	for(int i = 0; i < 5; i++){

		//-------------------------------------
		// �ϐ��錾
		//-------------------------------------
		std::string player_str = "player" + std::to_string(i + 1);
		Object *player_obj = object_manager_->Get(player_str);
		FbxPlayer *player = dynamic_cast<FbxPlayer*>(player_obj);
		Vector3 player_position(player->parameter().position_);
		Vector3 player_rotation(player->parameter().rotation_);

		float player_speed = player_speed_value;

		//-------------------------------------
		// �v���C���[��n�`�ɉ����Ĉړ�������
		//-------------------------------------
		bool input(true);
		if(i != 0)
		{
			if(child_death_[i - 1])
			{
				input = false;
			}
		}
		if(input)
		{
			if(GamePad::isPress(i, PAD_BUTTON_11)){
				player_speed = player_speed_value * 2.0f;
			}
			if(GamePad::isPress(i, PAD_BUTTON_11)){
				player_speed = player_speed_value * 2.0f;
			}
			player_position.x_ += (
				cosf(player_rotation.y_) * GamePad::isStick(i).lsx_ +
				sinf(-player_rotation.y_) * GamePad::isStick(i).lsy_) * player_speed;
			player_position.z_ -= (
				sinf(player_rotation.y_) * GamePad::isStick(i).lsx_ +
				cosf(-player_rotation.y_) * GamePad::isStick(i).lsy_) * player_speed;

			if(GamePad::isPress(i, PAD_RS_LEFT)){
				player_rotation.y_ -= CHAR_ROT_SPEED;
				if(player_rotation.y_ < D3DX_PI){
					player_rotation.y_ += D3DX_PI * 2.0f;
				}
			}
			if(GamePad::isPress(i, PAD_RS_RIGHT)){
				player_rotation.y_ += CHAR_ROT_SPEED;
				if(player_rotation.y_ > D3DX_PI){
					player_rotation.y_ -= D3DX_PI * 2.0f;
				}
			}
		}
		if(i == 0)
		{
			// ����������񃏁[�v
			if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_7)){
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.position_.x_ = player_position.x_;
				send_data.object_param_.position_.y_ = player_position.y_;
				send_data.object_param_.position_.z_ = player_position.z_;
				strcpy_s(send_data.name, MAX_NAME_LEN, "smoke");
				NetworkHost::SendTo(DELI_MULTI, send_data);

				switch(stage_){
				case 1:
					player_position = GRANDFATHER_POSITION_STAGE1;
					player_rotation.y_ = GRANDFATHER_ROTATION_STAGE1;
					break;
				case 2:
					player_position = GRANDFATHER_POSITION_STAGE2;
					player_rotation.y_ = GRANDFATHER_ROTATION_STAGE2;
					break;
				case 3:
					player_position = GRANDFATHER_POSITION_STAGE3;
					player_rotation.y_ = GRANDFATHER_ROTATION_STAGE3;
					break;
				}

				send_data.object_param_.position_.x_ = player_position.x_;
				send_data.object_param_.position_.y_ = player_position.y_;
				send_data.object_param_.position_.z_ = player_position.z_;
				NetworkHost::SendTo(DELI_MULTI, send_data);
			}
		}

		D3DXVECTOR3 player_pos(
			player_position.x_,
			player_position.y_,
			player_position.z_);
		player_position.y_ = field->GetHeight(player_pos);
		player_pos.y = player_position.y_;

		player->SetPosition(player_position);
		player->SetRotation(player_rotation);

		//-------------------------------------
		// �J�����Ǐ]
		//-------------------------------------
		std::string camera_str = "camera" + std::to_string(i + 1);
		Camera *main_camera = camera_manager_->Get(camera_str);
		D3DXVECTOR3 camera_position, camera_focus;
		D3DXVECTOR3 camera_rotation(main_camera->rotation());

		if(GamePad::isPress(i, PAD_RS_UP)){
			camera_rotation.x -= CAMERA_ROT_SPEED;
			if(camera_rotation.x < -CAMERA_ROT_X_LIMIT){
				camera_rotation.x = -CAMERA_ROT_X_LIMIT;
			}
		}
		if(GamePad::isPress(i, PAD_RS_DOWN)){
			camera_rotation.x += CAMERA_ROT_SPEED;
			if(camera_rotation.x > CAMERA_ROT_X_LIMIT){
				camera_rotation.x = CAMERA_ROT_X_LIMIT;
			}
		}

		// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
		camera_rotation.y = player_rotation.y_;
		// ��U���f���𒍎��_��
		camera_focus = player_pos;
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
		for (int j = 0; j < 10; ++j){
			// ���Ԓn�_���v�Z
			D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
			float pos_y = field->GetHeight(lay_point);
			// �������
			if (lay_point.y < pos_y + 0.1f){
				camera_re_calculate = true;
				camera_pos_len_[i] -= CAMARA_LEN_SPEED;
			}
		}
		
		//�J�������W�Čv�Z
		if (camera_re_calculate == true){ 
			camera_position = camera_focus;
			camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
			camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];
			camera_position.y = field->GetHeight(camera_position) + 0.1f;
		}
		
		camera_pos_len_[i] += CAMARA_LEN_SPEED;
		if (camera_pos_len_[i] > CAMERA_POS_LEN){
			camera_pos_len_[i] = CAMERA_POS_LEN;
		}

		// �J�����Ƀp�����[�^���ăZ�b�g
		main_camera->SetPosition(camera_position);
		main_camera->SetFocus(camera_focus);
		main_camera->SetRotation(camera_rotation);

		//-------------------------------------
		// �o���b�g����
		//-------------------------------------
		float watergauge = player->GetWaterGauge();
		shot_late[i]--;
		shot_late[i] = std::max<int>(shot_late[i], 0);
		if(GamePad::isPress(i, PAD_BUTTON_8) && watergauge > 0.0f && shot_late[i] == 0){
			shot_late[i] = 10;

			OBJECT_PARAMETER_DESC bullet_param;
			bullet_param.layer_ = LAYER_BULLET;
			if(i == 0)
			{
				bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
			}
			else
			{
				bullet_param.parent_layer_ = LAYER_MODEL_CHILD;
			}
			bullet_param.position_ = player_position;
			bullet_param.position_.y_ += 0.5f;
			bullet_param.rotation_ = player_rotation;

			// �J�����̉�]X�𗘗p
			bullet_param.rotation_.x_ = camera_rotation.x;

			bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};
			std::string str = "notice" + std::to_string(bullet_count_);
			object_manager_->Create(
				str,
				bullet_param);
			bullet_count_++;

			// �o���b�g����
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_BULLET;
			send_data.id_ = i;
			send_data.object_param_.ex_id_ = 0;
			
			send_data.object_param_.position_.x_ = bullet_param.position_.x_;
			send_data.object_param_.position_.y_ = bullet_param.position_.y_;
			send_data.object_param_.position_.z_ = bullet_param.position_.z_;
			
			send_data.object_param_.rotation_.x_ = bullet_param.rotation_.x_;
			send_data.object_param_.rotation_.y_ = bullet_param.rotation_.y_;
			send_data.object_param_.rotation_.z_ = bullet_param.rotation_.z_;
			
			strcpy_s(send_data.name, MAX_NAME_LEN, str.c_str());
			NetworkHost::SendTo(DELI_MULTI, send_data);

			// �G�t�F�N�g�Đ�
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = player_position.x_;
			send_data.object_param_.position_.y_ = player_position.y_ + 0.5f;
			send_data.object_param_.position_.z_ = player_position.z_;
			send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
			strcpy_s(send_data.name, MAX_NAME_LEN, "water");
			NetworkHost::SendTo(DELI_MULTI, send_data);

			//-------------------------------------
			// ���Q�[�W������������
			//-------------------------------------
			watergauge -= GRANDFATHER_SUB_WATERGAUGE;
			watergauge = std::max<float>(watergauge, 0.0f);
			player->SetWaterGauge(watergauge);
		}

		//-------------------------------------
		// �f�o�b�O���̂݁A���Q�[�W��
		//-------------------------------------
		if(GamePad::isPress(i, PAD_BUTTON_1)){
			watergauge += GRANDFATHER_SUB_WATERGAUGE;
			watergauge = std::min<float>(watergauge, 1.0f);
			player->SetWaterGauge(watergauge);
		}

		//-------------------------------------
		// �q�����S������
		//-------------------------------------
		if(i != 0)
		{
			float child_life = player->GetLife();
			if (child_life < 0 && !child_death_[i - 1]){
				player->PlayAnimation(FbxChild::DOWN);
				child_death_[i - 1] = true;
				child_respawn_waittime_[i - 1] = CHILD_RESPAWN_WAITTIME;
			
				// �G�t�F�N�g�Đ�
				ZeroMemory(&send_data, sizeof(send_data));
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.position_.x_ = player_position.x_;
				send_data.object_param_.position_.y_ = player_position.y_;
				send_data.object_param_.position_.z_ = player_position.z_;
				send_data.object_param_.rotation_ = {0.0f, 0.0f, 0.0f};
				strcpy_s(send_data.name, MAX_NAME_LEN, "dead");
				NetworkHost::SendTo(DELI_MULTI, send_data);
			}
			else if (child_death_[i - 1] && !child_respawn_waittime_[i - 1]){
				player->PlayAnimation(FbxChild::IDLE);
				child_death_[i - 1] = false;
				child_life = CHILD_LIFE;
				player->SetLife(child_life);
				player->SetPosition(CHILD_POSITION[i - 1]);
				player->SetRotation(CHILD_ROTATION[i - 1]);
			}

			child_respawn_waittime_[i - 1]--;
			child_respawn_waittime_[i - 1] = std::max<int>(child_respawn_waittime_[i - 1], 0);
		}

		//------------------------------------------------
		// �v���C���[�f�[�^�]��
		//------------------------------------------------
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.id_ = i;
		send_data.object_param_.ex_id_ = 0;
		if(i == 0)
		{
			send_data.object_param_.type_ = OBJ_GRANDFATHER;
		}
		else
		{
			send_data.object_param_.type_ = OBJ_CHILD;
		}
		if(input)
		{
			if(GamePad::isPress(i, PAD_LS_DOWN) ||
				GamePad::isPress(i, PAD_LS_UP) ||
				GamePad::isPress(i, PAD_LS_LEFT) ||
				GamePad::isPress(i, PAD_LS_RIGHT)){
				send_data.object_param_.ex_id_ = 1;
			}
		}else
		{
			send_data.object_param_.ex_id_ = 2;
		}

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
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_CAMERA;

		switch(set_camera)
		{
			case 0:
				send_data.object_param_.position_.x_ = camera_position.x;
				send_data.object_param_.position_.y_ = camera_position.y;
				send_data.object_param_.position_.z_ = camera_position.z;

				send_data.object_param_.rotation_.x_ = camera_focus.x;
				send_data.object_param_.rotation_.y_ = camera_focus.y;
				send_data.object_param_.rotation_.z_ = camera_focus.z;
				break;

			case 1:
				{
					Object *fort_object = object_manager_->Get("fort1");
					Vector3 focus = fort_object->parameter().position_;
					Vector3 pos(0.0f, 0.0f, 0.0f);
					// ���f���̒��S�ӂ�����
					focus.y_ += CAMERA_FOCUS_OFFSET_Y;
					// �����_����ɃJ�������W��ݒ�
					pos = focus;
					pos.x_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.z_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.y_ += CAMERA_FOCUS_OFFSET;
					// �f�[�^�Z�b�g
					send_data.object_param_.position_.x_ = pos.x_;
					send_data.object_param_.position_.y_ = pos.y_;
					send_data.object_param_.position_.z_ = pos.z_;

					send_data.object_param_.rotation_.x_ = focus.x_;
					send_data.object_param_.rotation_.y_ = focus.y_;
					send_data.object_param_.rotation_.z_ = focus.z_;
				}
				break;

			case 2:
				{
					Object *fort_object = object_manager_->Get("fort2");
					Vector3 focus = fort_object->parameter().position_;
					Vector3 pos(0.0f, 0.0f, 0.0f);
					// ���f���̒��S�ӂ�����
					focus.y_ += CAMERA_FOCUS_OFFSET_Y;
					// �����_����ɃJ�������W��ݒ�
					pos = focus;
					pos.x_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.z_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.y_ += CAMERA_FOCUS_OFFSET;
					// �f�[�^�Z�b�g
					send_data.object_param_.position_.x_ = pos.x_;
					send_data.object_param_.position_.y_ = pos.y_;
					send_data.object_param_.position_.z_ = pos.z_;

					send_data.object_param_.rotation_.x_ = focus.x_;
					send_data.object_param_.rotation_.y_ = focus.y_;
					send_data.object_param_.rotation_.z_ = focus.z_;
				}
				break;

			case 3:
				{
					Object *fort_object = object_manager_->Get("fort3");
					Vector3 focus = fort_object->parameter().position_;
					Vector3 pos(0.0f, 0.0f, 0.0f);
					// ���f���̒��S�ӂ�����
					focus.y_ += CAMERA_FOCUS_OFFSET_Y;
					// �����_����ɃJ�������W��ݒ�
					pos = focus;
					pos.x_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.z_ -= CAMERA_FOCUS_OFFSET * 2.0f;
					pos.y_ += CAMERA_FOCUS_OFFSET;
					// �f�[�^�Z�b�g
					send_data.object_param_.position_.x_ = pos.x_;
					send_data.object_param_.position_.y_ = pos.y_;
					send_data.object_param_.position_.z_ = pos.z_;

					send_data.object_param_.rotation_.x_ = focus.x_;
					send_data.object_param_.rotation_.y_ = focus.y_;
					send_data.object_param_.rotation_.z_ = focus.z_;
				}
				break;

			default:
				break;
		}
		NetworkHost::SendTo((DELI_TYPE)i, send_data);
		
		//------------------------------------------------
		// UI�f�[�^�]��
		//------------------------------------------------
		// ���Q�[�W
		send_data.type_ = DATA_UI_PARAM;
		send_data.object_param_.type_ = OBJ_UI;
		send_data.ui_param_.value_f_ = player->GetWaterGauge();
		strcpy_s(send_data.name, MAX_NAME_LEN, "water_gage");
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		// �_���[�W�G�t�F�N�g
		send_data.ui_param_.value_f_ = player->GetLife();
		strcpy_s(send_data.name, MAX_NAME_LEN, "damage_effect");
		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		Vector3 param = player->parameter().position_;
		font_->Add("POSITION(player) : %3.2f %3.2f %3.2f\n", param.x_, param.y_, param.z_);
	}
}

//-------------------------------------
// ChangeState(SERVER_STATE next)
//-------------------------------------
void GameServer::ChangeState(SERVER_STATE next)
{
	// �e��ݒ菉����
	for(int i = 0; i < MAX_GUEST; i++)
	{
		camera_pos_len_[i] = CAMERA_POS_LEN;
		shot_late[i] = 0;
	}
	bullet_count_ = 0;
	object_manager_->Clear(LAYER_BULLET);
	collision_manager_->Update();
	stage_ = 1;
	state = next;
	switch(state)
	{
		case STATE_MATCHING:
			{
				std::string player_str;
				Object *player;
				Vector3 pos(-55.0f, 0.0f, -50.0f), rot(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < MAX_GUEST; i++){
					player_str = "player" + std::to_string(i + 1);
					player = object_manager_->Get(player_str);

					pos.x_ += 2.0f;
					player->SetPosition(pos);
					player->SetRotation(rot);
				}
			}
			break;

		case STATE_GAME:
			{
				// �X�e�[�W
				stage_ = 1;
				// �����f�o�t�t���O
				grandfather_debuff_ = false;
				for(int i = 0; i < (MAX_GUEST - 1); i++)
				{
					// �q�����S�t���O
					child_death_[i] = false;
					// �q�����X�|�[���҂�����
					child_respawn_waittime_[i] = 0;
				}
				time_ = GAME_TIME * 60;
				std::string player_str;
				Object *player;
				for(int i = 0; i < MAX_GUEST; i++){
					player_str = "player" + std::to_string(i + 1);
					player = object_manager_->Get(player_str);

					Vector3 pos, rot;
					if(i == 0)
					{
						pos = GRANDFATHER_POSITION_STAGE1;
						rot = {0.0f, GRANDFATHER_ROTATION_STAGE1, 0.0f};
					}
					else
					{
						pos = CHILD_POSITION[i - 1];
						rot = CHILD_ROTATION[i - 1];
					}
					player->SetPosition(pos);
					player->SetRotation(rot);
				}
			}
			break;

		case STATE_RESULT:
			break;

		default:
			break;
	}

}


//-------------------------------------
// end of file
//-------------------------------------
