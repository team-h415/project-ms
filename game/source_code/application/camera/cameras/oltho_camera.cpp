//=========================================================
// oltho_camera.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../camera.h"
#include "oltho_camera.h"


//-------------------------------------
// OlthoCamera()
//-------------------------------------
OlthoCamera::OlthoCamera(
	const CAMERA_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	D3DXMatrixIdentity(&view_);
	D3DXMatrixIdentity(&projection_);
}


//-------------------------------------
// ~OlthoCamera()
//-------------------------------------
OlthoCamera::~OlthoCamera()
{
}


//-------------------------------------
// Update()
//-------------------------------------
void OlthoCamera::Update()
{
	D3DXMatrixIdentity(&view_);
	D3DXMatrixIdentity(&projection_);

	D3DXMatrixLookAtLH(
		&view_,
		&parameter_.position_,
		&parameter_.focus_,
		&parameter_.up_);

	D3DXMatrixOrthoLH(
		&projection_,
		parameter_.w_,
		parameter_.h_,
		parameter_.near_,
		parameter_.far_);
}


//-------------------------------------
// Set()
//-------------------------------------
void OlthoCamera::Set()
{
	DirectX9Holder::device_->SetTransform(
		D3DTS_VIEW, &view_);
	DirectX9Holder::device_->SetTransform(
		D3DTS_PROJECTION, &projection_);
}


//-------------------------------------
// end of file
//-------------------------------------
