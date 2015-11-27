//=========================================================
// fbx_my_container.cpp
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "fbx_my_container.h"


//-------------------------------------
// warning
//-------------------------------------
#pragma warning(disable:4996)
#pragma warning(disable:4244)


//-------------------------------------
// FbxMyContainer
//-------------------------------------
FbxMyContainer::FbxMyContainer(const std::string &path)
{
	file_pass_ = path;
	Load(path);
}


//-------------------------------------
// ~FbxMyContainer
//-------------------------------------
FbxMyContainer::~FbxMyContainer(void)
{
	for(int i = 0; i < mesh_count_; i++){
		SAFE_RELEASE(mesh_[i].vertex_);
		SAFE_RELEASE(mesh_[i].index_);
	}
	SAFE_DELETE_ARRAY(mesh_);

	for(int i = 0; i < bone_count_; i++){
		DeleteBone(&bone_[i]);
	}
	SAFE_DELETE_ARRAY(bone_);
}


//-------------------------------------
// ~Load
//-------------------------------------
void FbxMyContainer::Load(const std::string &path)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 device = nullptr;
	FbxManager *manager = nullptr;
	FbxImporter *importer = nullptr;
	FbxScene *scene = nullptr;
	FbxNode *root = nullptr;
	int i(0);

	// マネージャ生成
	manager = FbxManager::Create();
	// インポータ生成
	importer = FbxImporter::Create(manager, "");
	// シーンオブジェクト生成
	scene = FbxScene::Create(manager, path.c_str());


	// ファイル読み込み
	importer->Initialize(path.c_str());
	// シーンデータの抽出
	importer->Import(scene);

	// ルートボーンの抽出
	root = scene->GetRootNode();

	// ルートボーンの取得
	FbxNode *root_bone(GetRootBone(root));


	// ボーンを数える
	bone_count_ = 0;
	bone_cursor_ = 0;
	CountUpBone(root_bone);

	// ボーン配列を生成
	bone_ = new BONE[bone_count_];
	for(i = 0; i < bone_count_; i++)
	{
		bone_[i].id_ = i;
		D3DXMatrixIdentity(&bone_[i].bone_matrix_);
		D3DXMatrixIdentity(&bone_[i].offset_matrix_);
		D3DXMatrixIdentity(&bone_[i].transform_matrix_);
		D3DXMatrixIdentity(&bone_[i].world_matrix_);

		bone_[i].child_ = NULL;
		bone_[i].sibling_ = NULL;

		memset(bone_[i].name_, '\0', 128);

		bone_[i].key_max_ = 0;
		bone_[i].key_ = NULL;
	}

	// 名前を取得
	LoadBoneName(root_bone);

	// ボーンの階層読み込み
	bone_cursor_ = 0;
	LoadBoneHierarchy(root_bone);

	// アニメーションを取得
	LoadAnimationKeyFrame(root_bone, scene);

	// メッシュ配列生成
	mesh_count_ = 0;
	mesh_cursor_ = 0;
	CountUpMesh(root);

	// メッシュ配列を生成
	mesh_ = new MESH[mesh_count_];
	for(i = 0; i < mesh_count_; i++)
	{
		mesh_[i].vertex_ = NULL;
		mesh_[i].index_ = NULL;
		mesh_[i].vertex_max_ = 0;
		mesh_[i].index_max_ = 0;
		memset(&mesh_[i].material_, 0, sizeof(D3DMATERIAL9));
		memset(mesh_[i].texture_filename_, '\0', 128);
		mesh_[i].texture_ = NULL;
		memset(mesh_[i].uvset_name_, '\0', 128);
	}

	// メッシュ読み込み
	mesh_cursor_ = 0;
	LoadMesh(root);


	// 破棄処理
	scene->Destroy();
	scene = nullptr;

	importer->Destroy();
	importer = nullptr;

	manager->Destroy();
	manager = nullptr;
}

//-------------------------------------
// DeleteBone()
//-------------------------------------
void FbxMyContainer::DeleteBone(BONE *bone)
{
	if(bone->child_){
		DeleteBone(bone->child_);
	}
	if(bone->sibling_){
		DeleteBone(bone->sibling_);
	}
	SAFE_DELETE_ARRAY(bone->key_);
}

