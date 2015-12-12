//=========================================================
// DirectX9Holder.h
// author:ryuya nakamura
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __APP_RENDER_DIRECTX9HOLDER_H__
#define __APP_RENDER_DIRECTX9HOLDER_H__


struct MaterialColor
{
	MaterialColor() : r_(0), g_(0), b_(0), a_(0){}
	MaterialColor(int r, int g, int b, int a) :
		r_(r), g_(g), b_(b), a_(a){}
	int r_, g_, b_, a_;
};

class DirectX9Holder
{
public:
	static void DrawBegin();
	static void DrawEnd();
	static void Clear(MaterialColor color);
	static void SwapBuffer();
	static LPDIRECT3D9 directx9_;
	static LPDIRECT3DDEVICE9 device_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_2d_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_3d_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_field_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_shadow_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_x_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_fbx_;
	static LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_instancing_;
};


#endif //__APP_RENDER_DIRECTX9HOLDER_H__


//-------------------------------------
// end of file
//-------------------------------------
