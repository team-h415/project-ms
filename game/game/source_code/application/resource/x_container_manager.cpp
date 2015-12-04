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
#include "x_container.h"
#include "x_container_manager.h"


//================================================================================
// static
//================================================================================
std::list<XContainer*>		XContainerManager::x_container_list_;


//================================================================================
// FBX�R���e�i�̎擾
//================================================================================
XContainer* XContainerManager::GetContainer(const std::string &file_path)
{
	for(auto it = x_container_list_.begin(); it != x_container_list_.end(); it++)
	{
		// �t�@�C���p�X��r
		if(file_path == (*it)->file_path_)
		{
			return (*it);
		}
	}

	// �V�����ǉ�
	XContainer* x_container = new XContainer(file_path);
	x_container_list_.push_back(x_container);

	return x_container;
}

//================================================================================
// �SFBX���
//================================================================================
void XContainerManager::AllRelease(void)
{
	for(auto it = x_container_list_.begin(); it != x_container_list_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	x_container_list_.clear();
}