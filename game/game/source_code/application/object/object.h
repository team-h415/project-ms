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
	LAYER_MESH_FIELD,
	LAYER_MODEL_X,
	LAYER_MODEL_FBX,
	LAYER_MODEL_GRANDFATHER,
	LAYER_MODEL_CHILD,
	LAYER_SPRITE_2D,
    LAYER_TIMER,
    LAYER_WATER_GAGE,
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
	virtual void Action();
	
	// �p�����[�^�擾
	const OBJECT_PARAMETER_DESC &parameter(){
		return parameter_;
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
};


#endif //__Object_H__


//-------------------------------------
// end of file
//-------------------------------------
