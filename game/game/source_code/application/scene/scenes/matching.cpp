//=========================================================
// Matching.cpp
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
#include "../../config/config.h"
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
	// �Q�[�����[���p�p�����[�^������
	//-------------------------------------
	// �o�߃t���[����
	frame_ = 0;
	// �o�ߎ���
	timer_ = 0;

	//-------------------------------------
	// �e�}�l�[�W���E�f�o�b�O�V�X�e��������
	//-------------------------------------
	camera_manager_ = new CameraManager;
	object_manager_ = new ObjectManager;
	collision_manager_ = new CollisionManager;
	effect_manager_ = EffectManager::Get();
	font_ = new DebugFont;


	//-------------------------------------
	// �G�t�F�N�g�̓ǂݍ���
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

	//effect_manager_->Create(
	//	"damage",
	//	"resource/effect/Damage3_2.efk",
	//	water_param);

	//effect_manager_->Create(
	//	"dead",
	//	"resource/effect/Dead2.efk",
	//	water_param);

	//effect_manager_->Create(
	//	"smoke",
	//	"resource/effect/Smoke.efk",
	//	water_param);

	//effect_manager_->Create(
	//	"smoke2",
	//	"resource/effect/Smoke2.efk",
	//	water_param);

	//effect_manager_->Create(
	//	"dash",
	//	"resource/effect/Dash.efk",
	//	water_param);

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
	// ���C���J����
	//-------------------------------------
	CAMERA_PARAMETER_DESC camera_param;
	camera_param.acpect_ = SCREEN_WIDTH / SCREEN_HEIGHT;
	camera_param.fovy_ = D3DX_PI * 0.25f;
	camera_param.position_ = GRANDFATHER_POSITION;
	camera_param.focus_ = { 0.0f, 0.0f, 0.0f };
	camera_param.rotation_ = { 0.0f, 0.0f, 0.0f };
	camera_param.up_ = { 0.0f, 1.0f, 0.0f };
	camera_param.near_ = 0.1f;
	camera_param.far_ = 1000.0f;
	camera_pos_len_ = CAMERA_POS_LEN;

	camera_manager_->Create(
		"Perspective", "MainCamera", camera_param);

	
	//-------------------------------------
	// �n�`
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
	// ��
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
	// �r
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
	// FBX����
	//-------------------------------------
	OBJECT_PARAMETER_DESC grandfather_param;
	grandfather_param.name_ = "grandfather";
	grandfather_param.layer_ = LAYER_MODEL_GRANDFATHER;
	grandfather_param.position_ = GRANDFATHER_POSITION;
	grandfather_param.rotation_ = { 0.0f, 4.65f, 0.0f };
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
	// �x���`
	//-------------------------------------
	OBJECT_PARAMETER_DESC bench_param;
	bench_param.name_ = "bench";
	bench_param.layer_ = LAYER_BENCH;
	object_manager_->Create(
		bench_param);

	//-------------------------------------
	// �e
	//-------------------------------------
	OBJECT_PARAMETER_DESC shadow_param;
	shadow_param.name_ = "shadow";
	shadow_param.layer_ = LAYER_SHADOW;
	shadow_param.scaling_ = Vector3(1.0f, 1.0f, 1.0f);
	object_manager_->Create(
		shadow_param);

	//-------------------------------------
	// �o���b�g�������Ă�����
	//-------------------------------------
	OBJECT_PARAMETER_DESC bullet_param;
	bullet_param.layer_ = LAYER_BULLET;
	for (int i = 0; i < MAX_BULLET; i++)
	{
		bullet_param.name_ = "bullet" + std::to_string(i);;
		object_manager_->Create(
			bullet_param);
	}

	
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
	effect_manager_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void Matching::Update()
{
	//-------------------------------------
	// �ϐ��錾
	//-------------------------------------
	// ���I�ϐ�
	Object *grandfather_object = object_manager_->Get("grandfather");
	Vector3 grandfather_position(grandfather_object->parameter().position_);
	Vector3 grandfather_rotation(grandfather_object->parameter().rotation_);
	Field *field = dynamic_cast<Field*>(object_manager_->Get("field"));
	FbxGrandfather *grandfather = dynamic_cast<FbxGrandfather*>(grandfather_object);
	Camera *main_camera = camera_manager_->Get("MainCamera");
	D3DXVECTOR3 camera_position, camera_focus;
	D3DXVECTOR3 camera_rotation(main_camera->rotation());
	float player_speed = CHARANCTER_MOVESPEED;

	// �ÓI�ϐ�
	static Vector3 grandfather_prevposition(grandfather_object->parameter().position_);
	static int shot_late = 0;

	//-------------------------------------
	// ���Ԍo��
	//-------------------------------------
	frame_++;
	if (!(frame_ % 60)){
		timer_++;
	}

	//-------------------------------------
	// 1�񂾂��|�[�^���Đ�
	//-------------------------------------
	if (frame_ == 1){

		EFFECT_PARAMETER_DESC effect_param;
		MyEffect *effect = effect_manager_->Get("water");
		effect_param = effect->parameter();
		effect_param.position_ = grandfather_position;
		effect_param.position_.y_ -= 100.0f;
		effect_param.rotation_ = grandfather_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("water");

		effect = effect_manager_->Get("portal");
		effect_param = effect->parameter();
		effect_param.position_ = { 40.00f, 0.00f, -40.00f };;
		effect_param.position_.y_ += 0.3f;
		effect_param.rotation_ = grandfather_rotation;
		effect->SetParameter(effect_param);
		effect_manager_->Play("portal");

		effect = effect_manager_->Get("marker");
		effect->SetParameter(effect_param);
		effect_manager_->Play("marker");
	}


	//-------------------------------------
	// �v���C���[�ړ�����
	//-------------------------------------
	grandfather_position.x_ += (
		cosf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		sinf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;
	grandfather_position.z_ -= (
		sinf(grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsx_ +
		cosf(-grandfather_rotation.y_) * GamePad::isStick(GAMEPAD_GRANDFATHER).lsy_) * player_speed;

	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_LEFT)){
		grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
		if (grandfather_rotation.y_ < D3DX_PI){
			grandfather_rotation.y_ += D3DX_PI * 2.0f;
		}
	}
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_RS_RIGHT)){
		grandfather_rotation.y_ += CHAR_ROT_SPEED*GamePad::isStick(GAMEPAD_GRANDFATHER).rsx_;
		if (grandfather_rotation.y_ > D3DX_PI){
			grandfather_rotation.y_ -= D3DX_PI * 2.0f;
		}
	}

	D3DXVECTOR3 grandfather_pos(
		grandfather_position.x_,
		grandfather_position.y_,
		grandfather_position.z_);
	grandfather_position.y_ = field->GetHeight(grandfather_pos);
	if (grandfather_position.y_ > 0.4f ||
		grandfather_position.y_ < -0.4f){
		grandfather_position = grandfather_prevposition;
	}

	//-------------------------------------
	// �v���C���[���W��XZ���ʂŗ̈�w��
	//-------------------------------------
	grandfather_position.z_ = std::min<float>(grandfather_position.z_, -35.0f);
	grandfather_position.x_ = std::max<float>(grandfather_position.x_, 32.0f);

	grandfather_object->SetPosition(grandfather_position);
	grandfather_object->SetRotation(grandfather_rotation);


	//-------------------------------------
	// �A�j���[�V��������
	//-------------------------------------
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_DOWN) ||
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_UP) ||
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_LEFT) ||
		GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_LS_RIGHT)){
		if (grandfather->GetCurrentAnimationId() != FbxGrandfather::WALK)
		{
			grandfather->PlayAnimation(FbxGrandfather::WALK);
		}
	}
	else{
		if (grandfather->GetCurrentAnimationId() != FbxGrandfather::IDLE)
		{
			grandfather->PlayAnimation(FbxGrandfather::IDLE);
		}
	}


	//-------------------------------------
	// �S���W��������V�[���J��
	//-------------------------------------
	// ���͂��������W�܂�����J�ځA�S�����p�ӂ��邱��
	D3DXVECTOR3 portal_posiiton = PORTAL_POSITION;
	grandfather_pos = { grandfather_position.x_, grandfather_position.y_, grandfather_position.z_ };
	D3DXVECTOR3 vec(grandfather_pos - portal_posiiton);
	float distance = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	if (distance < PORTAL_DISTANCE){
		effect_manager_->StopAll();
		SceneManager::RequestScene("Game");
	}

	//-------------------------------------
	// �e�L�����N�^���W�ۑ�
	//-------------------------------------
	grandfather_prevposition = grandfather_position;

	//-------------------------------------
	// �e���W
	//-------------------------------------
	Object *shadow = object_manager_->Get("shadow");
	Vector3 shadow_pos;
	shadow_pos = grandfather->parameter().position_;
	shadow_pos.y_ += 0.001f;
	shadow->SetPosition(shadow_pos);

	//-------------------------------------
	// �J�����v�Z
	//-------------------------------------

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

	// ���f���̉�]Y�����̂܂܃J�����̉�]Y��
	camera_rotation.y = grandfather_rotation.y_;
	// ��U���f���𒍎��_��
	camera_focus = grandfather_pos;
	// ���������̂̒��S�ӂ�����
	camera_focus.y += CAMERA_FOCUS_OFFSET_Y;
	// ���f���̏����������悤�ɒ���
	camera_focus.x +=
		sinf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.z +=
		cosf(camera_rotation.y) * CAMERA_FOCUS_OFFSET * cosf(camera_rotation.x);
	camera_focus.y +=
		sinf(camera_rotation.x) * CAMERA_FOCUS_OFFSET;

	// �����_����ɃJ�������W��ݒ�
	camera_position = camera_focus;
	camera_position.x -=
		sinf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	camera_position.z -=
		cosf(camera_rotation.y) * camera_pos_len_ * cosf(camera_rotation.x);
	camera_position.y -=
		sinf(camera_rotation.x) * camera_pos_len_;


	// �J�����̒n�ʂ߂荞�݉������
	D3DXVECTOR3	vec_camera_to_focus = camera_focus - camera_position;

	// ���ԂɃJ�������߂荞�݂����ȂƂ��낪����������
	bool camera_re_calculate = false;
	for (int i = 0; i < 10; ++i){
		// ���Ԓn�_���v�Z
		D3DXVECTOR3 lay_point =
			camera_position + vec_camera_to_focus * static_cast<float>(i)* 0.1f;
		float pos_y = field->GetHeight(lay_point);
		// �������
		if (lay_point.y < pos_y + 0.1f){
			camera_re_calculate = true;
			camera_pos_len_ -= CAMARA_LEN_SPEED;
		}
	}

	//�J�������W�Čv�Z
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

	// �J�����Ƀp�����[�^���ăZ�b�g
	main_camera->SetPosition(camera_position);
	main_camera->SetFocus(camera_focus);
	main_camera->SetRotation(camera_rotation);


	//-------------------------------------
	// �e����
	//-------------------------------------
	shot_late--;
	shot_late = std::max<int>(shot_late, 0);
	if (GamePad::isPress(GAMEPAD_GRANDFATHER, PAD_BUTTON_8) &&
		shot_late == 0){
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

		// �J�����̉�]X�𗘗p
		bullet_param.rotation_.x_ = camera_rotation.x;

		bullet_param.scaling_ = { 1.0f, 1.0f, 1.0f };

		Bullet* bullet = object_manager_->GetNoUseBullet();
		bullet->Fire(bullet_param);

		shot_late = 10;
	}

	// �܁[���[
	EFFECT_PARAMETER_DESC effect_param;
	MyEffect *effect = effect_manager_->Get("water");
	effect_param = effect->parameter();
	effect->SetParameter(effect_param);
	Vector3 poseffect = grandfather_position;
	poseffect.y_ += 0.6f;
	Vector3 speed = { BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ };
	Vector3 rot = Vector3(camera_rotation.x, camera_rotation.y, camera_rotation.z);
	// ��]�l����������
	rot.x_ += BULLET_OFFSET_ROT;
	// ��]�l���Q�Ƃ��đ��x������
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
			MyEffect *effect = effect_manager_->Get("portal");
			effect_param = effect->parameter();
			effect_param.position_ = poseffect;
			effect_param.position_.y_ = poseffect.y_ + 0.1f;
			effect = effect_manager_->Get("marker");
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
			break;
		}
	}

	

	camera_manager_->Update();
	object_manager_->Update();
	effect_manager_->Update();
	collision_manager_->Update();

	font_->Add("�V�[����:");
	font_->Add("Matching\n");
	font_->Add("FPS : %d\n", Fps::GetFps());
	font_->Add("POSITION(Grandfather) : %3.2f %3.2f %3.2f\n",
		grandfather_position.x_,
		grandfather_position.y_,
		grandfather_position.z_);
	font_->Add("ROTATION(Grandfather) : %3.2f %3.2f %3.2f\n",
		grandfather_rotation.x_,
		grandfather_rotation.y_,
		grandfather_rotation.z_);
	font_->Add("OBJECT : %d\n", ObjectManager::GetCount());
	font_->Add("COLLISION : %d\n", CollisionManager::GetCount());


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
	collision_manager_->Draw();
	font_->Draw(rect, font_color);
	Fade::Draw();
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
