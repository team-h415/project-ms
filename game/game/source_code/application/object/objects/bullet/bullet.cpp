//=========================================================
// bullet.cpp
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
#include "../model/fbx_model.h"
#include "../model/fbx/fbx_player.h"
#include "../model/fbx/fbx_child.h"
#include "../model/fbx/fbx_grandfather.h"
#include "../model/x/x_fort.h"
#include "../mesh/field.h"
#include "../../../collision/collision.h"
#include "../../../collision/collision_manager.h"
#include "bullet.h"
#include "../../../scene/scene.h"
#include "../../../scene/scene_manager.h"
#include "../../../scene/scenes/game.h"
#include "../../../scene/scenes/game_server.h"
#include "../../../scene/scenes/matching.h"


//-------------------------------------
// Bullet()
//-------------------------------------
Bullet::Bullet(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// �e���̐���
	parameter_ = parameter;
	mesh_ = NULL;
	material_buffer_ = NULL;
	shader_ = nullptr;
	shader_ = ShaderManager::Get("resource/shader/bullet.hlsl");
	texture_ = NULL;
	LoadMesh("resource/model/x/ball.x");
	SetTexture("resource/texture/game/bullet.png");

	// �g�p�t���OOFF
	collision_ = nullptr;
	use_ = false;
}


//-------------------------------------
// ~Bullet()
//-------------------------------------
Bullet::~Bullet()
{
	if(collision_ != nullptr)
	{
		collision_->SetThisDelete(true);
		collision_ = nullptr;
	}
	shader_ = nullptr;
}


//-------------------------------------
// Fire()
//-------------------------------------
void Bullet::Fire(OBJECT_PARAMETER_DESC &parameter)
{
#ifdef NETWORK_HOST_MODE
	std::string temp = parameter_.name_;
	
	parameter_ = parameter;
	parameter_.name_ = temp;


	// ��]�l����������
	parameter_.rotation_.x_ += BULLET_OFFSET_ROT;

	// �Q�X�g�֏o����
	NETWORK_DATA send_data;
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_BULLET;
	send_data.object_param_.ex_id_ = 0;

	send_data.object_param_.position_.x_ = parameter_.position_.x_;
	send_data.object_param_.position_.y_ = parameter_.position_.y_;
	send_data.object_param_.position_.z_ = parameter_.position_.z_;

	send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
	send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
	send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;

	strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
	NetworkHost::SendTo(DELI_MULTI, send_data);

	if(collision_ == nullptr)
	{
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if(str != "GameServer"){
			ASSERT_ERROR("�e�����������ׂ��V�[���ł͂���܂���");
			return;
		}
		COLLISION_PARAMETER_DESC param;
		param.position_ = {
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_};
		param.range_ = 0.5f;
		param.offset_ = {0.0f, 0.0f, 0.0f};

		GameServer *game_server = dynamic_cast<GameServer*>(scene);
		collision_ = game_server->collision_manager()->Create(this, param);
	}
	collision_->SetUse(true);

	speed_ = {BULLET_DEF_SPEED_XZ, BULLET_DEF_SPEED_Y, BULLET_DEF_SPEED_XZ};
	// ��]�l���Q�Ƃ��đ��x������
	speed_.y += sinf(parameter_.rotation_.x_) * BULLET_ADD_SPEED_Y;

	// �g�p�t���OOFF
	use_ = true;
#endif 
}


//-------------------------------------
// Update()
//-------------------------------------
void Bullet::Update()
{
	if(!use_)
	{
		return;
	}
#ifdef NETWORK_HOST_MODE

	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BULLET_GRAVITY;

	// �Q�X�g�֍��W��
	NETWORK_DATA send_data;
	send_data.type_ = DATA_OBJ_PARAM;
	send_data.object_param_.type_ = OBJ_BULLET;
	send_data.object_param_.ex_id_ = 0;
	send_data.object_param_.position_.x_ = parameter_.position_.x_;
	send_data.object_param_.position_.y_ = parameter_.position_.y_;
	send_data.object_param_.position_.z_ = parameter_.position_.z_;
	send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
	send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
	send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;
	strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
	NetworkHost::SendTo(DELI_MULTI, send_data);

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();
	if (str == "GameServer"){
		GameServer *game_server = dynamic_cast<GameServer*>(scene);
		Object *obj = game_server->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));
		if (parameter_.position_.y_ < height){
			use_ = false;
			collision_->SetUse(false);
			parameter_.position_.y_ = 10000.0f;

			// �Q�X�g�֏������
			NETWORK_DATA send_data;
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_BULLET;
			send_data.object_param_.ex_id_ = 1;
			strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
			NetworkHost::SendTo(DELI_MULTI, send_data);
		}
	}
