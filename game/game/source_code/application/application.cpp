//=========================================================
// application.cpp
// author:ryuya nakamura
// アプリケーションの管理
//=========================================================

//-------------------------------------
// include 
//-------------------------------------
#include "../common/common.h"
#include "fps/fps.h"
#include "window/window.h"
#include "window/window_holder.h"
#include "render/renderer.h"
#include "render/renderer_factory.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "input/input.h"
#include "input/input_factory.h"
#include "application.h"


//-------------------------------------
// variable declaration
//-------------------------------------
Fps *Application::fps_ = NULL;


//-------------------------------------
// Application()
//-------------------------------------
Application::Application(
	HINSTANCE instance,
	int command_show)
{
	fps_ = new Fps;
	Window::Create(
		instance,
		command_show);
	renderer_ = RendererFactory::Create(
		WindowHolder::handle_,
		"DirectX9");
	scene_manager_ = new SceneManager;
	keyboard_ = InputFactory::Create(
		"KeyBoard");
	gamepad_ = InputFactory::Create(
		"GamePad");
}


//-------------------------------------
// ~Application
//-------------------------------------
Application::~Application()
{
	SAFE_DELETE(scene_manager_);
	SAFE_DELETE(fps_);
	SAFE_DELETE(renderer_);
	SAFE_DELETE(keyboard_);
	SAFE_DELETE(gamepad_);
}


//-------------------------------------
// Run()
//-------------------------------------
void Application::Run(
	HINSTANCE instance,
	int command_show)
{
	Application *app = new Application(
		instance,
		command_show);
	
	while (true)
	{
		if (Window::Messages()) break;
		if (!(fps_->CheckExecute())) continue;

		app->Update();
		app->Draw();
	}

	SAFE_DELETE(app);
}


//-------------------------------------
// Update()
//-------------------------------------
void Application::Update()
{
	keyboard_->Update();
	gamepad_->Update();
	scene_manager_->Update();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Application::Draw()
{
	keyboard_->Draw();
	gamepad_->Draw();
	scene_manager_->Draw();
}



//-------------------------------------
// end of file
//-------------------------------------
