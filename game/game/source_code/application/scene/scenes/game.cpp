//=========================================================
// game.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../config/config.h"
#include "../../fps/fps.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../../math/my_math.h"
#include "../../math/vector.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../input/inputs/gamepad.h"
#include "../../sound/sound.h"
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/mesh/field.h"
#include "../../object/objects/model/x_model.h"
#include "../../object/objects/model/x/x_fort.h"
#include "../../object/objects/model/x/instancing_tree.h"
#include "../../object/objects/model/x/instancing_playground.h"
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_player.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../object/objects/model/fbx/fbx_child.h"
#include "../../object/objects/sprite/sprite2d.h"
#include "../../object/objects/sprite/timer.h"
#include "../../object/objects/sprite/damage_effect.h"
#include "../../object/objects/sprite/countdown.h"
#include "../../object/objects/sprite/water_gage.h"
#include "../../object/objects/sprite/fort_gauge_manager.h"
#include "../../object/objects/sprite/message/message.h"
#include "../../object/objects/sprite/blind.h"
#include "../../object/objects/bullet/bullet.h"
#include "../../object/objects/bullet/bomb.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../collision/collision.h"
#include "../../collision/collision_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "game.h"
#include "../fade/fade.h"


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
    // サウンドSEが連続で再生させないためのフラグ
    start_only_se_flg_ = true;
    sound_se_flg_ = true;
    // プレイヤー移動時の経過時間
    walk_timer_ = 0;
    walk_flg_ = false;
    // デバフエフェクトのフラグ
    debuff_effect_flg_ = false;
	// シールド有効フラグ（はじめOFF)
	shield_flg_ = false;
	// ステージ移行フラグ(はじめON)
	change_stage_flg_ = true;

	// 乱数生成
	srand((unsigned)time(NULL));

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
    object_manager_->SetDrawEnable(LAYER_DAMAGE_EFFECT, false);

	

}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	effect_manager_ = nullptr;
    sound_->ReleaseSound(&sound_);
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font1_);
	SAFE_DELETE(font2_);
	SAFE_DELETE(collision_manager_);
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Game::Initialize()
{
	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	effect_param.position_ = { 0.0f, 0.0f, 0.0f };
	effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	effect_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	effect_param.speed_ = 1.0f;

	effect_manager_->Create(
		"water",
		"resource/effect/BulletFire.efk",
		effect_param);

	effect_manager_->Create(
		"damage",
		"resource/effect/Damage3_3x0.5.efk",
		effect_param);

	effect_manager_->Create(
		"dead",
		"resource/effect/Dead2_2.efk",
		effect_param);

	effect_manager_->Create(
		"smoke",
		"resource/effect/Smoke.efk",
		effect_param);

	effect_manager_->Create(
		"smoke2",
		"resource/effect/Smoke2.efk",
		effect_param);

	effect_manager_->Create(
		"dash",
		"resource/effect/Dash.efk",
		effect_param);

	effect_manager_->Create(
		"watersupply",
		"resource/effect/WaterSupply.efk",
		effect_param);

	effect_manager_->Create(
		"watersupplybubble",
		"resource/effect/WaterSupply2.efk",
		effect_param);

	effect_manager_->Create(
		"speeddown",
		"resource/effect/SpeedDown.efk",
		effect_param);

	effect_manager_->Create(
		"bombfire",
		"resource/effect/BombFire.efk",
		effect_param);

	effect_manager_->Create(
		"shieldin",
		"resource/effect/ShieldIn_3x2.efk",
		effect_param);

	effect_manager_->Create(
		"shieldout",
		"resource/effect/ShieldOut2x2.efk",
		effect_param);

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
	field_param.scaling_ = { 100.0f, 1.0f, 200.0f };
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
	lake_param.position_ = { 0.0f, -0.8f, 0.0f };
	lake_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	lake_param.scaling_ = { 300.0f, 1.0f, 300.0f };
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
		"resource/model/x/fort.x");

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
		"resource/model/x/fort.x");

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
		"resource/model/x/fort.x");

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
	grandfather_param.name_ = "grandfather";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION_STAGE1;
	grandfather_param.rotation_ = { 0.0f, GRANDFATHER_ROTATION_STAGE1, 0.0f };
	grandfather_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("grandfather");
	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_ };
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("grandfather"),
		fbx_collision_param);

	//-------------------------------------
	// FBX子供
	//-------------------------------------
	OBJECT_PARAMETER_DESC child_param;
	child_param.name_ = "child";
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.position_ = GRANDFATHER_POSITION_STAGE1;
	child_param.position_.x_ += 1.0f;
	child_param.rotation_ = CHILD1_ROTATION1;
	child_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		child_param);

	COLLISION_PARAMETER_DESC child_collision_param;
	Object *obj3 = object_manager_->Get("child");
	child_collision_param.position_ = {
		obj3->parameter().position_.x_,
		obj3->parameter().position_.y_,
		obj3->parameter().position_.z_ };
	child_collision_param.range_ = 1.0f;
	child_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("child"),
		child_collision_param);

	//-------------------------------------
	// まーかー
	//-------------------------------------
	EFFECT_PARAMETER_DESC marker_param;
	MyEffect *effect_marker = effect_manager_->Get("marker");
	marker_param = effect_marker->parameter();
	marker_param.position_ = GRANDFATHER_POSITION_STAGE1;
	marker_param.position_.y_ = -100.0f;
	marker_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	effect_marker->SetParameter(marker_param);
	effect_manager_->Play("marker");


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
	// 木
	//-------------------------------------
	OBJECT_PARAMETER_DESC wood_param;
	wood_param.name_ = "wood1";
	wood_param.layer_ = LAYER_TREE;
	InstancingTree *tree1 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
	tree1->SetMesh("resource/model/x/tree01.x");
	tree1->SetTexture("resource/texture/game/tree01.png");
	tree1->SetPositionPatern(0);

	wood_param.name_ = "wood2";
	InstancingTree *tree2 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
	tree2->SetMesh("resource/model/x/tree02.x");
	tree2->SetTexture("resource/texture/game/tree02.png");
	tree2->SetPositionPatern(1);

	wood_param.name_ = "wood3";
	InstancingTree *tree3 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
	tree3->SetMesh("resource/model/x/tree03.x");
	tree3->SetTexture("resource/texture/game/tree01.png");
	tree3->SetPositionPatern(2);

	//-------------------------------------
	// ベンチ
	//-------------------------------------
	OBJECT_PARAMETER_DESC bench_param;
	bench_param.name_ = "bench";
	bench_param.layer_ = LAYER_BENCH;
	object_manager_->Create(
		bench_param);

	//-------------------------------------
	// 遊具
	//-------------------------------------
	OBJECT_PARAMETER_DESC playground_param;
	playground_param.name_ = "playground1";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	InstancingPlayground *playground1 = dynamic_cast<InstancingPlayground*>(object_manager_->Create(playground_param));
	playground1->SetMesh("resource/model/x/suberi.x");
	playground1->SetTexture("resource/texture/game/suberi.jpg");
	playground1->SetPositionPatern(0);

	playground_param.name_ = "playground2";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	InstancingPlayground *playground2 = dynamic_cast<InstancingPlayground*>(object_manager_->Create(playground_param));
	playground2->SetMesh("resource/model/x/buranko.x");
	playground2->SetTexture("resource/texture/game/buranko.jpg");
	playground2->SetPositionPatern(1);

	playground_param.name_ = "playground3";
	playground_param.layer_ = LAYER_PLAYGROUND;
	playground_param.scaling_ = { 1.5f, 1.5f, 1.5f };
	InstancingPlayground *playground3 = dynamic_cast<InstancingPlayground*>(object_manager_->Create(playground_param));
	playground3->SetMesh("resource/model/x/taiya.x");
	playground3->SetTexture("resource/texture/game/taiya_x.jpg");
	playground3->SetPositionPatern(2);


	//-------------------------------------
	// 影
	//-------------------------------------
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.name_ = "shadow";
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	object_manager_->Create(
		shadow_param);

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
	for (int i = 0; i < MAX_BULLET; i++)
	{
		bullet_param.name_ = "bullet" + std::to_string(i);
		object_manager_->Create(
			bullet_param);
	}

	//-------------------------------------
	// ボムも生成しておくよ
	//-------------------------------------
	OBJECT_PARAMETER_DESC bomb_param;
	bomb_param.layer_ = LAYER_BOMB;
	for (int i = 0; i < MAX_BULLET; i++)
	{
		bomb_param.name_ = "bomb" + std::to_string(i);
		object_manager_->Create(
			bomb_param);
	}

	//-------------------------------------
	// ブラインド生成しておくよ
	//-------------------------------------
	OBJECT_PARAMETER_DESC blind_param;
	blind_param.layer_ = LAYER_BLIND;
	for (int i = 0; i < MAX_BLIND; i++)
	{
		blind_param.name_ = "blind" + std::to_string(i);
		Object* obj = object_manager_->Create(blind_param);
		Blind* blind = static_cast<Blind*>(obj);
		std::string texture_name = "resource/texture/game/blind_" + std::to_string(i % BLIND_TEXTURE_MAX) + ".png";
		blind->SetTexture(texture_name);
	}

	//-------------------------------------
	// メッセージ
	//-------------------------------------
	OBJECT_PARAMETER_DESC message_param;
	message_param.name_ = "message_child1_death";
	message_param.position_ = {
		0.0f,
		MESSAGE_DISP_POSITION_Y,
		0.0f };
	message_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	message_param.scaling_ = { 1.0f, 1.0f, 0.0f };
	message_param.layer_ = LAYER_MESSAGE;

	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_child1_death.png");
	message_param.name_ = "message_child2_death";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_child2_death.png");
	message_param.name_ = "message_child3_death";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_child3_death.png");
	message_param.name_ = "message_child4_death";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_child4_death.png");


	message_param.name_ = "message_fort_25";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_fort_25.png");
	message_param.name_ = "message_fort_50";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_fort_50.png");
	message_param.name_ = "message_fort_75";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_fort_75.png");
	message_param.name_ = "message_fort_100";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_fort_100.png");


	message_param.name_ = "message_grandfather_debuff";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_grandfather_debuff.png");
	message_param.name_ = "message_grandfather_return";
	object_manager_->Create(
		message_param,
		"resource/texture/game/message/message_grandfather_return.png");

	//-------------------------------------
	// サウンド(BGM)
	//-------------------------------------
	sound_ = nullptr;
	sound_ = Sound::LoadSound("resource/sound/bgm/game/game_bgm.wav");
	sound_->Play(true);
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
    //-------------------------------------
    // 変数宣言
    //-------------------------------------
    // 動的変数
    Object *fort1_object = object_manager_->Get("fort1");
    Object *fort2_object = object_manager_->Get("fort2");
    Object *fort3_object = object_manager_->Get("fort3");
    Object *grandfather_object = object_manager_->Get("grandfather");
    Object *child_object = object_manager_->Get("child");
    Object *countdown_object = object_manager_->Get("countdown");
    Vector3 fort1_position(fort1_object->parameter().position_);
    Vector3 fort2_position(fort2_object->parameter().position_);
    Vector3 fort3_position(fort3_object->parameter().position_);
    Vector3 grandfather_position(grandfather_object->parameter().position_);
    Vector3 grandfather_rotation(grandfather_object->parameter().rotation_);
    Vector3 child_position(child_object->parameter().position_);
    Vector3 child_rotation(child_object->parameter().rotation_);
    Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
    FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(grandfather_object);
    FbxChild *child = dynamic_cast<FbxChild*>(child_object);
    WaterGage *waterGage = dynamic_cast<WaterGage*>(object_manager_->Get("water_gage"));
    DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(
        object_manager_->Get("damage_effect"));
    FortGaugeManager *fort_gauge_manager = dynamic_cast<FortGaugeManager*>(
        object_manager_->Get("fort_gauge_manager"));
    XFort *fort1 = dynamic_cast<XFort*>(fort1_object);
    XFort *fort2 = dynamic_cast<XFort*>(fort2_object);
    XFort *fort3 = dynamic_cast<XFort*>(fort3_object);
    CountDown *countdown = dynamic_cast<CountDown*>(countdown_object);
    float player_speed = CHARANCTER_MOVESPEED;
    float father_life = grandfather->GetLife();
    float father_watergauge = grandfather->GetWaterGauge();
    float father_debuff_power = grandfather->GetDebuffPower();
    float child_life = child->GetLife();
    float child_watergauge = child->GetWaterGauge();
    float fort1_life = fort1->GetLife();
    float fort2_life = fort2->GetLife();
    float fort3_life = fort3->GetLife();
    player_speed -= father_debuff_power * player_speed;

    // 静的変数
    static int shot_late = 0;
    static D3DXVECTOR3 fort_underground(-3.0f, -3.0f, -3.0f);
    static Vector3 grandfather_prevposition(grandfather_object->parameter().position_);
    static int fort_damage_state = 0;


    //-------------------------------------
    // 時間経過
    //-------------------------------------
    frame_++;
    if (!(frame_ % 60)){
        timer_++;
        if (start_only_se_flg_)
            sound_se_flg_ = true;
    }

    //-------------------------------------
    // 時間経過でカウントダウン
    // カウントダウンSE再生
    //-------------------------------------
    switch (timer_)
    {
    case 0:
        countdown->ChangeTexture(4);
        break;
    case 1:
        countdown->ChangeTexture(3);
        if (sound_se_flg_)
        {
            Sound::LoadAndPlaySE("resource/sound/se/game/countdown.wav");
            sound_se_flg_ = false;
        }
        break;
    case 2:
        countdown->ChangeTexture(2);
        if (sound_se_flg_)
        {
            Sound::LoadAndPlaySE("resource/sound/se/game/countdown.wav");
            sound_se_flg_ = false;
        }
        break;
    case 3:
        countdown->ChangeTexture(1);
        if (sound_se_flg_)
        {
            Sound::LoadAndPlaySE("resource/sound/se/game/countdown.wav");
            sound_se_flg_ = false;
        }
        break;
    case 4:
        countdown->ChangeTexture(0);
        if (sound_se_flg_)
        {
            Sound::LoadAndPlaySE("resource/sound/se/game/start.wav");
            sound_se_flg_ = false;
        }
        break;
    default:
        countdown->ChangeTexture(4);
        start_only_se_flg_ = false;
        break;
    }

    //-------------------------------------
    // メッセージの再生
    //-------------------------------------
	Object *message_object_25 = object_manager_->Get("message_fort_25");
	Message *message_25 = dynamic_cast<Message*>(message_object_25);

	Object *message_object_50 = object_manager_->Get("message_fort_50");
	Message *message_50 = dynamic_cast<Message*>(message_object_50);

	Object *message_object_75 = object_manager_->Get("message_fort_75");
	Message *message_75 = dynamic_cast<Message*>(message_object_75);

	Object *message_object_100 = object_manager_->Get("message_fort_100");
	Message *message_100 = dynamic_cast<Message*>(message_object_100);

    // 砦が破壊された！
    if ((fort_damage_state == 75 && (fort1_life <= 0.0f) && stage_ == 1) ||
        (fort_damage_state == 75 && (fort2_life <= 0.0f) && stage_ == 2)){
		message_100->Play();
		message_25->Move(-100.0f);
		message_50->Move(-100.0f);
		message_75->Move(-100.0f);
        fort_damage_state = 0;
		
		// ステージ移行フラグ設定
		change_stage_flg_ = true;

		if (shield_flg_ == true){
			// シールドを消すと同時に
			// シールド解除エフェクト発生
			EFFECT_PARAMETER_DESC shieldout_param;
			MyEffect *effect_shieldout = effect_manager_->Get("shieldout");
			MyEffect *effect_shieldin = effect_manager_->Get("shieldin");
			shieldout_param = effect_shieldout->parameter();
			shieldout_param.position_ = effect_shieldin->parameter().position_;
			shieldout_param.rotation_ = { 0.0f, 0.0f, 0.0f };
			effect_shieldout->SetParameter(shieldout_param);
			effect_manager_->Stop("shieldin");
			effect_manager_->Play("shieldout");

			shield_flg_ = false;
		}
    }
    // 損壊率75%
    else if ((fort_damage_state == 50 && fort1_life < FORT1_LiFE * 0.25f && stage_ == 1) ||
        (fort_damage_state == 50 && fort2_life < FORT2_LiFE * 0.25f && stage_ == 2) ||
        (fort_damage_state == 50 && fort3_life < FORT3_LiFE * 0.25f && stage_ == 3)){
		message_75->Play();
		message_25->Move(-100.0f);
		message_50->Move(-100.0f);
		message_100->Move(-100.0f);
        fort_damage_state = 75;
    }
    // 損壊率50%
    else if ((fort_damage_state == 25 && fort1_life < FORT1_LiFE * 0.5f && stage_ == 1) ||
        (fort_damage_state == 25 && fort2_life < FORT2_LiFE * 0.5f && stage_ == 2) ||
        (fort_damage_state == 25 && fort3_life < FORT3_LiFE * 0.5f && stage_ == 3)){
 		message_50->Play();
		message_25->Move(-100.0f);
		message_75->Move(-100.0f);
		message_100->Move(1-00.0f);
        fort_damage_state = 50;
    }
    // 損壊率25%
    else if ((fort_damage_state == 0 && fort1_life < FORT1_LiFE * 0.75f && stage_ == 1) ||
        (fort_damage_state == 0 && fort2_life < FORT2_LiFE * 0.75f && stage_ == 2) ||
        (fort_damage_state == 0 && fort3_life < FORT3_LiFE * 0.75f && stage_ == 3)){
   		message_25->Play();
		message_50->Move(-100.0f);
		message_75->Move(-100.0f);
		message_100->Move(-100.0f);
        fort_damage_state = 25;
    }

    //-------------------------------------
    // ゲームステージデバッグ
    //-------------------------------------
    if (fort1_life <= 0.0f){
        stage_ = 2;
        if (fort2_life <= 0.0f){
            stage_ = 3;
        }
    }

    fort_gauge_manager->SetFortLife(0, fort1_life);
    fort_gauge_manager->SetFortLife(1, fort2_life);
    fort_gauge_manager->SetFortLife(2, fort3_life);

    //-------------------------------------
    // プレイヤー移動処理
    //-------------------------------------
    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_11)){

        player_speed = CHARANCTER_MOVESPEED * 2.0f;
        player_speed -= father_debuff_power * player_speed;
        // ダッシュエフェクト
		if (dash_effect_timer_ % 10 == 0 && player_speed > 0.0f){
			
			D3DXVECTOR2 move_vector;
			move_vector.x =
				cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
				sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_;
			// Z方向移動
			move_vector.y =
				sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
				cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_;

			if (move_vector.x != 0.0f && move_vector.y != 0.0f){
				// エフェクト回転角度を求める
				float dash_rotato_y = 0.0f;
				dash_rotato_y = atan2(move_vector.x, -move_vector.y);
				if (dash_rotato_y < -D3DX_PI){
					dash_rotato_y += D3DX_PI * 2.0f;
				}
				else if (dash_rotato_y > D3DX_PI){
					dash_rotato_y -= D3DX_PI * 2.0f;
				}

				// エフェクト出す
				EFFECT_PARAMETER_DESC effect_param;
				MyEffect *effect = effect_manager_->Get("dash");
				effect_param = effect->parameter();
				effect_param.position_ = grandfather_position;
				effect_param.position_.y_ += 0.1f;
				effect_param.rotation_ = { 0.0f, dash_rotato_y, 0.0f };
				effect->SetParameter(effect_param);
				effect_manager_->Play("dash");
			}

        }
        dash_effect_timer_++;
    }
    else{ dash_effect_timer_ = 0; }
    grandfather_position.x_ += (
        cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
        sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
    grandfather_position.z_ -= (
        sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
        cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;

    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
        grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
        if (grandfather_rotation.y_ < -D3DX_PI){
            grandfather_rotation.y_ += D3DX_PI * 2.0f;
        }
    }
    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
        grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
        if (grandfather_rotation.y_ > D3DX_PI){
            grandfather_rotation.y_ -= D3DX_PI * 2.0f;
        }
    }
    if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_7)){
        // ワープエフェクト再生（移動前）
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("smoke");
        effect_param = effect->parameter();
        effect_param.position_ = grandfather_position;
        effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke");

        switch (stage_){
        case 1:
            grandfather_position = GRANDFATHER_POSITION_STAGE1;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE1;
            break;
        case 2:
            grandfather_position = GRANDFATHER_POSITION_STAGE2;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE2;
            break;
        case 3:
            grandfather_position = GRANDFATHER_POSITION_STAGE3;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE3;
            break;
        }
        // ワープエフェクト再生（移動後）
        effect_param.position_ = grandfather_position;
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke");
    }

    //-------------------------------------
    // デバッグ時のプレイヤー操作
    //-------------------------------------
