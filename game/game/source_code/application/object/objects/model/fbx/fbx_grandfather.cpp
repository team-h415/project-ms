//=========================================================
// fbx_grandfather.cpp
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../../common/common.h"
#include "../../../../render/renderer.h"
#include "../../../../render/directx9/directx9.h"
#include "../../../../render/directx9/directx9_holder.h"
#include "../../../../math/vector.h"
#include "../../../../shader/shader.h"
#include "../../../object.h"
#include "../fbx_model.h"
#include "fbx_grandfather.h"


//-------------------------------------
// define
//-------------------------------------
#define ANIMATION_SWITCHING_TIME 8


//-------------------------------------
// FbxGrandfather()
//-------------------------------------
FbxGrandfather::FbxGrandfather(const OBJECT_PARAMETER_DESC &parameter) :
	FbxModel(parameter)
{
	// �����������p�̃A�j���[�V������ݒ�
	animation_ = new ANIMATION[MAX_TYPE];

	animation_[0].begin_ = 0.0f;
	animation_[0].end_ = 10.0f;
	animation_[0].loop_ = true;
	animation_[0].speed_ = 1.0f;
	animation_[0].time_ = 0.0f;
}


//-------------------------------------
// ~FbxGrandfather()
//-------------------------------------
FbxGrandfather::~FbxGrandfather()
{
	SAFE_DELETE_ARRAY(animation_);
}


//-------------------------------------
// Update()
//-------------------------------------
void FbxGrandfather::Update()
{
	ANIMATION* current_animation(&animation_[current_animation_id_]);
	current_animation->time_ += current_animation->speed_;

	float length(current_animation->end_ - current_animation->begin_);
	if(length > 0)
	{
		if(current_animation->time_ > length)
		{
			if(current_animation->loop_)
			{
				current_animation->time_ -= length;
			}
			else
			{
				current_animation->time_ = length;
			}
		}
	}
	else
	{
		current_animation->time_ = current_animation->begin_;
	}

	//�A�j���[�V�����ڍs���Ԃ̍X�V
	if(animation_blending_)
	{
		animation_switching_++;
		if(animation_switching_ >= ANIMATION_SWITCHING_TIME)
		{
			animation_switching_ = 0;
			animation_blending_ = false;
		}
	}

	// �}�g���b�N�X�̍X�V
	D3DXMATRIX element;
	D3DXMATRIX translate, rotate, scaling;
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&element);

	float anim_length = 0.0f;

	D3DXMatrixScaling(
		&scaling, parameter_.scaling_.x_, parameter_.scaling_.y_, parameter_.scaling_.z_);
	D3DXMatrixMultiply(
		&element, &element, &scaling);
	D3DXMatrixRotationYawPitchRoll(
		&rotate, parameter_.rotation_.y_, parameter_.rotation_.x_, parameter_.rotation_.z_);
	D3DXMatrixMultiply(
		&element, &element, &rotate);
	D3DXMatrixTranslation(
		&translate, parameter_.position_.x_, parameter_.position_.y_, parameter_.position_.z_);
	D3DXMatrixMultiply(
		&element, &element, &translate);

	UpdateBoneMatrix(&bone_[0], &element);

	//for(int i = 0; i < bone_count_; i++)
	//{
	//	int x = bone_[i].key_max_;
	//	int st = 0;
	//	for(int l = 0; l < x; l++)
	//	{
	//		float time = bone_[i].key_[l].time_;
	//		int st2 = 0;
	//	}
	//}

}


