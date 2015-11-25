//================================================================================
// �e�N�X�`���N���G�[�^�[���� [TextureCreator.cpp]
// Author : TAKESI IWASAWA
//================================================================================

//================================================================================
// �C���N���[�h�t�@�C��
//================================================================================
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "texture_creator.h"

//================================================================================
// �R���X�g���N�^
//================================================================================
TextureCreator::TextureCreator(void) :
	tex_(nullptr),
	size_w_(0.0f),
	size_h_(0.0f),
	dev_depth_(nullptr),
	my_depth_(nullptr),
	dev_buff_suff_(nullptr),
	tex_buff_suff_(nullptr),
	clear_color_(1.0f, 1.0f, 1.0f, 1.0f)
{
}

//================================================================================
// �f�X�g���N�^
//================================================================================
TextureCreator::~TextureCreator(void)
{
	Release();
}

//================================================================================
// �Z�b�g�A�b�v
//================================================================================
bool TextureCreator::Setup(const unsigned int& w, const unsigned int& h, const D3DXCOLOR& setColor)
{
	// �O�̂���
	Release();
	// �T�C�Y�ۑ�
	size_w_ = static_cast<float>(w);
	size_h_ = static_cast<float>(h);
	// �N���A�J���[�ۑ�
	clear_color_ = setColor;

	// �f�o�C�X�̎擾
	IDirect3DDevice9* device = DirectX9Holder::device_;

	HRESULT hr;

	// �e�N�X�`���̍쐬
	hr = D3DXCreateTexture(	device,
							w,
							h,
							1,
							D3DUSAGE_RENDERTARGET,
							D3DFMT_A8R8G8B8,
							D3DPOOL_DEFAULT,
							&tex_);

	if(FAILED(hr))
	{
		return false;
	}

	// �e�N�X�`���̃T�[�t�F�X���x���擾
	tex_->GetSurfaceLevel(0, &tex_buff_suff_);

	// �`��f�o�C�X�ɒ�`����Ă���o�b�t�@�̔\�͂��擾
	IDirect3DSurface9* surf(nullptr);
	device->GetDepthStencilSurface(&surf);
	D3DSURFACE_DESC desc;
	surf->GetDesc(&desc);
	SAFE_RELEASE(surf);

	// �Ǝ��[�x�o�b�t�@���쐬
	hr = device->CreateDepthStencilSurface(	w,
												h,
												desc.Format,
												desc.MultiSampleType,
												desc.MultiSampleQuality,
												FALSE,
												&my_depth_,
												nullptr);

	if(FAILED(hr))
	{
		return false;
	}

	// �f�o�C�X�̐[�x�o�b�t�@�|�C���^�̎擾
	device->GetDepthStencilSurface(&dev_depth_);
	// �f�o�C�X�̃T�[�t�F�X�|�C���^�̎擾
	device->GetRenderTarget(0, &dev_buff_suff_);

	// �r���[�|�[�g�̐ݒ�
	viewport_.X = 0;
	viewport_.Y = 0;
	viewport_.Width = w;
	viewport_.Height = h;
	viewport_.MinZ = 0.0f;
	viewport_.MaxZ = 1.0f;

	return true;
}

//================================================================================
// �e�N�X�`���ւ̃����_�����O�J�n
//================================================================================
void TextureCreator::StartRendering(void)
{
	// �f�o�C�X�̎擾
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// �[�x�T�[�t�F�X�ݒ�
	device->SetDepthStencilSurface(my_depth_);
	// �^�[�Q�b�g�T�[�t�F�X�ݒ�
	device->SetRenderTarget(0, tex_buff_suff_);
	// �f�o�C�X�r���[�|�[�g���擾
	device->GetViewport(&dev_viewport_);
	// �r���[�|�[�g��ݒ�
	device->SetViewport(&viewport_);

	// �T�[�t�F�C�X��������
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), clear_color_, 1.0f, 0);
}

//================================================================================
// �e�N�X�`���ւ̃����_�����O�I��
//================================================================================
void TextureCreator::EndRendering(void)
{
	// �f�o�C�X�̎擾
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// �r���[�|�[�g��߂�
	device->SetViewport(&dev_viewport_);
	// �[�x�T�[�t�F�X�ݒ�
	device->SetDepthStencilSurface(dev_depth_);
	// �^�[�Q�b�g�T�[�t�F�X�ݒ�
	device->SetRenderTarget(0, dev_buff_suff_);
}

//================================================================================
// �����[�X
//================================================================================
void TextureCreator::Release(void)
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(dev_depth_);
	SAFE_RELEASE(my_depth_);
	SAFE_RELEASE(dev_buff_suff_);
	SAFE_RELEASE(tex_buff_suff_);
}
