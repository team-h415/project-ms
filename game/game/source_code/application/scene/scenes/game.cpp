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
    debuff_effect_flg = false;

	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = EffectManager::Get();
	collision_manager_ = new CollisionManager;
    object_manager_->SetDrawEnable(LAYER_DAMAGE_EFFECT, false);
}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	setup_ = false;
	effect_manager_->StopAll();
	effect_manager_ = NULL;
	sound_->ReleaseSound(&sound_);
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(collision_manager_);
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Game::Initialize()
{
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

	camera_manager_->Create(
		"Perspective", "MainCamera", camera_param);


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
	skydome_param.position_ = { 0.0f, -1.0f, 0.0f };
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
		std::string name = "fort" + std::to_string(i);
		fort_param.name_ = name;
		fort_param.layer_ = LAYER_MODEL_FORT;
		fort_param.position_ = fort_pos;
		fort_param.rotation_ = {0.0f, 0.0f, 0.0f};
		fort_param.scaling_ = {1.0f, 1.0f, 1.0f};

		object_manager_->Create(
			fort_param,
			"resource/model/x/fort.x");

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
	grandfather_param.name_ = "player0";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION_STAGE[0];
	grandfather_param.rotation_ = {0.0f, GRANDFATHER_ROTATION_STAGE[0], 0.0f};
	grandfather_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("player0");

	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_ };
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("player0"), fbx_collision_param);

	//-------------------------------------
	// FBX子供
	//-------------------------------------
	OBJECT_PARAMETER_DESC child_param;
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.scaling_ = { 1.0f, 1.0f, 1.0f };
	for(int i = 1; i < MAX_GUEST; i++)
	{
		std::string name = "player" + std::to_string(i);
		child_param.name_ = name;
		child_param.position_ = CHILD_POSITION[i];
		child_param.rotation_ = {0.0f, CHILD_ROTATION[i], 0.0f};
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
		"resource/texture/game/water_gage_background.pmg");
		//"resource/texture/game/water_gage_background_" + std::to_string(my_id) + ".png");

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
		"resource/texture/game/water_gage_around_" + std::to_string(my_id) + ".png");

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
		"resource/texture/game/water_desine_" + std::to_string(my_id) + ".png");

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
	object_manager_->Create(bench_param);

	//-------------------------------------
	// 影
	//-------------------------------------
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < MAX_GUEST; i++)
	{
		shadow_param.name_ = "shadow" + std::to_string(i);
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
	// マーカー
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	ZeroMemory(&effect_param, sizeof(effect_param));
	MyEffect *effect = effect_manager_->Get("marker");
	effect_param = effect->parameter();
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;
	effect->SetParameter(effect_param);
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
	effect_manager_->Play(speed_down_name);

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

#ifdef NETWORK_HOST_MODE
#else
	NETWORK_DATA network_data;
	network_data.type_ = DATA_COMPLETE_SCENE_CHANGE;
	NetworkGuest::SendTo(network_data);
#endif

	//-------------------------------------
	// セットアップ完了
	//-------------------------------------
	setup_ = true;
}


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
	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	damage_effect->Draw();
	//collision_manager_->Draw();

	Fade::Draw();

	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