//-------------------------------------
// UpdateBoneMatrix()
//-------------------------------------
void FbxGrandfather::UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent)
{
	KEY_FRAME* prev_key(nullptr);
	KEY_FRAME* next_key(nullptr);
	int i(0);								//loop indexBuffPtr
	float t(0.0f);							//��ԃp�����[�^
	float length(0.0f);						//�t���[���Ԃ̒���
	D3DXMATRIX trans_mtx, scl_mtx, rot_mtx;	//��Ɨp

	int key_count(MAX_TYPE);

	//�؂�ւ���
	if(animation_blending_)
	{
		//���Ԃ̌v�Z
		float cur_time(animation_[current_animation_id_].begin_ + animation_[current_animation_id_].time_);
		float pre_time(animation_[previous_animation_id_].begin_ + animation_[previous_animation_id_].time_);

		float blend_weight((float)animation_switching_ / (float)ANIMATION_SWITCHING_TIME);

		//trans
		D3DXVECTOR3 trans(InterpolateBlendTranslation(subject, pre_time, cur_time, blend_weight));

		//rot
		D3DXQUATERNION rot(InterpolateBlendQuaternion(subject, pre_time, cur_time, blend_weight));

		//scl
		D3DXVECTOR3 scl(InterpolateBlendScaling(subject, pre_time, cur_time, blend_weight));

		//transform����
		D3DXMatrixTranslation(&trans_mtx, trans.x, trans.y, trans.z);
		D3DXMatrixRotationQuaternion(&rot_mtx, &rot);
		D3DXMatrixScaling(&scl_mtx, scl.x, scl.y, scl.z);
	}
	//�P��A�j���[�V����
	else
	{
		//���Ԃ̌v�Z
		float t(animation_[current_animation_id_].begin_ + animation_[current_animation_id_].time_);

		//trans
		D3DXVECTOR3 trans(InterpolateTranslation(subject, t));

		//rot
		D3DXQUATERNION rot(InterpolateQuaternion(subject, t));

		//scl
		D3DXVECTOR3 scl(InterpolateScaling(subject, t));

		//transform����
		D3DXMatrixTranslation(&trans_mtx, trans.x, trans.y, trans.z);
		D3DXMatrixRotationQuaternion(&rot_mtx, &rot);
		D3DXMatrixScaling(&scl_mtx, scl.x, scl.y, scl.z);
	}

	D3DXMatrixIdentity(&subject->bone_matrix_);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &scl_mtx);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &rot_mtx);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &trans_mtx);

	//���ɓn���s��̌v�Z
	D3DXMatrixMultiply(&subject->transform_matrix_, &subject->bone_matrix_, parent);

	//���[���h���X�V world = offset * �����̍s�� * �e�̍s��;
	D3DXMatrixMultiply(&subject->world_matrix_, &subject->offset_matrix_, &subject->transform_matrix_);

	//�q���̍X�V
	if(subject->child_ != nullptr)
	{
		UpdateBoneMatrix(subject->child_, &subject->transform_matrix_);
	}

	//�Z��̍X�V
	if(subject->sibling_ != nullptr)
	{
		UpdateBoneMatrix(subject->sibling_, parent);
	}
}