//-------------------------------------
// GetRootBone()
//-------------------------------------
FbxNode *FbxMyContainer::GetRootBone(FbxNode *node)
{
	// 親がなかったらリターン
	if (node == NULL)
	{
		return NULL;
	}

	// 変数宣言
	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	// ルートボーンだったらリターン
	if (attribute != NULL &&
		attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		return node;
	}

	// 再帰処理
	int child_count(node->GetChildCount());
	int i(0);

	for (i = 0; i < child_count; i++)
	{
		FbxNode *checked(NULL);
		FbxNode *child(node->GetChild(i));

		checked = GetRootBone(child);
		if (checked != NULL)
		{
			return checked;
		}
	}

	return NULL;
}


//-------------------------------------
// CountUpBone()
//-------------------------------------
void FbxMyContainer::CountUpBone(FbxNode *node)
{
	// 親がなかったらリターン
	if (node == NULL)
	{
		return;
	}

	// 変数宣言
	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	// ボーンの数を加算
	if (attribute != NULL &&
		attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		bone_count_++;
	}

	// 再帰処理
	int child_max(node->GetChildCount());
	int i(0);

	for (i = 0; i < child_max; i++)
	{
		FbxNode *child(node->GetChild(i));
		CountUpBone(child);
	}

}


//-------------------------------------
// LoadBoneName()
//-------------------------------------
void FbxMyContainer::LoadBoneName(FbxNode *node)
{
	// nullチェック
	if (node == NULL)
	{
		return;
	}

	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	if (attribute == NULL)
	{
		return;
	}

	if (attribute->GetAttributeType() != FbxNodeAttribute::eSkeleton)
	{
		return;
	}

	BONE *dst_bone(&bone_[bone_cursor_]);
	bone_cursor_++;

	// 名前の読み込み
	const char *name = node->GetName();
	strcpy(dst_bone->name_, name);

	// 再帰処理
	int child_count(node->GetChildCount());
	int i(0);
	for (i = 0; i < child_count; i++)
	{
		FbxNode *child(node->GetChild(i));
		LoadBoneName(child);
	}
}


//-------------------------------------
// LoadBoneHierarchy()
//-------------------------------------
void FbxMyContainer::LoadBoneHierarchy(FbxNode *node)
{
	if (node == NULL)
	{
		return;
	}

	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	if (attribute == NULL)
	{
		return;
	}

	if (attribute->GetAttributeType() != FbxNodeAttribute::eSkeleton)
	{
		return;
	}

	BONE *dst_bone(&bone_[bone_cursor_]);
	bone_cursor_++;

	int child_count(node->GetChildCount());
	int i(0);
	for (i = 0; i < child_count; i++)
	{
		FbxNode *child(node->GetChild(i));

		attribute = child->GetNodeAttribute();

		if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			const char *child_name(child->GetName());

			BONE *child_bone(SearchBoneByName((char*)child_name));

			if (dst_bone->child_ == NULL)
			{
				dst_bone->child_ = child_bone;
			}
			else
			{
				BONE *sibling_end(dst_bone->child_);
				while (sibling_end->sibling_ != NULL)
				{
					sibling_end = sibling_end->sibling_;
				}

				sibling_end->sibling_ = child_bone;
			}
		}
	}

	for (i = 0; i < child_count; i++)
	{
		FbxNode *child(node->GetChild(i));
		LoadBoneHierarchy(child);
	}

}


//-------------------------------------
// SearchBoneByName()
//-------------------------------------
BONE *FbxMyContainer::SearchBoneByName(char *name)
{
	if (bone_ == NULL)
	{
		return NULL;
	}
	if (name == NULL)
	{
		return NULL;
	}

	int i(0);

	for (i = 0; i < bone_count_; i++)
	{
		if (strcmp(bone_[i].name_, name) == 0)
		{
			return bone_ + i;
		}
	}

	return NULL;
}


//-------------------------------------
// LoadAnimationKeyFrame()
//-------------------------------------
void FbxMyContainer::LoadAnimationKeyFrame(FbxNode *node, FbxScene *scene)
{
	// 親がなかったらリターン
	if (node == NULL)
	{
		return;
	}

	// 変数宣言
	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	// ボーンの数を加算
	if (attribute != NULL &&
		attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		LoadAnimationKeyFrameFormNode(node, scene);
	}

	// 再帰処理
	int child_max(node->GetChildCount());
	int i(0);

	for (i = 0; i < child_max; i++)
	{
		FbxNode *child(node->GetChild(i));
		LoadAnimationKeyFrame(child, scene);
	}

}


