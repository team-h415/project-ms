//=========================================================
// fbx_model.cpp
// author:ryuya nakamura
//=========================================================


//-------------------------------------
// include
//-------------------------------------
#include "../../../../common/common.h"
#include "../../../render/renderer.h"
#include "../../../render/directx9/directx9.h"
#include "../../../render/directx9/directx9_holder.h"
#include "../../../math/vector.h"
#include "../../../shader/shader.h"
#include "../../../resource/fbx_my_container.h"
#include "../../../resource/fbx_container_manager.h"
#include "../../object.h"
#include "fbx_model.h"


//-------------------------------------
// define
//-------------------------------------
#define MAX_BONE 60


//-------------------------------------
// FbxModel()
//-------------------------------------
FbxModel::FbxModel(
const OBJECT_PARAMETER_DESC &parameter)
{
	parameter_ = parameter;
	mesh_count_ = 0;
	mesh_ = NULL;
	mesh_cursor_ = 0;
	bone_count_ = 0;
	bone_ = NULL;
	bone_cursor_ = 0;
	root_ = NULL;
	cur_time_ = 0;
	shader_ = new Shader("resource/shader/halflambert_lighting_fbx.hlsl");
	animation_ = nullptr;
	animation_previous_time_ = 0.0f;
	current_animation_id_ = 0;
	animation_switching_ = 0;
	animation_blending_ = false;
	texture_ = NULL;
}


//-------------------------------------
// ~FbxModel()
//-------------------------------------
FbxModel::~FbxModel()
{
	SAFE_DELETE_ARRAY(animation_);
	SAFE_DELETE(shader_);
	texture_ = NULL;
}


//-------------------------------------
// Update()
//-------------------------------------
void FbxModel::Update()
{
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

	cur_time_ += 1.0f;
}


//-------------------------------------
// Draw()
//-------------------------------------
void FbxModel::Draw()
{
	if (mesh_ == NULL)
	{
		return;
	}

	// 変数宣言
	D3DXMATRIX scale, rotate, translate;

	int i(0);
	D3DCAPS9 caps;
	int indexed_matrix_num = 0;

	D3DMATERIAL9 def_material;

	DirectX9Holder::device_->GetMaterial(&def_material);

	DirectX9Holder::device_->GetDeviceCaps(&caps);
	indexed_matrix_num = caps.MaxVertexBlendMatrixIndex;

	//if (indexed_matrix_num < bone_count_)
	//{
	//	DirectX9Holder::device_->SetSoftwareVertexProcessing(true);
	//}

	DirectX9Holder::device_->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	DirectX9Holder::device_->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	//-------------------------------------
	// 頂点宣言(FVF)
	DirectX9Holder::device_->SetFVF(FVF_VERTEX_BLEND_3D);

	DWORD def_cull = 0;
	DirectX9Holder::device_->GetRenderState(D3DRS_CULLMODE, &def_cull);

	DirectX9Holder::device_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (i = 0; i < bone_count_; i++)
	{
		DirectX9Holder::device_->SetTransform(D3DTS_WORLDMATRIX(i), &bone_[i].world_matrix_);
	}

	DirectX9Holder::device_->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);


	//-------------------------------------
	// shader code start

	DirectX9Holder::device_->SetVertexDeclaration(
		DirectX9Holder::vertex_declaration_fbx_);

	D3DXMATRIX view, projection, vp;
	DirectX9Holder::device_->GetTransform(D3DTS_VIEW, &view);
	DirectX9Holder::device_->GetTransform(D3DTS_PROJECTION, &projection);
	vp = view * projection;

	D3DXVECTOR3 light_vec(0.5f, -0.5f, 0.5f);
	D3DXVec3Normalize(&light_vec, &light_vec);
	D3DXCOLOR light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);

	// ボーンマトリックスを配列に換装
	D3DXMATRIX bone_worlds[MAX_BONE];
	for(int i = 0; i < MAX_BONE; i++){
		D3DXMatrixIdentity(&bone_worlds[i]);
	}
	for(int i = 0; i < bone_count_; i++){
		bone_worlds[bone_[i].id_] = bone_[i].world_matrix_;
	}

	shader_->vertex_table()->SetMatrixArray(
		DirectX9Holder::device_, "matrix_bone_w", bone_worlds, MAX_BONE);
	shader_->vertex_table()->SetMatrix(
		DirectX9Holder::device_, "matrix_vp", &vp);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_direction",
		reinterpret_cast<float*>(&light_vec),
		3);

	shader_->vertex_table()->SetFloatArray(
		DirectX9Holder::device_,
		"light_diffuse",
		reinterpret_cast<float*>(&light_diffuse),
		4);

	DirectX9Holder::device_->SetVertexShader(shader_->vertex_shader());
	DirectX9Holder::device_->SetPixelShader(shader_->pixel_shader());

	//DirectX9Holder::device_->SetTexture(
	//	shader_->pixel_table()->GetSamplerIndex("texture_0"), texture_);

	// shader code end
	//-------------------------------------

	D3DMATERIAL9 debug_material;
	for (i = 0; i < mesh_count_; i++)
	{
		if (mesh_[i].index_ == NULL || mesh_[i].vertex_ == NULL)
		{
			continue;
		}

		DirectX9Holder::device_->SetTexture(0, texture_);
		debug_material = mesh_[i].material_;
		DirectX9Holder::device_->SetMaterial(&debug_material);
		DirectX9Holder::device_->SetStreamSource(0, mesh_[i].vertex_, 0, sizeof(VertexBlend3D));
		DirectX9Holder::device_->SetIndices(mesh_[i].index_);
		DirectX9Holder::device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mesh_[i].vertex_max_, 0, mesh_[i].index_max_ / 3);

	}

	DirectX9Holder::device_->SetMaterial(&def_material);
	DirectX9Holder::device_->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	//DirectX9Holder::device_->SetSoftwareVertexProcessing(false);
	DirectX9Holder::device_->SetRenderState(D3DRS_CULLMODE, def_cull);
	DirectX9Holder::device_->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	DirectX9Holder::device_->SetVertexShader(NULL);
	DirectX9Holder::device_->SetPixelShader(NULL);
	DirectX9Holder::device_->SetTexture(0, NULL);
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void FbxModel::Load(
const std::string &path)
{
	// コンテナから値のコピー
	FbxMyContainer* container = FbxContainerManager::GetContainer(path);
	mesh_count_ = container->mesh_count_;
	mesh_ = container->mesh_;
	mesh_cursor_ = container->mesh_cursor_;
	bone_count_ = container->bone_count_;
	bone_ = container->bone_;
	root_ = container->root_;
	cur_time_ = container->cur_time_;
	bone_cursor_ = container->bone_cursor_;
}


