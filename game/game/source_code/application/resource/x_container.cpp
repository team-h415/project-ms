//=========================================================
// x_container.cpp
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "x_container.h"


//-------------------------------------
// XContainer
//-------------------------------------
XContainer::XContainer(const std::string &path)
{
	file_path_ = path;
	Load(path);
}


//-------------------------------------
// ~XContainer
//-------------------------------------
XContainer::~XContainer(void)
{
	SAFE_RELEASE(mesh_);
	SAFE_RELEASE(material_buffer_);
}


//-------------------------------------
// ~Load
//-------------------------------------
void XContainer::Load(const std::string &path)
{
	LPD3DXBUFFER adjacency_buffer;
	if (FAILED(D3DXLoadMeshFromX(
		path.c_str(),
		D3DXMESH_SYSTEMMEM,
		DirectX9Holder::device_,
		&adjacency_buffer,
		&material_buffer_,
		NULL,
		&material_count_,
		&mesh_)))
	{
		std::string warning;
		warning = path;
		warning += ": このファイルが見つかりません";
		ASSERT_WARNING(warning.c_str());
		ASSERT_ERROR("モデル読み込みに失敗");
		return;
	}

	if (FAILED(mesh_->OptimizeInplace(
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjacency_buffer->GetBufferPointer(),
		NULL, NULL, NULL)))
	{
		ASSERT_ERROR("モデルのオプティマイズに失敗");
		return;
	}

	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	LPD3DXMESH old_mesh = mesh_;
	if (FAILED(old_mesh->CloneMesh(
		D3DXMESH_MANAGED,
		elements,
		DirectX9Holder::device_,
		&mesh_)))
	{
		ASSERT_ERROR("モデルのコンバートに失敗");
		return;
	}

	SAFE_RELEASE(old_mesh);
}