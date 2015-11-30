//=========================================================
// game_server.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../network/network.h"
#ifdef NETWORK_HOST_MODE
	#include "../../network/network_host.h"
#else
	#include "../../network/network_guest.h"
#endif
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
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "game_server.h"
#include "../fade/fade.h"


//-------------------------------------
// GameServer()
//-------------------------------------
GameServer::GameServer()
{
	for(int i = 0; i < MAX_GUEST; i++)
	{
		guest_scene_change_[i] = false;
	}

	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	font_ = new DebugFont;

	//-------------------------------------
	// �G�t�F�N�g�̓ǂݍ���
	//-------------------------------------
	EFFECT_PARAMETER_DESC water_param;
	water_param.position_ = { 0.0f, 0.0f, 0.0f };
	water_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	water_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	water_param.speed_ = 1.0f;

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

	OBJECT_PARAMETER_DESC param;
	param.position_ = { 0.0f, 0.0f, 0.0f };
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 500.0f, 1.0f, 500.0f };
	param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		"field",
		param,
		"resource/mesh/field.heightmap");

	OBJECT_PARAMETER_DESC player_param;
	player_param.layer_ = LAYER_MODEL_X;
	player_param.position_ = { 0.0f, 0.0f, 0.0f };
	player_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	player_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"player1",
		player_param,
		"resource/model/x/pone.x");

	object_manager_->Create(
		"player2",
		player_param,
		"resource/model/x/pone.x");

	object_manager_->Create(
		"player3",
		player_param,
		"resource/model/x/pone.x");

	object_manager_->Create(
		"player4",
		player_param,
		"resource/model/x/pone.x");

	object_manager_->Create(
		"player5",
		player_param,
		"resource/model/x/pone.x");

	OBJECT_PARAMETER_DESC fbx_param;
	fbx_param.layer_ = LAYER_MODEL_FBX;
	fbx_param.position_ = { 0.0f, 0.0f, 0.0f };
	fbx_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fbx_param.scaling_ = { 0.1f, 0.1f, 0.1f };

	object_manager_->Create(
		"fbx",
		fbx_param,
		"resource/model/fbx/REuneune.fbx");

	// �X�e�[�g�Z�b�g
	ChangeState(STATE_MATCHING);
}


