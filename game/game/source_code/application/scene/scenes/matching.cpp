//=========================================================
// Matching.cpp
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
#include "../../config/config.h"
#include "../../input/input.h"
#include "../../input/inputs/keyboard.h"
#include "../../input/inputs/gamepad.h"
#include "../../sound/sound.h"
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/sprite/sprite2d.h"
#include "../../object/objects/sprite/message/message.h"
#include "../../object/objects/mesh/field.h"
#include "../../object/objects/model/x_model.h"
#include "../../object/objects/model/x/instancing_tree.h"
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_player.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../object/objects/bullet/bullet.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../collision/collision.h"
#include "../../collision/collision_manager.h"
#include "../scene.h"
#include "../scene_manager.h"
#include "matching.h"
#include "../fade/fade.h"


//-------------------------------------
// warning
//-------------------------------------
#pragma warning (disable:4996)


//-------------------------------------
// Matching()
//-------------------------------------
Matching::Matching()
{
	//-------------------------------------
	// ゲームルール用パラメータ初期化
	//-------------------------------------
	// 経過フレーム数
	frame_ = 0;
	// 経過時間
	timer_ = 0;
	// シーン終了までのフレーム計測(マッチング完了時)
	movescene_waitframe_ = 0;


	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	collision_manager_ = new CollisionManager;
	effect_manager_ = EffectManager::Get();
	font_ = new DebugFont;
}

