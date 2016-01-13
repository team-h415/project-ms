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
#include "../../math/vector.h"
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
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = CameraManager::Get();
	object_manager_ = ObjectManager::Get();
	effect_manager_ = EffectManager::Get();
    object_manager_->SetDrawEnable(LAYER_DAMAGE_EFFECT, false);
}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	object_manager_->Get("time")->SetThisDelete(true);
	object_manager_->Get("fort_state")->SetThisDelete(true);
	object_manager_->Get("fort_gauge_manager")->SetThisDelete(true);
	object_manager_->Get("water_poly")->SetThisDelete(true);
	object_manager_->Get("water_gage_around")->SetThisDelete(true);
	object_manager_->Get("water_design")->SetThisDelete(true);
	object_manager_->Get("water_gage")->SetThisDelete(true);
	object_manager_->Get("damage_effect")->SetThisDelete(true);
	object_manager_->Get("countdown")->SetThisDelete(true);
	object_manager_->Get("message_child1_death")->SetThisDelete(true);
	object_manager_->Get("message_child2_death")->SetThisDelete(true);
	object_manager_->Get("message_child3_death")->SetThisDelete(true);
	object_manager_->Get("message_child4_death")->SetThisDelete(true);
	object_manager_->Get("message_fort_25")->SetThisDelete(true);
	object_manager_->Get("message_fort_50")->SetThisDelete(true);
	object_manager_->Get("message_fort_75")->SetThisDelete(true);
	object_manager_->Get("message_fort_100")->SetThisDelete(true);
	object_manager_->Get("message_grandfather_debuff")->SetThisDelete(true);
	object_manager_->Get("message_grandfather_return")->SetThisDelete(true);
	object_manager_->Get("result_sprite")->SetThisDelete(true);
	object_manager_->Get("arrow")->SetThisDelete(true);

	object_manager_->DeleteCheck();
	object_manager_->ObjectUseOffLayer(LAYER_BULLET);
	object_manager_->ObjectUseOffLayer(LAYER_BOMB);
	object_manager_->ObjectUseOffLayer(LAYER_BLIND);

	effect_manager_->StopAll();
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Game::Initialize()
{
	//-------------------------------------
//<<<<<<< HEAD
//=======
//	// エフェクトの読み込み
//	//-------------------------------------
//	EFFECT_PARAMETER_DESC effect_param;
//	effect_param.position_ = { 0.0f, 0.0f, 0.0f };
//	effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	effect_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//	effect_param.speed_ = 1.0f;
//
//	effect_manager_->Create(
//		"water",
//		"resource/effect/BulletFire.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"damage",
//		"resource/effect/Damage3_3x0.5.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"dead",
//		"resource/effect/Dead2_2.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"smoke",
//		"resource/effect/Smoke.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"smoke2",
//		"resource/effect/Smoke2.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"dash",
//		"resource/effect/Dash2.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"watersupply",
//		"resource/effect/WaterSupply.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"watersupplybubble",
//		"resource/effect/WaterSupply2.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"speeddown",
//		"resource/effect/SpeedDown_3.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"bombfire",
//		"resource/effect/BombFire.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"shieldin",
//		"resource/effect/ShieldIn_2x2.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"shieldout",
//		"resource/effect/ShieldOut2x2.efk",
//		effect_param);
//
//	//-------------------------------------
//	// メインカメラ
//	//-------------------------------------
//	CAMERA_PARAMETER_DESC camera_param;
//	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
//	camera_param.fovy_ = D3DX_PI * 0.25f;
//	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
//	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
//	camera_param.near_ = 0.1f;
//	camera_param.far_ = 800.0f;
//	camera_pos_len_ = CAMERA_POS_LEN;
//
//	camera_manager_->Create(
//		"Perspective", "MainCamera", camera_param);
//
//	//-------------------------------------
//	// サブカメラ
//	//-------------------------------------
//	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
//	camera_param.fovy_ = D3DX_PI * 0.25f;
//	camera_param.position_ = { 0.0f, 0.0f, -10.0f };
//	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
//	camera_param.near_ = 0.1f;
//	camera_param.far_ = 800.0f;
//	camera_pos_len_ = CAMERA_POS_LEN;
//
//	camera_manager_->Create(
//		"Perspective", "SubCamera", camera_param);
//
//
//	//-------------------------------------
//	// 地形
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC field_param;
//	field_param.name_ = "field";
//	field_param.position_ = { 0.0f, 0.0f, 0.0f };
//	field_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	field_param.scaling_ = { 100.0f, 1.0f, 200.0f };
//	field_param.layer_ = LAYER_MESH_FIELD;
//
//	object_manager_->Create(
//		field_param,
//		"resource/mesh/map.heightmap");
//
//
//	//-------------------------------------
//	// 空
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC skydome_param;
//	skydome_param.name_ = "skydome_up";
//	skydome_param.position_ = { 0.0f, 0.0f, 0.0f };
//	skydome_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	skydome_param.scaling_ = { 500.0f, 1.0f, 500.0f };
//	skydome_param.layer_ = LAYER_MESH_SKYDOME;
//
//	object_manager_->Create(
//		skydome_param,
//		"resource/mesh/skydome_up.txt");
//
//	skydome_param.name_ = "skydome_bottom";
//	skydome_param.rotation_ = { 0.0f, 0.0f, D3DX_PI };
//
//	object_manager_->Create(
//		skydome_param,
//		"resource/mesh/skydome_bottom.txt");
//
//
//	//-------------------------------------
//	// 池
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC lake_param;
//	lake_param.name_ = "lake";
//	lake_param.position_ = { 0.0f, -0.8f, 0.0f };
//	lake_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	lake_param.scaling_ = { 300.0f, 1.0f, 300.0f };
//	lake_param.layer_ = LAYER_SPRITE_LAKE;
//
//	object_manager_->Create(
//		lake_param);
//
//
//	//-------------------------------------
//	// 砦
//	//-------------------------------------
//	// 砦1
//	OBJECT_PARAMETER_DESC fort1_param;
//	fort1_param.name_ = "fort1";
//	fort1_param.layer_ = LAYER_MODEL_FORT;
//	fort1_param.position_ = FORT1_POSITION;
//	fort1_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	fort1_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//	object_manager_->Create(
//		fort1_param,
//		"resource/model/x/fort.x");
//
//	COLLISION_PARAMETER_DESC fort1_collision_param;
//	Object *fort1_obj = object_manager_->Get("fort1");
//	fort1_collision_param.position_ = {
//		fort1_obj->parameter().position_.x_,
//		fort1_obj->parameter().position_.y_ + 0.5f,
//		fort1_obj->parameter().position_.z_ };
//	fort1_collision_param.range_ = 1.0f;
//	fort1_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };
//
//	collision_manager_->Create(fort1_obj,
//		fort1_collision_param);
//	XFort *fort1 = dynamic_cast<XFort*>(fort1_obj);
//	fort1->SetLife(FORT1_LiFE);
//
//
//	// 砦2
//	OBJECT_PARAMETER_DESC fort2_param;
//	fort2_param.name_ = "fort2";
//	fort2_param.layer_ = LAYER_MODEL_FORT;
//	fort2_param.position_ = FORT2_POSITION;
//	fort2_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	fort2_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//	object_manager_->Create(
//		fort2_param,
//		"resource/model/x/fort.x");
//
//	COLLISION_PARAMETER_DESC fort2_collision_param;
//	Object *fort2_obj = object_manager_->Get("fort2");
//	fort2_collision_param.position_ = {
//		fort2_obj->parameter().position_.x_,
//		fort2_obj->parameter().position_.y_ + 0.5f,
//		fort2_obj->parameter().position_.z_ };
//	fort2_collision_param.range_ = 1.0f;
//	fort2_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };
//
//	collision_manager_->Create(fort2_obj,
//		fort2_collision_param);
//	XFort *fort2 = dynamic_cast<XFort*>(fort2_obj);
//	fort2->SetLife(FORT2_LiFE);
//
//
//	// 砦3
//	OBJECT_PARAMETER_DESC fort3_param;
//	fort3_param.name_ = "fort3";
//	fort3_param.layer_ = LAYER_MODEL_FORT;
//	fort3_param.position_ = FORT3_POSITION;
//	fort3_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	fort3_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//	object_manager_->Create(
//		fort3_param,
//		"resource/model/x/fort.x");
//
//	COLLISION_PARAMETER_DESC fort3_collision_param;
//	Object *fort3_obj = object_manager_->Get("fort3");
//	fort3_collision_param.position_ = {
//		fort3_obj->parameter().position_.x_,
//		fort3_obj->parameter().position_.y_ + 0.5f,
//		fort3_obj->parameter().position_.z_ };
//	fort3_collision_param.range_ = 1.0f;
//	fort3_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };
//
//	collision_manager_->Create(fort3_obj,
//		fort3_collision_param);
//	XFort *fort3 = dynamic_cast<XFort*>(fort3_obj);
//	fort3->SetLife(FORT3_LiFE);
//
//
//	//-------------------------------------
//	// FBXおじ
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC grandfather_param;
//	grandfather_param.name_ = "grandfather";
//	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
//	grandfather_param.position_ = GRANDFATHER_POSITION_STAGE1;
//	grandfather_param.rotation_ = { 0.0f, GRANDFATHER_ROTATION_STAGE1, 0.0f };
//	grandfather_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//	object_manager_->Create(
//		grandfather_param);
//
//	COLLISION_PARAMETER_DESC fbx_collision_param;
//	Object *obj2 = object_manager_->Get("grandfather");
//	fbx_collision_param.position_ = {
//		obj2->parameter().position_.x_,
//		obj2->parameter().position_.y_,
//		obj2->parameter().position_.z_ };
//	fbx_collision_param.range_ = 1.0f;
//	fbx_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };
//
//	collision_manager_->Create(object_manager_->Get("grandfather"),
//		fbx_collision_param);
//
//	//-------------------------------------
//	// FBX子供
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC child_param;
//	child_param.name_ = "child";
//	child_param.layer_ = LAYER_MODEL_CHILD;
//	child_param.position_ = GRANDFATHER_POSITION_STAGE1;
//	child_param.position_.x_ += 1.0f;
//	child_param.rotation_ = CHILD1_ROTATION1;
//	child_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//	object_manager_->Create(
//		child_param);
//
//	COLLISION_PARAMETER_DESC child_collision_param;
//	Object *obj3 = object_manager_->Get("child");
//	child_collision_param.position_ = {
//		obj3->parameter().position_.x_,
//		obj3->parameter().position_.y_,
//		obj3->parameter().position_.z_ };
//	child_collision_param.range_ = 1.0f;
//	child_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };
//
//	collision_manager_->Create(object_manager_->Get("child"),
//		child_collision_param);
//
//	//-------------------------------------
//	// まーかー
//	//-------------------------------------
//	EFFECT_PARAMETER_DESC marker_param;
//	MyEffect *effect_marker = effect_manager_->Get("marker");
//	marker_param = effect_marker->parameter();
//	marker_param.position_ = GRANDFATHER_POSITION_STAGE1;
//	marker_param.position_.y_ = -100.0f;
//	marker_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	effect_marker->SetParameter(marker_param);
//	effect_manager_->Play("marker");
//
//
//	//-------------------------------------
//>>>>>>> master
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

	timer->SetTexture("resource/texture/number.png");
	timer->SetFigureOffset(-30.0f);
	timer->SetValue(GAME_TIME);
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
		fort_state_param,
		"resource/texture/game/TowerCollapse_Background.png");


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
		"resource/texture/game/TowerCollapse100.png");


	// UI用に自身のIDを取得
	int my_id = NetworkGuest::id();
	my_id = std::max<int>(my_id, 0);
	my_id = std::min<int>(my_id, 4);

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
	water_poly_param.rotation_ = {0.0f, 0.0f, 0.0f};
	water_poly_param.scaling_ = {192.0f, 192.0f, 0.0f};
	water_poly_param.layer_ = LAYER_SPRITE_2D;

	object_manager_->Create(
		water_poly_param,
		"resource/texture/game/water_desine_" + std::to_string(my_id) + ".png");

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
		"resource/texture/game/water_gage_around_" + std::to_string(my_id) + ".png");

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
		"resource/texture/game/water_gage_background_" + std::to_string(my_id) + ".png");

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