//-------------------------------------
// LoadAnimaitonKeyFrameFromNode()
//-------------------------------------
void FbxMyContainer::LoadAnimationKeyFrameFormNode(FbxNode *node, FbxScene *scene)
{
	// 変数宣言
	int i(0), j(0);

	// 名前を取得
	const char *bone_name(node->GetName());
	BONE *dst_bone(SearchBoneByName((char *)bone_name));

	// 念のためチェックする
	if (dst_bone == NULL) return;

	// シーンからアニメーションスタック数を取得
	int stack_count(scene->GetSrcObjectCount<FbxAnimStack>());

	// アニメーションスタック取得(0しか使わない)
	if (stack_count == 0) return;

	// アニメーションスタックを取得
	FbxAnimStack *stack(scene->GetSrcObject<FbxAnimStack>(0));

	// 現在のアニメーションスタックを設定
	scene->SetCurrentAnimationStack(stack);

	// スタックからアニメーションレイヤ数を取得
	int layer_count(stack->GetMemberCount<FbxAnimLayer>());

	// スタックからアニメーションレイヤを取得
	if (layer_count == 0) return;
	FbxAnimLayer *layer(stack->GetMember<FbxAnimLayer>(0));

	// フレーム比を取得
	uint64_t one_frame(0);
	float time_scale(0.0f);
	FbxTime::EMode time_mode(scene->GetGlobalSettings().GetTimeMode());

	switch (time_mode)
	{
	case FbxTime::EMode::eFrames24:
		time_scale = 60.0f / 24.0f;
		one_frame = 1924423250;
		break;
	case FbxTime::EMode::eFrames30:
		time_scale = 60.0f / 30.0f;
		one_frame = 1539538600;
		break;
	case FbxTime::EMode::eFrames60:
		time_scale = 60.0f / 60.0f;
		one_frame = 769769300;
		break;
	default:
		time_scale = 1.0f;
		break;
	}

	FbxTime time_end(stack->LocalStop);

	FbxAnimCurve *x_curve(NULL), *y_curve(NULL), *z_curve(NULL);
	int x_key_count(0), y_key_count(0), z_key_count(0);

	x_curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	y_curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	z_curve = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);

	// キーフレームの数を取得
	if (x_curve != NULL){ x_key_count = x_curve->KeyGetCount(); }
	if (y_curve != NULL){ y_key_count = y_curve->KeyGetCount(); }
	if (z_curve != NULL){ z_key_count = z_curve->KeyGetCount(); }

	// キーフレーム格納用コンテナ定義
	std::set<FbxTime> key_frames;
	std::set<FbxTime>::iterator it;
	key_frames.clear();

	//キーフレームの取得、同じものはまとめたいため、STLを使用
	for (int i = 0; i < x_key_count; i++)
	{
		FbxTime work = x_curve->KeyGetTime(i);
		key_frames.insert(work);
	}
	for (int i = 0; i < y_key_count; i++)
	{
		FbxTime work = y_curve->KeyGetTime(i);
		key_frames.insert(work);
	}
	for (int i = 0; i < z_key_count; i++)
	{
		FbxTime work = z_curve->KeyGetTime(i);
		key_frames.insert(work);
	}

	// ※translation, scalingは省く、たぶん一緒
	dst_bone->key_max_ = key_frames.size();
	dst_bone->key_ = new KEY_FRAME[dst_bone->key_max_];

	for (int i = 0; i < dst_bone->key_max_; i++)
	{
		dst_bone->key_[i].time_ = 0.0f;
		dst_bone->key_[i].translation_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		dst_bone->key_[i].rotation_ = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		dst_bone->key_[i].scaling_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	int sec(0);
	it = key_frames.begin();
	for (i = 0; i < dst_bone->key_max_; i++)
	{
		int work_frame(0);
		FbxTime key_frame(*it);
		int dummy(0);

		// キーの時間を取得
		work_frame = key_frame.Get() / one_frame;
		dst_bone->key_[i].time_ = (float)work_frame;

		// 各キーフレームでのrotate行列を取得
		FbxMatrix work_mtx = node->EvaluateLocalTransform(key_frame);
		D3DXMATRIX transform;
		for (int j = 0; j < 16; j++)
		{
			transform.m[j / 4][j % 4] = (float)work_mtx.Get(j / 4, j % 4);
		}

		D3DXMATRIX translation, rotation, scaling;

		// 行列から各要素を抜き出す
		ExtractTranslationFromMatrix(&translation, &transform);
		ExtractRotationFromMatrix(&rotation, &transform);
		ExtractScalingFromMatrix(&scaling, &transform);

		// translation格納
		dst_bone->key_[i].translation_.x = translation._41;
		dst_bone->key_[i].translation_.y = translation._42;
		dst_bone->key_[i].translation_.z = translation._43;

		// rotation格納
		D3DXQuaternionRotationMatrix(&dst_bone->key_[i].rotation_, &rotation);

		// scaling格納
		dst_bone->key_[i].scaling_.x = scaling._11;
		dst_bone->key_[i].scaling_.y = scaling._22;
		dst_bone->key_[i].scaling_.z = scaling._33;

		it++;
	}

	// オフセットの計算
	FbxTime zero_frame = FBXSDK_TIME_ZERO;
	FbxMatrix init_mtx = node->EvaluateGlobalTransform(zero_frame);
	D3DXMATRIX init;
	for (j = 0; j < 16; j++)
	{
		init.m[j / 4][j % 4] = (float)init_mtx.Get(j / 4, j % 4);
	}
	D3DXMatrixInverse(&dst_bone->offset_matrix_, NULL, &init);

	// コンテナクリア
	key_frames.clear();

	// 0スタートに矯正
	float min_time(0.0f);
	if (dst_bone->key_max_ > 0)
	{
		min_time = dst_bone->key_[0].time_;
	}

	for (i = 0; i < dst_bone->key_max_; i++)
	{
		if (min_time > dst_bone->key_[i].time_)
		{
			min_time = dst_bone->key_[i].time_;
		}
	}

	for (i = 0; i < dst_bone->key_max_; i++)
	{
		dst_bone->key_[i].time_ -= min_time;
	}

}


