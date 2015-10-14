//=========================================================
// shader.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "shader.h"


//-------------------------------------
// Shader()
//-------------------------------------
Shader::Shader(
	const std::string &path)
{
	if (FAILED(D3DXCompileShaderFromFile(
		path.c_str(),
		NULL,
		NULL,
		"VS",
		"vs_3_0",
		0,
		&code_,
		&error_,
		&vertex_table_)))
	{
		MessageBox(
			NULL,
			(LPSTR)error_->GetBufferPointer(),
			"error",
			MB_OK);
	}
	DirectX9Holder::device_->CreateVertexShader(
		(DWORD*)code_->GetBufferPointer(), &vertex_shader_);


	if (FAILED(D3DXCompileShaderFromFile(
		path.c_str(),
		NULL,
		NULL,
		"PS",
		"ps_3_0",
		0,
		&code_,
		&error_,
		&pixel_table_)))
	{
		MessageBox(
			NULL,
			(LPSTR)error_->GetBufferPointer(),
			"error",
			MB_OK);
	}

	DirectX9Holder::device_->CreatePixelShader(
		(DWORD*)code_->GetBufferPointer(), &pixel_shader_);

	SAFE_RELEASE(code_);
	SAFE_RELEASE(error_);
}


//-------------------------------------
// ~Shader()
//-------------------------------------
Shader::~Shader()
{
	SAFE_RELEASE(vertex_shader_);
	SAFE_RELEASE(vertex_table_);
	SAFE_RELEASE(pixel_shader_);
	SAFE_RELEASE(pixel_table_);
}


//-------------------------------------
// end of file
//-------------------------------------
