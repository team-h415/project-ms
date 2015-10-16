//=========================================================
// debug_font.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "debug_font.h"


//-------------------------------------
// warning
//-------------------------------------
#pragma warning (disable:4996)


//-------------------------------------
// DebugFont()
//-------------------------------------
DebugFont::DebugFont()
{
	font_ = NULL;
	string_.clear();
	
	D3DXCreateFont(
		DirectX9Holder::device_,
		0, 6, 0, 0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&font_);
}


//-------------------------------------
// ~DebugFont()
//-------------------------------------
DebugFont::~DebugFont()
{
	SAFE_RELEASE(font_);
}


//-------------------------------------
// Draw()
//-------------------------------------
void DebugFont::Draw(
	RECT rect,
	D3DXCOLOR color)
{
	font_->DrawTextA(
		NULL,
		string_.c_str(),
		-1,
		&rect,
		DT_LEFT,
		color);

	string_.clear();
}


//-------------------------------------
// Add()
//-------------------------------------
void DebugFont::Add(
	const char *format, ...)
{
	char work[1024];
	va_list args;

	va_start(args, format);
	vsprintf(work, format, args);
	va_end(args);

	string_ = string_ + work;
}


//-------------------------------------
// end of file
//-------------------------------------