//-------------------------------------
// CountUpMesh()
//-------------------------------------
void FbxMyContainer::CountUpMesh(FbxNode *node)
{
	// 親がなかったらリターン
	if (node == NULL)
	{
		return;
	}

	// 変数宣言
	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	// ボーンの数を加算
	if (attribute != NULL &&
		attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		int material_count(node->GetMaterialCount());
		mesh_count_ += material_count;
	}

	// 再帰処理
	int child_max(node->GetChildCount());
	int i(0);

	for (i = 0; i < child_max; i++)
	{
		FbxNode *child(node->GetChild(i));
		CountUpMesh(child);
	}
}


//-------------------------------------
// LoadMesh()
//-------------------------------------
void FbxMyContainer::LoadMesh(FbxNode *node)
{
	// 親がなかったらリターン
	if (node == NULL)
	{
		return;
	}

	// 変数宣言
	FbxNodeAttribute *attribute(node->GetNodeAttribute());

	// ボーンの数を加算
	if (attribute != NULL &&
		attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		LoadMeshFromNode(node);
	}

	// 再帰処理
	int child_max(node->GetChildCount());
	int i(0);

	for (i = 0; i < child_max; i++)
	{
		FbxNode *child(node->GetChild(i));
		LoadMesh(child);
	}
}


