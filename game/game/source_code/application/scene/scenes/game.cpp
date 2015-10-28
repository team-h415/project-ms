//=========================================================
// game.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../../math/vector.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../input/inputs/gamepad.h"
#include "../../debug/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/mesh/field.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "game.h"
#include "../fade/fade.h"


//-------------------------------------
// Game()
//-------------------------------------
Game::Game()
{
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = new EffectManager(5000);
	font_ = new DebugFont;

	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EFFECT_PARAMETER_DESC water_param;
	water_param.position_ = { 0.0f, 0.0f, 0.0f };
	water_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	water_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	water_param.speed_ = 1.0f;

	effect_manager_->Create(
		"water",
		"resource/effect/water.efk",
		water_param);


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
		"Perspective", "MainCamera", camera_param);

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
		"player",
		player_param,
		"resource/model/x/pone_red.x");

	OBJECT_PARAMETER_DESC fbx_param;
	fbx_param.layer_ = LAYER_MODEL_FBX;
	fbx_param.position_ = { 0.0f, 0.0f, 0.0f };
	fbx_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fbx_param.scaling_ = { 0.1f, 0.1f, 0.1f };

	object_manager_->Create(
		"fbx",
		fbx_param,
		"resource/model/fbx/UneUne2.fbx");
}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(effect_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
	//-------------------------------------
	// 変数宣言
	//-------------------------------------
	Object *player = object_manager_->Get("player");
	Object *fbx = object_manager_->Get("fbx");
	Vector3 player_position(player->parameter().position_);
	Vector3 player_rotation(player->parameter().rotation_);
	Vector3 fbx_position(fbx->parameter().position_);
	Vector3 fbx_rotation(fbx->parameter().rotation_);

	Field *field = dynamic_cast<Field*>(
		object_manager_->Get("field"));

	static float camera_pos_y = 0.0f;
	static float camera_focus_y = 0.0f;
	static const float player_speed_value = 0.05f;
	float player_speed = player_speed_value;

	//-------------------------------------
	// プレイヤーを地形に沿って移動させる
	//-------------------------------------
	if (GamePad::isPress(PAD_BUTTON_11)){
		player_speed = player_speed_value * 2.0f;
	}
	player_position.x_ += (
		cosf(player_rotation.y_) * GamePad::isStick().lsx_ +
		sinf(-player_rotation.y_) * GamePad::isStick().lsy_) * player_speed;
	player_position.z_ -= (
		sinf(player_rotation.y_) * GamePad::isStick().lsx_ +
		cosf(-player_rotation.y_) * GamePad::isStick().lsy_) * player_speed;

	if (GamePad::isPress(PAD_RS_LEFT)){
		player_rotation.y_ -= D3DX_PI * 0.01f;
		if (player_rotation.y_ < D3DX_PI){
			player_rotation.y_ += D3DX_PI * 2.0f;
		}
	}
	if (GamePad::isPress(PAD_RS_RIGHT)){
		player_rotation.y_ += D3DX_PI * 0.01f;
		if (player_rotation.y_ > D3DX_PI){
			player_rotation.y_ -= D3DX_PI * 2.0f;
		}
	}

	if (KeyBoard::isPress(DIK_A)){
		fbx_position.x_ -= 0.1f;
	}
	if (KeyBoard::isPress(DIK_D)){
		fbx_position.x_ += 0.1f;
	}
	if (KeyBoard::isPress(DIK_W)){
		fbx_position.z_ += 0.1f;
	}
	if (KeyBoard::isPress(DIK_S)){
		fbx_position.z_ -= 0.1f;
	}

	if (GamePad::isPress(PAD_RS_UP)){
		camera_pos_y -= 0.05f;
	}
	if (GamePad::isPress(PAD_RS_DOWN)){
		camera_pos_y += 0.05f;
	}
	camera_pos_y = std::min<float>(camera_pos_y, 2.0f);
	camera_pos_y = std::max<float>(camera_pos_y, -2.0f);
	camera_focus_y = -camera_pos_y;


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
	// カメラ追従
	//-------------------------------------
	Camera *main_camera = camera_manager_->Get("MainCamera");
	D3DXVECTOR3 camera_position(main_camera->position());
	D3DXVECTOR3 camera_focus(main_camera->focus());
	D3DXVECTOR3 camera_position_sub(
		-sinf(player_rotation.y_) * 6.0f,
		3.0f + camera_pos_y,
		-cosf(player_rotation.y_) * 6.0f);
	D3DXVECTOR3 camera_focus_sub(
		-sinf(player_rotation.y_ + D3DX_PI) * 6.0f,
		1.0f + camera_focus_y,
		-cosf(player_rotation.y_ + D3DX_PI) * 6.0f);

	camera_position = pos + camera_position_sub;
	camera_focus = pos + camera_focus_sub;

	main_camera->SetPosition(camera_position);
	main_camera->SetFocus(camera_focus);


	//-------------------------------------
	// エフェクト再生
	//-------------------------------------
	if (KeyBoard::isTrigger(DIK_1)){
		effect_manager_->Play("water");
	}
	if (GamePad::isTrigger(PAD_BUTTON_6)){
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = player_position;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");
	}

	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();

	font_->Add("シーン名:");
	font_->Add("Game\n");

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		SceneManager::RequestScene("Result");
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Game::Draw()
{
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(0.0f, 1.0f, 1.0f, 1.0f);
	MaterialColor color(255, 255, 255, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