#ifdef _DEBUG
    if (KeyBoard::isPress(DIK_Z)){
        player_speed = CHARANCTER_MOVESPEED * 2.0f;
        // ダッシュエフェクト
        if (dash_effect_timer_ % 10 == 0){
            EFFECT_PARAMETER_DESC effect_param;
            MyEffect *effect = effect_manager_->Get("dash");
            effect_param = effect->parameter();
            effect_param.position_ = grandfather_position;
            effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
            effect->SetParameter(effect_param);
            effect_manager_->Play("dash");
        }
        dash_effect_timer_++;
    }
    else{ dash_effect_timer_ = 0; }
    if (KeyBoard::isPress(DIK_W)){
        grandfather_position.x_ += sinf(grandfather_rotation.y_) * player_speed;
        grandfather_position.z_ += cosf(grandfather_rotation.y_) * player_speed;
    }
    if (KeyBoard::isPress(DIK_A)){
        grandfather_position.x_ += sinf(grandfather_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
        grandfather_position.z_ += cosf(grandfather_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
    }
    if (KeyBoard::isPress(DIK_S)){
        grandfather_position.x_ += sinf(grandfather_rotation.y_ + (D3DX_PI)) * player_speed;
        grandfather_position.z_ += cosf(grandfather_rotation.y_ + (D3DX_PI)) * player_speed;
    }
    if (KeyBoard::isPress(DIK_D)){
        grandfather_position.x_ += sinf(grandfather_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
        grandfather_position.z_ += cosf(grandfather_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
    }
    if (KeyBoard::isPress(DIK_RIGHT)){
        grandfather_rotation.y_ += CHAR_ROT_SPEED;
        if (grandfather_rotation.y_ > D3DX_PI){
            grandfather_rotation.y_ -= D3DX_PI * 2.0f;
        }
		if (grandfather_rotation.y_ < -D3DX_PI){
			grandfather_rotation.y_ += D3DX_PI * 2.0f;
		}
    }
    if (KeyBoard::isPress(DIK_LEFT)){
        grandfather_rotation.y_ -= CHAR_ROT_SPEED;
		if (grandfather_rotation.y_ > D3DX_PI){
			grandfather_rotation.y_ -= D3DX_PI * 2.0f;
		}
		if (grandfather_rotation.y_ < -D3DX_PI){
            grandfather_rotation.y_ += D3DX_PI * 2.0f;
        }
    }
    if (KeyBoard::isTrigger(DIK_3)){
        // ワープエフェクト再生（移動前）
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("smoke");
        effect_param = effect->parameter();
        effect_param.position_ = grandfather_position;
        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke");

        switch (stage_){
        case 1:
            grandfather_position = GRANDFATHER_POSITION_STAGE1;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE1;
            break;
        case 2:
            grandfather_position = GRANDFATHER_POSITION_STAGE2;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE2;
            break;
        case 3:
            grandfather_position = GRANDFATHER_POSITION_STAGE3;
            grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE3;
            break;
        }
        // ワープエフェクト再生（移動後）
        effect_param.position_ = grandfather_position;
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke");
    }

#endif //_DEBUG


    //-------------------------------------
    // 砦の座標管理
    //-------------------------------------
    switch (stage_)
    {
    case 1:
        fort_underground.x += 0.01f;
        fort_underground.y -= 0.01f;
        fort_underground.z -= 0.01f;
        fort_underground.x = std::min<float>(fort_underground.x, 0.0f);
        fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
        fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
		if (fort_underground.x == 0.0f ) change_stage_flg_ = false;	// ステージ移行フラグ圧し折る
        break;
    case 2:
        fort_underground.x -= 0.01f;
        if (fort_underground.x < -3.0f)fort_underground.y += 0.01f;
        fort_underground.z -= 0.01f;
        fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
        fort_underground.y = std::min<float>(fort_underground.y, 0.0f);
        fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
		if (fort_underground.y == 0.0f) change_stage_flg_ = false;	// ステージ移行フラグ圧し折る
        break;
    case 3:
        fort_underground.x -= 0.01f;
        fort_underground.y -= 0.01f;
        if (fort_underground.y < -3.0f)fort_underground.z += 0.01f;
        fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
        fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
        fort_underground.z = std::min<float>(fort_underground.z, 0.0f);
		if (fort_underground.z == 0.0f) change_stage_flg_ = false;	// ステージ移行フラグ圧し折る
        break;
    }

    D3DXVECTOR3 fort1_pos(
        fort1_position.x_,
        fort1_position.y_,
        fort1_position.z_);
    fort1_position.y_ = field->GetHeight(fort1_pos) + fort_underground.x;

    D3DXVECTOR3 fort2_pos(
        fort2_position.x_,
        fort2_position.y_,
        fort2_position.z_);
    fort2_position.y_ = field->GetHeight(fort2_pos) + fort_underground.y;

    D3DXVECTOR3 fort3_pos(
        fort3_position.x_,
        fort3_position.y_,
        fort3_position.z_);
    fort3_position.y_ = field->GetHeight(fort3_pos) + fort_underground.z;

    D3DXVECTOR3 grandfather_pos(
        grandfather_position.x_,
        grandfather_position.y_,
        grandfather_position.z_);
    grandfather_position.y_ = field->GetHeight(grandfather_pos);
    if (grandfather_position.y_ > 0.4f ||
        grandfather_position.y_ < -0.4f){
        grandfather_position = grandfather_prevposition;
    }

    D3DXVECTOR3 child_pos(
        child_position.x_,
        child_position.y_,
        child_position.z_);
    child_position.y_ = field->GetHeight(child_pos);

    fort1_object->SetPosition(fort1_position);
    fort2_object->SetPosition(fort2_position);
    fort3_object->SetPosition(fort3_position);
    grandfather_object->SetPosition(grandfather_position);
    grandfather_object->SetRotation(grandfather_rotation);
    child_object->SetPosition(child_position);


    //-------------------------------------
    // 砦にエフェクトだす
    //-------------------------------------
    if (fort_underground.x != 0.0f && fort_underground.x != -3.0f){
		// 土煙発生
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("smoke2");
        effect_param = effect->parameter();
        effect_param.position_ =
        { fort1_pos.x, field->GetHeight(fort1_pos), fort1_pos.z };
        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke2");

		// シールドはる
		if (fort_underground.x > -1.0f && shield_flg_ == false && stage_ == 1){
			EFFECT_PARAMETER_DESC shield_param;
			MyEffect *effect_shield = effect_manager_->Get("shieldin");
			shield_param = effect_shield->parameter();
			shield_param.position_ = FORT1_POSITION;
			shield_param.position_.y_ = SHIELD_POSITION_Y;
			shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
			effect_shield->SetParameter(shield_param);
			effect_manager_->Play("shieldin");

			// シールドフラグ建てる
			shield_flg_ = true;
		}

    }

    if (fort_underground.y != 0.0f && fort_underground.y != -3.0f){
		// 土煙発生
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("smoke2");
        effect_param = effect->parameter();
        effect_param.position_ =
        { fort2_pos.x, field->GetHeight(fort2_pos), fort2_pos.z };
        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke2");

		// シールドはる
		if (fort_underground.y > -1.0f && shield_flg_ == false && stage_ == 2){
			EFFECT_PARAMETER_DESC shield_param;
			MyEffect *effect_shield = effect_manager_->Get("shieldin");
			shield_param = effect_shield->parameter();
			shield_param.position_ = FORT2_POSITION;
			shield_param.position_.y_ = SHIELD_POSITION_Y;
			shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
			effect_shield->SetParameter(shield_param);
			effect_manager_->Play("shieldin");

			// シールドフラグ建てる
			shield_flg_ = true;
		}
    }

    if (fort_underground.z != 0.0f && fort_underground.z != -3.0f){
		// 土煙発生
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("smoke2");
        effect_param = effect->parameter();
        effect_param.position_ =
        { fort3_pos.x, field->GetHeight(fort3_pos), fort3_pos.z };
        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
        effect->SetParameter(effect_param);
        effect_manager_->Play("smoke2");

		// シールドはる
		if (fort_underground.z > -1.0f && shield_flg_ == false && stage_ == 3){
			EFFECT_PARAMETER_DESC shield_param;
			MyEffect *effect_shield = effect_manager_->Get("shieldin");
			shield_param = effect_shield->parameter();
			shield_param.position_ = FORT3_POSITION;
			shield_param.position_.y_ = SHIELD_POSITION_Y;
			shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
			effect_shield->SetParameter(shield_param);
			effect_manager_->Play("shieldin");

			// シールドフラグ建てる
			shield_flg_ = true;
		}
    }

    //-------------------------------------
    // カメラ追従
    //-------------------------------------
    Camera *main_camera = camera_manager_->Get("MainCamera");
    D3DXVECTOR3 camera_position, camera_focus;
    D3DXVECTOR3 camera_rotation(main_camera->rotation());




    // 入力
#ifdef _DEBUG
    if (KeyBoard::isPress(DIK_UP)){
        camera_rotation.x -= CAMERA_ROT_SPEED;
        if (camera_rotation.x < -CAMERA_ROT_X_LIMIT){
            camera_rotation.x = -CAMERA_ROT_X_LIMIT;
        }
    }
    if (KeyBoard::isPress(DIK_DOWN)){
        camera_rotation.x += CAMERA_ROT_SPEED;
        if (camera_rotation.x > CAMERA_ROT_X_LIMIT){
            camera_rotation.x = CAMERA_ROT_X_LIMIT;
        }
    }
#endif

    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_UP)){
        camera_rotation.x += CAMERA_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsy_;
        if (camera_rotation.x < -CAMERA_ROT_X_LIMIT){
            camera_rotation.x = -CAMERA_ROT_X_LIMIT;
        }
    }
    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_DOWN)){
        camera_rotation.x += CAMERA_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsy_;
        if (camera_rotation.x > CAMERA_ROT_X_LIMIT){
            camera_rotation.x = CAMERA_ROT_X_LIMIT;
        }
    }


    //-------------------------------------
    // カメラ計算
    //-------------------------------------

    // モデルの回転Yをそのままカメラの回転Yへ
    camera_rotation.y = grandfather_rotation.y_;
    // 一旦モデルを注視点に
    camera_focus = grandfather_pos;
    // 足元基準から体の中心辺りを基準に
    camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
    // モデルの少し先を見るように調整
    camera_focus.x +=
        sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
    camera_focus.z +=
        cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
    camera_focus.y +=
        sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

    // 注視点を基準にカメラ座標を設定
    camera_position = camera_focus;
    camera_position.x -=
        sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
    camera_position.z -=
        cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
    camera_position.y -=
        sinf(camera_rotation.x) * camera_pos_len_;


    // カメラの地面めり込み回避処理
    D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

    // 中間にカメラがめり込みそうなところが無いか検査
    bool camera_re_calculate = false;
    for (int i = 0; i < 10; ++i){
        // 中間地点を計算
        D3DXVECTOR3 lay_point =
            camera_position + vec_camera_to_focus * static_cast<float>(i)* 0.1f;
        float pos_y = field->GetHeight(lay_point);
        // 回避処理
        if (lay_point.y < pos_y + 0.1f){
            camera_re_calculate = true;
            camera_pos_len_ -= CAMARA_LEN_SPEED;
        }
    }

    //カメラ座標再計算
    if (camera_re_calculate == true){
        camera_position = camera_focus;
        camera_position.x -=
            sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
        camera_position.z -=
            cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
        camera_position.y -=
            sinf(camera_rotation.x) * camera_pos_len_;
        camera_position.y = field->GetHeight(camera_position) + 0.1f;
    }

    camera_pos_len_ += CAMARA_LEN_SPEED;
    if (camera_pos_len_ > CAMERA_POS_LEN){
        camera_pos_len_ = CAMERA_POS_LEN;
    }

    // カメラにパラメータを再セット
    main_camera->SetPosition(camera_position);
    main_camera->SetFocus(camera_focus);
    main_camera->SetRotation(camera_rotation);


    //-------------------------------------
    // サブカメラ計算
    //-------------------------------------
    Camera *sub_camera = camera_manager_->Get("SubCamera");
    D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
    sub_camera_rotation = sub_camera->rotation();
    sub_camera_position = sub_camera->position();
    sub_camera_focus = sub_camera->focus();

    // 適当な位置から眺める
    // 一旦砦を注視点に
    switch (stage_)
    {
    case 1:
        sub_camera_focus = fort1_pos;
        sub_camera_focus.y = 1.5f;
        if (fort_underground.x != 0.0f){
            use_camera_name_ = "SubCamera";
            sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
            sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
        }
        else{
            use_camera_name_ = "MainCamera";
            sub_camera_rotation.y = 0.0f;
            sub_camera_rotation.x = 0.0f;
        }
        break;
    case 2:
        if (fort_underground.x != -3.0f)
        {

            sub_camera_focus = fort1_pos;
            sub_camera_focus.y = 1.2f;
            use_camera_name_ = "SubCamera";
            //sub_camera_rotation.y = fort1_object->parameter().rotation_.y_;
            sub_camera_rotation.x -= CAMERA_SUB_ROT_SPEED*0.02f;
        }
        else if (fort_underground.y != 0.0f){
            sub_camera_focus = fort2_pos;
            sub_camera_focus.y = 1.5f;
            use_camera_name_ = "SubCamera";
            //sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
            sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
        }
        else{
            use_camera_name_ = "MainCamera";
            sub_camera_rotation.y = 0.0f;
            sub_camera_rotation.x = 0.0f;
        }
        break;
    case 3:
        if (fort_underground.y != -3.0f)
        {
            sub_camera_focus = fort2_pos;
            sub_camera_focus.y = 1.2f;
            use_camera_name_ = "SubCamera";
            //sub_camera_rotation.y = fort2_object->parameter().rotation_.y_;
            sub_camera_rotation.x -= CAMERA_SUB_ROT_SPEED*0.02f;
        }
        else if (fort_underground.z != 0.0f){
            sub_camera_focus = fort3_pos;
            sub_camera_focus.y = 1.5f;
            use_camera_name_ = "SubCamera";
            //sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
            sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
        }
        else{
            use_camera_name_ = "MainCamera";
            sub_camera_rotation.y = 0.0f;
            sub_camera_rotation.x = 0.0f;
        }
        break;
    }

    // 注視点を基準にカメラ座標を設定
    sub_camera_position = sub_camera_focus;
    sub_camera_position.x -=
        sinf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
    sub_camera_position.z -=
        cosf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
    sub_camera_position.y -=
        sinf(sub_camera_rotation.x) * camera_pos_len_;


    // カメラにパラメータを再セット
    sub_camera->SetPosition(sub_camera_position);
    sub_camera->SetFocus(sub_camera_focus);
    sub_camera->SetRotation(sub_camera_rotation);

    //-------------------------------------
    // 弾発射
    //-------------------------------------
    shot_late--;
    shot_late = std::max<int>(shot_late, 0);
    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_8) &&
        father_watergauge > 0 &&
        shot_late == 0){
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("water");
        effect_param = effect->parameter();
        effect_param.position_ = grandfather_position;
        effect_param.position_.y_ += 0.6f;
        effect_param.rotation_ = grandfather_rotation;
        effect->SetParameter(effect_param);
        effect_manager_->Play("water");

		for (int i = 0; i < 3; i++){
			OBJECT_PARAMETER_DESC bullet_param;
			bullet_param.layer_ = LAYER_BULLET;
			bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
			bullet_param.position_ = grandfather_position;
			bullet_param.position_.y_ += 0.6f;
			bullet_param.rotation_ = grandfather_rotation;

			// カメラの回転Xを利用
			bullet_param.rotation_.x_ = camera_rotation.x;
			float range = MyMath::Random_Range(-5, 5) * 0.01f;
			bullet_param.rotation_.y_ += range;

			bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };

			Bullet* bullet = object_manager_->GetNoUseBullet();
			bullet->Fire(bullet_param);
		}

        //-------------------------------------
        // おじの弾発射SE再生
        Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");

        //-------------------------------------
        // 水ゲージを減少させる
        //-------------------------------------
		father_watergauge -= GRANDFATHER_SUB_BULLET_WATERGAUGE;
        father_watergauge = std::max<float>(father_watergauge, 0.0f);
        grandfather->SetWaterGauge(father_watergauge);
        waterGage->SetChangeValue(father_watergauge);

        shot_late = 10;
    }

	//-------------------------------------
	// ボム発射
	//-------------------------------------
	if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_6)){
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = grandfather_position;
		effect_param.position_.y_ += 0.6f;
		effect_param.rotation_ = grandfather_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");

		OBJECT_PARAMETER_DESC bomb_param;
		bomb_param.layer_ = LAYER_BOMB;
		bomb_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bomb_param.position_ = grandfather_position;
		bomb_param.position_.y_ += 0.6f;
		bomb_param.rotation_ = grandfather_rotation;

		// カメラの回転Xを利用
		bomb_param.rotation_.x_ = camera_rotation.x;
		float range = MyMath::Random_Range(-5, 5) * 0.01f;
		bomb_param.rotation_.y_ += range;

		bomb_param.scaling_ = { 0.35f, 0.35f, 0.35f };

		Bomb* bomb = object_manager_->GetNoUseBomb();
		bomb->Fire(bomb_param);

		//-------------------------------------
		// おじの弾発射SE再生
		Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");

		//-------------------------------------
		// 水ゲージを減少させる
		//-------------------------------------
		father_watergauge -= GRANDFATHER_SUB_BOMB_WATERGAUGE;
		father_watergauge = std::max<float>(father_watergauge, 0.0f);
		grandfather->SetWaterGauge(father_watergauge);
		waterGage->SetChangeValue(father_watergauge);
	}


