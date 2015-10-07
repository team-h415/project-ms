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
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "title.h"
#include "../fade/fade.h"


//-------------------------------------
// Title()
//-------------------------------------
Title::Title()
{
}


//-------------------------------------
// ~Title()
//-------------------------------------
Title::~Title()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Title::Update()
{
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
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
