//=========================================================
// bomb.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
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


//-------------------------------------
// Bomb()
//-------------------------------------
Bomb::Bomb(
	const OBJECT_PARAMETER_DESC &parameter)
{
	// �e���̐���
	parameter_ = parameter;
	mesh_ = NULL;
	material_buffer_ = NULL;
	shader_ = nullptr;
	shader_ = ShaderManager::Get("resource/shader/bullet.hlsl");
	texture_ = NULL;
	LoadMesh("resource/model/x/bomb.x");
	SetTexture("resource/texture/game/bomb.png");

	// �g�p�t���OOFF
	collision_ = nullptr;
	use_ = false;

	if (collision_ == nullptr)
	{
		Scene *scene = SceneManager::GetCurrentScene();
		std::string str = SceneManager::GetCurrentSceneName();
		if (str != "Game" && str != "Matching"){
			ASSERT_ERROR("�e�����������ׂ��V�[���ł͂���܂���");
			return;
		}
		COLLISION_PARAMETER_DESC param;
		param.position_ = {
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_ };
		param.range_ = 3.0f;
		param.offset_ = { 0.0f, 0.0f, 0.0f };

		if (str == "Game"){
			Game *game = dynamic_cast<Game*>(scene);
			collision_ = game->collision_manager()->Create(this, param);
		}
		if (str == "Matching"){
			Matching *matching = dynamic_cast<Matching*>(scene);
			collision_ = matching->collision_manager()->Create(this, param);
		}
	}
	collision_->SetUse(false);
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
	parameter_ = parameter;
	frame_count_ = 0;

	speed_ = { BOMB_DEF_SPEED_XZ, BOMB_DEF_SPEED_Y, BOMB_DEF_SPEED_XZ };

	// ��]�l����������
	parameter_.rotation_.x_ += BOMB_OFFSET_ROT;
	// ��]�l���Q�Ƃ��đ��x������
	speed_.y += sinf(parameter_.rotation_.x_) * BOMB_ADD_SPEED_Y;


	// �g�p�t���OOFF
	use_ = true;
	collision_->SetUse(false);
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

	frame_count_++;
	parameter_.position_.x_ += sinf(parameter_.rotation_.y_) * speed_.x;
	parameter_.position_.y_ += speed_.y;
	parameter_.position_.z_ += cosf(parameter_.rotation_.y_) * speed_.z;
	speed_.y -= BOMB_GRAVITY;

	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();

	// �Q�[���Ȃ�
	if (str == "Game"){
		Game *game = dynamic_cast<Game*>(scene);
		Object *obj = game->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));

		if (parameter_.position_.y_ < height + 0.35f){
			parameter_.position_.y_ = height + 0.35f;
			speed_ *= 0.9f;
		}

		if (frame_count_ == BOMB_TIMER){
			collision_->SetUse(true);
			
			//-------------------------------------
			// �V�[������G�t�F�N�g�擾
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = game->effect_manager()->Get("bombfire");
			effect_param = effect->parameter();
			effect_param.position_ = parameter_.position_;
			effect_param.position_.y_ = height;
			effect_param.rotation_ = { 0.0f, parameter_.rotation_.y_, 0.0f };
			effect->SetParameter(effect_param);

			//-------------------------------------
			// �G�t�F�N�g�Đ�
			game->effect_manager()->Play("bombfire");
		}
	}



	if (str == "Matching"){
		Matching *matching = dynamic_cast<Matching*>(scene);
		Object *obj = matching->object_manager()->Get("field");
		Field *field = dynamic_cast<Field*>(obj);
		float height = field->GetHeight(
			D3DXVECTOR3(
			parameter_.position_.x_,
			parameter_.position_.y_,
			parameter_.position_.z_));
		if (parameter_.position_.y_ < height){
			parameter_.position_.y_ = height;
			speed_ *= 0.9f;
		}
		if (frame_count_ == BOMB_TIMER){
			collision_->SetUse(true);

			//-------------------------------------
			// �V�[������G�t�F�N�g�擾
			EFFECT_PARAMETER_DESC effect_param;
			MyEffect *effect = matching->effect_manager()->Get("bombfire");
			effect_param = effect->parameter();
			effect_param.position_ = parameter_.position_;
			effect_param.position_.y_ = height;
			effect_param.rotation_ = { 0.0f, parameter_.rotation_.y_, 0.0f };
			effect->SetParameter(effect_param);

			//-------------------------------------
			// �G�t�F�N�g�Đ�
			matching->effect_manager()->Play("bombfire");
		}
	}

	if (frame_count_ > BOMB_TIMER){
		use_ = false;
		collision_->SetUse(false);
		parameter_.position_.y_ = 10000.0f;

	}

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
	if (!use_)
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
				// �ډB���G�t�F�N�g����
				this->SetBlind(father->parameter().position_, father->parameter().rotation_);
			}
			// �q��
			else if (target->parameter().layer_ == LAYER_MODEL_CHILD){
				FbxChild *child = dynamic_cast<FbxChild*>(target);
				float life = child->GetLife();
				life -= CHILD_DAMAGE;
				child->SetLife(life);
				child->SetRecoverWaitTimer(0);
				// �ډB���G�t�F�N�g����
				this->SetBlind(child->parameter().position_, child->parameter().rotation_);
			}
			// ��(���q���ɍ����ւ��邱��!)
			else if (target->parameter().layer_ == LAYER_MODEL_FORT &&
				parameter_.parent_layer_ == LAYER_MODEL_GRANDFATHER){
				XFort *fort = dynamic_cast<XFort*>(target);
				float life = fort->GetLife();
				float damage = FORT_DAMAGE;
				//-------------------------------------
				// �V�[���擾
				Scene *scene = SceneManager::GetCurrentScene();
				std::string str = SceneManager::GetCurrentSceneName();
				if (str == "Game"){
					Game *game = dynamic_cast<Game*>(scene);
					// �V�[���h�����Ă��炻�̕���������
					if (game->shield_flg() == true)
						damage *= SHIELD_DAMAGE_ATTENUATION;

					// �X�e�[�W�ڍs���̓_���[�W����
					if (game->change_stage_flg() == false){
						life -= damage;
						fort->SetLife(life);
					}
				}
			}

			//-------------------------------------
			// �V�[���擾
			Scene *scene = SceneManager::GetCurrentScene();
			std::string str = SceneManager::GetCurrentSceneName();
			if (str == "Game"){
				Game *game = dynamic_cast<Game*>(scene);

				//-------------------------------------
				// �V�[������G�t�F�N�g�擾
				EFFECT_PARAMETER_DESC effect_param;
				MyEffect *effect = game->effect_manager()->Get("damage");
				effect_param = effect->parameter();
				effect_param.position_ = parameter_.position_;
				effect_param.position_.y_ += 0.5f;
				effect_param.rotation_ = parameter_.rotation_;
				effect->SetParameter(effect_param);

				//-------------------------------------
				// �G�t�F�N�g�Đ�
				game->effect_manager()->Play("damage");
				//-------------------------------------
				// �����͂�����SE�Đ�
				Sound::LoadAndPlaySE("resource/sound/se/game/waterBreak.wav");
			}
			use_ = false;
			collision_->SetUse(false);
		}
	}
}

