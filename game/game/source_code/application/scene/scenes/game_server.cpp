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
// GameServer()
//-------------------------------------
GameServer::GameServer()
{
	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	collision_manager_ = new CollisionManager;
	font_ = new DebugFont;

	//-------------------------------------
	// カメラ
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
	// 地形
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
	// 砦
	//-------------------------------------
	// 砦1
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


	// 砦2
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


	// 砦3
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
	// FBXおじ
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
	// FBX子供
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
	// ゲームルール用パラメータ初期化
	//-------------------------------------
	for(int i = 0; i < MAX_GUEST; i++)
	{
		guest_scene_change_[i] = false;
	}
	bullet_count_ = 0;
	// ステージ
	stage_ = 1;
	// おじデバフフラグ
	grandfather_debuff_ = false;
	for(int i = 0; i < (MAX_GUEST - 1); i++)
	{
		// 子供死亡フラグ
		child_death_[i] = false;
		// 子供リスポーン待ち時間
		child_respawn_waittime_[i] = 0;
	}

	//-------------------------------------
	// ステートセット
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
	// 状態分岐
	switch(state)
	{
		case STATE_MATCHING:
			{
				MatchingAndGame();
				font_->Add("シーン名:");
				font_->Add("Matching\n");
				if(KeyBoard::isTrigger(DIK_RETURN))
				{
					ChangeState(STATE_GAME);
					// シーンチェンジ命令送信
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
					send_data.object_param_.type_ = OBJ_UI;
					strcpy_s(send_data.name, MAX_NAME_LEN, "timer");
					send_data.ui_param_.value_i_ = time_ / 60;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}

				font_->Add("シーン名:");
				font_->Add("Game\n");
				// 勝者じじい
				if(KeyBoard::isTrigger(DIK_9))
				{
					ChangeState(STATE_RESULT);
					// シーンチェンジ命令送信
					NETWORK_DATA send_data;
					send_data.type_ = DATA_SCENE_CHANGE_RESULT;
					send_data.object_param_.type_ = OBJ_GRANDFATHER;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
				// 勝者ガキ
				else if(KeyBoard::isTrigger(DIK_0))
				{
					ChangeState(STATE_RESULT);
					// シーンチェンジ命令送信
					NETWORK_DATA send_data;
					send_data.type_ = DATA_SCENE_CHANGE_RESULT;
					send_data.object_param_.type_ = OBJ_CHILD;
					NetworkHost::SendTo(DELI_MULTI, send_data);
				}
			}
			break;

		case STATE_RESULT:
			font_->Add("シーン名:");
			font_->Add("Result\n");
			if(KeyBoard::isTrigger(DIK_RETURN))
			{
				ChangeState(STATE_MATCHING);
				// シーンチェンジ命令送信
				NETWORK_DATA send_data;
				send_data.type_ = DATA_SCENE_CHANGE_MATCHING;
				NetworkHost::SendTo(DELI_MULTI, send_data);
			}
			break;

		default:
			break;
	}

	//-------------------------------------
	// 実更新処理
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
#ifndef _DEBUG
	return;
#endif
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(0.0f, 1.0f, 1.0f, 1.0f);
	MaterialColor color(255, 255, 255, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);

	camera_manager_->Set("camera1");
	object_manager_->Draw();
	collision_manager_->Draw();

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
	// データ転送用構造体用意
	NETWORK_DATA send_data;

	static const float player_speed_value = 0.05f;

	for(int i = 0; i < 5; i++){

		//-------------------------------------
		// 変数宣言
		//-------------------------------------
		std::string player_str = "player" + std::to_string(i + 1);
		Object *player = object_manager_->Get(player_str);
		Vector3 player_position(player->parameter().position_);
		Vector3 player_rotation(player->parameter().rotation_);
		Field *field = dynamic_cast<Field*>(
			object_manager_->Get("field"));

		float player_speed = player_speed_value;

		//-------------------------------------
		// プレイヤーを地形に沿って移動させる
		//-------------------------------------
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

		D3DXVECTOR3 player_pos(
			player_position.x_,
			player_position.y_,
			player_position.z_);
		player_position.y_ = field->GetHeight(player_pos);
		player_pos.y = player_position.y_;

		player->SetPosition(player_position);
		player->SetRotation(player_rotation);

		//-------------------------------------
		// カメラ追従
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

		// モデルの回転Yをそのままカメラの回転Yへ
		camera_rotation.y = player_rotation.y_;
		// 一旦モデルを注視点に
		camera_focus = player_pos;
		// 足元基準から体の中心辺りを基準に
		camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
		// モデルの少し先を見るように調整
		camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
		camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

		// 注視点を基準にカメラ座標を設定
		camera_position = camera_focus;
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_[i] * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_[i];

		// カメラの地面めり込み回避処理
		D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;
			
		// 中間にカメラがめり込みそうなところが無いか検査
		bool camera_re_calculate = false;
		for (int j = 0; j < 10; ++j){
			// 中間地点を計算
			D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * (0.1f * j);
			float pos_y = field->GetHeight(lay_point);
			// 回避処理
			if (lay_point.y < pos_y + 0.1f){
				camera_re_calculate = true;
				camera_pos_len_[i] -= CAMARA_LEN_SPEED;
			}
		}
		
		//カメラ座標再計算
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

		// カメラにパラメータを再セット
		main_camera->SetPosition(camera_position);
		main_camera->SetFocus(camera_focus);
		main_camera->SetRotation(camera_rotation);

		//-------------------------------------
		// バレット発射
		//-------------------------------------

		if(GamePad::isTrigger(i, PAD_BUTTON_8)){
			float watergauge = 0.0f;
			if(i == 0)
			{
				FbxGrandfather* grandfather = dynamic_cast<FbxGrandfather*>(player);
				watergauge = grandfather->GetWaterGauge();
			}
			else
			{
				FbxChild* child = dynamic_cast<FbxChild*>(player);
				watergauge = child->GetWaterGauge();
			}
			if(watergauge > 0.0f)
			{
				OBJECT_PARAMETER_DESC bullet_param;
				bullet_param.layer_ = LAYER_BULLET;
				bullet_param.position_ = player_position;
				bullet_param.rotation_ = player_position;

				// カメラの回転Xを利用
				bullet_param.rotation_.x_ = camera_rotation.x;

				bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};
				std::string str = "notice" + std::to_string(bullet_count_);
				object_manager_->Create(
					str,
					bullet_param);
				bullet_count_++;

				// エフェクト再生
				send_data.type_ = DATA_OBJ_PARAM;
				send_data.object_param_.type_ = OBJ_EFFECT;
				send_data.object_param_.position_.x_ = player_position.x_;
				send_data.object_param_.position_.y_ = player_position.y_;
				send_data.object_param_.position_.z_ = player_position.z_;
				strcpy_s(send_data.name, MAX_NAME_LEN, "water");

				// オブジェクトデータ転送
				NetworkHost::SendTo(DELI_MULTI, send_data);

				//-------------------------------------
				// 水ゲージを減少させる
				//-------------------------------------
				watergauge -= GRANDFATHER_SUB_WATERGAUGE;
				watergauge = std::max<float>(watergauge, 0.0f);
				if(i == 0)
				{
					FbxGrandfather* grandfather = dynamic_cast<FbxGrandfather*>(player);
					grandfather->SetWaterGauge(watergauge);
				}
				else
				{
					FbxChild* child = dynamic_cast<FbxChild*>(player);
					child->SetWaterGauge(watergauge);
				}
			}
		}

		//-------------------------------------
		// デバッグ時のみ、水ゲージ回復
		//-------------------------------------
		if (KeyBoard::isPress(DIK_1)){
			if(i == 0)
			{
				FbxGrandfather* grandfather = dynamic_cast<FbxGrandfather*>(player);
				float watergauge = grandfather->GetWaterGauge();
				watergauge += GRANDFATHER_SUB_WATERGAUGE;
				watergauge = std::min<float>(watergauge, 1.0f);
				grandfather->SetWaterGauge(watergauge);
			}
			else
			{
				FbxChild* child = dynamic_cast<FbxChild*>(player);
				float watergauge = child->GetWaterGauge();
				watergauge += GRANDFATHER_SUB_WATERGAUGE;
				watergauge = std::min<float>(watergauge, 1.0f);
				child->SetWaterGauge(watergauge);
			}
		}

		//------------------------------------------------
		// プレイヤーデータ転送
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
		if(GamePad::isPress(i, PAD_LS_DOWN) ||
			GamePad::isPress(i, PAD_LS_UP) ||
			GamePad::isPress(i, PAD_LS_LEFT) ||
			GamePad::isPress(i, PAD_LS_RIGHT)){
			send_data.object_param_.ex_id_ = 1;
		}

		send_data.object_param_.position_.x_ = player_position.x_;
		send_data.object_param_.position_.y_ = player_position.y_;
		send_data.object_param_.position_.z_ = player_position.z_;

		send_data.object_param_.rotation_.x_ = player_rotation.x_;
		send_data.object_param_.rotation_.y_ = player_rotation.y_;
		send_data.object_param_.rotation_.z_ = player_rotation.z_;

		NetworkHost::SendTo(DELI_MULTI, send_data);

		//------------------------------------------------
		// カメラデータ転送
		//------------------------------------------------
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_CAMERA;

		send_data.object_param_.position_.x_ = camera_position.x;
		send_data.object_param_.position_.y_ = camera_position.y;
		send_data.object_param_.position_.z_ = camera_position.z;

		send_data.object_param_.rotation_.x_ = camera_focus.x;
		send_data.object_param_.rotation_.y_ = camera_focus.y;
		send_data.object_param_.rotation_.z_ = camera_focus.z;

		NetworkHost::SendTo((DELI_TYPE)i, send_data);

		//------------------------------------------------
		// UIデータ転送
		//------------------------------------------------
		if(i == 0)
		{
			FbxGrandfather* grandfather = dynamic_cast<FbxGrandfather*>(player);
			// 水ゲージ
			send_data.type_ = DATA_UI_PARAM;
			send_data.ui_param_.value_f_ = grandfather->GetWaterGauge();
			strcpy_s(send_data.name, MAX_NAME_LEN, "water_gage");
			NetworkHost::SendTo((DELI_TYPE)i, send_data);
			// ダメージエフェクト
			send_data.ui_param_.value_f_ = grandfather->GetLife();
			strcpy_s(send_data.name, MAX_NAME_LEN, "damage_effect");
			NetworkHost::SendTo((DELI_TYPE)i, send_data);
		}
		else
		{
			FbxChild* child = dynamic_cast<FbxChild*>(player);
			// 水ゲージ
			send_data.type_ = DATA_UI_PARAM;
			send_data.ui_param_.value_f_ = child->GetWaterGauge();
			strcpy_s(send_data.name, MAX_NAME_LEN, "water_gage");
			NetworkHost::SendTo((DELI_TYPE)i, send_data);
			// ダメージエフェクト
			send_data.ui_param_.value_f_ = child->GetLife();
			strcpy_s(send_data.name, MAX_NAME_LEN, "damage_effect");
			NetworkHost::SendTo((DELI_TYPE)i, send_data);
		}
	}
}

