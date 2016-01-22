//=========================================================
// bomb.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../network/network.h"
#ifdef NETWORK_HOST_MODE
#include "../../../network/network_host.h"
#endif
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../../resource/x_container.h"
#include "../../../resource/x_container_manager.h"
#include "../../../resource/texture_manager.h"
#include "../../../effect/effect.h"
#include "../../../effect/effect_manager.h"
#include "../../../sound/sound.h"
#include "../../../config/config.h"
#include "../../../shader/shader.h"
#include "../../../shader/shader_manager.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/matching.h"
#include "../../object.h"
#include "../../object_manager.h"
#include "../sprite/blind.h"
#include "../model/fbx_model.h"
#include "../model/fbx/fbx_player.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../mesh/field.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "bomb.h"
#include "../../../scene/scenes/game_server.h"

//-------------------------------------
// Bomb()
//-------------------------------------
Bomb::Bomb(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// 弾実体生成
	parameter_ = parameter;
	parameter_.scaling_ = {0.35f, 0.35f, 0.35f};
	mesh_ = NULL;
	material_buffer_ = NULL;
	shader_ = nullptr;
	shader_ = ShaderManager::Get("resource/shader/bullet.hlsl");
	texture_ = NULL;
	LoadMesh("resource/model/x/bomb.x");
	SetTexture("resource/texture/game/bomb.png");

	// 使用フラグOFF
	collision_ = nullptr;
	use_ = false;


#ifdef NETWORK_HOST_MODE
	// 当たり判定作成
	COLLISION_PARAMETER_DESC param;
	param.position_ = {
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_};
	param.range_ = 3.0f;
	param.offset_ = {0.0f, 0.0f, 0.0f};

	collision_ = CollisionManager::Get()->Create(this, param);
	collision_->SetUse(false);
#endif
}


//-------------------------------------
// ~Bomb()
//-------------------------------------
Bomb::~Bomb()
{
	if (collision_){
		collision_->SetThisDelete(true);
	}
	shader_ = nullptr;
}


//-------------------------------------
// Fire()
//-------------------------------------
void Bomb::Fire(OBJECT_PARAMETER_DESC &parameter)
{
#ifdef NETWORK_HOST_MODE
	std::string temp = parameter_.name_;

	parameter_ = parameter;
	parameter_.name_ = temp;
	parameter_.scaling_ = {0.35f, 0.35f, 0.35f};
	frame_count_ = 0;

	speed_ = { BOMB_DEF_SPEED_XZ, BOMB_DEF_SPEED_Y, BOMB_DEF_SPEED_XZ };

	// 回転値を少し調整
	parameter_.rotation_.x_ += BOMB_OFFSET_ROT;
	// 回転値を参照して速度を改良
	speed_.y += sinf(parameter_.rotation_.x_) * BOMB_ADD_SPEED_Y;

	// ゲストへ出現報告
	NETWORK_DATA send_data;
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_BOMB;
	send_data.object_param_.ex_id_ = 0;

	send_data.object_param_.position_.x_ = parameter_.position_.x_;
	send_data.object_param_.position_.y_ = parameter_.position_.y_;
	send_data.object_param_.position_.z_ = parameter_.position_.z_;

	send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
	send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
	send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;

	strcpy_s(send_data.name_, MAX_NAME_LEN, parameter_.name_.c_str());
	NetworkHost::SendTo(DELI_MULTI, send_data);

	collision_->SetUse(false);
#endif 
	// 使用フラグON
	use_ = true;
}


