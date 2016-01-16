//=========================================================
// bomb.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Bomb_H__
#define __Bomb_H__


//-------------------------------------
// class
//-------------------------------------
class Collision;
class MyEffect;
class Bomb : public Object
{
public:
	Bomb(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~Bomb();
	void Fire(OBJECT_PARAMETER_DESC &parameter);
	void Update();
	void Draw();
	void Action(
		Object *target,
		const float range,
		D3DXVECTOR3 my_position);
private:
	void LoadMesh(
		const std::string &path);
	void SetTexture(
		const std::string &path);
	void SetBlind(
		Vector3 player_position,
		Vector3 player_rotation);
	Collision *collision_;
	D3DXVECTOR3 speed_;
	LPD3DXMESH mesh_;
	LPD3DXBUFFER material_buffer_;
	DWORD material_count_;
	D3DXMATRIX world_;
	LPDIRECT3DTEXTURE9 texture_;
	Shader *shader_;
	int frame_count_;
};


#endif //__Bomb_H__


//-------------------------------------
// end of file
//-------------------------------------
