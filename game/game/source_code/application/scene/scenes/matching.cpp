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
	camera_manager_ = CameraManager::Get();
	object_manager_ = ObjectManager::Get();
	effect_manager_ = EffectManager::Get();
	font_ = new DebugFont;

}

//-------------------------------------
// ~Matching()
//-------------------------------------
Matching::~Matching()
{
	object_manager_->Get("parkstone")->SetThisDelete(true);
	object_manager_->Get("standby")->SetThisDelete(true);
	object_manager_->Get("search")->SetThisDelete(true);

	object_manager_->DeleteCheck();
	object_manager_->ObjectUseOffLayer(LAYER_BULLET);
	object_manager_->ObjectUseOffLayer(LAYER_BOMB);
	object_manager_->ObjectUseOffLayer(LAYER_BLIND);

	SAFE_DELETE(font_);
	effect_manager_->StopAll();
}


//-------------------------------------
// Initialize()
//-------------------------------------
void Matching::Initialize()
{
	//-------------------------------------
//<<<<<<< HEAD
	// カメラ初期座標演算
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
//		"fieldhit",
//		"resource/effect/FieldHit2_2.efk",
//		effect_param);
//
//	effect_param.position_ = PORTAL_POSITION;
//	effect_manager_->Create(
//		"marker",
//		"resource/effect/Marker.efk",
//		effect_param);
//
//	effect_manager_->Create(
//		"portal",
//		"resource/effect/Portal2x2.efk",
//		effect_param);
//
//	effect_manager_->Play("water");
//	effect_manager_->Play("portal");
//	effect_manager_->Play("marker");
//	
//	//-------------------------------------
//	// メインカメラ
//	//-------------------------------------
//	CAMERA_PARAMETER_DESC camera_param;
//	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
//	camera_param.fovy_ = D3DX_PI * 0.25f;
//	camera_param.position_ = GRANDFATHER_POSITION;
//	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
//	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
//	camera_param.near_ = 0.1f;
//	camera_param.far_ = 1000.0f;
//	camera_pos_len_ = CAMERA_POS_LEN;
//
//	camera_manager_->Create(
//		"Perspective", "MainCamera", camera_param);
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
//>>>>>>> master
	//-------------------------------------
	D3DXVECTOR3 camera_focus(34.94f, 0.0f, -34.39f);
	D3DXVECTOR3 camera_pos(camera_focus);
	float first_len(CAMERA_POS_LEN * 1.8f);
	camera_pos.x -= sinf(-1.55f + D3DX_PI) * first_len * cosf(D3DX_PI * -0.1f);
	camera_pos.z -= cosf(-1.55f + D3DX_PI) * first_len * cosf(D3DX_PI * -0.1f);
	camera_pos.y -= sinf(D3DX_PI * -0.1f) * CAMERA_POS_LEN;

	Camera* main_camera = camera_manager_->Get("MainCamera");
	main_camera->SetPosition(camera_pos);
	main_camera->SetFocus(camera_focus);


	//-------------------------------------
	// FBXおじ
	//-------------------------------------
	Object *grandfather = object_manager_->Get("player0");
	grandfather->SetPosition(MATCHING_POSITION[0]);
	grandfather->SetRotationY(MATCHING_ROTATION[0]);

	//-------------------------------------
	// FBX子供
	//-------------------------------------
	for(int i = 1; i < MAX_GUEST; i++)
	{
		Object *child = object_manager_->Get("player" + std::to_string(i));
		child->SetPosition(MATCHING_POSITION[i]);
		child->SetRotationY(MATCHING_ROTATION[i]);
	}

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
//<<<<<<< HEAD
	// あれあれ
//=======
//	// サウンド(BGM)
//	//-------------------------------------
//	sound_ = nullptr;
//	sound_ = Sound::LoadSound("resource/sound/bgm/matching/matching_bgm.wav");
//	sound_->Play(true);
//
//}
//
//
////-------------------------------------
//// Update()
////-------------------------------------
//void Matching::Update()
//{
//	//-------------------------------------
//	// 変数宣言
//>>>>>>> master
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

		fort->SetPosition(fort_pos);
	}

	//-------------------------------------
	// マーカー
	//-------------------------------------
	EFFECT_PARAMETER_DESC effect_param;
	MyEffect *effect = effect_manager_->Get("marker");
	ZeroMemory(&effect_param, sizeof(effect_param));
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;
	effect->SetParameter(effect_param);
	effect_manager_->Play("marker");

	//-------------------------------------
	// ポータル
	//-------------------------------------
	effect = effect_manager_->Get("portal");
	ZeroMemory(&effect_param, sizeof(effect_param));
	effect_param.position_ = PORTAL_POSITION;
	effect_param.scaling_ = {1.0f, 1.0f, 1.0f};
	effect_param.speed_ = 1.0f;
	effect->SetParameter(effect_param);
	effect_manager_->Play("water");
	effect_manager_->Play("portal");
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
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