//-------------------------------------
// Update()
//-------------------------------------
void Bomb::Update()
{
	if (!use_)
	{
		return;
	}

#ifdef NETWORK_HOST_MODE
	frame_count_++;
	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BOMB_GRAVITY;

	Scene *scene = SceneManager::GetCurrentScene();
	GameServer *game_server = dynamic_cast<GameServer*>(scene);
	Object *obj = game_server->object_manager()->Get("field");
	Field *field = dynamic_cast<Field*>(obj);
	float height = field->GetHeight(
		D3DXVECTOR3(
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_));

	if (parameter_.position_.y_ < height + 0.35f)
	{
		parameter_.position_.y_ = height + 0.35f;
		speed_ *= 0.9f;
	}

	if (frame_count_ == BOMB_TIMER)
	{
		collision_->SetUse(true);
			
		//-------------------------------------
		// エフェクト再生報告
		NETWORK_DATA send_data;
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_EFFECT;
		send_data.object_param_.position_.x_ = parameter_.position_.x_;
		send_data.object_param_.position_.y_ = height;
		send_data.object_param_.position_.z_ = parameter_.position_.z_;
		send_data.object_param_.rotation_ = {0.0f, parameter_.rotation_.y_, 0.0f};
		strcpy_s(send_data.name_, MAX_NAME_LEN, "BombFire");
		NetworkHost::SendTo(DELI_MULTI, send_data);

		//-------------------------------------
		// ゲストへ消える報告
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_BOMB;
		send_data.object_param_.ex_id_ = 1;
		strcpy_s(send_data.name_, MAX_NAME_LEN, parameter_.name_.c_str());
		NetworkHost::SendTo(DELI_MULTI, send_data);

		// サウンド
		Sound::LoadAndPlaySE("resource/sound/se/game/waterBreak.wav");
	}
	else if(frame_count_ > BOMB_TIMER)
	{
		use_ = false;
		collision_->SetUse(false);
		parameter_.position_.y_ = 10000.0f;
	}
	else
	{
		// ゲストへ座標報告
		NETWORK_DATA send_data;
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_BOMB;
		send_data.object_param_.ex_id_ = 0;
		send_data.object_param_.position_.x_ = parameter_.position_.x_;
		send_data.object_param_.position_.y_ = parameter_.position_.y_;
		send_data.object_param_.position_.z_ = parameter_.position_.z_;
		send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
		send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
		send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;
		strcpy_s(send_data.name_, MAX_NAME_LEN, parameter_.name_.c_str());
		NetworkHost::SendTo(DELI_MULTI, send_data);
	}
#endif

#if defined(_DEBUG) || !defined(NETWORK_HOST_MODE)
	// ワールド計算
	D3DXMATRIX translate, rotate, scaling;
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&world_);

	D3DXMatrixScaling(
		&scaling, parameter_.scaling_.x_, parameter_.scaling_.y_, parameter_.scaling_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &scaling);
	D3DXMatrixRotationYawPitchRoll(
		&rotate, parameter_.rotation_.y_, parameter_.rotation_.x_, parameter_.rotation_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &rotate);
	D3DXMatrixTranslation(
		&translate, parameter_.position_.x_, parameter_.position_.y_, parameter_.position_.z_);
	D3DXMatrixMultiply(
		&world_, &world_, &translate);
#endif
}


//-------------------------------------
// Draw()
//-------------------------------------
void Bomb::Draw()
{
	if (!use_)
	{
		return;
	}

	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAREF, 0x01);
	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	DirectX9Holder::device_->SetTransform(D3DTS_WORLD, &world_);
	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_x_);

	D3DXMATRIX view, projection, wvp;
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &projection);
	wvp = world_ * view * projection;


	D3DXVECTOR3 light_vec(0.5f, -0.5f, 0.5f);
	D3DXVec3Normalize(&light_vec, &light_vec);
	D3DXCOLOR light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);

	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_wvp", &wvp);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_w", &world_);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_direction",
		reinterpret_cast<float*>(&light_vec),
		3);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_diffuse",
		reinterpret_cast<float*>(&light_diffuse),
		4);

	DirectX9Holder::device_->SetTexture(
		shader_->pixel_table()->GetSamplerIndex("texture_0"), texture_);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	D3DMATERIAL9 default_material;
	D3DXMATERIAL *material;
	DirectX9Holder::device_->GetMaterial(&default_material);
	material = (D3DXMATERIAL*)material_buffer_->GetBufferPointer();

	for (DWORD i = 0; i < material_count_; i++)
	{
		mesh_->DrawSubset(i);
	}

	DirectX9Holder::device_->SetMaterial(&default_material);

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);

	DirectX9Holder::device_->SetTexture(0, NULL);

	DirectX9Holder::device_->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void Bomb::LoadMesh(
	const std::string &path)
{
	XContainer* container = XContainerManager::GetContainer(path);
	mesh_ = container->mesh_;
	material_buffer_ = container->material_buffer_;
	material_count_ = container->material_count_;
}