//-------------------------------------
// ~Matching()
//-------------------------------------
Matching::~Matching()
{
	setup_ = false;
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
	SAFE_DELETE(collision_manager_);
	effect_manager_->StopAll();
	effect_manager_ = nullptr;
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Matching::Initialize()
{
	//-------------------------------------
	// エフェクトの読み込み
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	effect_param.position_ = {0.0f, 0.0f, 0.0f};
	effect_param.rotation_ = {0.0f, 0.0f, 0.0f};
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;

	effect_manager_->Create(
		"water",
		"resource/effect/BulletFire.efk",
		effect_param);

	effect_manager_->Create(
		"fieldhit",
		"resource/effect/FieldHit2.efk",
		effect_param);

	effect_manager_->Create(
		"damage",
		"resource/effect/Damage3_3x0.5.efk",
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

	effect_manager_->Create(
		"SpeedDown",
		"resource/effect/SpeedDown.efk",
		effect_param);

	effect_manager_->Create(
		"water",
		"resource/effect/BulletFire.efk",
		effect_param);

	effect_manager_->Play("water");

	effect_param.position_ = {0.0f, 1000.0f, 0.0f};

	effect_manager_->Create(
		"marker",
		"resource/effect/Marker.efk",
		effect_param);

	effect_manager_->Play("marker");

	effect_param.position_ = PORTAL_POSITION;
	effect_manager_->Create(
		"portal",
		"resource/effect/Portal2x2.efk",
		effect_param);

	effect_manager_->Play("water");
	effect_manager_->Play("portal");

	//-------------------------------------
	// カメラ初期座標演算
	//-------------------------------------
	D3DXVECTOR3 camera_focus(34.94f, 0.0f, -34.39f);
	D3DXVECTOR3 camera_pos(camera_focus);
	float first_len(CAMERA_POS_LEN * 1.8f);
	camera_pos.x -= sinf(-1.55f + D3DX_PI) * first_len * cosf(D3DX_PI * -0.1f);
	camera_pos.z -= cosf(-1.55f + D3DX_PI) * first_len * cosf(D3DX_PI * -0.1f);
	camera_pos.y -= sinf(D3DX_PI * -0.1f) * CAMERA_POS_LEN;

	//-------------------------------------
	// メインカメラ
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = camera_pos;
	camera_param.focus_ = camera_focus;
	camera_param.position_.z -= 3.0f;
	camera_param.position_.y += 1.5f;
	camera_param.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;
	camera_pos_len_ = CAMERA_POS_LEN;

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
	// FBXおじ
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "player0";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = MATCHING_POSITION[0];
	grandfather_param.rotation_ = {0.0f, MATCHING_ROTATION[0], 0.0f};
	grandfather_param.scaling_ = {1.0f, 1.0f, 1.0f};

	object_manager_->Create(grandfather_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("player0");

	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_};
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

	collision_manager_->Create(object_manager_->Get("player0"), fbx_collision_param);

	//-------------------------------------
	// FBX子供
	//-------------------------------------
	OBJECT_PARAMETER_DESC child_param;
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.scaling_ = {1.0f, 1.0f, 1.0f};

	for(int i = 1; i < MAX_GUEST; i++)
	{
		std::string name = "player" + std::to_string(i);
		child_param.name_ = name;
		child_param.position_ = MATCHING_POSITION[i];
		child_param.rotation_ = {0.0f, MATCHING_ROTATION[i], 0.0f};
		object_manager_->Create(child_param);

		COLLISION_PARAMETER_DESC child_collision_param;
		Object *obj3 = object_manager_->Get(name);
		child_collision_param.position_ = {
			obj3->parameter().position_.x_,
			obj3->parameter().position_.y_,
			obj3->parameter().position_.z_};
		child_collision_param.range_ = 1.0f;
		child_collision_param.offset_ = {0.0f, 0.5f, 0.0f};

		collision_manager_->Create(object_manager_->Get(name),
			child_collision_param);
	}

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
	// 石碑
	//-------------------------------------
	OBJECT_PARAMETER_DESC parkstone_param;
	parkstone_param.name_ = "parkstone";
	parkstone_param.layer_ = LAYER_MODEL_X;
	parkstone_param.position_ = {34.94f, 0.0f, -34.39f};
	parkstone_param.rotation_ = {0.0f, -1.55f, 0.0f};
	parkstone_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		parkstone_param,
		"resource/model/x/parkstone.x");

	XModel *parkstone = dynamic_cast<XModel*>(
		object_manager_->Get("parkstone"));
	parkstone->SetTexture("resource/texture/matching/parkstone.png");


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
	for(int i = 0; i < MAX_BULLET; i++)
	{
		bomb_param.name_ = "bomb" + std::to_string(i);
		object_manager_->Create(
			bomb_param);
	}

	//-------------------------------------
	// 出撃準備案内UI
	//-------------------------------------
	OBJECT_PARAMETER_DESC standby_param;
	standby_param.name_ = "standby";
	standby_param.layer_ = LAYER_SPRITE_2D;
	standby_param.parent_layer_ = LAYER_NONE;
	standby_param.position_ = {
		SCREEN_WIDTH * 0.5f,
		-500.0f,
		0.0f
	};
	standby_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	standby_param.scaling_ = {
		SCREEN_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f,
		0.0f
	};

	Object *standby_object = object_manager_->Create(standby_param);
	Sprite2D *standby = dynamic_cast<Sprite2D*>(standby_object);
	standby->SetTexture("resource/texture/matching/standby.png");

	//-------------------------------------
	// UIの描画フラグを切る
	//-------------------------------------
	object_manager_->SetDrawEnable(LAYER_SPRITE_2D, false);

	//-------------------------------------
	// あれあれ
	//-------------------------------------
	OBJECT_PARAMETER_DESC search_param;
	search_param.name_ = "search";
	search_param.layer_ = LAYER_SPRITE_2D;
	search_param.position_ = {256.0f * 0.5f, 144.0f * 0.5f, 0.0f};
	search_param.scaling_ = {256.0f, 144.0f, 1.0f};
	object_manager_->Create(
		search_param,
		"./resource/texture/matching/search_host.png");

	sprite_alpha_ = 0.0f;
	alphar_wave_ = 0.0f;

	//-------------------------------------
	// セットアップ完了
	//-------------------------------------
	setup_ = true;
}


//-------------------------------------
// Update()
//-------------------------------------
void Matching::Update()
{
	// アルファ値変更
	alphar_wave_ += D3DX_PI * 0.02f;
	if (alphar_wave_ > D3DX_PI){
		alphar_wave_ -= D3DX_PI * 2.0f;
	}
	sprite_alpha_ = (sinf(alphar_wave_) + 1.0f) * 0.5f;

	//-------------------------------------
	// 親探索
	//-------------------------------------
	if(!NetworkGuest::disco_host())
	{
		frame_++;
		if(frame_ % (60 * 5) == 0)
		{
			NetworkGuest::TrySearchHost();
		}
		Sprite2D *search = dynamic_cast<Sprite2D*>(object_manager_->Get("search"));
		D3DXCOLOR search_color(1.0f, 1.0f, 1.0f, sprite_alpha_);
		search->SetColor(search_color);
		search->SetPosition({SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f});
	}
	else
	{
		Object* object = object_manager_->Get("search");
		object->SetPosition({-500.0f, -100.0f, 0.0f});

		Sprite2D *standby = dynamic_cast<Sprite2D*>(object_manager_->Get("standby"));
		D3DXCOLOR standby_color(1.0f, 1.0f, 1.0f, sprite_alpha_);
		standby->SetColor(standby_color);
		standby->SetPosition({SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f});
	}

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

		// 判定が取れたか
		bool hit_point_field = false;
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
				hit_point_field = true;
				break;
			}
		}
		// 判定外
		if(!hit_point_field)
		{
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = effect_manager_->Get("marker");
			effect_param = effect->parameter();
			effect_param.position_.y_ = -100.0f;
			effect->SetParameter(effect_param);
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
		SceneManager::RequestScene("Game");
	}
}


//-------------------------------------
// Draw()
//-------------------------------------
void Matching::Draw()
{
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(1.0f, 0.0f, 0.0f, 1.0f);
	MaterialColor color(32, 32, 32, 255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	Sprite2D *standby =
		dynamic_cast<Sprite2D*>(object_manager_->Get("standby"));
	standby->Draw();
	Sprite2D *search =
		dynamic_cast<Sprite2D*>(object_manager_->Get("search"));
	search->Draw();
	//collision_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
