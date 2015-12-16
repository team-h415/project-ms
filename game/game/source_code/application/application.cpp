//=========================================================
// application.cpp
// author:ryuya nakamura
// アプリケーションの管理
//=========================================================


//-------------------------------------
// include 
//-------------------------------------
#include "network/network.h"
#ifdef NETWORK_HOST_MODE
	#include "network/network_host.h"
#else
	#include "network/network_guest.h"
#endif
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
#include "sound/sound.h"
#include "resource/texture_manager.h"
#include "resource/fbx_container_manager.h"
#include "resource/x_container_manager.h"
#include "../common/common.h"
#include "render/renderer.h"
#include "render/directx9/directx9.h"
#include "render/directx9/directx9_holder.h"
#include "math/vector.h"
#include "effect/effect.h"
#include "effect/effect_manager.h"
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
	Sound::Setup();
#ifdef NETWORK_HOST_MODE
	NetworkHost::StartCommunication(scene_manager_);
#else
	NetworkGuest::StartCommunication(scene_manager_);
#endif
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
	#ifdef NETWORK_HOST_MODE
		NetworkHost::CloseCommunication();
	#else
		NetworkGuest::CloseCommunication();
	#endif
	TextureManager::AllRelease();
	FbxContainerManager::AllRelease();
	XContainerManager::AllRelease();
	EffectManager::Delete();
	Sound::End();
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
	Sound::UpdateAll();
	keyboard_->Update();
	gamepad_->Update();
	scene_manager_->Update();
}


//-------------------------------------
// Draw()
//-------------------------------------
void Application::Draw()
{
	scene_manager_->Draw();
}



//-------------------------------------
// end of file
//-------------------------------------
