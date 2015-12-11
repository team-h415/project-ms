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
// ���C���͌�X���������A�Q�[���ɂ���Đ����ς��
enum OBJECT_LAYER
{
	LAYER_NONE = 0,
	LAYER_MESH_SKYDOME,
	LAYER_MESH_FIELD,
	LAYER_MODEL_X,
	LAYER_MODEL_FBX,
	LAYER_MODEL_FORT,
	LAYER_MODEL_GRANDFATHER,
	LAYER_MODEL_CHILD,
	LAYER_BULLET,
	LAYER_TREE,
	LAYER_SHADOW,
    LAYER_DAMAGE_EFFECT,
    LAYER_WATER_GAGE,
    LAYER_TIMER,
    LAYER_SPRITE_2D,
    LAYER_FORT_GAUGE,
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
};


//-------------------------------------
// class
//-------------------------------------
class Object
{
public:
	// �R���X�g���N�^
	Object();
	virtual ~Object();
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;
	// �����蔻��
	virtual void Action(
		Object *target, 
		const float range);
	
	// �p�����[�^�擾
	const OBJECT_PARAMETER_DESC &parameter(){
		return parameter_;
	}
	bool this_delete(){
		return this_delete_;
	}

	// �p�����[�^�ݒ�
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
};


#endif //__Object_H__


//-------------------------------------
// end of file
//-------------------------------------
