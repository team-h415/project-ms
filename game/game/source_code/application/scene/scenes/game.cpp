//=========================================================
// game.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
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
#include "../../object/objects/model/fbx_model.h"
#include "../../object/objects/model/fbx/fbx_grandfather.h"
#include "../../effect/effect.h"
#include "../../effect/effect_manager.h"
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
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	effect_manager_ = new EffectManager(5000);
	collision_manager_ = new CollisionManager;
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
	camera_param.far_ = 100000.0f;

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
		"resource/mesh/test.heightmap");


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
	// Xモデル
	//-------------------------------------
	OBJECT_PARAMETER_DESC player_param;
	player_param.layer_ = LAYER_MODEL_X;
	player_param.position_ = { -5.0f, 0.0f, 0.0f };
	player_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	player_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"player",
		player_param,
		"resource/model/x/pone_red.x");

	COLLISION_PARAMETER_DESC player_collision_param;
	Object *obj = object_manager_->Get("player");
	player_collision_param.position_ = {
		obj->parameter().position_.x_,
		obj->parameter().position_.y_ + 0.5f,
		obj->parameter().position_.z_ };
	player_collision_param.range_ = 1.0f;
	player_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("player"),
		player_collision_param);


	//-------------------------------------
	// FBXモデル
	//-------------------------------------
	OBJECT_PARAMETER_DESC fbx_param;
	fbx_param.layer_ = LAYER_MODEL_GRANDFATHER;
	fbx_param.position_ = { 1.0f, 0.0f, 0.0f };
	fbx_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	fbx_param.scaling_ = { 1.0f, 1.0f, 1.0f };

	object_manager_->Create(
		"fbx",
		fbx_param);

	COLLISION_PARAMETER_DESC fbx_collision_param;
	Object *obj2 = object_manager_->Get("fbx");
	fbx_collision_param.position_ = {
		obj2->parameter().position_.x_,
		obj2->parameter().position_.y_,
		obj2->parameter().position_.z_ };
	fbx_collision_param.range_ = 1.0f;
	fbx_collision_param.offset_ = { 0.0f, 0.5f, 0.0f };

	collision_manager_->Create(object_manager_->Get("fbx"),
		fbx_collision_param);

	//-------------------------------------
	// FBX子供
	OBJECT_PARAMETER_DESC child_param;
	child_param.layer_ = LAYER_MODEL_CHILD;
	child_param.position_ = { -1.0f, 0.0f, 0.0f };
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
    time_param.scaling_ = { 160.0f, 80.0f, 0.0f };
    time_param.layer_ = LAYER_TIMER;

    object_manager_->Create(
        "time", time_param);


	//-------------------------------------
	// 砦
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
	// ミニマップ
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
	// 水
	//-------------------------------------
    OBJECT_PARAMETER_DESC water_design_param;
    water_design_param.position_ = {
        40.0f,
        680.0f,
        0.0f
    };
    water_design_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_design_param.scaling_ = { 75.0f, 75.0f, 0.0f };
    water_design_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "water_design", water_design_param,
        "resource/texture/title/logo.png");


	//-------------------------------------
	// 水ゲージ
	//-------------------------------------
    OBJECT_PARAMETER_DESC water_gage_param;
    water_gage_param.position_ = {
        250.0f,
        680.0f,
        0.0f
    };
    water_gage_param.rotation_ = { 0.0f, 0.0f, 0.0f };
    water_gage_param.scaling_ = { 300.0f, 50.0f, 0.0f };
    water_gage_param.layer_ = LAYER_SPRITE_2D;

    object_manager_->Create(
        "water_gage", water_gage_param);


    //-------------------------------------
    // ダメージエフェクト
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

}


