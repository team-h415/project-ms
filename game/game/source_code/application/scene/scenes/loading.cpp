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
#include "../../object/objects/sprite/sprite2d.h"
#include "../scene.h"
#include "loading.h"


//-------------------------------------
// Loading()
//-------------------------------------
Loading::Loading()
{
	background_ = nullptr;
}


//-------------------------------------
// ~Loading()
//-------------------------------------
Loading::~Loading()
{
	SAFE_DELETE(background_);
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Loading::Initialize()
{
	OBJECT_PARAMETER_DESC param;
	param.name_ = "now_loading";
	param.position_ = {
		SCREEN_WIDTH - 200.0f,
		SCREEN_HEIGHT - 100.0f,
		0.0f
	};
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 200.0f, 50.0f, 0.0f };
	param.layer_ = LAYER_SPRITE_2D;

	background_ = new Sprite2D(param);
	background_->SetTexture(
		"resource/texture/loading/now_loading_00.png");
}


//-------------------------------------
// Update()
//-------------------------------------
void Loading::Update()
{
	background_->Update();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Loading::Draw()
{
	MaterialColor color(0, 0, 0, 255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	background_->Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
