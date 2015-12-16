//=========================================================
// effect.cpp
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


//-------------------------------------
// Effect()
//-------------------------------------
MyEffect::MyEffect(
	Effekseer::Manager *manager,
	const EFFECT_PARAMETER_DESC &parameter,
	const std::string &path)
{
	parameter_ = parameter;
	std::wstring string;
	int size = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0);
	string.resize(size);
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &string[0], size);
	effect_ = Effekseer::Effect::Create(manager, (EFK_CHAR*)string.c_str());
	manager->Flip();
}


//-------------------------------------
// ~Effect()
//-------------------------------------
MyEffect::~MyEffect()
{
	ES_SAFE_RELEASE(effect_);
}


//-------------------------------------
// Update()
//-------------------------------------
void MyEffect::Update(
	Effekseer::Manager *manager)
{
	manager->SetLocation(
		handle_,
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_);
	manager->SetRotation(
		handle_,
		parameter_.rotation_.x_,
		parameter_.rotation_.y_,
		parameter_.rotation_.z_);
	manager->SetScale(
		handle_,
		parameter_.scaling_.x_,
		parameter_.scaling_.y_,
		parameter_.scaling_.z_);

	manager->SetSpeed(
		handle_,
		parameter_.speed_);

}


//-------------------------------------
// Play()
//-------------------------------------
void MyEffect::Play(
	Effekseer::Manager *manager)
{
	handle_ = manager->Play(
		effect_,
		parameter_.position_.x_,
		parameter_.position_.y_,
		parameter_.position_.z_);
}


//-------------------------------------
// Stop()
//-------------------------------------
void MyEffect::Stop(
	Effekseer::Manager *manager)
{
	manager->StopEffect(handle_);
}


//-------------------------------------
// end of file
//-------------------------------------
