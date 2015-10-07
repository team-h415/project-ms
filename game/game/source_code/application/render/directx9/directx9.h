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