//-------------------------------------
// ~GameServer()
//-------------------------------------
GameServer::~GameServer()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void GameServer::Update()
{
#ifdef NETWORK_HOST_MODE

	// �f�[�^�]���p�\���̗p��
	NETWORK_DATA send_data;

	static float camera_pos_y[5] = { 0.0f };
	static float camera_focus_y[5] = { 0.0f };
	static const float player_speed_value = 0.05f;

	// ��ԕ���
	switch(state)
	{
		case STATE_MATCHING:
		case STATE_GAME:
			{
				for (int i = 0; i < 5; i++){

					//-------------------------------------
					// �ϐ��錾
					//-------------------------------------
					std::string player_str = "player" + std::to_string(i + 1);
					Object *player = object_manager_->Get(player_str);
					Object *fbx = object_manager_->Get("fbx");
					Vector3 player_position(player->parameter().position_);
					Vector3 player_rotation(player->parameter().rotation_);
					Vector3 fbx_position(fbx->parameter().position_);
					Vector3 fbx_rotation(fbx->parameter().rotation_);
					Field *field = dynamic_cast<Field*>(
						object_manager_->Get("field"));

					float player_speed = player_speed_value;

					//-------------------------------------
					// �v���C���[��n�`�ɉ����Ĉړ�������
					//-------------------------------------
					if (GamePad::isPress(i, PAD_BUTTON_11)){
						player_speed = player_speed_value * 2.0f;
					}
					player_position.x_ += (
						cosf(player_rotation.y_) * GamePad::isStick(i).lsx_ +
						sinf(-player_rotation.y_) * GamePad::isStick(i).lsy_) * player_speed;
					player_position.z_ -= (
						sinf(player_rotation.y_) * GamePad::isStick(i).lsx_ +
						cosf(-player_rotation.y_) * GamePad::isStick(i).lsy_) * player_speed;

					if (GamePad::isPress(i, PAD_RS_LEFT)){
						player_rotation.y_ -= D3DX_PI * 0.01f;
						if (player_rotation.y_ < D3DX_PI){
							player_rotation.y_ += D3DX_PI * 2.0f;
						}
					}
					if (GamePad::isPress(i, PAD_RS_RIGHT)){
						player_rotation.y_ += D3DX_PI * 0.01f;
						if (player_rotation.y_ > D3DX_PI){
							player_rotation.y_ -= D3DX_PI * 2.0f;
						}
					}

					if (GamePad::isPress(i, PAD_RS_UP)){
						camera_pos_y[i] -= 0.05f;
					}
					if (GamePad::isPress(i, PAD_RS_DOWN)){
						camera_pos_y[i] += 0.05f;
					}
					camera_pos_y[i] = std::min<float>(camera_pos_y[i], 2.0f);
					camera_pos_y[i] = std::max<float>(camera_pos_y[i], -2.0f);
					camera_focus_y[i] = -camera_pos_y[i];


					D3DXVECTOR3 pos(
						player_position.x_,
						player_position.y_,
						player_position.z_);
					player_position.y_ = field->GetHeight(pos);

					D3DXVECTOR3 fbx_pos(
						fbx_position.x_,
						fbx_position.y_,
						fbx_position.z_);
					fbx_position.y_ = field->GetHeight(fbx_pos);


					player->SetPosition(player_position);
					player->SetRotation(player_rotation);
					fbx->SetPosition(fbx_position);
					fbx->SetRotation(fbx_rotation);

					//-------------------------------------
					// �J�����Ǐ]
					//-------------------------------------
					std::string camera_str = "camera" + std::to_string(i + 1);
					Camera *main_camera = camera_manager_->Get(camera_str);
					D3DXVECTOR3 camera_position(main_camera->position());
					D3DXVECTOR3 camera_focus(main_camera->focus());
					D3DXVECTOR3 camera_position_sub(
						-sinf(player_rotation.y_) * 6.0f,
						3.0f + camera_pos_y[i],
						-cosf(player_rotation.y_) * 6.0f);
					D3DXVECTOR3 camera_focus_sub(
						-sinf(player_rotation.y_ + D3DX_PI) * 6.0f,
						1.0f + camera_focus_y[i],
						-cosf(player_rotation.y_ + D3DX_PI) * 6.0f);

					camera_position = pos + camera_position_sub;
					camera_focus = pos + camera_focus_sub;

					main_camera->SetPosition(camera_position);
					main_camera->SetFocus(camera_focus);

					//-------------------------------------
					// �G�t�F�N�g�Đ�
					//-------------------------------------
					if (GamePad::isTrigger(i, PAD_BUTTON_6)){

						send_data.type_ = DATA_OBJ_PARAM;
						send_data.object_param_.type_ = OBJ_EFFECT;
						send_data.object_param_.position_.x_ = player_position.x_;
						send_data.object_param_.position_.y_ = player_position.y_;
						send_data.object_param_.position_.z_ = player_position.z_;

						// �I�u�W�F�N�g�f�[�^�]��
						NetworkHost::SendTo(DELI_MULTI, send_data);
					}

					// �I�u�W�F�N�g�f�[�^�]���p��
					send_data.type_ = DATA_OBJ_PARAM;
					send_data.id_ = i;
					if(i == 0)
					{
						send_data.object_param_.type_ = OBJ_GRANDFATHER;
					}
					else
					{
						send_data.object_param_.type_ = OBJ_CHILD;
					}

					send_data.object_param_.position_.x_ = player_position.x_;
					send_data.object_param_.position_.y_ = player_position.y_;
					send_data.object_param_.position_.z_ = player_position.z_;

					send_data.object_param_.rotation_.x_ = player_rotation.x_;
					send_data.object_param_.rotation_.y_ = player_rotation.y_;
					send_data.object_param_.rotation_.z_ = player_rotation.z_;

					// �I�u�W�F�N�g�f�[�^�]��
					NetworkHost::SendTo(DELI_MULTI, send_data);

					// �J�����f�[�^�]���p��
					send_data.type_ = DATA_OBJ_PARAM;
					send_data.object_param_.type_ = OBJ_CAMERA;

					send_data.object_param_.position_.x_ = camera_position.x;
					send_data.object_param_.position_.y_ = camera_position.y;
					send_data.object_param_.position_.z_ = camera_position.z;

					send_data.object_param_.rotation_.x_ = camera_focus.x;
					send_data.object_param_.rotation_.y_ = camera_focus.y;
					send_data.object_param_.rotation_.z_ = camera_focus.z;

					// �J�����f�[�^�]��
					NetworkHost::SendTo((DELI_TYPE)i, send_data);
				}

				if(state == STATE_MATCHING)
				{
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
				else
				{
					font_->Add("�V�[����:");
					font_->Add("Game\n");
					if (KeyBoard::isTrigger(DIK_RETURN))
					{
						ChangeState(STATE_RESULT);
						// �V�[���`�F���W���ߑ��M
						NETWORK_DATA send_data;
						send_data.type_ = DATA_SCENE_CHANGE_RESULT;
						NetworkHost::SendTo(DELI_MULTI, send_data);
					}
				}

				//-------------------------------------
				// ���X�V����
				//-------------------------------------
				//camera_manager_->Update();
				//object_manager_->Update();
				//effect_manager_->Update();

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

#else
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

	//std::string camera_str = "camera" + std::to_string(current_id_);
	//camera_manager_->Set(camera_str);
	//object_manager_->Draw();
	//effect_manager_->Draw();

	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();

	#ifdef NETWORK_HOST_MODE
	#else
	#endif
}



//-------------------------------------
// ChangeState(SERVER_STATE next)
//-------------------------------------
void GameServer::ChangeState(SERVER_STATE next)
{
	state = next;
	switch(state)
	{
		case STATE_MATCHING:
			{
				std::string player_str;
				Object *player;
				Vector3 pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < 5; i++){
					player_str = "player" + std::to_string(i + 1);
					player = object_manager_->Get(player_str);

					player->SetPosition(pos);
					player->SetRotation(rot);
				}
			}
			break;

		case STATE_GAME:
			{
				std::string player_str;
				Object *player;
				Vector3 pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < 5; i++){
					player_str = "player" + std::to_string(i + 1);
					player = object_manager_->Get(player_str);

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
