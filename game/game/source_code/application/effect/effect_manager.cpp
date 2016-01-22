//=========================================================
// effect_manager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../network/network.h"
#include "../network/network_guest.h"
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "../math/vector.h"
#include "effect.h"
#include "effect_manager.h"


//-------------------------------------
// variable
//-------------------------------------
EffectManager* EffectManager::effect_manager_ = nullptr;
int EffectManager::effect_count_ = 0;


//-------------------------------------
// Get
//-------------------------------------
EffectManager* EffectManager::Get()
{
	if(effect_manager_ == nullptr)
	{
		effect_manager_ = new EffectManager(100000);
	}

	return effect_manager_;
}


//-------------------------------------
// Delete
//-------------------------------------
void EffectManager::Delete()
{
	SAFE_DELETE(effect_manager_);
}


//-------------------------------------
// EffectManager()
//-------------------------------------
EffectManager::EffectManager(
	const int max_sprites)
{
	// �}�b�v������
	effects_.clear();

	// �C���X�^���X����
	renderer_ = EffekseerRendererDX9::Renderer::Create(
		DirectX9Holder::device_, max_sprites);
	manager_ = Effekseer::Manager::Create(max_sprites, false);

	// �`��V�X�e���̐ݒ�
	manager_->SetSpriteRenderer(renderer_->CreateSpriteRenderer());
	manager_->SetRibbonRenderer(renderer_->CreateRibbonRenderer());
	manager_->SetRingRenderer(renderer_->CreateRingRenderer());
	manager_->SetModelRenderer(renderer_->CreateModelRenderer());
	manager_->SetTrackRenderer(renderer_->CreateTrackRenderer());

	// ���[�_�[�̐ݒ�
	manager_->SetModelLoader(renderer_->CreateModelLoader());
	manager_->SetTextureLoader(renderer_->CreateTextureLoader());

	// ���W�n�̐ݒ�
	manager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// �X�g�b�N���Z�b�g
	stock_effect_.clear();

	// �N���e�B�J���Z�N�V�����쐬
	InitializeCriticalSection(&critical_section_);
}


//-------------------------------------
// ~EffectManager()
//-------------------------------------
EffectManager::~EffectManager()
{
	stock_effect_.clear();
	manager_->StopAllEffects();
	for (auto it = effects_.begin(); it != effects_.end(); ++it){
		SAFE_DELETE((*it).second);
	}
	effects_.clear();
	renderer_->Destory();
	manager_->Destroy();
}


//-------------------------------------
// Update()
//-------------------------------------
void EffectManager::Update()
{
	SetViewMatrix();
	SetProjectionMatrix();
#ifndef NETWORK_HOST_MODE
	// ���b�N
	NetworkGuest::deta_stock(true);
	while(true)
	{
		Sleep(5);
		if(NetworkGuest::data_process() == false)
		{
			break;
		}
	}
#endif
	AdaptationEffect();
#ifndef NETWORK_HOST_MODE
	// �A�����b�N
	NetworkGuest::deta_stock(false);
#endif
	for(auto it = effects_.begin(); it != effects_.end(); ++it)
	{
		(*it).second->Update(manager_);
	}
	manager_->Flip();
	manager_->Update();
	effect_count_ = 0;
}


//-------------------------------------
// Draw()
//-------------------------------------
void EffectManager::Draw()
{
	renderer_->BeginRendering();
	manager_->Draw();
	renderer_->EndRendering();

	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	DirectX9Holder::device_->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, 1);
	DirectX9Holder::device_->SetSamplerState(1, D3DSAMP_MIPMAPLODBIAS, 1);
	DirectX9Holder::device_->SetSamplerState(2, D3DSAMP_MIPMAPLODBIAS, 1);
	DirectX9Holder::device_->SetSamplerState(3, D3DSAMP_MIPMAPLODBIAS, 1);
	DirectX9Holder::device_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DirectX9Holder::device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DirectX9Holder::device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	DirectX9Holder::device_->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DirectX9Holder::device_->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DirectX9Holder::device_->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	DirectX9Holder::device_->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DirectX9Holder::device_->SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DirectX9Holder::device_->SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	DirectX9Holder::device_->SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DirectX9Holder::device_->SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DirectX9Holder::device_->SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}