//-------------------------------------
// LoadMeshFromNode()
//-------------------------------------
void FbxMyContainer::LoadMeshFromNode(FbxNode *node)
{
	// 変数宣言
	int i(0);

	FbxMesh *mesh(node->GetMesh());

	int material_count(node->GetMaterialCount());

	// マテリアルレイヤ取得
	int layer_count(mesh->GetLayerCount());
	FbxLayerElementMaterial *material_layer(NULL);

	for (i = 0; i < layer_count; i++)
	{
		material_layer = mesh->GetLayer(i)->GetMaterials();
		if (material_layer != NULL)
		{
			break;
		}
	}

	// スキンの読み込み
	int skin_count(mesh->GetDeformerCount(FbxDeformer::eSkin));
	int cluster_count(0);
	FbxSkin *skin(NULL);
	if (skin_count > 0)
	{
		skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
	}
	else
	{
		// スキニングされていないので、weightやindexを0クリア推奨
	}


	int primitive_count(mesh->GetPolygonCount());

	int *layer_table(new int[primitive_count]);
	memset(layer_table, 0, sizeof(int) * primitive_count);

	for (i = 0; i < primitive_count; i++)
	{
		int material_index = material_layer->GetIndexArray().GetAt(i);
		layer_table[i] = material_index;
	}


	for (i = 0; i < material_count; i++)
	{
		MESH *dst_mesh(&this->mesh_[mesh_cursor_ + i]);

		// マテリアル取得
		LoadMaterialFromNode(node, i);

		LoadTextureFromNode(node, i, dst_mesh->uvset_name_);
	}


	// メッシュの中身を取得
	int material_index(0);
	int primitive_index(0);
	for (material_index = 0; material_index < material_count; material_index++)
	{
		std::vector<int> index_array;
		std::vector<VertexBlend3D> vertex_array;
		std::vector<VertexBlend3D>::iterator it;

		int mesh_index(mesh_cursor_ + material_index);
		MESH *dst_mesh(&this->mesh_[mesh_index]);

		for (primitive_index = 0; primitive_index < primitive_count; primitive_index++)
		{
			if (layer_table[primitive_index] != material_index)
			{
				continue;
			}

			int primitive_vertex_num(mesh->GetPolygonSize(primitive_index));

			int base(0);
			for (base = 0; base < primitive_vertex_num - 2; base++)
			{
				for (i = 0; i < 3; i++)
				{
					VertexBlend3D work_vertex = {};

					int point_index(i);
					if (point_index != 0)
					{
						point_index += base;
					}

					int vertex_id = mesh->GetPolygonVertex(primitive_index, point_index);

					FbxVector4 position = mesh->GetControlPointAt(vertex_id);

					if (position[3] != 0.0f)
					{
						work_vertex.position.x = (float)position[0] / (float)position[3];
						work_vertex.position.y = (float)position[1] / (float)position[3];
						work_vertex.position.z = (float)position[2] / (float)position[3];
					}
					else
					{
						work_vertex.position.x = (float)position[0];
						work_vertex.position.y = (float)position[1];
						work_vertex.position.z = (float)position[2];
					}

					FbxVector4 normal;
					mesh->GetPolygonVertexNormal(primitive_index, point_index, normal);
					if (normal[3] != 0.0f)
					{
						work_vertex.normal.x = (float)normal[0] / (float)normal[3];
						work_vertex.normal.y = (float)normal[1] / (float)normal[3];
						work_vertex.normal.z = (float)normal[2] / (float)normal[3];
					}
					else
					{
						work_vertex.normal.x = (float)normal[0];
						work_vertex.normal.y = (float)normal[1];
						work_vertex.normal.z = (float)normal[2];
					}

					FbxVector2 texcoord(0.0, 0.0);
					bool dummy = false;
					mesh->GetPolygonVertexUV(primitive_index, point_index, dst_mesh->uvset_name_, texcoord, dummy);
					work_vertex.texture.x = (float)texcoord[0];
					work_vertex.texture.y = 1.0f - (float)texcoord[1];

					LoadBoneIndexAndWeight(skin, &work_vertex, vertex_id);

					work_vertex.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					it = find(vertex_array.begin(), vertex_array.end(), work_vertex);

					int vertex_index = distance(vertex_array.begin(), it);

					if (it == vertex_array.end())
					{
						vertex_array.push_back(work_vertex);
					}

					index_array.push_back(vertex_index);
				}
			}
		}

		LPDIRECT3DDEVICE9 device = NULL;

		int size = index_array.size();
		dst_mesh->index_max_ = size;

		if (FAILED(DirectX9Holder::device_->CreateIndexBuffer(
			sizeof(WORD)* size,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&(dst_mesh->index_),
			NULL)))
		{
			dst_mesh->index_ = NULL;
		}

		WORD *work_index(NULL);
		dst_mesh->index_->Lock(0, 0, (void**)&work_index, 0);

		for (i = 0; i < size; i++)
		{
			work_index[i] = index_array[i];
		}

		dst_mesh->index_->Unlock();


		size = vertex_array.size();
		dst_mesh->vertex_max_ = size;
		if (FAILED(DirectX9Holder::device_->CreateVertexBuffer(
			sizeof(VertexBlend3D) * size,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_BLEND_3D,
			D3DPOOL_MANAGED,
			&dst_mesh->vertex_,
			NULL)))
		{
			dst_mesh->vertex_ = NULL;
		}

		VertexBlend3D *work_vertex(NULL);
		dst_mesh->vertex_->Lock(0, 0, (void **)&work_vertex, 0);

		for (i = 0; i < size; i++)
		{
			work_vertex[i] = vertex_array[i];
		}

		dst_mesh->vertex_->Unlock();

		index_array.clear();
		vertex_array.clear();
	}

	delete[] layer_table;

	mesh_cursor_ += material_count;

}


