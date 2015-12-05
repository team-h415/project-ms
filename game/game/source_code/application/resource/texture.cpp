//================================================================================
// �e�N�X�`������ [Texture.cpp]
// Author : TAKESI IWASAWA
//================================================================================


//================================================================================
// �C���N���[�h�t�@�C��
//================================================================================
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "texture.h"

//================================================================================
// �R���X�g���N�^
//================================================================================
Texture::Texture(const char* path) :
	tex_(nullptr),
	tex_path_(nullptr)
{
	// �p�Xnullptr�`�F�b�N
	if(path == nullptr)
	{
		return;
	}

	// �e�N�X�`�����[�h
	if(!Load(path))
	{
		return;
	}

	// �e�N�X�`���l�[���u������
	int name_len(strnlen(path, 255) + sizeof(char));
	tex_path_ = new char[name_len];
	strcpy_s(tex_path_, name_len, path);
}

//================================================================================
// �f�X�g���N�^
//================================================================================
Texture::~Texture(void)
{
	// ���
	SAFE_RELEASE(tex_);
	SAFE_DELETE_ARRAY(tex_path_);
}

//================================================================================
// �e�N�X�`���擾
//================================================================================
bool Texture::Load(const char* path)
{
	// �f�o�C�X�̎擾
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// �e�N�X�`�����擾
	D3DXIMAGE_INFO	info;
	D3DXGetImageInfoFromFile(path, &info);

	// �ǂݍ���
	if(FAILED(D3DXCreateTextureFromFileEx(	device,
											path,
											info.Width,
											info.Height,
											1,
											0,
											D3DFMT_A8R8G8B8,
											D3DPOOL_MANAGED,
											D3DX_FILTER_NONE,
											D3DX_FILTER_NONE,
											0,
											nullptr,
											nullptr,
											&tex_)))
	{
		std::string warning;
		warning = path;
		warning += ": ���̃t�@�C����������܂���";
		ASSERT_WARNING(warning.c_str());

		return false;
	}

	return true;
}
