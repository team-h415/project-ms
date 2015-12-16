//=========================================================
// fbx_player.h
// author:takeshi iwasawa
//=========================================================


//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxPlayer_H__
#define __FbxPlayer_H__


//-------------------------------------
// class
//-------------------------------------
class FbxPlayer : public FbxModel
{
public:
	FbxPlayer(
		const OBJECT_PARAMETER_DESC &parameter);
	virtual ~FbxPlayer();
	virtual void Update();

	void SetCurrentAnimation(int animId, bool blend = true)
	{
		if(0 <= animId && animId < animation_count_)
		{
			animation_previous_time_ = animation_[current_animation_id_].begin_ + animation_[current_animation_id_].time_;
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
		if(0 <= animId && animId < animation_count_)
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
		for(int i = 0; i < animation_count_; i++)
		{
			ResetAnimation(i);
		}
	}


	//-------------------------------------
	// パラメータ系
	//-------------------------------------
	// パラメータリセット
	void ResetGameParameters(){
		life_ = 1.0f;
		water_gauge_ = 1.0f;
	}
	// 体力
	const float &GetLife(){
		return life_;
	}
	void SetLife(const float &life){
		life_ = life;
	}

	//水ゲージ
	const float &GetWaterGauge(){
		return water_gauge_;
	}
	void SetWaterGauge(const float &gauge){
		water_gauge_ = gauge;
	}

	//体力回復ウェイトタイマー
	const int &GetRecoverWaitTimer(){
		return recover_wait_timer_;
	}
	void SetRecoverWaitTimer(const int &timer){
		recover_wait_timer_ = timer;
	}

protected:
	void UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent);

	void UpdateHP();

	//キーフレーム間の補間
	D3DXVECTOR3 InterpolateTranslation(BONE* subject, float time);
	D3DXQUATERNION InterpolateQuaternion(BONE* subject, float time);
	D3DXVECTOR3 InterpolateScaling(BONE* subject, float time);

	D3DXVECTOR3 InterpolateBlendTranslation(BONE* subject, float prev_time, float next_time, float t);
	D3DXQUATERNION InterpolateBlendQuaternion(BONE* subject, float prev_time, float next_time, float t);
	D3DXVECTOR3 InterpolateBlendScaling(BONE* subject, float prev_time, float next_time, float t);

	//-------------------------------------
	// ゲーム用パラメータ
	//-------------------------------------
	// 体力
	float life_;
	// 水ゲージ
	float water_gauge_;
	//  体力回復ウェイトタイマー
	int recover_wait_timer_;
};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