//<<<<<<< HEAD
//=======
//	//-------------------------------------
//	// 木
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC wood_param;
//	wood_param.name_ = "wood1";
//	wood_param.layer_ = LAYER_TREE;
//	InstancingTree *tree1 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
//	tree1->SetMesh("resource/model/x/tree01.x");
//	tree1->SetTexture("resource/texture/game/tree01.png");
//	tree1->SetPositionPatern(0);
//
//	wood_param.name_ = "wood2";
//	InstancingTree *tree2 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
//	tree2->SetMesh("resource/model/x/tree02.x");
//	tree2->SetTexture("resource/texture/game/tree02.png");
//	tree2->SetPositionPatern(1);
//
//	wood_param.name_ = "wood3";
//	InstancingTree *tree3 = dynamic_cast<InstancingTree*>(object_manager_->Create(wood_param));
//	tree3->SetMesh("resource/model/x/tree03.x");
//	tree3->SetTexture("resource/texture/game/tree01.png");
//	tree3->SetPositionPatern(2);
//
//	//-------------------------------------
//	// ベンチ
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC bench_param;
//	bench_param.name_ = "bench";
//	bench_param.layer_ = LAYER_BENCH;
//	object_manager_->Create(
//		bench_param);
//
//	//-------------------------------------
//	// 遊具
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC playground_param;
//	playground_param.name_ = "playground";
//	playground_param.layer_ = LAYER_PLAYGROUND;
//	object_manager_->Create(
//		playground_param);
//
//	//-------------------------------------
//	// 影
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC shadow_param;
//	shadow_param.name_ = "shadow";
//	shadow_param.layer_ = LAYER_SHADOW;
//	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
//	object_manager_->Create(
//		shadow_param);
//>>>>>>> master

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
//<<<<<<< HEAD
//=======
//	// バレット生成しておくよ
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC bullet_param;
//	bullet_param.layer_ = LAYER_BULLET;
//	for (int i = 0; i < MAX_BULLET; i++)
//	{
//		bullet_param.name_ = "bullet" + std::to_string(i);
//		object_manager_->Create(
//			bullet_param);
//	}
//
//	//-------------------------------------
//	// ボムも生成しておくよ
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC bomb_param;
//	bomb_param.layer_ = LAYER_BOMB;
//	for (int i = 0; i < MAX_BULLET; i++)
//	{
//		bomb_param.name_ = "bomb" + std::to_string(i);
//		object_manager_->Create(
//			bomb_param);
//	}
//
//	//-------------------------------------
//	// ブラインド生成しておくよ
//	//-------------------------------------
//	OBJECT_PARAMETER_DESC blind_param;
//	blind_param.layer_ = LAYER_BLIND;
//	for (int i = 0; i < MAX_BLIND; i++)
//	{
//		blind_param.name_ = "blind" + std::to_string(i);
//		Object* obj = object_manager_->Create(blind_param);
//		Blind* blind = static_cast<Blind*>(obj);
//		std::string texture_name = "resource/texture/game/blind_" + std::to_string(i % BLIND_TEXTURE_MAX) + ".png";
//		blind->SetTexture(texture_name);
//	}
//
//	//-------------------------------------
//>>>>>>> master
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
	// アロー作成
	//-------------------------------------
	OBJECT_PARAMETER_DESC arrow_param;
	arrow_param.name_ = "arrow";
	arrow_param.layer_ = LAYER_ARROW;
	arrow_param.position_ = {0.0f, -5000.0f, 0.0f};
	arrow_param.scaling_ = {1.0f, 1.0f, 1.0f};
	object_manager_->Create(arrow_param);

	//-------------------------------------
	// 勝敗
	//-------------------------------------
	OBJECT_PARAMETER_DESC result_sprite_param;
	result_sprite_param.name_ = "result_sprite";
	result_sprite_param.layer_ = LAYER_SPRITE_2D;
	result_sprite_param.position_ = {0.0f, -5000.0f, 0.0f};
	result_sprite_param.scaling_ = {SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 1.0f};
	object_manager_->Create(result_sprite_param,
		"resource/texture/game/win.png");

	//-------------------------------------
	// 砦座標
	//-------------------------------------
	Field* field = dynamic_cast<Field*>(object_manager_->Get("field"));
	for(int i = 0; i < 3; i++)
	{
		Object *fort = object_manager_->Get("fort" + std::to_string(i));
		Vector3 fort_pos = FORT_POSITION[i];
		D3DXVECTOR3 temp;
		temp.x = fort_pos.x_;
		temp.y = fort_pos.y_;
		temp.z = fort_pos.z_;

		fort_pos.y_ = field->GetHeight(temp);
		if(i != 0)
		{
			fort_pos.y_ -= 3.0f;
		}

		fort->SetPosition(fort_pos);
	}

	//-------------------------------------
	// 初期シールド展開
	//-------------------------------------
	EFFECT_PARAMETER_DESC shield_param;
	Object *fort = object_manager_->Get("fort0");
	MyEffect *effect_shield = effect_manager_->Get("shieldin");
	shield_param = effect_shield->parameter();
	shield_param.position_ = fort->parameter().position_;
	shield_param.position_.y_ = SHIELD_POSITION_Y;
	shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	effect_shield->SetParameter(shield_param);
	effect_manager_->Play("shieldin");

	//-------------------------------------
	// マーカー
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	ZeroMemory(&effect_param, sizeof(effect_param));
	MyEffect *effect = effect_manager_->Get("marker");
	effect_param = effect->parameter();
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;
	effect->SetParameter(effect_param);
	effect_manager_->Play("water");
	effect_manager_->Play("marker");

	//-------------------------------------
	// スピードダウンエフェクト☆
	//-------------------------------------
	std::string speed_down_name = "SpeedDown";
	effect = effect_manager_->Get(speed_down_name);
	effect_param = effect->parameter();
	effect_param.position_ = {0.0f, 50000.0f, 0.0f};
	effect_param.rotation_ = {0.0f, 0.0f, 0.0f};
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;
	effect->SetParameter(effect_param);
	effect_manager_->Play("water");
	effect_manager_->Play(speed_down_name);

	//-------------------------------------
	// UIの描画フラグを切る
	//-------------------------------------
	object_manager_->SetDrawEnable(LAYER_SPRITE_2D, true);

