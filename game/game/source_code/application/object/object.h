//=========================================================
// object.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __Object_H__
#define __Object_H__


//-------------------------------------
// enum
//-------------------------------------
// レイヤは後々書き換え、ゲームによって数も変わる
enum OBJECT_LAYER
{
	LAYER_NONE = 0,
	LAYER_MESH_SKYDOME,
	LAYER_MESH_FIELD,
	LAYER_SPRITE_LAKE,
	LAYER_MODEL_X,
	LAYER_MODEL_FBX,
	LAYER_MODEL_FORT,
	LAYER_MODEL_GRANDFATHER,
	LAYER_MODEL_CHILD,
	LAYER_BULLET,
	LAYER_BOMB,
	LAYER_TREE,
	LAYER_BENCH,
	LAYER_PLAYGROUND,
	LAYER_SHADOW,
	LAYER_MESSAGE,
    LAYER_DAMAGE_EFFECT,
	LAYER_BLIND,
    LAYER_WATER_GAGE,
    LAYER_TIMER,
    LAYER_SPRITE_2D,
    LAYER_FORT_GAUGE,
	LAYER_COUNTDOWN,
	LAYER_MAX,
};


//-------------------------------------
// struct
//-------------------------------------
struct OBJECT_PARAMETER_DESC
{
	Vector3 position_;
	Vector3 rotation_;
	Vector3 scaling_;
	OBJECT_LAYER layer_;
	OBJECT_LAYER parent_layer_;
	std::string name_;
};


//-------------------------------------
// class
//-------------------------------------
class Object
{
public:
	// コンストラクタ
	Object();
	virtual ~Object();
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// あたり判定
	virtual void Action(
		Object *target, 
		const float range,
		D3DXVECTOR3 my_position);
	
	// パラメータ取得
	const OBJECT_PARAMETER_DESC &parameter(){
		return parameter_;
	}
	bool this_delete(){
		return this_delete_;
	}
	bool use(){
		return use_;
	}

	void SetThisDelete(bool flag){ this_delete_ = flag; }
	void SetUse(bool flag){ use_ = flag; }

	// パラメータ設定
	void SetPosition(
		const Vector3 &position){
		parameter_.position_ = position;
	}
	void SetRotation(
		const Vector3 &rotation){
		parameter_.rotation_ = rotation;
	}
	void SetScaling(
		const Vector3 &scaling){
		parameter_.scaling_ = scaling;
	}

protected:
	OBJECT_PARAMETER_DESC parameter_;
	bool this_delete_;
	bool use_;
};


#endif //__Object_H__


//-------------------------------------
// end of file
//-------------------------------------
