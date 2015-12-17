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
#include "../../font/debug_font.h"
#include "../../object/object.h"
#include "../../object/object_manager.h"
#include "../../object/objects/sprite/sprite2d.h"
#include "../../object/objects/sprite/message.h"
#include "../../object/objects/mesh/field.h"
#include "../../object/objects/model/x_model.h"
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_player.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../object/objects/notice/bullet.h"
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

	//-------------------------------------
	// 各マネージャ・デバッグシステム初期化
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	collision_manager_ = new CollisionManager;
	effect_manager_ = EffectManager::Get();
	font_ = new DebugFont;

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

	water_param.position_ = { 40.00f, 0.00f, -40.00f };
	effect_manager_->Create(
		"marker",
		"resource/effect/Marker.efk",
		water_param);
	effect_manager_->Create(
		"portal",
		"resource/effect/Portal2x2.efk",
		water_param);

	//-------------------------------------
	// メインカメラ
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = GRANDFATHER_POSITION;
	camera_param.focus_ = GRANDFATHER_POSITION;
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
	// FBXおじ
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "player0";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION;
	grandfather_param.rotation_ = {0.0f, 4.65f, 0.0f};
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
	child_param.rotation_ = {0.0f, 4.65f, 0.0f};
	child_param.scaling_ = {1.0f, 1.0f, 1.0f};

	for(int i = 1; i < MAX_GUEST; i++)
	{
		std::string name = "player" + std::to_string(i);
		child_param.name_ = name;
		child_param.position_ = GRANDFATHER_POSITION;
		child_param.position_.x_ += i * 2.0f;
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
	// ベンチ
	//-------------------------------------
	OBJECT_PARAMETER_DESC bench_param;
	bench_param.name_ = "bench";
	bench_param.layer_ = LAYER_BENCH;
	object_manager_->Create(
		bench_param);

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
		bullet_param.name_ = "bullet" + std::to_string(i);;
		object_manager_->Create(
			bullet_param);
	}

	//-------------------------------------
	// ポータルエフェクト
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	MyEffect *effect = effect_manager_->Get("water");
	effect_param = effect->parameter();
	effect_param.position_ = {40.00f, 0.00f, -40.00f};
	effect_param.position_.y_ -= 100.0f;
	effect_param.rotation_ = {0.0f, 0.0f, 0.0f};
	effect->SetParameter(effect_param);
	effect_manager_->Play("water");

	effect = effect_manager_->Get("portal");
	effect_param = effect->parameter();
	effect_param.position_ = { 40.00f, 0.00f, -40.00f };
	effect_param.position_.y_ += 0.3f;
	effect_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	effect->SetParameter(effect_param);
	effect_manager_->Play("portal");

	//-------------------------------------
	// マーカー
	//-------------------------------------
	effect = effect_manager_->Get("marker");
	effect->SetParameter(effect_param);
	effect_manager_->Play("marker");

	//-------------------------------------
	// 木を生やす
	//-------------------------------------
	OBJECT_PARAMETER_DESC wood_param;
	wood_param.name_ = "wood";
	wood_param.layer_ = LAYER_TREE;
	object_manager_->Create(wood_param);

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
}


