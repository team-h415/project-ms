//=========================================================
// title.cpp
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
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "title.h"
#include "../fade/fade.h"


//-------------------------------------
// Title()
//-------------------------------------
Title::Title()
{
	object_manager_ = new ObjectManager;

	OBJECT_PARAMETER_DESC param;
	param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f
	};
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 100.0f, 100.0f, 0.0f };
	param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create("test", param);
}


//-------------------------------------
// ~Title()
//-------------------------------------
Title::~Title()
{
	SAFE_DELETE(object_manager_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Title::Update()
{
	object_manager_->Update();

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		SceneManager::RequestScene("Game");
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Title::Draw()
{
	Color color(32, 255, 32, 255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	object_manager_->Draw();
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
