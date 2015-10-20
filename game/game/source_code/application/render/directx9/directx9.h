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


//-------------------------------------
// library link
//-------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")


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

struct VertexBlend3D
{
	D3DXVECTOR3 position;
	float weight[3];
	DWORD matrix_index;
	D3DXVECTOR3 normal;
	D3DCOLOR diffuse;
	D3DXVECTOR2 texture;

	bool operator == (const VertexBlend3D &src) const
	{
		return memcmp(this, &src, sizeof(VertexBlend3D)) == 0;
	}
};

//-------------------------------------
// class
//-------------------------------------
class DirectX9 : public Renderer
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
