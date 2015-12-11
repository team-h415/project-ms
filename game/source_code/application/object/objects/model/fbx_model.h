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
	float begin_;				// 開始時間
	float end_;					// 終了時間
	float speed_;				// 再生速度 1.0fがデフォルト
	float time_;				// 現在の位置
	bool loop_;					// loop falg
	// コンストラクタ
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

	ANIMATION*	animation_;					// アニメーション管理構造体
	int			current_animation_id_;		// 現在のアニメーション
	float		animation_previous_time_;	// 前のアニメーション時間
	int			animation_switching_;		// アニメーション移行時間
	bool		animation_blending_;		// アニメーションの合成
	bool		animation_play_;			// アニメーションの再生
	int			animation_count_;

	LPDIRECT3DTEXTURE9 texture_;
};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
