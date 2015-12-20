//=========================================================
// effect_manager.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
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
		effect_manager_ = new EffectManager(50000);
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
	// マップ初期化
	effects_.clear();

	// インスタンス生成
	renderer_ = EffekseerRendererDX9::Renderer::Create(
		DirectX9Holder::device_, max_sprites);
	manager_ = Effekseer::Manager::Create(max_sprites, false);

	// 描画システムの設定
	manager_->SetSpriteRenderer(renderer_->CreateSpriteRenderer());
	manager_->SetRibbonRenderer(renderer_->CreateRibbonRenderer());
	manager_->SetRingRenderer(renderer_->CreateRingRenderer());
	manager_->SetModelRenderer(renderer_->CreateModelRenderer());
	manager_->SetTrackRenderer(renderer_->CreateTrackRenderer());

	// ローダーの設定
	manager_->SetModelLoader(renderer_->CreateModelLoader());
	manager_->SetTextureLoader(renderer_->CreateTextureLoader());

	// 座標系の設定
	manager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}


//-------------------------------------
// ~EffectManager()
//-------------------------------------
EffectManager::~EffectManager()
{
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
	manager_->Flip();
	for (auto it = effects_.begin(); it != effects_.end(); ++it){
		(*it).second->Update(manager_);
	}
	manager_->Update();
	effect_count_ = effects_.size();
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
	ASSERT_ERROR("指定した名前のエフェクトは存在しません");
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
// end of file
//-------------------------------------
