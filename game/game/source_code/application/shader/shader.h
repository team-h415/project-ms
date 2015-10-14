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
