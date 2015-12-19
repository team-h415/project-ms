//=========================================================
// Result.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../network/network.h"
#include "../../network/network_guest.h"
#include "../../network/network_host.h"
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../../math/vector.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/sprite/sprite2d.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "Result.h"
#include "../fade/fade.h"


//-------------------------------------
// Result()
//-------------------------------------
Result::Result()
{
	setup_ = false;
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	font_ = new DebugFont;
}


//-------------------------------------
// ~Result()
//-------------------------------------
Result::~Result()
{
	setup_ = false;
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Result::Initialize()
{
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 10.0f, -20.0f };
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;

	camera_manager_->Create(
		"Perspective", "MainCamera", camera_param);

	OBJECT_PARAMETER_DESC param;

	param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f
	};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f};
	param.layer_ = LAYER_SPRITE_2D;
	param.name_ = "winner";

	if(NetworkGuest::winner() == 0)
	{
		// じじい
		object_manager_->Create(
			 param,
			"resource/texture/result/win_grandfather.png");
	}
	else
	{
		// 子供
		object_manager_->Create(
			param,
			"resource/texture/result/win_child.png");
	}

	param.name_ = "push8";
	param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f
	};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f};
	param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create(
		param,
		"resource/texture/result/push_8_button.png");

	alphar_wave_ = 0.0f;
	sprite_alpha_ = 0.0f;

	//-------------------------------------
	// セットアップ完了
	//-------------------------------------
	setup_ = true;
}


//-------------------------------------
// Update()
//-------------------------------------
void Result::Update()
{
	// アルファ値変更
	alphar_wave_ += D3DX_PI * 0.02f;
	if(alphar_wave_ > D3DX_PI){
		alphar_wave_ -= D3DX_PI * 2.0f;
	}
	sprite_alpha_ = (sinf(alphar_wave_) + 1.0f) * 0.5f;

	Sprite2D *push8 = dynamic_cast<Sprite2D*>(object_manager_->Get("push8"));
	D3DXCOLOR search_color(1.0f, 1.0f, 1.0f, sprite_alpha_);
	push8->SetColor(search_color);

	camera_manager_->Update();
	object_manager_->Update();

	font_->Add("シーン名:");
	font_->Add("Result");

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		SceneManager::RequestScene("Matching");
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Result::Draw()
{
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(0.0f, 1.0f, 1.0f, 1.0f);
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