#endif

#if defined(_DEBUG) || !defined(NETWORK_HOST_MODE)
	// ���[���h�v�Z
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
// Action()
//-------------------------------------
void Bullet::Draw()
{
	if(!use_)
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
void Bullet::LoadMesh(
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
void Bullet::SetTexture(
	const std::string &path)
{
	//LoadTexture
	texture_ = TextureManager::GetTexture(path.c_str());
}


//-------------------------------------
// Action()
//-------------------------------------
void Bullet::Action(
	Object *target,
	const float range)
{
#ifdef NETWORK_HOST_MODE
	if(!use_)
	{
		return;
	}

	//-------------------------------------
	// ����X���f���Ɠ���������
	if (target->parameter().layer_ == LAYER_MODEL_FORT ||
		target->parameter().layer_ == LAYER_MODEL_GRANDFATHER ||
		target->parameter().layer_ == LAYER_MODEL_CHILD){

		//-------------------------------------
		// �����̐e�̃��C���[���m�F
		if (parameter_.parent_layer_ != target->parameter().layer_){

			//-------------------------------------
			// ���������ΏۂɃp�����[�^���f
			// ����
			if (target->parameter().layer_ == LAYER_MODEL_GRANDFATHER){
				FbxGrandfather *father = dynamic_cast<FbxGrandfather*>(target);
				float life = father->GetLife();
				life -= GRANDFATHER_DAMAGE;
				father->SetLife(life);
				father->SetRecoverWaitTimer(0);
			}
			// �q��
			else if (target->parameter().layer_ == LAYER_MODEL_CHILD){
				FbxChild *child = dynamic_cast<FbxChild*>(target);
				float life = child->GetLife();
				life -= CHILD_DAMAGE;
				child->SetLife(life);
				child->SetRecoverWaitTimer(0);
			}
			// ��
			else if (target->parameter().layer_ == LAYER_MODEL_FORT &&
				(parameter_.parent_layer_ == LAYER_MODEL_CHILD ||
				parameter_.parent_layer_ == LAYER_MODEL_GRANDFATHER)){
				XFort *fort = dynamic_cast<XFort*>(target);
				float life = fort->GetLife();
				life -= FORT_DAMAGE;
				fort->SetLife(life);
			}

			// �f�[�^�]���p�\���̗p��
			NETWORK_DATA send_data;
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_EFFECT;
			send_data.object_param_.position_.x_ = parameter_.position_.x_;
			send_data.object_param_.position_.y_ = parameter_.position_.y_;
			send_data.object_param_.position_.z_ = parameter_.position_.z_;
			send_data.object_param_.rotation_.x_ = parameter_.rotation_.x_;
			send_data.object_param_.rotation_.y_ = parameter_.rotation_.y_;
			send_data.object_param_.rotation_.z_ = parameter_.rotation_.z_;
			strcpy_s(send_data.name, MAX_NAME_LEN, "damage");

			// �I�u�W�F�N�g�f�[�^�]��
			NetworkHost::SendTo(DELI_MULTI, send_data);

			// �g�p�t���OOFF
			use_ = false;
			collision_->SetUse(false);

			// �Q�X�g�֏������
			ZeroMemory(&send_data, sizeof(send_data));
			send_data.type_ = DATA_OBJ_PARAM;
			send_data.object_param_.type_ = OBJ_BULLET;
			send_data.object_param_.ex_id_ = 1;
			strcpy_s(send_data.name, MAX_NAME_LEN, parameter_.name_.c_str());
			NetworkHost::SendTo(DELI_MULTI, send_data);

			////-------------------------------------
			//// �V�[���擾
			//Scene *scene = SceneManager::GetCurrentScene();
			//std::string str = SceneManager::GetCurrentSceneName();
			//if(str == "Game"){
			//	Game *game = dynamic_cast<Game*>(scene);

			//	//-------------------------------------
			//	// �V�[������G�t�F�N�g�擾
			//	EFFECT_PARAMETER_DESC effect_param;
			//	MyEffect *effect = game->effect_manager()->Get("damage");
			//	effect_param = effect->parameter();
			//	effect_param.position_ = parameter_.position_;
			//	effect_param.position_.y_ += 0.5f;
			//	effect_param.rotation_ = parameter_.rotation_;
			//	effect->SetParameter(effect_param);

			//	//-------------------------------------
			//	// �G�t�F�N�g�Đ�
			//	game->effect_manager()->Play("damage");
			//	//-------------------------------------
			//	// �����͂�����SE�Đ�
			//	Sound::LoadAndPlaySE("resource/sound/se/game/waterBreak.wav");
			//}

		}
	}
#endif
}





//-------------------------------------
// end of file
//-------------------------------------