//-------------------------------------
// LoadMaterialFromNode()
//-------------------------------------
void FbxMyContainer::LoadMaterialFromNode(FbxNode *node, int material_index)
{
	FbxSurfaceMaterial *material(node->GetMaterial(material_index));

	if (material == NULL)
	{
		return;
	}

	MESH *dst_mesh(&this->mesh_[mesh_cursor_ + material_index]);

	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		FbxSurfaceLambert *lambert = (FbxSurfaceLambert*)material;

		dst_mesh->material_.Ambient.r = (float)lambert->Ambient.Get()[0];
		dst_mesh->material_.Ambient.g = (float)lambert->Ambient.Get()[1];
		dst_mesh->material_.Ambient.b = (float)lambert->Ambient.Get()[2];
		//dst_mesh->material_.Ambient.a = (float)lambert->Ambient.Get()[3];



		dst_mesh->material_.Diffuse.r = (float)lambert->Diffuse.Get()[0];
		dst_mesh->material_.Diffuse.g = (float)lambert->Diffuse.Get()[1];
		dst_mesh->material_.Diffuse.b = (float)lambert->Diffuse.Get()[2];
		//dst_mesh->material_.Diffuse.a = (float)lambert->Diffuse.Get()[3];



		dst_mesh->material_.Emissive.r = (float)lambert->Emissive.Get()[0];
		dst_mesh->material_.Emissive.g = (float)lambert->Emissive.Get()[1];
		dst_mesh->material_.Emissive.b = (float)lambert->Emissive.Get()[2];
		//dst_mesh->material_.Emissive.a = (float)lambert->Emissive.Get()[3];


		dst_mesh->material_.Ambient.a = (float)lambert->TransparencyFactor.Get();
		dst_mesh->material_.Diffuse.a = (float)lambert->TransparencyFactor.Get();
		dst_mesh->material_.Emissive.a = (float)lambert->TransparencyFactor.Get();
	}
	else if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		FbxSurfacePhong *phong = (FbxSurfacePhong*)material;

		dst_mesh->material_.Ambient.r = (float)phong->Ambient.Get()[0];
		dst_mesh->material_.Ambient.g = (float)phong->Ambient.Get()[1];
		dst_mesh->material_.Ambient.b = (float)phong->Ambient.Get()[2];
		//dst_mesh->material_.Ambient.a = (float)phong->Ambient.Get()[3];



		dst_mesh->material_.Diffuse.r = (float)phong->Diffuse.Get()[0];
		dst_mesh->material_.Diffuse.g = (float)phong->Diffuse.Get()[1];
		dst_mesh->material_.Diffuse.b = (float)phong->Diffuse.Get()[2];
		//dst_mesh->material_.Diffuse.a = (float)phong->Diffuse.Get()[3];



		dst_mesh->material_.Emissive.r = (float)phong->Emissive.Get()[0];
		dst_mesh->material_.Emissive.g = (float)phong->Emissive.Get()[1];
		dst_mesh->material_.Emissive.b = (float)phong->Emissive.Get()[2];
		//dst_mesh->material_.Emissive.a = (float)phong->Emissive.Get()[3];


		dst_mesh->material_.Specular.r = (float)phong->Specular.Get()[0];
		dst_mesh->material_.Specular.g = (float)phong->Specular.Get()[1];
		dst_mesh->material_.Specular.b = (float)phong->Specular.Get()[2];
		//dst_mesh->material_.Specular.a = (float)phong->Specular.Get()[3];


		dst_mesh->material_.Ambient.a = (float)phong->TransparencyFactor.Get();
		dst_mesh->material_.Diffuse.a = (float)phong->TransparencyFactor.Get();
		dst_mesh->material_.Emissive.a = (float)phong->TransparencyFactor.Get();
		dst_mesh->material_.Specular.a = (float)phong->TransparencyFactor.Get();

		dst_mesh->material_.Power = (float)phong->Shininess.Get();
	}
}


