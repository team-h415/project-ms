//=========================================================
// game.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../network/network.h"
#include "../../network/network_guest.h"
#include "../../../common/common.h"
#include "../../fps/fps.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../../math/vector.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../input/inputs/gamepad.h"
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/mesh/field.h"
#include "../../object/objects/model/x_model.h"
#include "../../object/objects/model/x/x_fort.h"
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_player.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../object/objects/model/fbx/fbx_child.h"
#include "../../object/objects/sprite/timer.h"
#include "../../object/objects/sprite/damage_effect.h"
#include "../../object/objects/sprite/countdown.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../object/objects/sprite/water_gage.h"
#include "../../object/objects/sprite/fort_gauge_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../collision/collision.h"
#include "../../collision/collision_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "../../config/config.h"
#include "game.h"
#include "../fade/fade.h"
#include "../../object/objects/notice/bullet.h"


//-------------------------------------
// warning
//-------------------------------------
#pragma warning (disable:4996)


//-------------------------------------
// Game()
//-------------------------------------
Game::Game()
{
	//-------------------------------------
	// ゲームルール用パラメータ初期化
	//-------------------------------------
	// ステージ
	stage_ = 1;
	// おじデバフフラグ
	grandfather_debuff_ = false;
	// 子供死亡フラグ
	child_death_ = false;
	// 子供リスポーン待ち時間
	child_respawn_waittime_ = 0;
	// ダッシュエフェクトタイマー初期化
	dash_effect_timer_ = 0;
	// 経過フレーム数
	frame_ = 0;
	// 経過時間
	timer_ = 0;

	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = EffectManager::Get();
	collision_manager_ = new CollisionManager;
	font1_ = new DebugFont;
	font2_ = new DebugFont;
	use_camera_name_ = "MainCamera";

	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EFFECT_PARAMETER_DESC water_param;
	water_param.position_ = { 0.0f, 0.0f, 0.0f };
	water_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	water_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	water_param.speed_ = 1.0f;

	effect_manager_->Create(
		"water",
		"resource/effect/BulletFire.efk",
		water_param);

	effect_manager_->Create(
		"damage",
		"resource/effect/Damage3_2.efk",
		water_param);

	effect_manager_->Create(
		"dead",
		"resource/effect/Dead2.efk",
		water_param);

	effect_manager_->Create(
		"smoke",
		"resource/effect/Smoke.efk",
		water_param);

	effect_manager_->Create(
		"smoke2",
		"resource/effect/Smoke2.efk",
		water_param);

	effect_manager_->Create(
		"dash",
		"resource/effect/Dash.efk",
		water_param);

	//-------------------------------------
	// メインカメラ
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 800.0f;
	camera_pos_len_ = CAMERA_POS_LEN;

	camera_manager_->Create(
		"Perspective", "MainCamera", camera_param);

	//-------------------------------------
	// サブカメラ
	//-------------------------------------
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 800.0f;
	camera_pos_len_ = CAMERA_POS_LEN;

	camera_manager_->Create(
		"Perspective", "SubCamera", camera_param);


	//-------------------------------------
	// 地形
	//-------------------------------------
	OBJECT_PARAMETER_DESC field_param;
	field_param.name_ = "field";
	field_param.position_ = { 0.0f, 0.0f, 0.0f };
	field_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	field_param.scaling_ = { 200.0f, 1.0f, 200.0f };
	field_param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		field_param,
		"resource/mesh/map.heightmap");


	//-------------------------------------
	// 空
	//-------------------------------------
	OBJECT_PARAMETER_DESC skydome_param;
	skydome_param.name_ = "skydome_up";
	skydome_param.position_ = { 0.0f, 0.0f, 0.0f };
	skydome_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	skydome_param.scaling_ = { 500.0f, 1.0f, 500.0f };
	skydome_param.layer_ = LAYER_MESH_SKYDOME;

	object_manager_->Create(
		skydome_param,
		"resource/mesh/skydome_up.txt");

	skydome_param.name_ = "skydome_bottom";
	skydome_param.rotation_ = { 0.0f, 0.0f, D3DX_PI };

	object_manager_->Create(
		skydome_param,
		"resource/mesh/skydome_bottom.txt");


	//-------------------------------------
	// 池
	//-------------------------------------
	OBJECT_PARAMETER_DESC lake_param;
	lake_param.name_ = "lake";
	lake_param.position_ = { 0.0f, -0.5f, 0.0f };
	lake_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	lake_param.scaling_ = { 30.0f, 1.0f, 30.0f };
	lake_param.layer_ = LAYER_SPRITE_LAKE;

	object_manager_->Create(
		lake_param);

	//-------------------------------------
	// 砦
	//-------------------------------------
	// 砦1
	OBJECT_PARAMETER_DESC fort1_param;
	fort1_param.name_ = "fort1";
	fort1_param.layer_ = LAYER_MODEL_FORT;
	fort1_param.position_ = FORT1_POSITION;
	fort1_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort1_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		fort1_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort1_collision_param;
	Object *fort1_obj = object_manager_->Get("fort1");
	fort1_collision_param.position_ = {
		fort1_obj->parameter().position_.x_,
		fort1_obj->parameter().position_.y_ + 0.5f,
		fort1_obj->parameter().position_.z_ };
	fort1_collision_param.range_ = 1.0f;
	fort1_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(fort1_obj,
		fort1_collision_param);
	XFort *fort1 = dynamic_cast<XFort*>(fort1_obj);
	fort1->SetLife(FORT1_LiFE);


	// 砦2
	OBJECT_PARAMETER_DESC fort2_param;
	fort2_param.name_ = "fort2";
	fort2_param.layer_ = LAYER_MODEL_FORT;
	fort2_param.position_ = FORT2_POSITION;
	fort2_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort2_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		fort2_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort2_collision_param;
	Object *fort2_obj = object_manager_->Get("fort2");
	fort2_collision_param.position_ = {
		fort2_obj->parameter().position_.x_,
		fort2_obj->parameter().position_.y_ + 0.5f,
		fort2_obj->parameter().position_.z_ };
	fort2_collision_param.range_ = 1.0f;
	fort2_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(fort2_obj,
		fort2_collision_param);
	XFort *fort2 = dynamic_cast<XFort*>(fort2_obj);
	fort2->SetLife(FORT2_LiFE);


	// 砦3
	OBJECT_PARAMETER_DESC fort3_param;
	fort3_param.name_ = "fort3";
	fort3_param.layer_ = LAYER_MODEL_FORT;
	fort3_param.position_ = FORT3_POSITION;
	fort3_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort3_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		fort3_param,
		"resource/model/x/test.x");

	COLLISION_PARAMETER_DESC fort3_collision_param;
	Object *fort3_obj = object_manager_->Get("fort3");
	fort3_collision_param.position_ = {
		fort3_obj->parameter().position_.x_,
		fort3_obj->parameter().position_.y_ + 0.5f,
		fort3_obj->parameter().position_.z_ };
	fort3_collision_param.range_ = 1.0f;
	fort3_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(fort3_obj,
		fort3_collision_param);
	XFort *fort3 = dynamic_cast<XFort*>(fort3_obj);
	fort3->SetLife(FORT3_LiFE);


	//-------------------------------------
	// FBXおじ
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "player1";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION_STAGE1;
	grandfather_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	grandfather_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("player1");

	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_ };
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("player1"), fbx_collision_param);

	//-------------------------------------
	// FBX子供
	//-------------------------------------
	OBJECT_PARAMETER_DESC child_param;
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	child_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	for(int i = 1; i < MAX_GUEST; i++)
	{
		std::string name = "player" + std::to_string(i + 1);
		child_param.name_ = name;
		child_param.position_ = CHILD_POSITION[i];
		object_manager_->Create(child_param);

		COLLISION_PARAMETER_DESC child_collision_param;
		Object *obj3 = object_manager_->Get(name);
		child_collision_param.position_ = {
			obj3->parameter().position_.x_,
			obj3->parameter().position_.y_,
			obj3->parameter().position_.z_ };
		child_collision_param.range_ = 1.0f;
		child_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

		collision_manager_->Create(object_manager_->Get(name),
			child_collision_param);
	}

	//-------------------------------------
	// タイマー
	//-------------------------------------
    OBJECT_PARAMETER_DESC time_param;
	time_param.name_ = "time";
    time_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        40.0f,
        0.0f
    };
    time_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    time_param.scaling_ = { 80.0f, 80.0f, 0.0f };
    time_param.layer_ = LAYER_TIMER;

	Timer* timer = static_cast<Timer*>(
		object_manager_->Create(
		time_param));

	timer->SetTexture("resource/texture/figure_all.png");
	timer->SetFigureOffset(-30.0f);
	timer->SetValue(GAME_TIMER);
	timer->SetState(TIMER_STOP);
	//パラメータ設定後に実行
	timer->GenerateNumber();

	//-------------------------------------
	// 砦UI
	//-------------------------------------
    OBJECT_PARAMETER_DESC fort_state_param;
	fort_state_param.name_ = "fort_state";
    fort_state_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        100.0f,
        0.0f
    };
    fort_state_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    fort_state_param.scaling_ = { 160.0f, 40.0f, 0.0f };
    fort_state_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
		fort_state_param);


    OBJECT_PARAMETER_DESC fort_gauge_param;
	fort_gauge_param.name_ = "fort_gauge_manager";
    fort_gauge_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        100.0f,
        0.0f
    };
    fort_gauge_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    fort_gauge_param.scaling_ = { 40.0f, 40.0f, 0.0f };
    fort_gauge_param.layer_ = LAYER_FORT_GAUGE;

    object_manager_->Create(
		fort_gauge_param,
        "resource/texture/game/Child_01.jpg");


    //-------------------------------------
    // 水ゲージ下地UI
    //-------------------------------------
	OBJECT_PARAMETER_DESC water_design_param;
	water_design_param.name_ = "water_design";
    water_design_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_design_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_design_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_design_param.layer_ = LAYER_WATER_GAGE;

    object_manager_->Create(
		 water_design_param,
        "resource/texture/game/water_gage_background.png");

    //-------------------------------------
    // 水ゲージ（ゲージ本体）UI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_gage_param;
	water_gage_param.name_ = "water_gage";
    water_gage_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_gage_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_gage_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_gage_param.layer_ = LAYER_WATER_GAGE;

    object_manager_->Create(
        water_gage_param,
        "resource/texture/game/water_gage_diffuse.png");

    //-------------------------------------
    // 水ゲージ（周り）UI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_gage_around_param;
	water_gage_around_param.name_ = "water_gage_around";
    water_gage_around_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_gage_around_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_gage_around_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_gage_around_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
         water_gage_around_param,
        "resource/texture/game/water_gage_around.png");

    //-------------------------------------
    // 水ポリゴンUI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_poly_param;
	water_poly_param.name_ = "water_poly";
    water_poly_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_poly_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_poly_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_poly_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        water_poly_param,
        "resource/texture/game/water_desine.png");

    //-------------------------------------
    // ダメージエフェクトUI
    //-------------------------------------
    OBJECT_PARAMETER_DESC hit_point_param;
	hit_point_param.name_ = "damage_effect";
    hit_point_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        SCREEN_HEIGHT * 0.5f,
        0.0f
    };
    hit_point_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    hit_point_param.scaling_ = { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f };
    hit_point_param.layer_ = LAYER_DAMAGE_EFFECT;

    object_manager_->Create(
        hit_point_param);

	//-------------------------------------
	// インスタンシングテスト
	//-------------------------------------
	/*OBJECT_PARAMETER_DESC wood_param;
	wood_param.layer_ = LAYER_TREE;
	object_manager_->Create(
		"wood", wood_param);*/

	//-------------------------------------
	// ベンチ
	//-------------------------------------
	OBJECT_PARAMETER_DESC bench_param;
	bench_param.name_ = "bench";
	bench_param.layer_ = LAYER_BENCH;
	object_manager_->Create(
		bench_param);

	// ダッシュエフェクトタイマー初期化
	dash_effect_timer_ = 0;
	//-------------------------------------
	// 影
	//-------------------------------------
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < MAX_GUEST; i++)
	{
		shadow_param.name_ = "shadow" + std::to_string(i + 1);
		object_manager_->Create(shadow_param);
	}

	//-------------------------------------
	// カウントダウン
	//-------------------------------------
	OBJECT_PARAMETER_DESC countdown_param;
	countdown_param.name_ = "countdown";
	countdown_param.position_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f };
	countdown_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	countdown_param.scaling_ = {
		SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f };
	countdown_param.layer_ = LAYER_COUNTDOWN;
	object_manager_->Create(
		countdown_param);

	//-------------------------------------
	// バレット生成しておくよ
	//-------------------------------------
	OBJECT_PARAMETER_DESC bullet_param;
	bullet_param.layer_ = LAYER_BULLET;
	for(int i = 0; i < MAX_BULLET; i++)
	{
		bullet_param.name_ = "bullet" + std::to_string(i);
		object_manager_->Create(
			bullet_param);
	}

	//-------------------------------------
	// ゲームルール用パラメータ初期化
	//-------------------------------------
	// ステージ
	stage_ = 1;
	// おじデバフフラグ
	grandfather_debuff_ = false;
	// 子供死亡フラグ
	child_death_ = false;
	// 子供リスポーン待ち時間
	child_respawn_waittime_ = 0;


#ifdef NETWORK_HOST_MODE
#else
	NETWORK_DATA network_data;
	network_data.type_ = DATA_COMPLETE_SCENE_CHANGE;
	NetworkGuest::SendTo(network_data);
#endif
}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	effect_manager_ = nullptr;
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font1_);
	SAFE_DELETE(font2_);
	SAFE_DELETE(collision_manager_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
}


//-------------------------------------
// Draw()
//-------------------------------------
void Game::Draw()
{
	RECT rect1 = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH) / 2,
		static_cast<LONG>(SCREEN_HEIGHT) / 2 };
	RECT rect2 = {
		static_cast<LONG>(SCREEN_WIDTH)-400,
		static_cast<LONG>(SCREEN_HEIGHT)-200,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font1_color(0.0f, 0.2f, 0.0f, 1.0f);
	D3DXCOLOR font2_color(0.0f, 0.0f, 1.0f, 1.0f);
	MaterialColor color(32, 32, 32, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);

	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	//collision_manager_->Draw();
	
	font1_->Draw(rect1, font1_color);
	font2_->Draw(rect2, font2_color);
	Fade::Draw();

	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