//-------------------------------------
// SetTexture()
//-------------------------------------
void Bomb::SetTexture(
	const std::string &path)
{
	//LoadTexture
	texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// Action()
//-------------------------------------
void Bomb::Action(
	Object *target,
	const float range,
	D3DXVECTOR3 my_position)
{
	//-------------------------------------
	// もし子供に当たったら
	if (target->parameter().layer_ == LAYER_MODEL_CHILD)
	{
		FbxChild *child = dynamic_cast<FbxChild*>(target);
		if(!child->GetDamageShutOut())
		{
			float life = child->GetLife();
			life -= 10.0f;
			child->SetLife(life);
			child->SetRecoverWaitTimer(0);
			// 目隠し
			SetBlind(child->parameter().ex_id_, child->parameter().position_, child->parameter().rotation_);
		}
	}
}

void Bomb::SetUse(bool flag)
{
	use_ = flag;
	if(collision_ != nullptr)
	{
		collision_->SetUse(use_);
	}
}

//-------------------------------------
// SetBlind()
//-------------------------------------
void Bomb::SetBlind(
	int id,
	Vector3 player_position,
	Vector3 player_rotation)
{
#ifdef NETWORK_HOST_MODE
	// プレイヤーから見てどの位置に当たったか計算する
	D3DXVECTOR2 vec = {
		parameter_.position_.x_ - player_position.x_,
		parameter_.position_.z_ - player_position.z_ };
	D3DXVec2Normalize(&vec, &vec);

	D3DXVECTOR2 vec2 = {
		sinf(player_rotation.y_),
		cosf(player_rotation.y_) };
	D3DXVec2Normalize(&vec2, &vec2);

	float rotato_y = atan2(D3DXVec2Dot(&vec, &vec2), (vec.x * vec2.y - vec.y * vec2.x));

	for (int i = 0; i < BLIND_BOMB_NUM; i++)
	{
		float rotato_dest_y = rotato_y + float((rand() % 314)-156) * 0.01f;	// 分散
		float length = BLIND_LEN_MIN + float((rand() % 10)) * 0.1f * (BLIND_LEN_MAX - BLIND_LEN_MIN);
		float scaling = float((rand() % (BLIND_SCALING_MAX - BLIND_SCALING_MIN) + BLIND_SCALING_MIN)) * BLIND_BOMB_MAGNIFICATION;
		float rotato_z = float((rand() % 314))*0.01f;

		//-------------------------------------
		// ブラインドを発生させる
		//-------------------------------------

		// データ転送用構造体用意
		NETWORK_DATA send_data;
		ZeroMemory(&send_data, sizeof(send_data));
		send_data.type_ = DATA_OBJ_PARAM;
		send_data.object_param_.type_ = OBJ_BLIND;
		send_data.object_param_.position_.x_ = SCREEN_WIDTH * 0.5f + cosf(rotato_dest_y) * length * 1.777f;
		send_data.object_param_.position_.y_ = SCREEN_HEIGHT * 0.5f - sinf(rotato_dest_y) * length;
		send_data.object_param_.rotation_.z_ = rotato_z;

		send_data.object_param_.rotation_.x_ = scaling;
		// オブジェクトデータ転送
		NetworkHost::SendTo((DELI_TYPE)id, send_data);
	}
#endif
}

//-------------------------------------
// end of file
//-------------------------------------
