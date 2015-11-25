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
	struct MESH
	{
		LPDIRECT3DVERTEXBUFFER9 vertex_;
		LPDIRECT3DINDEXBUFFER9 index_;
		int vertex_max_;
		int index_max_;
		D3DMATERIAL9 material_;
		char texture_filename_[128];
		LPDIRECT3DTEXTURE9 texture_;
		char uvset_name_[128];
	};

	struct KEY_FRAME
	{
		float time_;
		D3DXVECTOR3 translation_;
		D3DXQUATERNION rotation_;
		D3DXVECTOR3 scaling_;
	};

	struct BONE
	{
		int id_;
		D3DXMATRIX bone_matrix_;
		D3DXMATRIX offset_matrix_;
		D3DXMATRIX transform_matrix_;
		D3DXMATRIX world_matrix_;
		BONE *child_;
		BONE *sibling_;
		char name_[128];
		int key_max_;
		KEY_FRAME *key_;
		BONE(){
			child_ = nullptr;
			sibling_ = nullptr;
			memset(name_, 0, 128);
			id_ = -1;
		}
	};
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

	FbxNode *GetRootBone(FbxNode *node);
	void CountUpBone(FbxNode *node);
	void LoadBoneName(FbxNode *node);
	void LoadBoneHierarchy(FbxNode *node);
	FbxModel::BONE *SearchBoneByName(char *name);
	void LoadAnimationKeyFrame(FbxNode *node, FbxScene *scene);
	void LoadAnimationKeyFrameFormNode(FbxNode *node, FbxScene *scene);
	void CountUpMesh(FbxNode *node);
	void LoadMesh(FbxNode *node);
	void LoadMeshFromNode(FbxNode *node);
	void LoadMaterialFromNode(FbxNode *node, int material_index);
	void LoadTextureFromNode(FbxNode *node, int material_index, char *dst_uvset);
	void LoadBoneIndexAndWeight(FbxSkin *skin, VertexBlend3D *dst_vertex, int vertex_id);
	void ExtractTranslationFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src);
	void ExtractRotationFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src);
	void ExtractScalingFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src);
	virtual void UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent);
	void DeleteBone(BONE *bone);

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

};


#endif //__FbxModel_H__


//-------------------------------------
// end of file
//-------------------------------------
