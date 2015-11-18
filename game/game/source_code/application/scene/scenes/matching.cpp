//=========================================================
// Matching.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../network/network.h"
#include "../../network/network_guest.h"
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
#include "matching.h"
#include "../fade/fade.h"


//-------------------------------------
// Matching()
//-------------------------------------
Matching::Matching()
{
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = new EffectManager(5000);
	font_ = new DebugFont;

	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = new EffectManager(5000);
	font_ = new DebugFont;

	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EFFECT_PARAMETER_DESC water_param;
	water_param.position_ = {0.0f, 0.0f, 0.0f};
	water_param.rotation_ = {0.0f, 0.0f, 0.0f};
	water_param.scaling_ = {1.0f, 1.0f, 1.0f};
	water_param.speed_ = 1.0f;

	effect_manager_->Create(
		"water",
		"resource/effect/water.efk",
		water_param);

	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = {0.0f, 0.0f, -10.0f};
	camera_param.focus_ = {0.0f, 0.0f, 0.0f};
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = {0.0f, 1.0f, 0.0f};
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;

	camera_manager_->Create(
		"Perspective", "MainCamera", camera_param);

	OBJECT_PARAMETER_DESC param;
	param.position_ = {0.0f, 0.0f, 0.0f};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {500.0f, 1.0f, 500.0f};
	param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		"field",
		param,
		"resource/mesh/field.heightmap");

	OBJECT_PARAMETER_DESC player_param;
	player_param.layer_ = LAYER_MODEL_X;
	player_param.position_ = {0.0f, 0.0f, 0.0f};
	player_param.rotation_ = {0.0f, 0.0f, 0.0f};
	player_param.scaling_ = {1.0f, 1.0f, 1.0f};

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
	fbx_param.position_ = {0.0f, 0.0f, 0.0f};
	fbx_param.rotation_ = {0.0f, 0.0f, 0.0f};
	fbx_param.scaling_ = {0.1f, 0.1f, 0.1f};

	object_manager_->Create(
		"fbx",
		fbx_param,
		"resource/model/fbx/REuneune.fbx");

	ZeroMemory(&param, sizeof(param));
	param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.25f,
		0.0f
	};
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 200.0f, 100.0f, 0.0f };
	param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create(
		"test",	param,
		"resource/texture/matching/logo.png");

#ifdef NETWORK_HOST_MODE
#else
	NETWORK_DATA network_data;
	network_data.type_ = DATA_COMPLETE_SCENE_CHANGE;
	NetworkGuest::SendTo(network_data);
#endif
}


//-------------------------------------
// ~Matching()
//-------------------------------------
Matching::~Matching()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(effect_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Matching::Update()
{
	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();

	// デバッグ表示
	font_->Add("シーン名:");
	font_->Add("Matching");
}


//-------------------------------------
// Draw()
//-------------------------------------
void Matching::Draw()
{
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(1.0f, 0.0f, 0.0f, 1.0f);
	MaterialColor color(32, 32, 32, 255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
