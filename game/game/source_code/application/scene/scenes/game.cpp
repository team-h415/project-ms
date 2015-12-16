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
#include "../../object/objects/sprite/sprite2d.h"
#include "../../object/objects/sprite/timer.h"
#include "../../object/objects/sprite/damage_effect.h"
#include "../../object/objects/sprite/countdown.h"
#include "../../object/objects/sprite/water_gage.h"
#include "../../object/objects/sprite/fort_gauge_manager.h"
#include "../../object/objects/sprite/message.h"
#include "../../object/objects/notice/bullet.h"
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
		"resource/effect/Damage3_2.efk",
		effect_param);

	effect_manager_->Create(
		"dead",
		"resource/effect/Dead2.efk",
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

	// 湖
	Object *obj_lake = object_manager_->Get("lake");
	COLLISION_PARAMETER_DESC lake_collision_param;
	lake_collision_param.position_ = {
		obj_lake->parameter().position_.x_,
		5.0f,
		obj_lake->parameter().position_.z_ };
	lake_collision_param.range_ = LAKE_COLLISION_RANGE;
	// 上
	lake_collision_param.offset_ = { -0.5f, 0.0f, 11.5f };
	collision_manager_->Create(object_manager_->Get("lake"),
		lake_collision_param);
	// 右下
	lake_collision_param.offset_ = { 5.0f, 0.0f, 2.0f };
	collision_manager_->Create(object_manager_->Get("lake"),
		lake_collision_param);
	// 左上
	lake_collision_param.offset_ = { -3.5f, 0.0f, 0.0f };
	collision_manager_->Create(object_manager_->Get("lake"),
		lake_collision_param);



	//-------------------------------------
	// 砦
	//-------------------------------------
	OBJECT_PARAMETER_DESC fort_param;
	COLLISION_PARAMETER_DESC fort_collision_param;
	Object *fort_obj;
	XFort *fort;
	Vector3 fort_pos;
	D3DXVECTOR3 temp;
	Field* field = dynamic_cast<Field*>(object_manager_->Get("field"));
	for(int i = 0; i < 3; i++)
	{
		fort_pos = FORT_POSITION[i];
		temp.x = fort_pos.x_;
		temp.y = fort_pos.y_;
		temp.z = fort_pos.z_;

		fort_pos.y_ = field->GetHeight(temp);
		if(i != 0)
		{
			fort_pos.y_ -= 3.0f;
		}

		std::string name = "fort" + std::to_string(i + 1);
		fort_param.name_ = name;
		fort_param.layer_ = LAYER_MODEL_FORT;
		fort_param.position_ = fort_pos;
		fort_param.rotation_ = {0.0f, 0.0f, 0.0f};
		fort_param.scaling_ = {1.0f, 1.0f, 1.0f};

		object_manager_->Create(
			fort_param,
			"resource/model/x/test.x");

		fort_obj = object_manager_->Get(name);
		fort = dynamic_cast<XFort*>(fort_obj);

		fort_collision_param.position_ = {
			fort_pos.x_,
			fort_pos.y_ + 0.5f,
			fort_pos.z_};
		fort_collision_param.range_ = 1.0f;
		fort_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

		collision_manager_->Create(fort_obj,
			fort_collision_param);
		fort->SetLife(FORT_LIFE[i]);
	}


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
	OBJECT_PARAMETER_DESC wood_param;
	wood_param.name_ = "wood";
	wood_param.layer_ = LAYER_TREE;
	object_manager_->Create(wood_param);

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
	// メッセージ
	//-------------------------------------
	OBJECT_PARAMETER_DESC message_param;
	message_param.name_ = "message_child1_death";
	message_param.position_ = {
		SCREEN_WIDTH + 200.0f,
		SCREEN_HEIGHT - 200.0f,
		0.0f };
	message_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	message_param.scaling_ = { 400.0f, 100.0f, 0.0f };
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
//	//-------------------------------------
//	// 変数宣言
//	//-------------------------------------
//	// 動的変数
//	Object *fort1_object = object_manager_->Get("fort1");
//	Object *fort2_object = object_manager_->Get("fort2");
//	Object *fort3_object = object_manager_->Get("fort3");
//	Object *grandfather_object = object_manager_->Get("grandfather");
//	Object *child_object = object_manager_->Get("child");
//	Object *countdown_object = object_manager_->Get("countdown");
//	Vector3 fort1_position(fort1_object->parameter().position_);
//	Vector3 fort2_position(fort2_object->parameter().position_);
//	Vector3 fort3_position(fort3_object->parameter().position_);
//	Vector3 grandfather_position(grandfather_object->parameter().position_);
//	Vector3 grandfather_rotation(grandfather_object->parameter().rotation_);
//	Vector3 child_position(child_object->parameter().position_);
//	Vector3 child_rotation(child_object->parameter().rotation_);
//	Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
//	FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(grandfather_object);
//	FbxChild *child = dynamic_cast<FbxChild*>(child_object);
//    WaterGage *waterGage = dynamic_cast<WaterGage*>(object_manager_->Get("water_gage"));
//	DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(
//		object_manager_->Get("damage_effect"));
//    FortGaugeManager *fort_gauge_manager = dynamic_cast<FortGaugeManager*>(
//        object_manager_->Get("fort_gauge_manager"));
//	XFort *fort1 = dynamic_cast<XFort*>(fort1_object);
//	XFort *fort2 = dynamic_cast<XFort*>(fort2_object);
//	XFort *fort3 = dynamic_cast<XFort*>(fort3_object);
//	CountDown *countdown = dynamic_cast<CountDown*>(countdown_object);
//	float player_speed = CHARANCTER_MOVESPEED;
//	float father_life = grandfather->GetLife();
//	float father_watergauge = grandfather->GetWaterGauge();
//	float child_life = child->GetLife();
//	float child_watergauge = child->GetWaterGauge();
//	float fort1_life = fort1->GetLife();
//	float fort2_life = fort2->GetLife();
//	float fort3_life = fort3->GetLife();
//
//	// 静的変数
//	static int shot_late = 0;
//	static D3DXVECTOR3 fort_underground(-3.0f, -3.0f, -3.0f);
//	static Vector3 grandfather_prevposition(grandfather_object->parameter().position_);
//	static int fort_damage_state = 0;
//
//
//	//-------------------------------------
//	// 時間経過
//	//-------------------------------------
//	frame_++;
//	if (!(frame_ % 60)){
//		timer_++;
//	}
//
//	//-------------------------------------
//	// 時間経過でカウントダウン
//	//-------------------------------------
//	switch (timer_)
//	{
//	case 0:
//		countdown->ChangeTexture(4);
//		break;
//	case 1:
//		countdown->ChangeTexture(3);
//		break;
//	case 2:
//		countdown->ChangeTexture(2);
//		break;
//	case 3:
//		countdown->ChangeTexture(1);
//		break;
//	case 4:
//		countdown->ChangeTexture(0);
//		break;
//	default:
//		countdown->ChangeTexture(4);
//		break;
//	}
//
//	//-------------------------------------
//	// メッセージの再生
//	//-------------------------------------
//	// 砦が破壊された！
//	if ((fort_damage_state == 75 && (fort1_life <= 0.0f) && stage_ == 1) ||
//		(fort_damage_state == 75 && (fort2_life <= 0.0f) && stage_ == 2)){
//		Object *message_object = object_manager_->Get("message_fort_100");
//		Message *message = dynamic_cast<Message*>(message_object);
//		Vector3 message_position = {
//			SCREEN_WIDTH + 200.0f,
//			SCREEN_HEIGHT - 200.0f,
//			0.0f };
//		message_object->SetPosition(message_position);
//		message->Play();
//		fort_damage_state = 0;
//	}
//	// 損壊率75%
//	else if ((fort_damage_state == 50 && fort1_life < FORT1_LiFE * 0.25f && stage_ == 1) ||
//		(fort_damage_state == 50 && fort2_life < FORT2_LiFE * 0.25f && stage_ == 2) ||
//		(fort_damage_state == 50 && fort3_life < FORT3_LiFE * 0.25f && stage_ == 3)){
//		Object *message_object = object_manager_->Get("message_fort_75");
//		Message *message = dynamic_cast<Message*>(message_object);
//		Vector3 message_position = {
//			SCREEN_WIDTH + 200.0f,
//			SCREEN_HEIGHT - 200.0f,
//			0.0f };
//		message_object->SetPosition(message_position);
//		message->Play();
//		fort_damage_state = 75;
//	}
//	// 損壊率50%
//	else if ((fort_damage_state == 25 && fort1_life < FORT1_LiFE * 0.5f && stage_ == 1) ||
//		(fort_damage_state == 25 && fort2_life < FORT2_LiFE * 0.5f && stage_ == 2) ||
//		(fort_damage_state == 25 && fort3_life < FORT3_LiFE * 0.5f && stage_ == 3)){
//		Object *message_object = object_manager_->Get("message_fort_50");
//		Message *message = dynamic_cast<Message*>(message_object);
//		Vector3 message_position = {
//			SCREEN_WIDTH + 200.0f,
//			SCREEN_HEIGHT - 200.0f,
//			0.0f };
//		message_object->SetPosition(message_position);
//		message->Play();
//		fort_damage_state = 50;
//	}
//	// 損壊率25%
//	else if ((fort_damage_state == 0 && fort1_life < FORT1_LiFE * 0.75f && stage_ == 1) ||
//		(fort_damage_state == 0 && fort2_life < FORT2_LiFE * 0.75f && stage_ == 2) ||
//		(fort_damage_state == 0 && fort3_life < FORT3_LiFE * 0.75f && stage_ == 3)){
//		Object *message_object = object_manager_->Get("message_fort_25");
//		Message *message = dynamic_cast<Message*>(message_object);
//		Vector3 message_position = {
//			SCREEN_WIDTH + 200.0f,
//			SCREEN_HEIGHT - 200.0f,
//			0.0f };
//		message_object->SetPosition(message_position);
//		message->Play();
//		fort_damage_state = 25;
//	}
//
//	//-------------------------------------
//	// ゲームステージデバッグ
//	//-------------------------------------
//	if (fort1_life <= 0.0f){
//		stage_ = 2;
//		if (fort2_life <= 0.0f){
//			stage_ = 3;
//		}
//	}
//	fort_gauge_manager->SetFortLife(0, fort1_life);
//	fort_gauge_manager->SetFortLife(1, fort2_life);
//	fort_gauge_manager->SetFortLife(2, fort3_life);
//
//	//-------------------------------------
//	// プレイヤー移動処理
//	//-------------------------------------
//	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_11)){
//
//		player_speed = CHARANCTER_MOVESPEED * 2.0f;
//		// ダッシュエフェクト
//		if (dash_effect_timer_ % 10 == 0){
//			EFFECT_PARAMETER_DESC effect_param;
//			MyEffect *effect = effect_manager_->Get("dash");
//			effect_param = effect->parameter();
//			effect_param.position_ = grandfather_position;
//			effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//			effect->SetParameter(effect_param);
//			effect_manager_->Play("dash");
//		}
//		dash_effect_timer_++;
//	}
//	else{ dash_effect_timer_ = 0; }
//	grandfather_position.x_ += (
//		cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
//		sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
//	grandfather_position.z_ -= (
//		sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
//		cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
//
//	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
//		grandfather_rotation.y_ -= CHAR_ROT_SPEED;
//		if (grandfather_rotation.y_ < D3DX_PI){
//			grandfather_rotation.y_ += D3DX_PI * 2.0f;
//		}
//	}
//	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
//		grandfather_rotation.y_ += CHAR_ROT_SPEED;
//		if (grandfather_rotation.y_ > D3DX_PI){
//			grandfather_rotation.y_ -= D3DX_PI * 2.0f;
//		}
//	}
//	if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_7)){
//		// ワープエフェクト再生（移動前）
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("smoke");
//		effect_param = effect->parameter();
//		effect_param.position_ = grandfather_position;
//		effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke");
//
//		switch(stage_){
//		case 1:
//			grandfather_position = GRANDFATHER_POSITION_STAGE1;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE1;
//			break;
//		case 2:
//			grandfather_position = GRANDFATHER_POSITION_STAGE2;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE2;
//			break;
//		case 3:
//			grandfather_position = GRANDFATHER_POSITION_STAGE3;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE3;
//			break;
//		}
//		// ワープエフェクト再生（移動後）
//		effect_param.position_ = grandfather_position;
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke");
//	}
//
//	//-------------------------------------
//	// デバッグ時のプレイヤー操作
//	//-------------------------------------
//#ifdef _DEBUG
//	if (KeyBoard::isPress(DIK_Z)){
//		player_speed = CHARANCTER_MOVESPEED * 2.0f;
//		// ダッシュエフェクト
//		if (dash_effect_timer_ % 10 == 0){
//			EFFECT_PARAMETER_DESC effect_param;
//			MyEffect *effect = effect_manager_->Get("dash");
//			effect_param = effect->parameter();
//			effect_param.position_ = grandfather_position;
//			effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//			effect->SetParameter(effect_param);
//			effect_manager_->Play("dash");
//		}
//		dash_effect_timer_++;
//	}
//	else{ dash_effect_timer_ = 0; }
//	if (KeyBoard::isPress(DIK_W)){
//		grandfather_position.x_ += sinf(grandfather_rotation.y_) * player_speed;
//		grandfather_position.z_ += cosf(grandfather_rotation.y_) * player_speed;
//	}
//	if (KeyBoard::isPress(DIK_A)){
//		grandfather_position.x_ += sinf(grandfather_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
//		grandfather_position.z_ += cosf(grandfather_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
//	}
//	if (KeyBoard::isPress(DIK_S)){
//		grandfather_position.x_ += sinf(grandfather_rotation.y_ + (D3DX_PI)) * player_speed;
//		grandfather_position.z_ += cosf(grandfather_rotation.y_ + (D3DX_PI)) * player_speed;
//	}
//	if (KeyBoard::isPress(DIK_D)){
//		grandfather_position.x_ += sinf(grandfather_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
//		grandfather_position.z_ += cosf(grandfather_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
//	}
//	if (KeyBoard::isPress(DIK_RIGHT)){
//		grandfather_rotation.y_ += CHAR_ROT_SPEED;
//		if (grandfather_rotation.y_ > D3DX_PI){
//			grandfather_rotation.y_ -= D3DX_PI * 2.0f;
//		}
//	}
//	if (KeyBoard::isPress(DIK_LEFT)){
//		grandfather_rotation.y_ -= CHAR_ROT_SPEED;
//		if (grandfather_rotation.y_ < -D3DX_PI){
//			grandfather_rotation.y_ += D3DX_PI * 2.0f;
//		}
//	}
//	if (KeyBoard::isTrigger(DIK_3)){
//		// ワープエフェクト再生（移動前）
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("smoke");
//		effect_param = effect->parameter();
//		effect_param.position_ = grandfather_position;
//		effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke");
//
//		switch (stage_){
//		case 1:
//			grandfather_position = GRANDFATHER_POSITION_STAGE1;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE1;
//			break;
//		case 2:
//			grandfather_position = GRANDFATHER_POSITION_STAGE2;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE2;
//			break;
//		case 3:
//			grandfather_position = GRANDFATHER_POSITION_STAGE3;
//			grandfather_rotation.y_ = GRANDFATHER_ROTATION_STAGE3;
//			break;
//		}
//		// ワープエフェクト再生（移動後）
//		effect_param.position_ = grandfather_position;
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke");
//	}
//
//#endif //_DEBUG
//
//
//	//-------------------------------------
//	// 砦の座標管理
//	//-------------------------------------
//	switch (stage_)
//	{
//	case 1:
//		fort_underground.x += 0.01f;
//		fort_underground.y -= 0.01f;
//		fort_underground.z -= 0.01f;
//		fort_underground.x = std::min<float>(fort_underground.x, 0.0f);
//		fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
//		fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
//		break;
//	case 2:
//		fort_underground.x -= 0.01f;
//		fort_underground.y += 0.01f;
//		fort_underground.z -= 0.01f;
//		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
//		fort_underground.y = std::min<float>(fort_underground.y, 0.0f);
//		fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
//		break;
//	case 3:
//		fort_underground.x -= 0.01f;
//		fort_underground.y -= 0.01f;
//		fort_underground.z += 0.01f;
//		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
//		fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
//		fort_underground.z = std::min<float>(fort_underground.z, 0.0f);
//		break;
//	}
//
//	D3DXVECTOR3 fort1_pos(
//		fort1_position.x_,
//		fort1_position.y_,
//		fort1_position.z_);
//	fort1_position.y_ = field->GetHeight(fort1_pos) + fort_underground.x;
//
//	D3DXVECTOR3 fort2_pos(
//		fort2_position.x_,
//		fort2_position.y_,
//		fort2_position.z_);
//	fort2_position.y_ = field->GetHeight(fort2_pos) + fort_underground.y;
//
//	D3DXVECTOR3 fort3_pos(
//		fort3_position.x_,
//		fort3_position.y_,
//		fort3_position.z_);
//	fort3_position.y_ = field->GetHeight(fort3_pos) + fort_underground.z;
//
//	D3DXVECTOR3 grandfather_pos(
//		grandfather_position.x_,
//		grandfather_position.y_,
//		grandfather_position.z_);
//	grandfather_position.y_ = field->GetHeight(grandfather_pos);
//	/*if (grandfather_position.y_ > 0.4f ||
//		grandfather_position.y_ < -0.4f){
//		grandfather_position = grandfather_prevposition;
//	}*/
//	
//	D3DXVECTOR3 child_pos(
//		child_position.x_,
//		child_position.y_,
//		child_position.z_);
//	child_position.y_ = field->GetHeight(child_pos);
//
//	fort1_object->SetPosition(fort1_position);
//	fort2_object->SetPosition(fort2_position);
//	fort3_object->SetPosition(fort3_position);
//	grandfather_object->SetPosition(grandfather_position);
//	grandfather_object->SetRotation(grandfather_rotation);
//	child_object->SetPosition(child_position);
//
//
//	//-------------------------------------
//	// 砦にとりあえずエフェクトだす
//	//-------------------------------------
//	if (fort_underground.x != 0.0f && fort_underground.x != -3.0f){
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("smoke2");
//		effect_param = effect->parameter();
//		effect_param.position_ =
//		{ fort1_pos.x, field->GetHeight(fort1_pos), fort1_pos.z };
//		effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke2");
//	}
//	
//	if (fort_underground.y != 0.0f && fort_underground.y != -3.0f){
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("smoke2");
//		effect_param = effect->parameter();
//		effect_param.position_ =
//		{ fort2_pos.x, field->GetHeight(fort2_pos), fort2_pos.z };
//		effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke2");
//	}
//
//	if (fort_underground.z != 0.0f && fort_underground.z != -3.0f){
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("smoke2");
//		effect_param = effect->parameter();
//		effect_param.position_ =
//		{ fort3_pos.x, field->GetHeight(fort3_pos), fort3_pos.z };
//		effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("smoke2");
//	}
//
//	//-------------------------------------
//	// カメラ追従
//	//-------------------------------------
//	Camera *main_camera = camera_manager_->Get("MainCamera");
//	D3DXVECTOR3 camera_position, camera_focus;
//	D3DXVECTOR3 camera_rotation(main_camera->rotation());
//
//	
//	
//
//	// 入力
//#ifdef _DEBUG
//	if(KeyBoard::isPress(DIK_UP)){
//		camera_rotation.x -= CAMERA_ROT_SPEED;
//		if(camera_rotation.x < -CAMERA_ROT_X_LIMIT){
//			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
//		}
//	}
//	if(KeyBoard::isPress(DIK_DOWN)){
//		camera_rotation.x += CAMERA_ROT_SPEED;
//		if(camera_rotation.x > CAMERA_ROT_X_LIMIT){
//			camera_rotation.x = CAMERA_ROT_X_LIMIT;
//		}
//	}
//#endif
//	
//	if(GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_UP)){
//		camera_rotation.x -= CAMERA_ROT_SPEED;
//		if(camera_rotation.x < -CAMERA_ROT_X_LIMIT){
//			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
//		}
//	}
//	if(GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_DOWN)){
//		camera_rotation.x += CAMERA_ROT_SPEED;
//		if(camera_rotation.x > CAMERA_ROT_X_LIMIT){
//			camera_rotation.x = CAMERA_ROT_X_LIMIT;
//		}
//	}
//
//
//	//-------------------------------------
//	// カメラ計算
//	//-------------------------------------
//
//	// モデルの回転Yをそのままカメラの回転Yへ
//	camera_rotation.y = grandfather_rotation.y_;
//	// 一旦モデルを注視点に
//	camera_focus = grandfather_pos;
//	// 足元基準から体の中心辺りを基準に
//	camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
//	// モデルの少し先を見るように調整
//	camera_focus.x +=
//		sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
//	camera_focus.z +=
//		cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
//	camera_focus.y +=
//		sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;
//
//	// 注視点を基準にカメラ座標を設定
//	camera_position = camera_focus;
//	camera_position.x -=
//		sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
//	camera_position.z -=
//		cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
//	camera_position.y -=
//		sinf(camera_rotation.x) * camera_pos_len_;
//
//
//	// カメラの地面めり込み回避処理
//	D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;
//	
//	// 中間にカメラがめり込みそうなところが無いか検査
//	bool camera_re_calculate = false;
//	for (int i = 0; i < 10; ++i){
//		// 中間地点を計算
//		D3DXVECTOR3 lay_point =
//			camera_position + vec_camera_to_focus * static_cast<float>(i) * 0.1f;
//		float pos_y = field->GetHeight(lay_point);
//		// 回避処理
//		if (lay_point.y < pos_y + 0.1f){
//			camera_re_calculate = true;
//			camera_pos_len_ -= CAMARA_LEN_SPEED;
//		}
//	}
//
//	//カメラ座標再計算
//	if (camera_re_calculate == true){ 
//		camera_position = camera_focus;
//		camera_position.x -=
//			sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
//		camera_position.z -=
//			cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
//		camera_position.y -=
//			sinf(camera_rotation.x) * camera_pos_len_;
//		camera_position.y = field->GetHeight(camera_position) + 0.1f;
//	}
//
//	camera_pos_len_ += CAMARA_LEN_SPEED;
//	if (camera_pos_len_ > CAMERA_POS_LEN){
//		camera_pos_len_ = CAMERA_POS_LEN;
//	}
//	
//	// カメラにパラメータを再セット
//	main_camera->SetPosition(camera_position);
//	main_camera->SetFocus(camera_focus);
//	main_camera->SetRotation(camera_rotation);
//
//
//	//-------------------------------------
//	// サブカメラ計算
//	//-------------------------------------
//	Camera *sub_camera = camera_manager_->Get("SubCamera");
//	D3DXVECTOR3 sub_camera_position, sub_camera_rotation, sub_camera_focus;
//	sub_camera_rotation = sub_camera->rotation();
//	sub_camera_position = sub_camera->position();
//	sub_camera_focus = sub_camera->focus();
//	
//	// 適当な位置から眺める
//	// 一旦砦を注視点に
//	switch (stage_)
//	{
//	case 1:
//		sub_camera_focus = fort1_pos;
//		sub_camera_focus.y = 1.5f;
//		if (fort_underground.x != 0.0f){
//			use_camera_name_ = "SubCamera";
//			sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
//			sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
//		}
//		else{
//			use_camera_name_ = "MainCamera";
//			sub_camera_rotation.y = 0.0f;
//			sub_camera_rotation.x = 0.0f;
//		}
//		break;
//	case 2:
//		sub_camera_focus = fort2_pos;
//		sub_camera_focus.y = 1.5f;
//		if (fort_underground.y != 0.0f){
//			use_camera_name_ = "SubCamera";
//			sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
//			sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
//		}
//		else{
//			use_camera_name_ = "MainCamera";
//			sub_camera_rotation.y = 0.0f;
//			sub_camera_rotation.x = 0.0f;
//		}
//		break;
//	case 3:
//		sub_camera_focus = fort3_pos;
//		sub_camera_focus.y = 1.5f;
//		if (fort_underground.z != 0.0f){
//			use_camera_name_ = "SubCamera";
//			sub_camera_rotation.y += CAMERA_SUB_ROT_SPEED;
//			sub_camera_rotation.x += CAMERA_SUB_ROT_SPEED*0.02f;
//		}
//		else{
//			use_camera_name_ = "MainCamera";
//			sub_camera_rotation.y = 0.0f;
//			sub_camera_rotation.x = 0.0f;
//		}
//		break;
//	}
//	
//	// 注視点を基準にカメラ座標を設定
//	sub_camera_position = sub_camera_focus;
//	sub_camera_position.x -= 
//		sinf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
//	sub_camera_position.z -= 
//		cosf(sub_camera_rotation.y) * camera_pos_len_ * cosf(sub_camera_rotation.x);
//	sub_camera_position.y -= 
//		sinf(sub_camera_rotation.x) * camera_pos_len_;
//
//
//	// カメラにパラメータを再セット
//	sub_camera->SetPosition(sub_camera_position);
//	sub_camera->SetFocus(sub_camera_focus);
//	sub_camera->SetRotation(sub_camera_rotation);
//
//	//-------------------------------------
//	// 弾発射
//	//-------------------------------------
//	shot_late--;
//	shot_late = std::max<int>(shot_late, 0);
//	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_8) &&
//		father_watergauge > 0 &&
//		shot_late == 0){
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("water");
//		effect_param = effect->parameter();
//		effect_param.position_ = grandfather_position;
//		effect_param.position_.y_ += 0.6f;
//		effect_param.rotation_ = grandfather_rotation;
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("water");
//
//
//		OBJECT_PARAMETER_DESC bullet_param;
//		bullet_param.layer_ = LAYER_BULLET;
//		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
//		bullet_param.position_ = grandfather_position;
//		bullet_param.position_.y_ += 0.6f;
//		bullet_param.rotation_ = grandfather_rotation;
//
//		// カメラの回転Xを利用
//		bullet_param.rotation_.x_ = camera_rotation.x;
//
//		bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };
//		
//		Bullet* bullet = object_manager_->GetNoUseBullet();
//		bullet->Fire(bullet_param);
//
//		//-------------------------------------
//		// 水ゲージを減少させる
//		//-------------------------------------
//		father_watergauge -= GRANDFATHER_SUB_WATERGAUGE;
//		father_watergauge = std::max<float>(father_watergauge, 0.0f);
//		grandfather->SetWaterGauge(father_watergauge);
//		waterGage->SetChangeValue(father_watergauge);
//
//		shot_late = 10;
//
//	}
//
//
//#ifdef _DEBUG
//	if (KeyBoard::isPress(DIK_9)){
//		grandfather->SetWaterGauge(0.0f);
//		waterGage->SetChangeValue(0.0f);
//	}
//	if(KeyBoard::isPress(DIK_SPACE)){
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("water");
//		effect_param = effect->parameter();
//		effect_param.position_ = grandfather_position;
//		effect_param.position_.y_ += 0.6f;
//		effect_param.rotation_ = grandfather_rotation;
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("water");
//
//		OBJECT_PARAMETER_DESC bullet_param;
//		bullet_param.layer_ = LAYER_BULLET;
//		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
//		bullet_param.position_ = grandfather_position;
//		bullet_param.position_.y_ += 0.6f;
//		bullet_param.rotation_ = grandfather_rotation;
//		bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};
//
//		// カメラの回転Xを利用
//		bullet_param.rotation_.x_ = camera_rotation.x;
//
//		Bullet* bullet = object_manager_->GetNoUseBullet();
//		bullet->Fire(bullet_param);
//	}
//
//	//-------------------------------------
//	// デバッグ時のみ、水ゲージ回復
//	//-------------------------------------
//	if (KeyBoard::isPress(DIK_1)){
//		father_watergauge += GRANDFATHER_SUB_WATERGAUGE;
//		father_watergauge = std::min<float>(father_watergauge, 1.0f);
//		grandfather->SetWaterGauge(father_watergauge);
//		waterGage->SetChangeValue(father_watergauge);
//	}
//	//-------------------------------------
//	// カメラ切り替えテスト
//	//-------------------------------------
//	if (KeyBoard::isPress(DIK_4)){
//		use_camera_name_ = "MainCamera";
//	}
//	if (KeyBoard::isPress(DIK_5)){
//		use_camera_name_ = "SubCamera";
//	}
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
//	//-------------------------------------
//	// アニメーション制御
//	//-------------------------------------
//	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) || 
//		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) || 
//		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) || 
//		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
//		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
//		{
//			grandfather->PlayAnimation(FbxGrandfather::WALK);
//		}
//	}
//	else{
//		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
//		{
//			grandfather->PlayAnimation(FbxGrandfather::IDLE);
//		}
//	}
//	
//
//	//-------------------------------------
//	// 子供死亡時制御
//	//-------------------------------------
//	if (child_life < 0 && !child_death_){
//		child->PlayAnimation(FbxChild::DOWN);
//		child_death_ = true;
//		child_respawn_waittime_ = CHILD_RESPAWN_WAITTIME;
//		EFFECT_PARAMETER_DESC effect_param;
//		MyEffect *effect = effect_manager_->Get("dead");
//		effect_param = effect->parameter();
//		effect_param.position_ = child_position;
//		effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
//		effect->SetParameter(effect_param);
//		effect_manager_->Play("dead");
//
//	}
//	else if (child_death_ && !child_respawn_waittime_){
//		child->PlayAnimation(FbxChild::IDLE);
//		child_death_ = false;
//		child_life = CHILD_LIFE;
//		child->SetLife(child_life);
//		child_position = CHILD_POSITION1;
//		child_rotation.y_ = CHILD_ROTATION1;
//		child->SetPosition(child_position);
//		child->SetRotation(child_rotation);
//	}
//
//	child_respawn_waittime_--;
//	child_respawn_waittime_ = std::max<int>(child_respawn_waittime_, 0);
//
//	//-------------------------------------
//	// 子供体力自動回復制御
//	//-------------------------------------
//	if (child_life < 1.0f && !child_death_){
//		int child_recover_wait_timer = child->GetRecoverWaitTimer();
//		
//		if (child_recover_wait_timer > CHILD_RECOVER_WAITE_TIME){
//			float child_life = child->GetLife();
//			child_life += CHILD_RECOVER_HP;
//			std::min<float>(child_life, 1.0f);
//			child->SetLife(child_life);
//		}
//		child_recover_wait_timer++;
//		child->SetRecoverWaitTimer(child_recover_wait_timer);
//	}
//
//	//-------------------------------------
//	// ダメージエフェクトの処理
//	//-------------------------------------
//	// 今はテスト用に、子供に当てたら主観(おじ)のUIを反映させている
//	damage_effect->SetHP(child_life);
//
//
//	//-------------------------------------
//	// 各キャラクタ座標保存
//	//-------------------------------------
//	grandfather_prevposition = grandfather_position;
//
//
//	//-------------------------------------
//	// 影座標
//	//-------------------------------------
//	Object *shadow = object_manager_->Get("shadow");
//	Vector3 shadow_pos;
//	shadow_pos = grandfather->parameter().position_;
//	shadow_pos.y_ += 0.001f;
//	shadow->SetPosition(shadow_pos);

	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();

	if (KeyBoard::isTrigger(DIK_RETURN))
	{
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
