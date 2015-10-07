//=========================================================
// renderer_factory.cpp
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include 
//-------------------------------------
#include "../../common/common.h"
#include "renderer.h"
#include "directx9\directx9.h"
#include "renderer_factory.h"


//-------------------------------------
// Create()
//-------------------------------------
Renderer *RendererFactory::Create(
	const HWND &window_handle,
	const std::string &renderer)
{
	// ���̂Ƃ��떢�g�p
	UNREFERENCED_PARAMETER(window_handle);

	if (renderer == "DirectX9")
	{
		DirectX9 *pointer = new DirectX9();
		return pointer;
	}

	ASSERT_WARNING("�f�o�C�X�̐������s�Ȃ��܂���ł���");
	return nullptr;
}


//-------------------------------------
// end of file
//-------------------------------------

