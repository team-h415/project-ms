//=========================================================
// directx9.h
// author:ryuya nakamura
// 描画インターフェース
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_RENDER_DIRECTX9_H__
#define __APP_RENDER_DIRECTX9_H__


//-------------------------------------
// include
//-------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include "../../../../../include/Effekseer.h"
#include "../../../../../include/EffekseerRendererDX9.h"


//-------------------------------------
// library link
//-------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#ifdef _DEBUG
#pragma comment(lib,"Effekseer.Debug.lib")
#pragma comment(lib,"EffekseerRendererDX9.Debug.lib")
#else
#pragma comment(lib,"Effekseer.Release.lib")
#pragma comment(lib,"EffekseerRendererDX9.Release.lib")
#endif //_DEBUG


//-------------------------------------
// define 
//-------------------------------------
#define FVF_VERTEX_BLEND_3D (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//-------------------------------------
// struct
//-------------------------------------
struct Vertex2D
{
	D3DXVECTOR3 position_;
	float rhw_;
	D3DCOLOR diffuse_;
	D3DXVECTOR2 texture_;
};

struct Vertex3D
{
	D3DXVECTOR3 position_;
	D3DXVECTOR3 normal_;
	D3DCOLOR diffuse_;
	D3DXVECTOR2 texture_;
};

struct VertexInstancing3D
{
	D3DXVECTOR3 position_;
	D3DXVECTOR3 normal_;
	D3DXVECTOR2 texture_;
};

struct VertexBlend3D
{
	D3DXVECTOR3 position;
	float weight[3];
	DWORD matrix_index;
	D3DXVECTOR3 normal;
	D3DCOLOR diffuse;
	D3DXVECTOR2 texture;

	// コンストラクタ
	VertexBlend3D(void) :
		position(0.0f, 0.0f, 0.0f),
		matrix_index(0),
		normal(0.0f, 0.0f, 0.0f),
		diffuse(0),
		texture(0.0f, 0.0f)
	{
		weight[0] = 0.0f;
		weight[1] = 0.0f;
		weight[2] = 0.0f;
	}

	bool operator == (const VertexBlend3D &src) const
	{
		return memcmp(this, &src, sizeof(VertexBlend3D)) == 0;
	}
};


//-------------------------------------
// class
//-------------------------------------
class DirectX9 : public GameRenderer
{
public:
	DirectX9();
	virtual ~DirectX9();
private:
};


#endif //__APP_RENDER_DIRECRX9_H__

//-------------------------------------
// end of file
//-------------------------------------
