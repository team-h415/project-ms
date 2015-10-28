//=========================================================
// Matching.cpp
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
#include "../../debug/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
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
	font_ = new DebugFont;

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
		SCREEN_HEIGHT * 0.25f,
		0.0f
	};
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 100.0f, 100.0f, 0.0f };
	param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create("test", param);
}


//-------------------------------------
// ~Matching()
//-------------------------------------
Matching::~Matching()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Matching::Update()
{
	camera_manager_->Update();
	object_manager_->Update();

	font_->Add("ƒV[ƒ“–¼:");
	font_->Add("Matching");

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		SceneManager::RequestScene("Game");
	}
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