//-------------------------------------
// ~Game()
//-------------------------------------
Game::~Game()
{
	SAFE_DELETE(object_manager_);
	SAFE_DELETE(camera_manager_);
	SAFE_DELETE(effect_manager_);
	SAFE_DELETE(font_);
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
	Object *player = object_manager_->Get("player");
	Object *fbx = object_manager_->Get("fbx");
	Object *child = object_manager_->Get("child");
	Vector3 player_position(player->parameter().position_);
	Vector3 player_rotation(player->parameter().rotation_);
	Vector3 fbx_position(fbx->parameter().position_);
	Vector3 fbx_rotation(fbx->parameter().rotation_);
	Vector3 child_position(child->parameter().position_);

	Field *field = dynamic_cast<Field*>(
		object_manager_->Get("field"));

	static const float player_speed_value = 0.05f;
	static int bullet_count = 0;
	float player_speed = player_speed_value;

	//-------------------------------------
	// プレイヤーを地形に沿って移動させる
	//-------------------------------------
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_11)){
		player_speed = player_speed_value * 2.0f;
	}
	fbx_position.x_ += (
		cosf(fbx_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		sinf(-fbx_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
	fbx_position.z_ -= (
		sinf(fbx_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		cosf(-fbx_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;

	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
		fbx_rotation.y_ -= CHAR_ROT_SPEED;
		if (fbx_rotation.y_ < D3DX_PI){
			fbx_rotation.y_ += D3DX_PI * 2.0f;
		}
	}
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
		fbx_rotation.y_ += CHAR_ROT_SPEED;
		if (fbx_rotation.y_ > D3DX_PI){
			fbx_rotation.y_ -= D3DX_PI * 2.0f;
		}
	}

	//-------------------------------------
	// デバッグ時のプレイヤー操作
	//-------------------------------------
#ifdef _DEBUG
	if (KeyBoard::isPress(DIK_W)){
		fbx_position.x_ += sinf(fbx_rotation.y_) * player_speed;
		fbx_position.z_ += cosf(fbx_rotation.y_) * player_speed;
	}
	if (KeyBoard::isPress(DIK_A)){
		fbx_position.x_ += sinf(fbx_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
		fbx_position.z_ += cosf(fbx_rotation.y_ - (D3DX_PI * 0.5f)) * player_speed;
	}
	if (KeyBoard::isPress(DIK_S)){
		fbx_position.x_ += sinf(fbx_rotation.y_ + (D3DX_PI)) * player_speed;
		fbx_position.z_ += cosf(fbx_rotation.y_ + (D3DX_PI)) * player_speed;
	}
	if (KeyBoard::isPress(DIK_D)){
		fbx_position.x_ += sinf(fbx_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
		fbx_position.z_ += cosf(fbx_rotation.y_ + (D3DX_PI * 0.5f)) * player_speed;
	}
	if (KeyBoard::isPress(DIK_RIGHT)){
		fbx_rotation.y_ += CHAR_ROT_SPEED;
		if (fbx_rotation.y_ > D3DX_PI){
			fbx_rotation.y_ -= D3DX_PI * 2.0f;
		}
	}
	if (KeyBoard::isPress(DIK_LEFT)){
		fbx_rotation.y_ -= CHAR_ROT_SPEED;
		if (fbx_rotation.y_ < -D3DX_PI){
			fbx_rotation.y_ += D3DX_PI * 2.0f;
		}
	}

#endif //_DEBUG

	D3DXVECTOR3 pos(
		player_position.x_,
		player_position.y_,
		player_position.z_);
	player_position.y_ = field->GetHeight(pos);

	D3DXVECTOR3 fbx_pos(
		fbx_position.x_,
		fbx_position.y_,
		fbx_position.z_);
	fbx_position.y_ = field->GetHeight(fbx_pos);
	
	D3DXVECTOR3 child_pos(
		child_position.x_,
		child_position.y_,
		child_position.z_);
	child_position.y_ = field->GetHeight(child_pos);

	player->SetPosition(player_position);
	player->SetRotation(player_rotation);
	fbx->SetPosition(fbx_position);
	fbx->SetRotation(fbx_rotation);
	child->SetPosition(child_position);

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

	// モデルの回転Yをそのままカメラの回転Yへ
	camera_rotation.y = fbx_rotation.y_;
	// 一旦モデルを注視点に
	camera_focus = fbx_pos;
	// 足元基準から体の中心辺りを基準に
	camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	// モデルの少し先を見るように調整
	camera_focus.x += sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.z += cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.y += sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	// 注視点を基準にカメラ座標を設定
	camera_position = camera_focus;
	camera_position.x -= sinf(camera_rotation.y) * CAMERA_POS_LEN * cosf(camera_rotation.x);
	camera_position.z -= cosf(camera_rotation.y) * CAMERA_POS_LEN * cosf(camera_rotation.x);
	camera_position.y -= sinf(camera_rotation.x) * CAMERA_POS_LEN;

	// カメラにパラメータを再セット
	main_camera->SetPosition(camera_position);
	main_camera->SetFocus(camera_focus);
	main_camera->SetRotation(camera_rotation);

	//-------------------------------------
	// エフェクト再生
	//-------------------------------------
	if (KeyBoard::isTrigger(DIK_1)){
		effect_manager_->Play("water");
	}
	if (GamePad::isTrigger(GAMEPAD_GRANDFATHER, PAD_BUTTON_8)){
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = fbx_position;
		effect_param.position_.y_ += 0.5f;
		effect_param.rotation_ = fbx_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");


		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.position_ = fbx_position;
		bullet_param.rotation_ = fbx_rotation;

		// カメラの回転Xを利用
		bullet_param.rotation_.x_ = camera_rotation.x;

		bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };
		std::string str = "notice" + std::to_string(bullet_count);
		object_manager_->Create(
			str,
			bullet_param);
		bullet_count++;
	}
#ifdef _DEBUG
	if(KeyBoard::isPress(DIK_SPACE)){
		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = fbx_position;
		effect_param.position_.y_ += 0.5f;
		effect_param.rotation_ = fbx_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");

		OBJECT_PARAMETER_DESC bullet_param;
		bullet_param.layer_ = LAYER_BULLET;
		bullet_param.position_ = fbx_position;
		bullet_param.rotation_ = fbx_rotation;
		bullet_param.scaling_ = {1.0f, 1.0f, 1.0f};

		// カメラの回転Xを利用
		bullet_param.rotation_.x_ = camera_rotation.x;

		std::string str = "notice" + std::to_string(bullet_count);
		object_manager_->Create(
			str,
			bullet_param);
		bullet_count++;
	}
#endif //_DEBUG


	//-------------------------------------
	// アニメーション制御
	//-------------------------------------
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) || 
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
		FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(fbx);
		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
		{
			grandfather->PlayAnimation(FbxGrandfather::WALK);
		}
	}
	else{
		FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(fbx);
		if(grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
		{
			grandfather->PlayAnimation(FbxGrandfather::IDLE);
		}
	}
	
	if (KeyBoard::isTrigger(DIK_5)){
		FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(fbx);
		grandfather->PlayAnimation(FbxGrandfather::DOWN);
	}

	//-------------------------------------
	// 実更新処理
	//-------------------------------------
	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();
	collision_manager_->Update();

	font_->Add("シーン名:");
	font_->Add("Game\n");

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
	RECT rect = {
		0, 0,
		static_cast<LONG>(SCREEN_WIDTH),
		static_cast<LONG>(SCREEN_HEIGHT) };
	D3DXCOLOR font_color(0.0f, 1.0f, 1.0f, 1.0f);
	MaterialColor color(32, 32, 32, 0);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	camera_manager_->Set("MainCamera");
	object_manager_->Draw();
	effect_manager_->Draw();
	collision_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
