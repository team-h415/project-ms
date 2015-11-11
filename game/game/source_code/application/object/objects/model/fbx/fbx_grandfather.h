//=========================================================
// fbx_grandfather.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxGrandfather_H__
#define __FbxGrandfather_H__


//-------------------------------------
// enum
//-------------------------------------
enum ANIMATION_TYPE
{
	TYPE_0,
	TYPE_1,

	MAX_TYPE
};


//-------------------------------------
// class
//-------------------------------------
class FbxGrandfather : public FbxModel
{
public:
	FbxGrandfather(
		const OBJECT_PARAMETER_DESC &parameter);
	~FbxGrandfather();
	void Update();

	void SetCurrentAnimation(int animId, bool blend = true)
	{
		if(0 <= animId && animId < MAX_TYPE)
		{
			previous_animation_id_ = current_animation_id_;
			current_animation_id_ = animId;
			animation_switching_ = 0;
			animation_blending_ = true;
		}
	}
	void PlayCurrentAnimation(void){ animation_play_ = true; }
	void StopAnimation(void){ animation_play_ = false; }
	void PlayAnimation(int animId, bool blend = true)
	{
		SetCurrentAnimation(animId, blend);
		ResetCurrentAnimation();
		PlayCurrentAnimation();
	}
	void ResetAnimation(int animId)
	{
		if(0 <= animId && animId < MAX_TYPE)
		{
			animation_[animId].time_ = 0.0f;
		}
	}
	void ResetCurrentAnimation(void)
	{
		ResetAnimation(current_animation_id_);
	}
	void ResetAllAnimation(void)
	{
		for(int i = 0; i < MAX_TYPE; i++)
		{
			ResetAnimation(i);
		}
	}

private:
	void UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent);

	//キーフレーム間の補間
	D3DXVECTOR3 InterpolateTranslation(BONE* subject, float time);
	D3DXQUATERNION InterpolateQuaternion(BONE* subject, float time);
	D3DXVECTOR3 InterpolateScaling(BONE* subject, float time);

	D3DXVECTOR3 InterpolateBlendTranslation(BONE* subject, float prev_time, float next_time, float t);
	D3DXQUATERNION InterpolateBlendQuaternion(BONE* subject, float prev_time, float next_time, float t);
	D3DXVECTOR3 InterpolateBlendScaling(BONE* subject, float prev_time, float next_time, float t);

};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
