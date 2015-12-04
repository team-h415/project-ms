//=========================================================
// fbx_my_container.h
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include guard
//-------------------------------------
#pragma once
#ifndef __FbxMyContainer_H__
#define __FbxMyContainer_H__


//-------------------------------------
// struct
//-------------------------------------
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

//================================================================================
// ƒNƒ‰ƒX
//================================================================================
class FbxMyContainer
{
	public:
		FbxMyContainer(const std::string &path);
		~FbxMyContainer(void);

		int mesh_count_;
		MESH *mesh_;
		int mesh_cursor_;
		int bone_count_;
		BONE *bone_;
		BONE *root_;
		float cur_time_;
		int bone_cursor_;

		std::string file_path_;

	private:
		void Load(const std::string &path);

		void DeleteBone(BONE *bone);
		FbxNode *GetRootBone(FbxNode *node);
		void CountUpBone(FbxNode *node);
		void LoadBoneName(FbxNode *node);
		void LoadBoneHierarchy(FbxNode *node);
		BONE *SearchBoneByName(char *name);
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
};

#endif // _FBX_MANAGER_H_