//�L�[�t���[���Ԃ̕��
D3DXVECTOR3 FbxGrandfather::InterpolateTranslation(BONE* subject, float time)
{
	if(subject == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	if(subject->key_ == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	//���Ԃ̓��ł�
	float cur_time(time);
	cur_time = max(cur_time, 0.0f);
	cur_time = min(cur_time, subject->key_[subject->key_max_ - 1].time_);

	//����̃L�[�t���[���擾
	int next(0);
	for(next = 1; next < subject->key_max_; next++)
	{
		if(cur_time <= subject->key_[next].time_)
		{
			break;
		}
	}

	//���O�̃L�[�t���[���擾
	int prev(next - 1);

	//�O���trans�擾
	D3DXVECTOR3 prev_trans(subject->key_[prev].translation_);
	D3DXVECTOR3 next_trans(subject->key_[next].translation_);

	//parameter�擾
	float t(0.0f);
	float length(subject->key_[next].time_ - subject->key_[prev].time_);
	if(length != 0.0f)
	{
		t = (cur_time - subject->key_[prev].time_) / length;
	}

	D3DXVECTOR3 ret(prev_trans * (1.0f - t) + next_trans * t);

	return ret;
}
D3DXQUATERNION FbxGrandfather::InterpolateQuaternion(BONE* subject, float time){
	if(subject == nullptr){ return D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); }
	if(subject->key_ == nullptr){ return D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); }

	//���Ԃ̓��ł�
	float cur_time(time);
	cur_time = max(cur_time, 0.0f);
	cur_time = min(cur_time, subject->key_[subject->key_max_ - 1].time_);

	//����̃L�[�t���[���擾
	int next(0);
	for(next = 1; next < subject->key_max_; next++)
	{
		if(cur_time <= subject->key_[next].time_)
		{
			break;
		}
	}

	//���O�̃L�[�t���[���擾
	int prev(next - 1);

	//�O���trans�擾
	D3DXQUATERNION prev_rot(subject->key_[prev].rotation_);
	D3DXQUATERNION next_rot(subject->key_[next].rotation_);

	//parameter�擾
	float t(0.0f);
	float length(subject->key_[next].time_ - subject->key_[prev].time_);
	if(length != 0.0f)
	{
		t = (cur_time - subject->key_[prev].time_) / length;
	}

	D3DXQUATERNION ret(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQuaternionSlerp(&ret, &prev_rot, &next_rot, t);

	return ret;
}
D3DXVECTOR3 FbxGrandfather::InterpolateScaling(BONE* subject, float time)
{
	if(subject == nullptr){ return D3DXVECTOR3(1.0f, 1.0f, 1.0f); }
	if(subject->key_ == nullptr){ return D3DXVECTOR3(1.0f, 1.0f, 1.0f); }

	//���Ԃ̓��ł�
	float cur_time(time);
	cur_time = max(cur_time, 0.0f);
	cur_time = min(cur_time, subject->key_[subject->key_max_ - 1].time_);

	//����̃L�[�t���[���擾
	int next(0);
	for(next = 1; next < subject->key_max_; next++)
	{
		if(cur_time <= subject->key_[next].time_)
		{
			break;
		}
	}

	//���O�̃L�[�t���[���擾
	int prev(next - 1);

	//�O���trans�擾
	D3DXVECTOR3 prev_scl(subject->key_[prev].scaling_);
	D3DXVECTOR3 next_scl(subject->key_[next].scaling_);

	//parameter�擾
	float t(0.0f);
	float length(subject->key_[next].time_ - subject->key_[prev].time_);
	if(length != 0.0f)
	{
		t = (cur_time - subject->key_[prev].time_) / length;
	}

	D3DXVECTOR3 ret(prev_scl * (1.0f - t) + next_scl * t);

	return ret;
}

D3DXVECTOR3 FbxGrandfather::InterpolateBlendTranslation(BONE* subject, float prev_time, float next_time, float t)
{
	if(subject == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	if(subject->key_ == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	//�؂�ւ��O��trans
	D3DXVECTOR3 prev_trans(InterpolateTranslation(subject, prev_time));

	//�؂�ւ����trans
	D3DXVECTOR3 next_trans(InterpolateTranslation(subject, next_time));

	//blend
	D3DXVECTOR3 blend((1.0f - t) * prev_trans + t * next_trans);

	return blend;
}
D3DXQUATERNION FbxGrandfather::InterpolateBlendQuaternion(BONE* subject, float prev_time, float next_time, float t)
{
	if(subject == nullptr){ return D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); }
	if(subject->key_ == nullptr){ return D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); }

	//�؂�ւ��O��rot
	D3DXQUATERNION prev_rot(InterpolateQuaternion(subject, prev_time));

	//�؂�ւ����rot
	D3DXQUATERNION next_rot(InterpolateQuaternion(subject, next_time));

	//blend
	D3DXQUATERNION blend(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQuaternionSlerp(&blend, &prev_rot, &next_rot, t);

	return blend;
}
D3DXVECTOR3 FbxGrandfather::InterpolateBlendScaling(BONE* subject, float prev_time, float next_time, float t)
{
	if(subject == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	if(subject->key_ == nullptr){ return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	//�؂�ւ��O��scl
	D3DXVECTOR3 prev_scl(InterpolateScaling(subject, prev_time));

	//�؂�ւ����scl
	D3DXVECTOR3 next_scl(InterpolateScaling(subject, next_time));

	//blend
	D3DXVECTOR3 blend((1.0f - t) * prev_scl + t * next_scl);

	return blend;
}


//-------------------------------------
// end of file
//-------------------------------------