#ifdef NETWORK_HOST_MODE
#else
	NETWORK_DATA network_data;
	network_data.type_ = DATA_COMPLETE_SCENE_CHANGE;
	NetworkGuest::SendTo(network_data);
#endif
}

//<<<<<<< HEAD

//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
	//-------------------------------------
	// マーカー
	//-------------------------------------
	int player_id = NetworkGuest::id();
	FbxPlayer* player = dynamic_cast<FbxPlayer*>(object_manager_->Get("player" + std::to_string(player_id)));
	if(player != nullptr)
	{
		Vector3 player_pos = player->parameter().position_;
		Camera *main_camera = camera_manager_->Get("MainCamera");
		D3DXVECTOR3 camera_position, camera_focus;
		D3DXVECTOR3 camera_rotation(main_camera->rotation());
		Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));

		Vector3 poseffect = player_pos;
		poseffect.y_ += 0.6f;
		Vector3 speed = {BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ};
		Vector3 rot = Vector3(camera_rotation.x, camera_rotation.y, camera_rotation.z);
		// 回転値を少し調整
		rot.x_ += BULLET_OFFSET_ROT;
		// 回転値を参照して速度を改良
		speed.y_ += sinf(rot.x_) * BULLET_ADD_SPEED_Y;

		for(int i = 0; i < 120; i++)
		{
			poseffect.x_ += sinf(rot.y_) * speed.x_;
			poseffect.y_ += speed.y_;
			poseffect.z_ += cosf(rot.y_) * speed.z_;
			speed.y_ -= BULLET_GRAVITY;

			float height = field->GetHeight(D3DXVECTOR3(poseffect.x_, poseffect.y_, poseffect.z_));
			if(height > poseffect.y_)
			{
				EFFECT_PARAMETER_DESC effect_param;
				MyEffect *effect = effect_manager_->Get("marker");
				effect_param = effect->parameter();
				effect_param.position_ = poseffect;
				effect_param.position_.y_ = height;
				effect_param.rotation_ = {0.0f, camera_rotation.y, 0.0f};
				Vector3 vec = poseffect - player_pos;
				float len = sqrt(vec.x_*vec.x_ + vec.z_ * vec.z_);
				float scaling = 1.0f + len * 0.15f;

				scaling = std::min<float>(scaling, 3.0f);

				effect_param.scaling_ = {scaling, scaling, scaling};
				effect->SetParameter(effect_param);
				break;
			}
		}
	}