//-------------------------------------
// UpdateBoneMatrix()
//-------------------------------------
void FbxModel::UpdateBoneMatrix(BONE *subject, D3DXMATRIX *parent)
{
	KEY_FRAME *prev(NULL), *next(NULL);
	int i(0);
	float t(0.0f);
	float length(0.0f);
	D3DXMATRIX trs_mtx, scl_mtx, rot_mtx;
	float time(cur_time_);
	int key_max(subject->key_max_);

	for (i = 0; i < key_max - 1; i++)
	{
		if (subject->key_[i].time_ <= cur_time_ && cur_time_ <= subject->key_[i + 1].time_)
		{
			prev = &subject->key_[i];
			next = &subject->key_[i + 1];
			length = next->time_ - prev->time_;

			if (length < 0.0001f)
			{
				length = 0.0f;
				t = 0.0f;
			}
			else
			{
				t = (cur_time_ - prev->time_) / length;
			}
			break;
		}
	}

	if (i == key_max - 1)
	{
		length = subject->key_[key_max - 1].time_ - subject->key_[key_max - 2].time_;
		prev = &subject->key_[key_max - 2];
		next = &subject->key_[key_max - 1];
		t = 1.0f;
	}

	// scalingの設定
	D3DXMatrixIdentity(&scl_mtx);
	scl_mtx._11 = t * (next->scaling_.x) + (1.0f - t) * (prev->scaling_.x);
	scl_mtx._22 = t * (next->scaling_.y) + (1.0f - t) * (prev->scaling_.y);
	scl_mtx._33 = t * (next->scaling_.z) + (1.0f - t) * (prev->scaling_.z);
	
	// rotationの設定
	D3DXMatrixIdentity(&rot_mtx);
	D3DXQUATERNION quat;
	D3DXQuaternionSlerp(&quat, &prev->rotation_, &next->rotation_, t);
	D3DXMatrixRotationQuaternion(&rot_mtx, &quat);


	// translationの設定
	D3DXMatrixIdentity(&trs_mtx);
	trs_mtx._41 = (1.0f - t) * (prev->translation_.x) + t * (next->translation_.x);
	trs_mtx._42 = (1.0f - t) * (prev->translation_.y) + t * (next->translation_.y);
	trs_mtx._43 = (1.0f - t) * (prev->translation_.z) + t * (next->translation_.z);

	D3DXMatrixIdentity(&subject->bone_matrix_);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &scl_mtx);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &rot_mtx);
	D3DXMatrixMultiply(&subject->bone_matrix_, &subject->bone_matrix_, &trs_mtx);


	// 次に渡す行列の計算
	D3DXMatrixMultiply(&subject->transform_matrix_, &subject->bone_matrix_, parent);

	// ワールドの更新
	D3DXMatrixMultiply(&subject->world_matrix_, &subject->offset_matrix_, &subject->transform_matrix_);

	if (subject->child_ != NULL)
	{
		UpdateBoneMatrix(subject->child_, &subject->transform_matrix_);
	}
	if (subject->sibling_ != NULL)
	{
		UpdateBoneMatrix(subject->sibling_, parent);
	}
}


//-------------------------------------
// end of file
//-------------------------------------
