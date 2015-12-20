//=========================================================
// x_fort.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../../common/common.h"
#include "../../../../config/config.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../math/vector.h"
#include "../../../object.h"
#include "../../../../shader/shader.h"
#include "../../../object.h"
#include "../x_model.h"
#include "x_fort.h"


//-------------------------------------
// XFort()
//-------------------------------------
XFort::XFort(
	const OBJECT_PARAMETER_DESC &parameter) :
	XModel(parameter)
{
	//-------------------------------------
	// パラメータ初期化
	life_ = 0.0f;

	SetTexture("resource/texture/game/fort.jpg");
}


//-------------------------------------
// ~XFort()
//-------------------------------------
XFort::~XFort()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void XFort::Update()
{
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

	//-------------------------------------
	// 体力を指定領域内に丸め込む
	//-------------------------------------
	life_ = std::max<float>(life_, 0.0f);
}


//-------------------------------------
// Action()
//-------------------------------------
void XFort::Action(
	Object *target,
	const float range)
{

}


//-------------------------------------
// end of file
//-------------------------------------
