//=========================================================
// fbx_model.h
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxModel_H__
#define __FbxModel_H__


//-------------------------------------
// include
//-------------------------------------
#include "../../../resource/fbx_my_container.h"


//-------------------------------------
// struct
//-------------------------------------
struct ANIMATION
{
	float begin_;				// �J�n����
	float end_;					// �I������
	float speed_;				// �Đ����x 1.0f���f�t�H���g
	float time_;				// ���݂̈ʒu
	bool loop_;					// loop falg
	// �R���X�g���N�^
	ANIMATION(void) :
		begin_(0.0f),
		end_(0.0f),
		speed_(1.0f),
		time_(0.0f),
		loop_(false)
	{
	}
};


//-------------------------------------
// class
//-------------------------------------
class Shader;
class FbxModel : public Object
{
public:
	FbxModel(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~FbxModel();
	virtual void Update();
	void Draw();
	void Load(
		const std::string &path);

	int GetCurrentAnimationId(void){ return current_animation_id_; }

protected:

	virtual void UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent);

	int mesh_count_;
	MESH *mesh_;
	int mesh_cursor_;
	int bone_count_;
	BONE *bone_;
	BONE *root_;
	float cur_time_;
	int bone_cursor_;
	Shader *shader_;

	ANIMATION*	animation_;					// �A�j���[�V�����Ǘ��\����
	int			current_animation_id_;		// ���݂̃A�j���[�V����
	float		animation_previous_time_;	// �O�̃A�j���[�V��������
	int			animation_switching_;		// �A�j���[�V�����ڍs����
	bool		animation_blending_;		// �A�j���[�V�����̍���
	bool		animation_play_;			// �A�j���[�V�����̍Đ�
	int			animation_count_;

	LPDIRECT3DTEXTURE9 texture_;
};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
