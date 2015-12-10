//=========================================================
// game.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
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
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../object/objects/sprite/water_gage.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../collision/collision.h"
#include "../../collision/collision_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "../../config/config.h"
#include "game.h"
#include "../fade/fade.h"


//-------------------------------------
// Game()
//-------------------------------------
Game::Game()
{
	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = new EffectManager(10000);
	collision_manager_ = new CollisionManager;
	font1_ = new DebugFont;
	font2_ = new DebugFont;

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
		"warp",
		"resource/effect/Warp.efk",
		water_param);

	effect_manager_->Create(
		"dead",
		"resource/effect/Dead2.efk",
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
	// 地形
	//-------------------------------------
	OBJECT_PARAMETER_DESC param;
	param.position_ = { 0.0f, 0.0f, 0.0f };
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 200.0f, 1.0f, 200.0f };
	param.layer_ = LAYER_MESH_FIELD;

	object_manager_->Create(
		"field",
		param,
		"resource/mesh/map.heightmap");


	//-------------------------------------
	// 空
	//-------------------------------------
	param.position_ = { 0.0f, 0.0f, 0.0f };
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 500.0f, 1.0f, 500.0f };
	param.layer_ = LAYER_MESH_SKYDOME;

	object_manager_->Create(
		"skydome_up",
		param,
		"resource/mesh/skydome_up.txt");

	param.rotation_ = { 0.0f, 0.0f, D3DX_PI };

	object_manager_->Create(
		"skydome_bottom",
		param,
		"resource/mesh/skydome_bottom.txt");


	//-------------------------------------
	// 池
	//-------------------------------------
	param.position_ = { 0.0f, -0.5f, 0.0f };
	param.rotation_ = { 0.0f, 0.0f, 0.0f };
	param.scaling_ = { 30.0f, 1.0f, 30.0f };
	param.layer_ = LAYER_SPRITE_LAKE;

	object_manager_->Create(
		"lake",
		param);

	//-------------------------------------
	// 砦
	//-------------------------------------
	// 砦1
	OBJECT_PARAMETER_DESC fort1_param;
	fort1_param.layer_ = LAYER_MODEL_FORT;
	fort1_param.position_ = FORT1_POSITION;
	fort1_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort1_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"fort1",
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
	fort2_param.layer_ = LAYER_MODEL_FORT;
	fort2_param.position_ = FORT2_POSITION;
	fort2_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort2_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"fort2",
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
	fort3_param.layer_ = LAYER_MODEL_FORT;
	fort3_param.position_ = FORT3_POSITION;
	fort3_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fort3_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"fort3",
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
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION_STAGE1;
	grandfather_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	grandfather_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"grandfather",
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
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.position_ = CHILD_POSITION1;
	child_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	child_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"child",
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
	// タイマー
	//-------------------------------------
    OBJECT_PARAMETER_DESC time_param;
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
		"time",
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
    fort_state_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        100.0f,
        0.0f
    };
    fort_state_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    fort_state_param.scaling_ = { 160.0f, 40.0f, 0.0f };
    fort_state_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "fort_state", fort_state_param,
        "resource/texture/title/logo.png");


	//-------------------------------------
	// ミニマップUI
	//-------------------------------------
    OBJECT_PARAMETER_DESC mini_map_param;
    mini_map_param.position_ = {
        1180.0f,
        100.0f,
        0.0f
    };
    mini_map_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    mini_map_param.scaling_ = { 200.0f, 200.0f, 0.0f };
    mini_map_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "mini_map", mini_map_param,
        "resource/texture/title/logo.png");


    //-------------------------------------
    // 水ゲージ下地UI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_design_param;
    water_design_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_design_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_design_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_design_param.layer_ = LAYER_WATER_GAGE;

    object_manager_->Create(
        "water_design", water_design_param,
        "resource/texture/game/water_gage_background.png");

    //-------------------------------------
    // 水ゲージ（ゲージ本体）UI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_gage_param;
    water_gage_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_gage_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_gage_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_gage_param.layer_ = LAYER_WATER_GAGE;

    object_manager_->Create(
        "water_gage", water_gage_param,
        "resource/texture/game/water_gage_diffuse.png");

    //-------------------------------------
    // 水ゲージ（周り）UI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_gage_around_param;
    water_gage_around_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_gage_around_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_gage_around_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_gage_around_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "water_gage_around", water_gage_around_param,
        "resource/texture/game/water_gage_around.png");

    //-------------------------------------
    // 水ポリゴンUI
    //-------------------------------------
    OBJECT_PARAMETER_DESC water_poly_param;
    water_poly_param.position_ = {
        128.0f,
        624.0f,
        0.0f
    };
    water_poly_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_poly_param.scaling_ = { 192.0f, 192.0f, 0.0f };
    water_poly_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "water_poly", water_poly_param,
        "resource/texture/game/water_desine.png");

    //-------------------------------------
    // ダメージエフェクトUI
    //-------------------------------------
    OBJECT_PARAMETER_DESC hit_point_param;
    hit_point_param.position_ = {
        SCREEN_WIDTH * 0.5f,
        SCREEN_HEIGHT * 0.5f,
        0.0f
    };
    hit_point_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    hit_point_param.scaling_ = { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f };
    hit_point_param.layer_ = LAYER_DAMAGE_EFFECT;

    object_manager_->Create(
        "damage_effect", hit_point_param);

	//-------------------------------------
	// インスタンシングテスト
	//-------------------------------------
	/*OBJECT_PARAMETER_DESC wood_param;
	wood_param.layer_ = LAYER_TREE;
	object_manager_->Create(
		"wood", wood_param);*/


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
	//-------------------------------------
	// 影
	//-------------------------------------
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	object_manager_->Create(
		"shadow", shadow_param);

}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(effect_manager_);
	SAFE_DELETE(font1_);
	SAFE_DELETE(font2_);
	SAFE_DELETE(collision_manager_);
}