//-------------------------------------
// ChangeState(SERVER_STATE next)
//-------------------------------------
void GameServer::ChangeState(SERVER_STATE next)
{
	// 各種設定初期化
	for(int i = 0; i < MAX_GUEST; i++)
	{
		camera_pos_len_[i] = CAMERA_POS_LEN;
	}
	bullet_count_ = 0;
	object_manager_->Clear(LAYER_BULLET);
	state = next;
	switch(state)
	{
		case STATE_MATCHING:
			{
				std::string player_str;
				Object *player;
				Vector3 pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < MAX_GUEST; i++){
					player_str = "player" + std::to_string(i + 1);
					player = object_manager_->Get(player_str);

					player->SetPosition(pos);
					player->SetRotation(rot);
				}
			}
			break;

		case STATE_GAME:
			{
				// ステージ
				stage_ = 1;
				// おじデバフフラグ
				grandfather_debuff_ = false;
				for(int i = 0; i < (MAX_GUEST - 1); i++)
				{
					// 子供死亡フラグ
					child_death_[i] = false;
					// 子供リスポーン待ち時間
					child_respawn_waittime_[i] = 0;
				}
				time_ = GAME_TIME * 60;
				std::string player_str;
				Object *player;
				Vector3 pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f);
				for(int i = 0; i < MAX_GUEST; i++){
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
