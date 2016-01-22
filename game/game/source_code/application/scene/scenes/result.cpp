//=========================================================
// Result.cpp
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
#include "../../object/object.h"
#include "../../object/objects/sprite/sprite2d.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "result.h"
#include "../fade/fade.h"


//-------------------------------------
// Result()
//-------------------------------------
Result::Result()
{
	background_ = nullptr;
}


//-------------------------------------
// ~Result()
//-------------------------------------
Result::~Result()
{
	SAFE_DELETE(background_);
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Result::Initialize()
{
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

	background_ = new Sprite2D(param);
	if(NetworkGuest::winner() == 0)
	{
		// ‚¶‚¶‚¢
		background_->SetTexture(
			"resource/texture/result/win_grandfather.jpg");
	}
	else
	{
		// Žq‹Ÿ
		background_->SetTexture(
			"resource/texture/result/win_child.jpg");
	}

#ifdef NETWORK_HOST_MODE
#else
	NETWORK_DATA network_data;
	network_data.type_ = DATA_COMPLETE_SCENE_CHANGE;
	NetworkGuest::SendTo(network_data);
#endif
}


//-------------------------------------
// Update()
//-------------------------------------
void Result::Update()
{
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
	background_->Draw();
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
