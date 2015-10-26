//=========================================================
// directx9_holder.cpp
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include 
//-------------------------------------
#include "../../../common/common.h"
#include "../renderer.h"
#include "directx9.h"
#include "directx9_holder.h"


//-------------------------------------
// variable decralation
//-------------------------------------
LPDIRECT3D9 DirectX9Holder::directx9_ = NULL;
LPDIRECT3DDEVICE9 DirectX9Holder::device_ = NULL;
LPDIRECT3DVERTEXDECLARATION9 DirectX9Holder::vertex_declaration_2d_ = NULL;
LPDIRECT3DVERTEXDECLARATION9 DirectX9Holder::vertex_declaration_3d_ = NULL;
LPDIRECT3DVERTEXDECLARATION9 DirectX9Holder::vertex_declaration_fbx_ = NULL;


//-------------------------------------
// Begin()
//-------------------------------------
void DirectX9Holder::DrawBegin()
{
	SUCCEEDED(device_->BeginScene());
}


//-------------------------------------
// End()
//-------------------------------------
void DirectX9Holder::DrawEnd()
{
	device_->EndScene();
}


//-------------------------------------
// Clear()
//-------------------------------------
void DirectX9Holder::Clear(
	MaterialColor color)
{
	color.a_ = std::min<int>(color.a_, 255);
	color.r_ = std::min<int>(color.r_, 255);
	color.g_ = std::min<int>(color.g_, 255);
	color.b_ = std::min<int>(color.b_, 255);
	color.a_ = std::max<int>(color.a_, 0);
	color.r_ = std::max<int>(color.r_, 0);
	color.g_ = std::max<int>(color.g_, 0);
	color.b_ = std::max<int>(color.b_, 0);

	device_->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET |
		D3DCLEAR_ZBUFFER |
		D3DCLEAR_STENCIL,
		D3DCOLOR_RGBA(color.r_, color.g_, color.b_, color.a_),
		1.0f,
		0);
}



//-------------------------------------
// Draw()
//-------------------------------------
void DirectX9Holder::SwapBuffer()
{
	device_->Present(
		NULL,
		NULL,
		NULL,
		NULL);
}

//-------------------------------------
// end of file
//-------------------------------------