//-------------------------------------
// SetViewMatrix()
//-------------------------------------
void EffectManager::SetViewMatrix()
{
	D3DXMATRIX view;
	Effekseer::Matrix44 e_view;
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			e_view.Values[i][j] = view.m[i][j];
		}
	}
	renderer_->SetCameraMatrix(e_view);
}


//-------------------------------------
// SetProjectionMatrix()
//-------------------------------------
void EffectManager::SetProjectionMatrix()
{
	D3DXMATRIX proj;
	Effekseer::Matrix44 e_proj;
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &proj);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			e_proj.Values[i][j] = proj.m[i][j];
		}
	}
	renderer_->SetProjectionMatrix(e_proj);
}


//-------------------------------------
// Create()
//-------------------------------------
void EffectManager::Create(
	const std::string &name,
	const std::string &path,
	const EFFECT_PARAMETER_DESC &parameter)
{
	if(effects_[name] == nullptr)
	{
		effects_[name] = new MyEffect(manager_, parameter, path);
	}
}


//-------------------------------------
// Play()
//-------------------------------------
void EffectManager::Play(
	const std::string &name)
{
	for (auto it = effects_.begin(); it != effects_.end(); ++it){
		if ((*it).first == name){
			(*it).second->Play(manager_);
		}
	}
}


//-------------------------------------
// Get()
//-------------------------------------
MyEffect *EffectManager::Get(
	const std::string &name)
{
	for (auto it = effects_.begin(); it != effects_.end(); ++it){
		if ((*it).first == name){
			return (*it).second;
		}
	}
	//ASSERT_ERROR("�w�肵�����O�̃G�t�F�N�g�͑��݂��܂���");
	return nullptr;
}


//-------------------------------------
// Stop()
//-------------------------------------
void EffectManager::Stop(
	const std::string &name)
{
	for (auto it = effects_.begin(); it != effects_.end(); ++it){
		if ((*it).first == name){
			(*it).second->Stop(manager_);
		}
	}
}


//-------------------------------------
// StopAll()
//-------------------------------------
void EffectManager::StopAll()
{
	manager_->StopAllEffects();
}


//-------------------------------------
// StockEffect()
//-------------------------------------
void EffectManager::StockEffect(STOCK_EFFECT_PARAM stock_param)
{
	// ���b�N
	EnterCriticalSection(&critical_section_);
	// �f�[�^IN
	stock_effect_.push_back(stock_param);
	// �A�����b�N
	LeaveCriticalSection(&critical_section_);
}


//-------------------------------------
// StockEffect()
//-------------------------------------
void EffectManager::AdaptationEffect()
{
	// ���b�N
	EnterCriticalSection(&critical_section_);
	// �G�t�F�N�g�ǉ�
	EFFECT_PARAMETER_DESC effect_param;
	MyEffect *effect;
	for(auto it = stock_effect_.begin(); it != stock_effect_.end(); it++)
	{
		effect = Get(it->name_);
		if(effect == nullptr)
		{
			continue;
		}
		switch(it->state_)
		{
			case EFFECT_ADD:
				effect_param = effect->parameter();
				effect_param.position_ = it->position_;
				effect_param.rotation_ = it->rotation_;
				effect_param.scaling_ = it->scaling_;

				effect->SetParameter(effect_param);
				Play(it->name_);
				break;

			case EFFECT_SET_PARAM:
				effect_param = effect->parameter();
				effect_param.position_ = it->position_;
				effect_param.rotation_ = it->rotation_;
				effect_param.scaling_ = it->scaling_;

				effect->SetParameter(effect_param);
				break;

			case EFFECT_STOP:
				Stop(it->name_);
				break;

			default:
				break;
		}
	}
	// �X�g�b�N���
	stock_effect_.clear();
	// �A�����b�N
	LeaveCriticalSection(&critical_section_);
}

//-------------------------------------
// end of file
//-------------------------------------
