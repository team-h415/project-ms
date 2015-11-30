//=========================================================
// scene_namager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include 
//-------------------------------------
#include "../../common/common.h"
#include "../network/network.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../thread/thread.h"
#include "fade/fade.h"
#include "scene.h"
#include "scene_factory.h"
#include "scene_manager.h"


//-------------------------------------
// variable declaration
//-------------------------------------
Scene *SceneManager::current_scene_;
Scene *SceneManager::loading_scene_;
std::string SceneManager::current_name_;
std::string SceneManager::next_name_;
bool SceneManager::next_request_ = false;
bool SceneManager::loading_ = false;
Fade *SceneManager::fade_ = nullptr;
MyThread *SceneManager::thread_ = nullptr;


//-------------------------------------
// SceneManager()
//-------------------------------------
SceneManager::SceneManager()
{
<<<<<<< HEAD
	#ifdef NETWORK_HOST_MODE
		current_scene_ = Create("GameServer");
	#else
		//current_scene_ = Create("Title");
		current_scene_ = Create("Matching");
	#endif
=======
>>>>>>> master
	loading_scene_ = Create("Loading");
	current_scene_ = Create("Matching");
	fade_ = new Fade();
	fade_->SetFade(FADE_OUT);
}


//-------------------------------------
// ~SceneManager()
//-------------------------------------
SceneManager::~SceneManager()
{
	SAFE_DELETE(current_scene_);
	SAFE_DELETE(loading_scene_);
	SAFE_DELETE(fade_);
	SAFE_DELETE(thread_);
}


//-------------------------------------
// Create()
//-------------------------------------
Scene *SceneManager::Create(const std::string &name)
{
	current_name_ = name;
	Scene *scene = SceneFactory::Create(name);
	return scene;
}


//-------------------------------------
// Update()
//-------------------------------------
void SceneManager::Update()
{
	if (!loading_){
		if (next_request_){
			if (fade_->GetFade() == FADE_IN){
				ChangeScene();
			}
			
		}
		else{

			if (fade_->GetFade() == FADE_OUT){
				SAFE_DELETE(thread_);
			}

			current_scene_->Update();
		}
		fade_->Update();
	}
	else{
		loading_scene_->Update();
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void SceneManager::Draw()
{
	if (loading_){
		loading_scene_->Draw();
	}
	else{
		current_scene_->Draw();
	}
}


//-------------------------------------
// ChangeScene()
//-------------------------------------
void SceneManager::RequestScene(const std::string &name)
{
	fade_->SetFade(FADE_IN);
	next_name_ = name;
	next_request_ = true;
}


//-------------------------------------
// ChangeScene()
//-------------------------------------
void SceneManager::ChangeScene()
{
	SAFE_DELETE(current_scene_);
	current_name_ = next_name_;
	loading_ = true;
	if (!thread_){
		thread_ = new MyThread;
	}
	thread_->Create(CreateScene);
	next_request_ = false;
	fade_->SetFade(FADE_OUT);
}


//-------------------------------------
// CreateScene()
//-------------------------------------
void SceneManager::CreateScene()
{
	current_scene_ = Create(next_name_);
	loading_ = false;
}

//-------------------------------------
// end of file
//-------------------------------------