//-------------------------------------
// LoadTextureFromNode()
//-------------------------------------
void FbxMyContainer::LoadTextureFromNode(FbxNode *node, int material_index, char *dst_uvset)
{
	// 変数宣言
	FbxSurfaceMaterial *material(node->GetMaterial(material_index));

	if (material == NULL)
	{
		return;
	}


	MESH *dst_mesh(&this->mesh_[mesh_cursor_ + material_index]);
	FbxProperty diffuse_property(material->FindProperty(FbxSurfaceMaterial::sDiffuse));
	int layered_texture_count(diffuse_property.GetSrcObjectCount<FbxLayeredTexture>());

	if (layered_texture_count == 0)
	{
		int texture_count = diffuse_property.GetSrcObjectCount<FbxFileTexture>();
		int i(0);
		for (i = 0; i < texture_count; i++)
		{
			FbxFileTexture *texture = diffuse_property.GetSrcObject<FbxFileTexture>(i);

			if (texture == NULL)
			{
				continue;
			}

			const char *texture_filename = texture->GetRelativeFileName();

			strcpy(dst_mesh->texture_filename_, "data/TEXTURE/chara.tga");

			strcpy(dst_uvset, texture->UVSet.Get().Buffer());

			break;
		}
	}

	return;
}


//-------------------------------------
// LoadBoneIndexAndWeight()
//-------------------------------------
void FbxMyContainer::LoadBoneIndexAndWeight(FbxSkin *skin, VertexBlend3D *dst_vertex, int vertex_id)
{
	if (skin == NULL)
	{
		return;
	}

	int cluster_count(skin->GetClusterCount());
	int inful_bone_index = 0;
	int i(0), j(0);

	for (i = 0; i < cluster_count; i++)
	{
		FbxCluster *cluster(skin->GetCluster(i));

		int inful_count(cluster->GetControlPointIndicesCount());
		for (j = 0; j < inful_count; j++)
		{
			if (vertex_id == cluster->GetControlPointIndices()[j])
			{
				break;
			}
		}

		if (j == inful_count)
		{
			continue;
		}

		char *bone_name = (char*)cluster->GetLink()->GetName();

		BONE *work_bone = SearchBoneByName(bone_name);

		dst_vertex->matrix_index |= work_bone->id_ << (8 * inful_bone_index);

		if (inful_bone_index != 3)
		{
			dst_vertex->weight[inful_bone_index] = (float)cluster->GetControlPointWeights()[j];
		}

		inful_bone_index++;

		if (inful_bone_index == 4)
		{
			break;
		}
	}
}

//-------------------------------------
// ExatractTranslationFromMatrix()
//-------------------------------------
void FbxMyContainer::ExtractTranslationFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src)
{
	D3DXMATRIX work;
	D3DXMatrixIdentity(&work);
	work._41 = src->_41;
	work._42 = src->_42;
	work._43 = src->_43;
	*out = work;
}


//-------------------------------------
// ExtractRotationFromMatrix()
//-------------------------------------
void FbxMyContainer::ExtractRotationFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src)
{
	D3DXMATRIX work;
	D3DXVECTOR3 e;
	work = *src;

	work._41 = 0.0f;
	work._42 = 0.0f;
	work._43 = 0.0f;

	e.x = work._11;
	e.y = work._21;
	e.z = work._31;
	D3DXVec3Normalize(&e, &e);
	work._11 = e.x;
	work._21 = e.y;
	work._31 = e.z;

	e.x = work._12;
	e.y = work._22;
	e.z = work._32;
	D3DXVec3Normalize(&e, &e);
	work._12 = e.x;
	work._22 = e.y;
	work._32 = e.z;

	e.x = work._13;
	e.y = work._23;
	e.z = work._33;
	D3DXVec3Normalize(&e, &e);
	work._13 = e.x;
	work._23 = e.y;
	work._33 = e.z;

	*out = work;
}


//-------------------------------------
// ExtractScalingEromMatrix()
//-------------------------------------
void FbxMyContainer::ExtractScalingFromMatrix(D3DXMATRIX *out, D3DXMATRIX *src)
{
	D3DXMATRIX work;
	D3DXVECTOR3 e;
	D3DXVECTOR3 scale;

	work = *src;

	e.x = work._11;
	e.y = work._12;
	e.z = work._13;
	scale.x = D3DXVec3Length(&e);


	e.x = work._21;
	e.y = work._22;
	e.z = work._23;
	scale.y = D3DXVec3Length(&e);


	e.x = work._31;
	e.y = work._32;
	e.z = work._33;
	scale.z = D3DXVec3Length(&e);

	D3DXMatrixIdentity(&work);
	work._11 = scale.x;
	work._22 = scale.y;
	work._33 = scale.z;

	*out = work;
}