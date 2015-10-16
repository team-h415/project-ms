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
	font_ = new DebugFont;

	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 5.0f, -10.0f };
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

	//object_manager_->Create(
	//	"field",
	//	param,
	//	"resource/mesh/field.heightmap");

	OBJECT_PARAMETER_DESC model_param;
	model_param.layer_ = LAYER_MODEL_X;
	model_param.position_ = { 0.0f, 0.0f, 0.0f };
	model_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	model_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"model",
		model_param,
		"resource/model/x/pone_red.x");

}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
	camera_manager_->Update();
	object_manager_->Update();

	font_->Add("シーン名:");
	font_->Add("Game\n");

	//-------------------------------------
	// GamePadスティックチェック
	//-------------------------------------
	font_->Add("ゲームパッド:LEFT_STICK:");
	for (int i = 0; i < 4; i++){
		if (GamePad::isPress(i)){
			switch (i)
			{
			case 0:
				font_->Add("LEFT_");
				break;
			case 1:
				font_->Add("RIGHT_");
				break;
			case 2:
				font_->Add("UP_");
				break;
			case 3:
				font_->Add("DOWN_");
				break;
			}
		}
	}
	font_->Add("\nゲームパッド:RIGHT_STICK:");
	for (int i = 4; i < 8; i++){
		if (GamePad::isPress(i)){
			switch (i)
			{
			case 4:
				font_->Add("LEFT_");
				break;
			case 5:
				font_->Add("RIGHT_");
				break;
			case 6:
				font_->Add("UP_");
				break;
			case 7:
				font_->Add("DOWN_");
				break;
			}
		}
	}


	//-------------------------------------
	// GamePadボタン押下チェック
	//-------------------------------------
	font_->Add("\nゲームパッド：PRESS:");
	for (int i = 8; i < PAD_MAX; i++){
		if (GamePad::isPress(i)){
			font_->Add("%d_", i - 7);
		}
	}
	font_->Add("\nゲームパッド：TRIGGER:");
	for (int i = 8; i < PAD_MAX; i++){
		if (GamePad::isTrigger(i)){
			font_->Add("%d_", i - 7);
		}
	}
	font_->Add("\nゲームパッド：RELEASE:");
	for (int i = 8; i < PAD_MAX; i++){
		if (GamePad::isRelease(i)){
			font_->Add("%d_", i - 7);
		}
	}
	font_->Add("\nゲームパッド：REPEAT:");
	for (int i = 8; i < PAD_MAX; i++){
		if (GamePad::isRepeat(i)){
			font_->Add("%d_", i - 7);
		}
	}

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		SceneManager::RequestScene("Title");
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
	Color color(32, 32, 32, 0);
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
