//=========================================================
// loading.cpp
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include 
//-------------------------------------
#include "../../../common/common.h"
#include "../../render/renderer.h"
#include "../../render/directx9/directx9.h"
#include "../../render/directx9/directx9_holder.h"
#include "../scene.h"
#include "loading.h"


//-------------------------------------
// Loading()
//-------------------------------------
Loading::Loading()
{

}


//-------------------------------------
// ~Loading()
//-------------------------------------
Loading::~Loading()
{

}


//-------------------------------------
// Update()
//-------------------------------------
void Loading::Update()
{

}


//-------------------------------------
// Draw()
//-------------------------------------
void Loading::Draw()
{
	Color color(0,0,0,255);
	DirectX9Holder::DrawBegin();
	DirectX9Holder::Clear(color);
	DirectX9Holder::DrawEnd();
	DirectX9Holder::SwapBuffer();
}


//-------------------------------------
// end of file
//-------------------------------------
