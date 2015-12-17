//=========================================================
// shader_manager.cpp
// author:takeshi iwasawa
//=========================================================

//-------------------------------------
// include
//-------------------------------------
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "shader.h"
#include "shader_manager.h"


//================================================================================
// static
//================================================================================
std::list<Shader*>		ShaderManager::shader_list_;


//================================================================================
// �V�F�[�_�[�̎擾
//================================================================================
Shader* ShaderManager::Get(const std::string &file_path)
{
	for(auto it = shader_list_.begin(); it != shader_list_.end(); it++)
	{
		// �t�@�C���p�X��r
		if(file_path == (*it)->file_path())
		{
			return (*it);
		}
	}

	// �V�����ǉ�
	Shader* shader = new Shader(file_path);
	shader_list_.push_back(shader);

	return shader;
}

//================================================================================
// �S�V�F�[�_�[�̉��
//================================================================================
void ShaderManager::AllRelease(void)
{
	for(auto it = shader_list_.begin(); it != shader_list_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	shader_list_.clear();
}