#ifdef _DEBUG
    if (KeyBoard::isPress(DIK_9)){
        grandfather->SetWaterGauge(0.0f);
        waterGage->SetChangeValue(0.0f);
    }
    if (KeyBoard::isPress(DIK_SPACE)){
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("water");
        effect_param = effect->parameter();
        effect_param.position_ = grandfather_position;
        effect_param.position_.y_ += 0.6f;
        effect_param.rotation_ = grandfather_rotation;
        effect->SetParameter(effect_param);
        effect_manager_->Play("water");

        OBJECT_PARAMETER_DESC bullet_param;
        bullet_param.layer_ = LAYER_BULLET;
        bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
        bullet_param.position_ = grandfather_position;
        bullet_param.position_.y_ += 0.6f;
        bullet_param.rotation_ = grandfather_rotation;
        bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };

        // カメラの回転Xを利用
        bullet_param.rotation_.x_ = camera_rotation.x;

		Bullet* bullet = object_manager_->GetNoUseBullet();
		bullet->Fire(bullet_param);
        //-------------------------------------
        // おじの弾発射SE再生
        Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
    }

    //-------------------------------------
    // デバッグ時のみ、水ゲージ回復
    //-------------------------------------
    if (KeyBoard::isPress(DIK_1)){
		father_watergauge += GRANDFATHER_SUB_BULLET_WATERGAUGE;
        father_watergauge = std::min<float>(father_watergauge, 1.0f);
        grandfather->SetWaterGauge(father_watergauge);
        waterGage->SetChangeValue(father_watergauge);
    }
    //-------------------------------------
    // カメラ切り替えテスト
    //-------------------------------------
    if (KeyBoard::isPress(DIK_4)){
        use_camera_name_ = "MainCamera";
    }
    if (KeyBoard::isPress(DIK_5)){
        use_camera_name_ = "SubCamera";
    }

