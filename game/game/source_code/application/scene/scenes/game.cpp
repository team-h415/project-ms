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
#include "../../debug/debug_font.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "game.h"
#include "../fade/fade.h"



//-------------------------------------
// Game()
//-------------------------------------
Game::Game()
{
}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
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
	Color color(32, 32, 32, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
