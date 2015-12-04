//=========================================================
// fbx_container_manager.cpp
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
#include "fbx_container_manager.h"


//================================================================================
// static
//================================================================================
std::list<FbxMyContainer*>		FbxContainerManager::fbx_container_list_;


//================================================================================
// FBXコンテナの取得
//================================================================================
FbxMyContainer* FbxContainerManager::GetContainer(const std::string &file_path)
{
	for(auto it = fbx_container_list_.begin(); it != fbx_container_list_.end(); it++)
	{
		// ファイルパス比較
		if(file_path == (*it)->file_path_)
		{
			return (*it);
		}
	}

	// 新しく追加
	FbxMyContainer* fbx_container = new FbxMyContainer(file_path);
	fbx_container_list_.push_back(fbx_container);

	return fbx_container;
}

//================================================================================
// 全FBX解放
//================================================================================
void FbxContainerManager::AllRelease(void)
{
	for(auto it = fbx_container_list_.begin(); it != fbx_container_list_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	fbx_container_list_.clear();
}