//-------------------------------------
// ~Matching()
//-------------------------------------
Matching::~Matching()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(font_);
	SAFE_DELETE(collision_manager_);
	effect_manager_->StopAll();
	effect_manager_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void Matching::Update()
{
	////-------------------------------------
	//// 変数宣言
	////-------------------------------------
	//// 動的変数
	//Object *grandfather_object = object_manager_->Get("grandfather");
	//Vector3 grandfather_position(grandfather_object->parameter().position_);
	//Vector3 grandfather_rotation(grandfather_object->parameter().rotation_);
	//Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
	//FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(grandfather_object);
	//Camera *main_camera = camera_manager_->Get("MainCamera");
	//D3DXVECTOR3 camera_position, camera_focus;
	//D3DXVECTOR3 camera_rotation(main_camera->rotation());
	//float player_speed = CHARANCTER_MOVESPEED;

	//// 静的変数
	//static Vector3 grandfather_prevposition(grandfather_object->parameter().position_);
	//static int shot_late = 0;

	////-------------------------------------
	//// 時間経過
	////-------------------------------------
	//frame_++;
	//if (!(frame_ % 60)){
	//	timer_++;
	//}

	////-------------------------------------
	//// 1回だけポータル再生
	////-------------------------------------
	//if (frame_ == 1){

	//	EFFECT_PARAMETER_DESC effect_param;
	//	MyEffect *effect = effect_manager_->Get("water");
	//	effect_param = effect->parameter();
	//	effect_param.position_ = grandfather_position;
	//	effect_param.position_.y_ -= 100.0f;
	//	effect_param.rotation_ = grandfather_rotation;
	//	effect->SetParameter(effect_param);
	//	effect_manager_->Play("water");

	//	effect = effect_manager_->Get("portal");
	//	effect_param = effect->parameter();
	//	effect_param.position_ = { 40.00f, 0.00f, -40.00f };;
	//	effect_param.position_.y_ += 0.3f;
	//	effect_param.rotation_ = grandfather_rotation;
	//	effect->SetParameter(effect_param);
	//	effect_manager_->Play("portal");

	//	effect = effect_manager_->Get("marker");
	//	effect->SetParameter(effect_param);
	//	effect_manager_->Play("marker");
	//}


	////-------------------------------------
	//// プレイヤー移動処理
	////-------------------------------------
	//grandfather_position.x_ += (
	//	cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
	//	sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
	//grandfather_position.z_ -= (
	//	sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
	//	cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;

	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
	//	grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
	//	if (grandfather_rotation.y_ < D3DX_PI){
	//		grandfather_rotation.y_ += D3DX_PI * 2.0f;
	//	}
	//}
	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
	//	grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
	//	if (grandfather_rotation.y_ > D3DX_PI){
	//		grandfather_rotation.y_ -= D3DX_PI * 2.0f;
	//	}
	//}

	//D3DXVECTOR3 grandfather_pos(
	//	grandfather_position.x_,
	//	grandfather_position.y_,
	//	grandfather_position.z_);
	//grandfather_position.y_ = field->GetHeight(grandfather_pos);
	//if (grandfather_position.y_ > 0.4f ||
	//	grandfather_position.y_ < -0.4f){
	//	grandfather_position = grandfather_prevposition;
	//}

	////-------------------------------------
	//// プレイヤー座標をXZ平面で領域指定
	////-------------------------------------
	//grandfather_position.z_ = std::min<float>(grandfather_position.z_, -35.0f);
	//grandfather_position.x_ = std::max<float>(grandfather_position.x_, 32.0f);

	//grandfather_object->SetPosition(grandfather_position);
	//grandfather_object->SetRotation(grandfather_rotation);


	////-------------------------------------
	//// アニメーション制御
	////-------------------------------------
	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) ||
	//	GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) ||
	//	GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) ||
	//	GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
	//	if (grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
	//	{
	//		grandfather->PlayAnimation(FbxGrandfather::WALK);
	//	}
	//}
	//else{
	//	if (grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
	//	{
	//		grandfather->PlayAnimation(FbxGrandfather::IDLE);
	//	}
	//}


	////-------------------------------------
	//// 全員集合したらシーン遷移
	////-------------------------------------
	//// 今はおじだけ集まったら遷移、全員分用意すること
	//D3DXVECTOR3 portal_posiiton = PORTAL_POSITION;
	//grandfather_pos = { grandfather_position.x_, grandfather_position.y_, grandfather_position.z_ };
	//D3DXVECTOR3 vec(grandfather_pos - portal_posiiton);
	//float distance = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	//if (distance < PORTAL_DISTANCE){
	//	effect_manager_->StopAll();
	//	SceneManager::RequestScene("Game");
	//}

	////-------------------------------------
	//// 各キャラクタ座標保存
	////-------------------------------------
	//grandfather_prevposition = grandfather_position;

	////-------------------------------------
	//// 影座標
	////-------------------------------------
	//Object *shadow = object_manager_->Get("shadow");
	//Vector3 shadow_pos;
	//shadow_pos = grandfather->parameter().position_;
	//shadow_pos.y_ += 0.001f;
	//shadow->SetPosition(shadow_pos);

	////-------------------------------------
	//// カメラ計算
	////-------------------------------------

	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_UP)){
	//	camera_rotation.x += CAMERA_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsy_;
	//	if (camera_rotation.x < -CAMERA_ROT_X_LIMIT){
	//		camera_rotation.x = -CAMERA_ROT_X_LIMIT;
	//	}
	//}
	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_DOWN)){
	//	camera_rotation.x += CAMERA_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsy_;
	//	if (camera_rotation.x > CAMERA_ROT_X_LIMIT){
	//		camera_rotation.x = CAMERA_ROT_X_LIMIT;
	//	}
	//}

	//// モデルの回転Yをそのままカメラの回転Yへ
	//camera_rotation.y = grandfather_rotation.y_;
	//// 一旦モデルを注視点に
	//camera_focus = grandfather_pos;
	//// 足元基準から体の中心辺りを基準に
	//camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	//// モデルの少し先を見るように調整
	//camera_focus.x +=
	//	sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	//camera_focus.z +=
	//	cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	//camera_focus.y +=
	//	sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	//// 注視点を基準にカメラ座標を設定
	//camera_position = camera_focus;
	//camera_position.x -=
	//	sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	//camera_position.z -=
	//	cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	//camera_position.y -=
	//	sinf(camera_rotation.x) * camera_pos_len_;


	//// カメラの地面めり込み回避処理
	//D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

	//// 中間にカメラがめり込みそうなところが無いか検査
	//bool camera_re_calculate = false;
	//for (int i = 0; i < 10; ++i){
	//	// 中間地点を計算
	//	D3DXVECTOR3 lay_point =
	//		camera_position + vec_camera_to_focus * static_cast<float>(i)* 0.1f;
	//	float pos_y = field->GetHeight(lay_point);
	//	// 回避処理
	//	if (lay_point.y < pos_y + 0.1f){
	//		camera_re_calculate = true;
	//		camera_pos_len_ -= CAMARA_LEN_SPEED;
	//	}
	//}

	////カメラ座標再計算
	//if (camera_re_calculate == true){
	//	camera_position = camera_focus;
	//	camera_position.x -=
	//		sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	//	camera_position.z -=
	//		cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	//	camera_position.y -=
	//		sinf(camera_rotation.x) * camera_pos_len_;
	//	camera_position.y = field->GetHeight(camera_position) + 0.1f;
	//}

	//camera_pos_len_ += CAMARA_LEN_SPEED;
	//if (camera_pos_len_ > CAMERA_POS_LEN){
	//	camera_pos_len_ = CAMERA_POS_LEN;
	//}

	//// カメラにパラメータを再セット
	//main_camera->SetPosition(camera_position);
	//main_camera->SetFocus(camera_focus);
	//main_camera->SetRotation(camera_rotation);


	////-------------------------------------
	//// 弾発射
	////-------------------------------------
	//shot_late--;
	//shot_late = std::max<int>(shot_late, 0);
	//if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_8) &&
	//	shot_late == 0){
	//	EFFECT_PARAMETER_DESC effect_param;
	//	MyEffect *effect = effect_manager_->Get("water");
	//	effect_param = effect->parameter();
	//	effect_param.position_ = grandfather_position;
	//	effect_param.position_.y_ += 0.6f;
	//	effect_param.rotation_ = grandfather_rotation;
	//	effect->SetParameter(effect_param);
	//	effect_manager_->Play("water");


	//	OBJECT_PARAMETER_DESC bullet_param;
	//	bullet_param.layer_ = LAYER_BULLET;
	//	bullet_param.parent_layer_ = LAYER_MODEL_GRANDFATHER;
	//	bullet_param.position_ = grandfather_position;
	//	bullet_param.position_.y_ += 0.6f;
	//	bullet_param.rotation_ = grandfather_rotation;

	//	// カメラの回転Xを利用
	//	bullet_param.rotation_.x_ = camera_rotation.x;

	//	bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	//	Bullet* bullet = object_manager_->GetNoUseBullet();
	//	bullet->Fire(bullet_param);

	//	shot_late = 10;
	//}

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
	}
	else
	{
		Object* search = object_manager_->Get("search");
		search->SetPosition({-500.0f, -100.0f, 0.0f});
	}

	//-------------------------------------
	// マーカー
	//-------------------------------------
	int player_id =	NetworkGuest::id();
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
		Vector3 speed = { BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ };
		Vector3 rot = Vector3(camera_rotation.x, camera_rotation.y, camera_rotation.z);
		// 回転値を少し調整
		rot.x_ += BULLET_OFFSET_ROT;
		// 回転値を参照して速度を改良
		speed.y_ += sinf(rot.x_) * BULLET_ADD_SPEED_Y;

		for (int i = 0; i < 120; i++)
		{
			poseffect.x_ += sinf(rot.y_) * speed.x_;
			poseffect.y_ += speed.y_;
			poseffect.z_ += cosf(rot.y_) * speed.z_;
			speed.y_ -= BULLET_GRAVITY;

			float height = field->GetHeight(D3DXVECTOR3(poseffect.x_, poseffect.y_, poseffect.z_));
			if (height > poseffect.y_)
			{
				EFFECT_PARAMETER_DESC effect_param;
				MyEffect *effect = effect_manager_->Get("marker");
				effect_param = effect->parameter();
				effect_param.position_ = poseffect;
				effect_param.position_.y_ = height;
				effect_param.rotation_ = { 0.0f, camera_rotation.y, 0.0f };
				Vector3 vec = poseffect - player_pos;
				float len = sqrt(vec.x_*vec.x_ + vec.z_ * vec.z_);
				float scaling = 1.0f + len * 0.15f;

				scaling = std::min<float>(scaling, 3.0f);

				effect_param.scaling_ = { scaling, scaling, scaling };
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
	//collision_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
