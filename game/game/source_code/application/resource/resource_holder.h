//=========================================================
// resource_holder.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __ResourceHolder_H__
#define __ResourceHolder_H__


//-------------------------------------
// struct
//-------------------------------------
struct TexturePackage
{
	TexturePackage(){
		texture_ = NULL;
	}
	TexturePackage(const std::string &path){
		D3DXCreateTextureFromFile(
			DirectX9Holder::device_,
			path.c_str(),
			&texture_);
	}
	LPDIRECT3DTEXTURE9 texture_;
};


//-------------------------------------
// class
//-------------------------------------
class ResourceHolder
{
public:
	ResourceHolder();
	virtual ~ResourceHolder();
	void Load(
		const std::string &path);
private:
	
};


#endif //__ResourceHolder_H__


//-------------------------------------
// end of file
//-------------------------------------