//=======
//#ifdef _DEBUG
//    if (KeyBoard::isPress(DIK_9)){
//        grandfather->SetWaterGauge(0.0f);
//        waterGage->SetChangeValue(0.0f);
//    }
//    if (KeyBoard::isPress(DIK_SPACE)){
//        EFFECT_PARAMETER_DESC effect_param;
//        MyEffect *effect = effect_manager_->Get("water");
//        effect_param = effect->parameter();
//        effect_param.position_ = grandfather_position;
//        effect_param.position_.y_ += 0.6f;
//        effect_param.rotation_ = grandfather_rotation;
//        effect->SetParameter(effect_param);
//        effect_manager_->Play("water");
//
//        OBJECT_PARAMETER_DESC bullet_param;
//        bullet_param.layer_ = LAYER_BULLET;
//        bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
//        bullet_param.position_ = grandfather_position;
//        bullet_param.position_.y_ += 0.6f;
//        bullet_param.rotation_ = grandfather_rotation;
//        bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//
//        // カメラの回転Xを利用
//        bullet_param.rotation_.x_ = camera_rotation.x;
//
//		Bullet* bullet = object_manager_->GetNoUseBullet();
//		bullet->Fire(bullet_param);
//        //-------------------------------------
//        // おじの弾発射SE再生
//        Sound::LoadAndPlaySE("resource/sound/se/game/shootGrandfather.wav");
//    }
//
//    //-------------------------------------
//    // デバッグ時のみ、水ゲージ回復
//    //-------------------------------------
//    if (KeyBoard::isPress(DIK_1)){
//		father_watergauge += GRANDFATHER_SUB_BULLET_WATERGAUGE;
//        father_watergauge = std::min<float>(father_watergauge, 1.0f);
//        grandfather->SetWaterGauge(father_watergauge);
//        waterGage->SetChangeValue(father_watergauge);
//    }
//    //-------------------------------------
//    // カメラ切り替えテスト
//    //-------------------------------------
//    if (KeyBoard::isPress(DIK_4)){
//        use_camera_name_ = "MainCamera";
//    }
//    if (KeyBoard::isPress(DIK_5)){
//        use_camera_name_ = "SubCamera";
//    }
//
//#endif //_DEBUG
//
//#ifdef _DEBUG
//    //-------------------------------------
//    // ダメージエフェクト確認
//    //-------------------------------------
//    if (KeyBoard::isPress(DIK_L)){
//        float life = child->GetLife();
//        life -= CHILD_DAMAGE;
//        if (life < 0.0f){
//            life = 0.0f;
//        }
//        child->SetLife(life);
//    }
//    else if (KeyBoard::isPress(DIK_K)){
//        float life = child->GetLife();
//        life += CHILD_DAMAGE;
//        if (life > 1.0f){
//            life = 1.0f;
//        }
//        child->SetLife(life);
//    }
//#endif // _DEBUG
//
//    //-------------------------------------
//    // アニメーション制御
//    //-------------------------------------
//    if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) ||
//        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) ||
//        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) ||
//        GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
//        if (grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
//        {
//            grandfather->PlayAnimation(FbxGrandfather::WALK);
//            walk_flg_ = true;
//        }
//    }
//    else{
//        if (grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
//        {
//            grandfather->PlayAnimation(FbxGrandfather::IDLE);
//            walk_flg_ = false;
//        }
//    }
//
//
//    //-------------------------------------
//    // 移動SE確認用
//    //-------------------------------------
//    if (walk_flg_)
//    {
//        ++walk_timer_;
//        if (!(walk_timer_ % 30))
//        {
//            Sound::LoadAndPlaySE("resource/sound/se/game/footstep.wav");
//            walk_timer_ = 0;
//        }
//    }
//    else
//    {
//        walk_timer_ = 30;
//    }
//
//    //-------------------------------------
//    // 子供死亡時制御
//    //-------------------------------------
//    if (child_life < 0 && !child_death_){
//        child->PlayAnimation(FbxChild::DOWN);
//        child_death_ = true;
//        child_respawn_waittime_ = CHILD_RESPAWN_WAITTIME;
//        EFFECT_PARAMETER_DESC effect_param;
//        MyEffect *effect = effect_manager_->Get("dead");
//        effect_param = effect->parameter();
//        effect_param.position_ = child_position;
//        effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
//        effect->SetParameter(effect_param);
//        effect_manager_->Play("dead");
//
//    }
//    else if (child_death_ && !child_respawn_waittime_){
//        child->PlayAnimation(FbxChild::IDLE);
//        child_death_ = false;
//        child_life = CHILD_LIFE;
//        child->SetLife(child_life);
//		child_position = CHILD1_POSITION1;
//		child_rotation = CHILD1_ROTATION1;
//        child->SetPosition(child_position);
//        child->SetRotation(child_rotation);
//    }
//
//    child_respawn_waittime_--;
//    child_respawn_waittime_ = std::max<int>(child_respawn_waittime_, 0);
//
//    //-------------------------------------
//    // 子供体力自動回復制御
//    //-------------------------------------
//    if (child_life < 1.0f && !child_death_){
//        int child_recover_wait_timer = child->GetRecoverWaitTimer();
//
//        if (child_recover_wait_timer > CHILD_RECOVER_WAITE_TIME){
//            float child_life = child->GetLife();
//            child_life += CHILD_RECOVER_HP;
//            std::min<float>(child_life, 1.0f);
//            child->SetLife(child_life);
//        }
//        child_recover_wait_timer++;
//        child->SetRecoverWaitTimer(child_recover_wait_timer);
//    }
//
//    //-------------------------------------
//    // ダメージエフェクトの処理
//    //-------------------------------------
//    // 今はテスト用に、子供に当てたら主観(おじ)のUIを反映させている
//    damage_effect->SetHP(child_life);
//
//
//    //-------------------------------------
//    // 各キャラクタ座標保存
//    //-------------------------------------
//    grandfather_prevposition = grandfather_position;
//
//
//    //-------------------------------------
//    // 影座標
//    //-------------------------------------
//    Object *shadow = object_manager_->Get("shadow");
//    Vector3 shadow_pos;
//    shadow_pos = grandfather->parameter().position_;
//    shadow_pos.y_ += 0.001f;
//    shadow->SetPosition(shadow_pos);
//
//
//    //-------------------------------------
//    // まーかー表示
//    //-------------------------------------
//    Vector3 poseffect = grandfather_position;
//    poseffect.y_ += 0.6f;
//    Vector3 speed = { BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ };
//    Vector3 rot = Vector3(camera_rotation.x, camera_rotation.y, camera_rotation.z);
//    // 回転値を少し調整
//    rot.x_ += BULLET_OFFSET_ROT;
//    // 回転値を参照して速度を改良
//    speed.y_ += sinf(rot.x_) * BULLET_ADD_SPEED_Y;
//    // 判定が取れたか
//    bool hit_point_field = false;
//
//    for (int i = 0; i < 120; i++){
//        poseffect.x_ += sinf(rot.y_) * speed.x_;
//        poseffect.y_ += speed.y_;
//        poseffect.z_ += cosf(rot.y_) * speed.z_;
//        speed.y_ -= BULLET_GRAVITY;
//
//        float height = field->GetHeight(D3DXVECTOR3(poseffect.x_, poseffect.y_, poseffect.z_));
//        if (height > poseffect.y_){
//            EFFECT_PARAMETER_DESC effect_param;
//            MyEffect *effect = effect_manager_->Get("marker");
//            effect_param = effect->parameter();
//            effect_param.position_ = poseffect;
//            effect_param.position_.y_ = height;
//            effect_param.rotation_ = { 0.0f, camera_rotation.y, 0.0f };
//            Vector3 vec = poseffect - grandfather_position;
//            float len = sqrt(vec.x_*vec.x_ + vec.z_ * vec.z_);
//            float scaling = 1.0f + len * 0.15f;
//
//            scaling = std::min<float>(scaling, 3.0f);
//
//            effect_param.scaling_ = { scaling, scaling, scaling };
//            effect->SetParameter(effect_param);
//            hit_point_field = true;
//            break;
//        }
//    }
//
//    // 判定外
//    if (!hit_point_field){
//        EFFECT_PARAMETER_DESC effect_param;
//        MyEffect *effect = effect_manager_->Get("marker");
//        effect_param = effect->parameter();
//        effect_param.position_.y_ = -100.0f;
//        effect->SetParameter(effect_param);
//    }
//
//    //-------------------------------------
//    // おじのデバフパワーをセット
//    //-------------------------------------
//    //-------------------------------------
//    // おじのデバフなので
//    // ここでは子供のライフでやってるから
//    // おじに直しておいて
//    //-------------------------------------
//    if (child_life < 0.5f){
//        float debuff_power = child_life - 0.75f;
//        debuff_power = abs(debuff_power * 0.5f);
//        grandfather->SetDebuffPower(debuff_power);
//
//        EFFECT_PARAMETER_DESC effect_param;
//        MyEffect *effect = effect_manager_->Get("speeddown");
//        effect_param = effect->parameter();
//        effect_param.position_ = grandfather_position;
//        effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//        effect->SetParameter(effect_param);
//        if (debuff_effect_flg_){
//            effect_manager_->Play("speeddown");
//            debuff_effect_flg_ = false;
//        }
//    }
//    else{
//		grandfather->SetDebuffPower(0.0f);
//        effect_manager_->Stop("speeddown");
//        debuff_effect_flg_ = true;
//    }
//
//	//-------------------------------------
//	// シールドのON,OFF設定 ステージ移行中は無効
//	//-------------------------------------
//	//-------------------------------------
//	// デバッグ用に子供のライフでやってるので
//	// おじのライフに直す
//	//-------------------------------------
//	if (child_life < SHIELD_SWITCH_LIFE && shield_flg_ == true && change_stage_flg_ == false){
//
//		// シールドを消すと同時に
//		// シールド解除エフェクト発生
//		EFFECT_PARAMETER_DESC shieldout_param;
//		MyEffect *effect_shieldout = effect_manager_->Get("shieldout");
//		MyEffect *effect_shieldin = effect_manager_->Get("shieldin");
//		shieldout_param = effect_shieldout->parameter();
//		shieldout_param.position_ = effect_shieldin->parameter().position_;
//		shieldout_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect_shieldout->SetParameter(shieldout_param);
//		effect_manager_->Stop("shieldin");
//		effect_manager_->Play("shieldout");
//
//		shield_flg_ = false;
//	}
//	else if (child_life > SHIELD_SWITCH_LIFE && shield_flg_ == false && change_stage_flg_ == false){
//		// シールドはる
//		EFFECT_PARAMETER_DESC shield_param;
//		MyEffect *effect_shield = effect_manager_->Get("shieldin");
//		shield_param = effect_shield->parameter();
//		shield_param.position_ = fort1_position;
//		shield_param.position_.y_ = SHIELD_POSITION_Y;
//		shield_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect_shield->SetParameter(shield_param);
//		effect_manager_->Play("shieldin");
//
//		// シールドフラグ建てる
//		shield_flg_ = true;
//	}
//
//
//>>>>>>> master

	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();

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
	MaterialColor color(32, 32, 32, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);

	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(object_manager_->Get("damage_effect"));
	damage_effect->Draw();

	Fade::Draw();

	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