//-------------------------------------
// SetBlind()
//-------------------------------------
void Bomb::SetBlind(
	Vector3 player_position,
	Vector3 player_rotation)
{
	//-------------------------------------
	// �V�[���擾
	Scene *scene = SceneManager::GetCurrentScene();
	std::string str = SceneManager::GetCurrentSceneName();
	if (str == "Game"){
		Game *game = dynamic_cast<Game*>(scene);

		// �v���C���[���猩�Ăǂ̈ʒu�ɓ����������v�Z����
		D3DXVECTOR2 vec = {
			parameter_.position_.x_ - player_position.x_,
			parameter_.position_.z_ - player_position.z_ };
		D3DXVec2Normalize(&vec, &vec);

		D3DXVECTOR2 vec2 = {
			sinf(player_rotation.y_),
			cosf(player_rotation.y_) };
		D3DXVec2Normalize(&vec2, &vec2);

		float rotato_y = atan2(D3DXVec2Dot(&vec, &vec2), (vec.x * vec2.y - vec.y * vec2.x));

		for (int i = 0; i < BLIND_BOMB_NUM; i++){
			float rotato_dest_y = rotato_y + float((rand() % 314)-156) * 0.01f;	// ���U
			float length = BLIND_LEN_MIN + float((rand() % 10)) * 0.1f * (BLIND_LEN_MAX - BLIND_LEN_MIN);
			float scaling = float((rand() % (BLIND_SCALING_MAX - BLIND_SCALING_MIN) + BLIND_SCALING_MIN)) * BLIND_BOMB_MAGNIFICATION;
			float rotato_z = float((rand() % 314))*0.01f;

			//-------------------------------------
			// �u���C���h�𔭐�������
			//-------------------------------------
			OBJECT_PARAMETER_DESC blind_param;
			blind_param.name_ = "blind";
			blind_param.position_ = {
				SCREEN_WIDTH * 0.5f + cosf(rotato_dest_y) * length * 1.777f,		// ��ʂ�����������������
				SCREEN_HEIGHT * 0.5f - sinf(rotato_dest_y) * length,
				0.0f };

			blind_param.rotation_ = { 0.0f, 0.0f, rotato_z };
			blind_param.scaling_ = { scaling, scaling, 0.0f };
			blind_param.layer_ = LAYER_BLIND;

			Blind* blind = game->object_manager()->GetNoUseBlind();
			if (blind != nullptr){
				blind->SetBlind(blind_param);
			}
			else{ break; }
		}
	}

}

//-------------------------------------
// end of file
//-------------------------------------