#endif //_DEBUG

#ifdef _DEBUG
    //-------------------------------------
    // ダメージエフェクト確認
    //-------------------------------------
    if (KeyBoard::isPress(DIK_L)){
        float life = child->GetLife();
        life -= CHILD_DAMAGE;
        if (life < 0.0f){
            life = 0.0f;
        }
        child->SetLife(life);
    }
    else if (KeyBoard::isPress(DIK_K)){
        float life = child->GetLife();
        life += CHILD_DAMAGE;
        if (life > 1.0f){
            life = 1.0f;
        }
        child->SetLife(life);
    }
#endif // _DEBUG

    //-------------------------------------
    // アニメーション制御
    //-------------------------------------
    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) ||
        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) ||
        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) ||
        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
        if (grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
        {
            grandfather->PlayAnimation(FbxGrandfather::WALK);
            walk_flg_ = true;
        }
    }
    else{
        if (grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
        {
            grandfather->PlayAnimation(FbxGrandfather::IDLE);
            walk_flg_ = false;
        }
    }


    //-------------------------------------
    // 移動SE確認用
    //-------------------------------------
    if (walk_flg_)
    {
        ++walk_timer_;
        if (!(walk_timer_ % 30))
        {
            Sound::LoadAndPlaySE("resource/sound/se/game/footstep.wav");
            walk_timer_ = 0;
        }
    }
    else
    {
        walk_timer_ = 30;
    }

    //-------------------------------------
    // 子供死亡時制御
    //-------------------------------------
    if (child_life < 0 && !child_death_){
        child->PlayAnimation(FbxChild::DOWN);
        child_death_ = true;
        child_respawn_waittime_ = CHILD_RESPAWN_WAITTIME;
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("dead");
        effect_param = effect->parameter();
        effect_param.position_ = child_position;
        effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
        effect->SetParameter(effect_param);
        effect_manager_->Play("dead");

    }
    else if (child_death_ && !child_respawn_waittime_){
        child->PlayAnimation(FbxChild::IDLE);
        child_death_ = false;
        child_life = CHILD_LIFE;
        child->SetLife(child_life);
		child_position = CHILD1_POSITION1;
		child_rotation = CHILD1_ROTATION1;
        child->SetPosition(child_position);
        child->SetRotation(child_rotation);
    }

    child_respawn_waittime_--;
    child_respawn_waittime_ = std::max<int>(child_respawn_waittime_, 0);

    //-------------------------------------
    // 子供体力自動回復制御
    //-------------------------------------
    if (child_life < 1.0f && !child_death_){
        int child_recover_wait_timer = child->GetRecoverWaitTimer();

        if (child_recover_wait_timer > CHILD_RECOVER_WAITE_TIME){
            float child_life = child->GetLife();
            child_life += CHILD_RECOVER_HP;
            std::min<float>(child_life, 1.0f);
            child->SetLife(child_life);
        }
        child_recover_wait_timer++;
        child->SetRecoverWaitTimer(child_recover_wait_timer);
    }

    //-------------------------------------
    // ダメージエフェクトの処理
    //-------------------------------------
    // 今はテスト用に、子供に当てたら主観(おじ)のUIを反映させている
    damage_effect->SetHP(child_life);


    //-------------------------------------
    // 各キャラクタ座標保存
    //-------------------------------------
    grandfather_prevposition = grandfather_position;


    //-------------------------------------
    // 影座標
    //-------------------------------------
    Object *shadow = object_manager_->Get("shadow");
    Vector3 shadow_pos;
    shadow_pos = grandfather->parameter().position_;
    shadow_pos.y_ += 0.001f;
    shadow->SetPosition(shadow_pos);


    //-------------------------------------
    // まーかー表示
    //-------------------------------------
    Vector3 poseffect = grandfather_position;
    poseffect.y_ += 0.6f;
    Vector3 speed = { BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ };
    Vector3 rot = Vector3(camera_rotation.x, camera_rotation.y, camera_rotation.z);
    // 回転値を少し調整
    rot.x_ += BULLET_OFFSET_ROT;
    // 回転値を参照して速度を改良
    speed.y_ += sinf(rot.x_) * BULLET_ADD_SPEED_Y;
    // 判定が取れたか
    bool hit_point_field = false;

    for (int i = 0; i < 120; i++){
        poseffect.x_ += sinf(rot.y_) * speed.x_;
        poseffect.y_ += speed.y_;
        poseffect.z_ += cosf(rot.y_) * speed.z_;
        speed.y_ -= BULLET_GRAVITY;

        float height = field->GetHeight(D3DXVECTOR3(poseffect.x_, poseffect.y_, poseffect.z_));
        if (height > poseffect.y_){
            EFFECT_PARAMETER_DESC effect_param;
            MyEffect *effect = effect_manager_->Get("marker");
            effect_param = effect->parameter();
            effect_param.position_ = poseffect;
            effect_param.position_.y_ = height;
            effect_param.rotation_ = { 0.0f, camera_rotation.y, 0.0f };
            Vector3 vec = poseffect - grandfather_position;
            float len = sqrt(vec.x_*vec.x_ + vec.z_ * vec.z_);
            float scaling = 1.0f + len * 0.15f;

            scaling = std::min<float>(scaling, 3.0f);

            effect_param.scaling_ = { scaling, scaling, scaling };
            effect->SetParameter(effect_param);
            hit_point_field = true;
            break;
        }
    }

    // 判定外
    if (!hit_point_field){
        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("marker");
        effect_param = effect->parameter();
        effect_param.position_.y_ = -100.0f;
        effect->SetParameter(effect_param);
    }

    //-------------------------------------
    // おじのデバフパワーをセット
    //-------------------------------------
    //-------------------------------------
    // おじのデバフなので
    // ここでは子供のライフでやってるから
    // おじに直しておいて
    //-------------------------------------
    if (child_life < 0.5f){
        float debuff_power = child_life - 0.75f;
        debuff_power = abs(debuff_power * 0.5f);
        grandfather->SetDebuffPower(debuff_power);

        EFFECT_PARAMETER_DESC effect_param;
        MyEffect *effect = effect_manager_->Get("speeddown");
        effect_param = effect->parameter();
        effect_param.position_ = grandfather_position;
        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
        effect->SetParameter(effect_param);
        if (debuff_effect_flg_){
            effect_manager_->Play("speeddown");
            debuff_effect_flg_ = false;
        }
    }
    else{
		grandfather->SetDebuffPower(0.0f);
        effect_manager_->Stop("speeddown");
        debuff_effect_flg_ = true;
    }

	//-------------------------------------
	// シールドのON,OFF設定 ステージ移行中は無効
	//-------------------------------------
	//-------------------------------------
	// デバッグ用に子供のライフでやってるので
	// おじのライフに直す
	//-------------------------------------
	if (child_life < SHIELD_SWITCH_LIFE && shield_flg_ == true && change_stage_flg_ == false){

		// シールドを消すと同時に
		// シールド解除エフェクト発生
		EFFECT_PARAMETER_DESC shieldout_param;
		MyEffect *effect_shieldout = effect_manager_->Get("shieldout");
		MyEffect *effect_shieldin = effect_manager_->Get("shieldin");
		shieldout_param = effect_shieldout->parameter();
		shieldout_param.position_ = effect_shieldin->parameter().position_;
		shieldout_param.rotation_ = { 0.0f, 0.0f, 0.0f };
		effect_shieldout->SetParameter(shieldout_param);
		effect_manager_->Stop("shieldin");
		effect_manager_->Play("shieldout");

		shield_flg_ = false;
	}
	else if (child_life > SHIELD_SWITCH_LIFE && shield_flg_ == false && change_stage_flg_ == false){
		// シールドはる
		Vector3 fort_position = FORT1_POSITION;

		if (stage_ == 2){ fort_position = FORT2_POSITION; }
		else if (stage_ == 3){ fort_position = FORT3_POSITION; }

		EFFECT_PARAMETER_DESC shield_param;
		MyEffect *effect_shield = effect_manager_->Get("shieldin");
		shield_param = effect_shield->parameter();
		shield_param.position_ = fort_position;
		shield_param.position_.y_ = SHIELD_POSITION_Y;
		shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
		effect_shield->SetParameter(shield_param);
		effect_manager_->Play("shieldin");

		// シールドフラグ建てる
		shield_flg_ = true;
	}



	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	// オブジェクトの更新の前にカメラの更新した行列適応させておく
	camera_manager_->Set(use_camera_name_);
	object_manager_->Update();
	effect_manager_->Update();
	collision_manager_->Update();

	font1_->Add("シーン名:");
	font1_->Add("Game\n");
	font1_->Add("FPS : %d\n", Fps::GetFps());
	font1_->Add("STAGE : %d\n", stage_);
	font1_->Add("LIFE(GrandFather) : %3.2f\n", father_life);
	font1_->Add("LIFE(Child)       : %3.2f\n", child_life);
	font1_->Add("GAUGE(GrandFather) : %3.2f\n", father_watergauge);
	font1_->Add("GAUGE(Child)       : %3.2f\n", child_watergauge);
	font1_->Add("POSITION(Grandfather) : %3.2f %3.2f %3.2f\n",
		grandfather_position.x_,
		grandfather_position.y_,
		grandfather_position.z_);
	font1_->Add("ROTATION(Grandfather) : %3.2f %3.2f %3.2f\n",
		grandfather_rotation.x_,
		grandfather_rotation.y_,
		grandfather_rotation.z_);
	font1_->Add("OBJECT : %d\n", ObjectManager::GetCount());
	font1_->Add("EFFECT : %d\n", EffectManager::GetCount());
	font1_->Add("COLLISION : %d\n", CollisionManager::GetCount());
	font1_->Add("MESSAGE_25 : %3.2f\n", message_object_25->parameter().position_.y_);
	
	font2_->Add("----------操作説明----------\n");
	font2_->Add("【ゲームパッド使用時】\n");
	font2_->Add("左スティック：移動\n");
	font2_->Add("右スティック：エイム移動\n");
	font2_->Add("8ボタン：射撃\n");
	font2_->Add("7ボタン：拠点に戻る ※おじいちゃんのみ\n");
	font2_->Add("【キーボード使用時】\n");
	font2_->Add("WASDキー：移動\n");
	font2_->Add("方向キー：エイム移動\n");
	font2_->Add("SPACEキー：射撃\n");


	//-------------------------------------
	// デバッグ出力
	//-------------------------------------
	if (KeyBoard::isTrigger(DIK_F1)){
		object_manager_->ExportObjectParameter(
			"resource/object_patameter.txt");
	}

	if (KeyBoard::isTrigger(DIK_F2)){
		FILE *file = fopen("DebugParam.txt", "a");
		fprintf(file, "\n{ %3.2ff, %3.2ff, %3.2ff },\n",
			grandfather_position.x_,
			grandfather_position.y_,
			grandfather_position.z_);
		fprintf(file, "{ %3.2ff, %3.2ff, %3.2ff },\n",
			grandfather_rotation.x_,
			grandfather_rotation.y_,
			grandfather_rotation.z_);
		fclose(file);

	}

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
		effect_manager_->StopAll();
		SceneManager::RequestScene("Result");
	}
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
    DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(
                                object_manager_->Get("damage_effect"));
	camera_manager_->Set(use_camera_name_);
	object_manager_->Draw();
	effect_manager_->Draw();
    damage_effect->Draw();
	collision_manager_->Draw();
	font1_->Draw(rect1, font1_color);
	font2_->Draw(rect2, font2_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
