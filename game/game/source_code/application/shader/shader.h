//=========================================================
// shader.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Shader_H__
#define __Shader_H__


//-------------------------------------
// class
//-------------------------------------
class Shader
{
public:
	Shader(
		const std::string &path);
	virtual ~Shader();
	const LPD3DXCONSTANTTABLE &vertex_table(){
		return vertex_table_;
	}
	const LPDIRECT3DVERTEXSHADER9 &vertex_shader(){
		return vertex_shader_;
	}
	const LPD3DXCONSTANTTABLE &pixel_table(){
		return pixel_table_;
	}
	const LPDIRECT3DPIXELSHADER9 &pixel_shader(){
		return pixel_shader_;
	}
private:
	LPD3DXCONSTANTTABLE vertex_table_;
	LPD3DXCONSTANTTABLE pixel_table_;
	LPDIRECT3DVERTEXSHADER9 vertex_shader_;
	LPDIRECT3DPIXELSHADER9 pixel_shader_;
	LPD3DXBUFFER error_;
	LPD3DXBUFFER code_;
};


#endif //__Shader_H__


//-------------------------------------
// end of file
//-------------------------------------