//-------------------------------------
// Update()
//-------------------------------------
void Game::Update()
{
	//-------------------------------------
	// 変数宣言
	//-------------------------------------
	Object *fort1_object = object_manager_->Get("fort1");
	Object *fort2_object = object_manager_->Get("fort2");
	Object *fort3_object = object_manager_->Get("fort3");
	Object *grandfather_object = object_manager_->Get("grandfather");
	Object *child_object = object_manager_->Get("child");
	Vector3 fort1_position(fort1_object->parameter().position_);
	Vector3 fort2_position(fort2_object->parameter().position_);
	Vector3 fort3_position(fort3_object->parameter().position_);
	Vector3 grandfather_position(grandfather_object->parameter().position_);
	Vector3 grandfather_rotation(grandfather_object->parameter().rotation_);
	Vector3 child_position(child_object->parameter().position_);
	Vector3 child_rotation(child_object->parameter().rotation_);
	static Vector3 grandfather_prevposition(grandfather_object->parameter().position_);

	Field *field = dynamic_cast<Field*>(
		object_manager_->Get("field"));
	FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(grandfather_object);
	FbxChild *child = dynamic_cast<FbxChild*>(child_object);
    WaterGage *waterGage = dynamic_cast<WaterGage*>(
		object_manager_->Get("water_gage"));
	DamageEffect *damage_effect = dynamic_cast<DamageEffect*>(
		object_manager_->Get("damage_effect"));
	XFort *fort1 = dynamic_cast<XFort*>(fort1_object);
	XFort *fort2 = dynamic_cast<XFort*>(fort2_object);
	XFort *fort3 = dynamic_cast<XFort*>(fort3_object);

	static const float player_speed_value = 0.05f;
	static int bullet_count = 0;
	static int shot_late = 0;
	static D3DXVECTOR3 fort_underground(0.0f, 0.0f, 0.0f);
	float player_speed = player_speed_value;
	float father_life = grandfather->GetLife();
	float father_watergauge = grandfather->GetWaterGauge();
	float child_life = child->GetLife();
	float child_watergauge = child->GetWaterGauge();
	float fort1_life = fort1->GetLife();
	float fort2_life = fort2->GetLife();
	float fort3_life = fort3->GetLife();

	//-------------------------------------
	// ゲームステージデバッグ
	//-------------------------------------
	if (fort1_life == 0.0f){
		stage_ = 2;
		if (fort2_life == 0.0f){
			stage_ = 3;
		}
	}


	//-------------------------------------
	// プレイヤー移動処理
	//-------------------------------------
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_11)){
		player_speed = player_speed_value * 2.0f;
	}
	grandfather_position.x_ += (
		cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
	grandfather_position.z_ -= (
		sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;

	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
		grandfather_rotation.y_ -= CHAR_ROT_SPEED;
		if (grandfather_rotation.y_ < D3DX_PI){
			grandfather_rotation.y_ += D3DX_PI * 2.0f;
		}
	}
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
		grandfather_rotation.y_ += CHAR_ROT_SPEED;
		if (grandfather_rotation.y_ > D3DX_PI){
			grandfather_rotation.y_ -= D3DX_PI * 2.0f;
		}
	}
	if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_7)){
		// ワープエフェクト再生（移動前）
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("warp");
		effect_param = effect->parameter();
		effect_param.position_ = grandfather_position;
		effect_param.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
		effect->SetParameter(effect_param);
		effect_manager_->Play("warp");

		switch(stage_){
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
		effect_manager_->Play("warp");
	}

	//-------------------------------------
	// デバッグ時のプレイヤー操作
	//-------------------------------------
#ifdef _DEBUG
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
	}
	if (KeyBoard::isPress(DIK_LEFT)){
		grandfather_rotation.y_ -= CHAR_ROT_SPEED;
		if (grandfather_rotation.y_ < -D3DX_PI){
			grandfather_rotation.y_ += D3DX_PI * 2.0f;
		}
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
		break;
	case 2:
		fort_underground.x -= 0.01f;
		fort_underground.y += 0.01f;
		fort_underground.z -= 0.01f;
		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
		fort_underground.y = std::min<float>(fort_underground.y, 0.0f);
		fort_underground.z = std::max<float>(fort_underground.z, -3.0f);
		break;
	case 3:
		fort_underground.x -= 0.01f;
		fort_underground.y -= 0.01f;
		fort_underground.z += 0.01f;
		fort_underground.x = std::max<float>(fort_underground.x, -3.0f);
		fort_underground.y = std::max<float>(fort_underground.y, -3.0f);
		fort_underground.z = std::min<float>(fort_underground.z, 0.0f);
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
	if (grandfather_position.y_ > 0.5f ||
		grandfather_position.y_ < -0.5f){
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
	// カメラ追従
	//-------------------------------------
	Camera *main_camera = camera_manager_->Get("MainCamera");
	D3DXVECTOR3 camera_position, camera_focus;
	D3DXVECTOR3 camera_rotation(main_camera->rotation());

	// 入力
#ifdef _DEBUG
	if(KeyBoard::isPress(DIK_UP)){
		camera_rotation.x -= CAMERA_ROT_SPEED;
		if(camera_rotation.x < -CAMERA_ROT_X_LIMIT){
			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
		}
	}
	if(KeyBoard::isPress(DIK_DOWN)){
		camera_rotation.x += CAMERA_ROT_SPEED;
		if(camera_rotation.x > CAMERA_ROT_X_LIMIT){
			camera_rotation.x = CAMERA_ROT_X_LIMIT;
		}
	}
#endif
	
	if(GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_UP)){
		camera_rotation.x -= CAMERA_ROT_SPEED;
		if(camera_rotation.x < -CAMERA_ROT_X_LIMIT){
			camera_rotation.x = -CAMERA_ROT_X_LIMIT;
		}
	}
	if(GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_DOWN)){
		camera_rotation.x += CAMERA_ROT_SPEED;
		if(camera_rotation.x > CAMERA_ROT_X_LIMIT){
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
	camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	// 注視点を基準にカメラ座標を設定
	camera_position = camera_focus;
	camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_;


	// カメラの地面めり込み回避処理
	D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;
	
	// 中間にカメラがめり込みそうなところが無いか検査
	bool camera_re_calculate = false;
	for (int i = 0; i < 10; ++i){
		// 中間地点を計算
		D3DXVECTOR3 lay_point = camera_position + vec_camera_to_focus * static_cast<float>(i) * 0.1f;
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
		camera_position.x -= sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
		camera_position.z -= cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
		camera_position.y -= sinf(camera_rotation.x) * camera_pos_len_;
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
		effect_param.position_.y_ += 0.5f;
		effect_param.rotation_ = grandfather_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");


		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bullet_param.position_ = grandfather_position;
		bullet_param.rotation_ = grandfather_rotation;

		// カメラの回転Xを利用
		bullet_param.rotation_.x_ = camera_rotation.x;

		bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };
		std::string str = "notice" + std::to_string(bullet_count);
		object_manager_->Create(
			str,
			bullet_param);
		bullet_count++;

		//-------------------------------------
		// 水ゲージを減少させる
		//-------------------------------------
		father_watergauge -= GRANDFATHER_SUB_WATERGAUGE;
		father_watergauge = std::max<float>(father_watergauge, 0.0f);
		grandfather->SetWaterGauge(father_watergauge);
		waterGage->SetChangeValue(father_watergauge);

		shot_late = 10;

	}
#ifdef _DEBUG
	if(KeyBoard::isPress(DIK_SPACE)){
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = grandfather_position;
		effect_param.position_.y_ += 0.5f;
		effect_param.rotation_ = grandfather_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");

		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
		bullet_param.position_ = grandfather_position;
		bullet_param.rotation_ = grandfather_rotation;
		bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

		// カメラの回転Xを利用
		bullet_param.rotation_.x_ = camera_rotation.x;

		std::string str = "notice" + std::to_string(bullet_count);
		object_manager_->Create(
			str,
			bullet_param);
		bullet_count++;
	}

	//-------------------------------------
	// デバッグ時のみ、水ゲージ回復
	//-------------------------------------
	if (KeyBoard::isPress(DIK_1)){
		father_watergauge += GRANDFATHER_SUB_WATERGAUGE;
		father_watergauge = std::min<float>(father_watergauge, 1.0f);
		grandfather->SetWaterGauge(father_watergauge);
		waterGage->SetChangeValue(father_watergauge);
	}
#endif //_DEBUG

	//-------------------------------------
	// アニメーション制御
	//-------------------------------------
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
		{
			grandfather->PlayAnimation(FbxGrandfather::WALK);
		}
	}
	else{
		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
		{
			grandfather->PlayAnimation(FbxGrandfather::IDLE);
		}
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
		child_position = CHILD_POSITION1;
		child_rotation.y_ = CHILD_ROTATION1;
		child->SetPosition(child_position);
		child->SetRotation(child_rotation);
	}

	child_respawn_waittime_--;
	child_respawn_waittime_ = std::max<int>(child_respawn_waittime_, 0);

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
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
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
		grandfather_position.x_, grandfather_position.y_, grandfather_position.z_);
	font1_->Add("ROTATION(Grandfather) : %3.2f %3.2f %3.2f\n",
		grandfather_rotation.x_, grandfather_rotation.y_, grandfather_rotation.z_);
	font1_->Add("OBJECT : %d\n", ObjectManager::GetCount());
	font1_->Add("EFFECT : %d\n", EffectManager::GetCount());
	font1_->Add("COLLISION : %d\n", CollisionManager::GetCount());

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
