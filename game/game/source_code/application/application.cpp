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
#include "object/object.h"
#include "object/objects/model/x/instancing_tree.h"
#include "object/objects/model/x/instancing_playground.h"
#include "object/objects/sprite/blind.h"
#include "object/objects/model/fbx_model.h"
#include "object/objects/model/fbx/fbx_player.h"
#include "object/objects/model/fbx/fbx_child.h"
#include "object/object_manager.h"
#include "camera/camera.h"
#include "camera/camera_manager.h"
#include "collision/collision.h"
#include "collision/collision_manager.h"
#include "shader/shader_manager.h"
#include "application.h"

#include "config/config.h"


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
	// コンフィグ読み込み
	Config::SetupConfig();

	fps_ = new Fps;
	Window::Create(
		instance,
		command_show);
	renderer_ = RendererFactory::Create(
		WindowHolder::handle_,
		"DirectX9");
	SetupManagers();
	Sound::Setup();
	scene_manager_ = new SceneManager;
	keyboard_ = InputFactory::Create(
		"KeyBoard");
	gamepad_ = InputFactory::Create(
		"GamePad");
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
	ShaderManager::AllRelease();
	TextureManager::AllRelease();
	FbxContainerManager::AllRelease();
	XContainerManager::AllRelease();
	Sound::End();
	EffectManager::Delete();
	ObjectManager::Delete();
	CameraManager::Delete();
	CollisionManager::Delete();
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
// SetupManagers()
//-------------------------------------
void Application::SetupManagers()
{
	//-------------------------------------
	// カメラ作成
	//-------------------------------------
	CameraManager* camera_manager(CameraManager::Get());
#ifdef NETWORK_HOST_MODE
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = {0.0f, 0.0f, -10.0f};
	camera_param.focus_ = {0.0f, 0.0f, 0.0f};
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = {0.0f, 1.0f, 0.0f};
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;

	for(int i = 0; i < MAX_GUEST; i++)
	{
		camera_manager->Create(
			"Perspective", "camera" + std::to_string(i), camera_param);
	}
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = {0.0f, 0.0f, -10.0f};
	camera_param.focus_ = {0.0f, 0.0f, 0.0f};
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = {0.0f, 1.0f, 0.0f};
	camera_param.near_ = 0.1f;
	camera_param.far_ = 800.0f;

	camera_manager->Create(
		"Perspective", "SubCamera", camera_param);
#else
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 800.0f;

	camera_manager->Create(
		"Perspective", "MainCamera", camera_param);
#endif


	//-------------------------------------
	// オブジェクト及び当たり判定作成
	//-------------------------------------
	ObjectManager* object_manager(ObjectManager::Get());
	CollisionManager* collision_manager(CollisionManager::Get());

	// 地形
	OBJECT_PARAMETER_DESC param;
	param.name_ = "field";
	param.position_ = {0.0f, 0.0f, 0.0f};
	param.rotation_ = {0.0f, 0.0f, 0.0f};
	param.scaling_ = {100.0f, 1.0f, 200.0f};
	param.layer_ = LAYER_MESH_FIELD;
	object_manager->Create(param, "resource/mesh/map.heightmap");

	// 海
	OBJECT_PARAMETER_DESC sea_param;
	sea_param.name_ = "sea";
	sea_param.position_ = {0.0f, SEA_HEIGHT, 0.0f};
	sea_param.rotation_ = {0.0f, 0.0f, 0.0f};
	sea_param.scaling_ = {300.0f, 1.0f, 300.0f};
	sea_param.layer_ = LAYER_SPRITE_LAKE;
	object_manager->Create(sea_param);

	// 砦
	for(int i = 0; i < 3; i++)
	{
		OBJECT_PARAMETER_DESC fort_param;
		fort_param.name_ = "fort" + std::to_string(i);
		fort_param.layer_ = LAYER_MODEL_FORT;
		fort_param.position_ = FORT_POSITION[i];
		fort_param.rotation_ = {0.0f, 0.0f, 0.0f};
		fort_param.scaling_ = {1.0f, 1.0f, 1.0f};

		Object *fort = object_manager->Create(
									fort_param,
									"resource/model/x/fort.x");

		Vector3 pos = fort->parameter().position_;
		COLLISION_PARAMETER_DESC fort_collision_param;
		fort_collision_param.position_ = {
			pos.x_,
			pos.y_ + 0.5f,
			pos.z_};
		fort_collision_param.range_ = 1.0f;
		fort_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

		collision_manager->Create(fort, fort_collision_param);
	}

	// FBXおじ
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "player0";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = {1.0f, 0.0f, 0.0f};
	grandfather_param.rotation_ = {0.0f, 0.0f, 0.0f};
	grandfather_param.scaling_ = {1.0f, 1.0f, 1.0f};
	grandfather_param.ex_id_ = 0;
	Object *grandfather = object_manager->Create(grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	fbx_collision_param.position_ = {
		grandfather->parameter().position_.x_,
		grandfather->parameter().position_.y_,
		grandfather->parameter().position_.z_};
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager->Create(grandfather, fbx_collision_param);

	// FBX子供
	for(int i = 1; i < MAX_GUEST; i++)
	{
		OBJECT_PARAMETER_DESC child_param;
		child_param.layer_ = LAYER_MODEL_CHILD;
		child_param.position_ = {-1.0f, 0.0f, 0.0f};
		child_param.rotation_ = {0.0f, 0.0f, 0.0f};
		child_param.scaling_ = {1.0f, 1.0f, 1.0f};
		child_param.name_ = "player" + std::to_string(i);
		child_param.ex_id_ = i;
		Object *object = object_manager->Create(child_param);
		FbxChild* child = dynamic_cast<FbxChild*>(object);
		std::string path = "resource/texture/game/Child_0";
		path += std::to_string(i);
		path += ".jpg";
		child->SetTexture(path);

		COLLISION_PARAMETER_DESC child_collision_param;
		child_collision_param.position_ = {
			child->parameter().position_.x_,
			child->parameter().position_.y_,
			child->parameter().position_.z_};
		child_collision_param.range_ = 1.0f;
		child_collision_param.offset_ = {0.0f, 0.5f, 0.0f};
		collision_manager->Create(child, child_collision_param);
	}

	// バレット生成しておくよ
	OBJECT_PARAMETER_DESC bullet_param;
	bullet_param.layer_ = LAYER_BULLET;
	for(int i = 0; i < MAX_BULLET; i++)
	{
		bullet_param.name_ = "bullet" + std::to_string(i);
		object_manager->Create(bullet_param);
	}

	// ボムも生成しておくよ
	OBJECT_PARAMETER_DESC bomb_param;
	bomb_param.layer_ = LAYER_BOMB;
	for(int i = 0; i < MAX_BOMB; i++)
	{
		bomb_param.name_ = "bomb" + std::to_string(i);
		object_manager->Create(bomb_param);
	}

	// ベンチ
	OBJECT_PARAMETER_DESC bench_param;
	bench_param.name_ = "bench";
	bench_param.layer_ = LAYER_BENCH;
	object_manager->Create(bench_param);

	// 遊具
	OBJECT_PARAMETER_DESC playground_param;
	playground_param.name_ = "playground1";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = {1.0f, 1.0f, 1.0f};
	InstancingPlayground *playground1 = dynamic_cast<InstancingPlayground*>(object_manager->Create(playground_param));
	playground1->SetMesh("resource/model/x/suberi.x");
	playground1->SetTexture("resource/texture/game/suberi.jpg");
	playground1->SetPositionPatern(0);

	playground_param.name_ = "playground2";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = {1.0f, 1.0f, 1.0f};
	InstancingPlayground *playground2 = dynamic_cast<InstancingPlayground*>(object_manager->Create(playground_param));
	playground2->SetMesh("resource/model/x/buranko.x");
	playground2->SetTexture("resource/texture/game/buranko.jpg");
	playground2->SetPositionPatern(1);

	playground_param.name_ = "playground3";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = {1.5f, 1.5f, 1.5f};
	InstancingPlayground *playground3 = dynamic_cast<InstancingPlayground*>(object_manager->Create(playground_param));
	playground3->SetMesh("resource/model/x/taiya.x");
	playground3->SetTexture("resource/texture/game/taiya_x.jpg");
	playground3->SetPositionPatern(2);

#ifdef NETWORK_HOST_MODE

#else
	// 空
	OBJECT_PARAMETER_DESC skydome_param;
	skydome_param.name_ = "skydome_up";
	skydome_param.position_ = { 0.0f, -1.0f, 0.0f };
	skydome_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	skydome_param.scaling_ = { 500.0f, 1.0f, 500.0f };
	skydome_param.layer_ = LAYER_MESH_SKYDOME;

	object_manager->Create(
		skydome_param,
		"resource/mesh/skydome_up.txt");

	skydome_param.name_ = "skydome_bottom";
	skydome_param.rotation_ = { 0.0f, 0.0f, D3DX_PI };

	object_manager->Create(
		skydome_param,
		"resource/mesh/skydome_bottom.txt");

	// 木
	OBJECT_PARAMETER_DESC wood_param;
	wood_param.name_ = "wood1";
	wood_param.layer_ = LAYER_TREE;
	InstancingTree *tree1 = dynamic_cast<InstancingTree*>(object_manager->Create(wood_param));
	tree1->SetMesh("resource/model/x/tree01.x");
	tree1->SetTexture("resource/texture/game/tree01.png");
	tree1->SetPositionPatern(0);

	wood_param.name_ = "wood2";
	InstancingTree *tree2 = dynamic_cast<InstancingTree*>(object_manager->Create(wood_param));
	tree2->SetMesh("resource/model/x/tree02.x");
	tree2->SetTexture("resource/texture/game/tree02.png");
	tree2->SetPositionPatern(1);

	wood_param.name_ = "wood3";
	InstancingTree *tree3 = dynamic_cast<InstancingTree*>(object_manager->Create(wood_param));
	tree3->SetMesh("resource/model/x/tree03.x");
	tree3->SetTexture("resource/texture/game/tree01.png");
	tree3->SetPositionPatern(2);

	// 影
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < MAX_GUEST; i++)
	{
		shadow_param.name_ = "shadow" + std::to_string(i);
		object_manager->Create(shadow_param);
	}

	// ブラインド生成しておくよ
	OBJECT_PARAMETER_DESC blind_param;
	blind_param.layer_ = LAYER_BLIND;
	for (int i = 0; i < MAX_BLIND; i++)
	{
		blind_param.name_ = "blind" + std::to_string(i);
		Object* obj = object_manager->Create(blind_param);
		Blind* blind = static_cast<Blind*>(obj);
		std::string texture_name = "resource/texture/game/blind_" + std::to_string(i % BLIND_TEXTURE_MAX) + ".png";
		blind->SetTexture(texture_name);
	}

	//-------------------------------------
	// 2Dオブジェクトの描画フラグをOFFに
	//-------------------------------------
	for(int i = LAYER_DAMAGE_EFFECT; i < LAYER_MAX; i++)
	{
		object_manager->SetDrawEnable(i, false);
	}

	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EffectManager* effect_manager(EffectManager::Get());
	EFFECT_PARAMETER_DESC effect_param;
	ZeroMemory(&effect_param, sizeof(effect_param));
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;

	const int MAX_EFFECT(15);
	const char* EFFECT_NAME[MAX_EFFECT]
	{
		"water",
		"BombFire",
		"damage",
		"dash",
		"dead",
		"fieldhit",
		"marker",
		"portal",
		"shieldin",
		"shieldout",
		"smoke",
		"smoke2",
		"SpeedDown",
		"watersupply",
		"watersupplybubble",
	};

	const char* EFFECT_PATH[MAX_EFFECT]
	{
		"resource/effect/BulletFire.efk",
		"resource/effect/BombFire.efk",
		"resource/effect/Damage3_3x0.5.efk",
		"resource/effect/Dash.efk",
		"resource/effect/Dead2.efk",
		"resource/effect/FieldHit2.efk",
		"resource/effect/Marker.efk",
		"resource/effect/Portal2x2.efk",
		"resource/effect/ShieldIn_2x2.efk",
		"resource/effect/ShieldOut2x2.efk",
		"resource/effect/Smoke.efk",
		"resource/effect/Smoke2.efk",
		"resource/effect/SpeedDown.efk",
		"resource/effect/WaterSupply.efk",
		"resource/effect/WaterSupply2.efk",
	};

	for(int i = 0; i < MAX_EFFECT; i++)
	{
		effect_manager->Create(
			EFFECT_NAME[i],
			EFFECT_PATH[i],
			effect_param);
	}
#endif
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
