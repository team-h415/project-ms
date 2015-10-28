//=========================================================
// loading.cpp
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
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../scene.h"
#include "loading.h"
#include "../fade/fade.h"


//-------------------------------------
// Loading()
//-------------------------------------
Loading::Loading()
{
	object_manager_ = new ObjectManager;

	OBJECT_PARAMETER_DESC param;
	param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f
	};
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 200.0f, 100.0f, 0.0f };
	param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create(
		"now_loading", param,
		"resource/texture/loading/now_loading_00.png");
}


//-------------------------------------
// ~Loading()
//-------------------------------------
Loading::~Loading()
{
	SAFE_DELETE(object_manager_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Loading::Update()
{
	object_manager_->Update();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Loading::Draw()
{
	MaterialColor color(0, 0, 0, 255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	object_manager_->Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
