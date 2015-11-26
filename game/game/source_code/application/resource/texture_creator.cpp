//================================================================================
// テクスチャクリエーター処理 [TextureCreator.cpp]
// Author : TAKESI IWASAWA
//================================================================================

//================================================================================
// インクルードファイル
//================================================================================
#include "../../common/common.h"
#include "../render/renderer.h"
#include "../render/directx9/directx9.h"
#include "../render/directx9/directx9_holder.h"
#include "texture_creator.h"

//================================================================================
// コンストラクタ
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
// デストラクタ
//================================================================================
TextureCreator::~TextureCreator(void)
{
	Release();
}

//================================================================================
// セットアップ
//================================================================================
bool TextureCreator::Setup(const unsigned int& w, const unsigned int& h, const D3DXCOLOR& setColor)
{
	// 念のため
	Release();
	// サイズ保存
	size_w_ = static_cast<float>(w);
	size_h_ = static_cast<float>(h);
	// クリアカラー保存
	clear_color_ = setColor;

	// デバイスの取得
	IDirect3DDevice9* device = DirectX9Holder::device_;

	HRESULT hr;

	// テクスチャの作成
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

	// テクスチャのサーフェスレベル取得
	tex_->GetSurfaceLevel(0, &tex_buff_suff_);

	// 描画デバイスに定義されているバッファの能力を取得
	IDirect3DSurface9* surf(nullptr);
	device->GetDepthStencilSurface(&surf);
	D3DSURFACE_DESC desc;
	surf->GetDesc(&desc);
	SAFE_RELEASE(surf);

	// 独自深度バッファを作成
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

	// デバイスの深度バッファポインタの取得
	device->GetDepthStencilSurface(&dev_depth_);
	// デバイスのサーフェスポインタの取得
	device->GetRenderTarget(0, &dev_buff_suff_);

	// ビューポートの設定
	viewport_.X = 0;
	viewport_.Y = 0;
	viewport_.Width = w;
	viewport_.Height = h;
	viewport_.MinZ = 0.0f;
	viewport_.MaxZ = 1.0f;

	return true;
}

//================================================================================
// テクスチャへのレンダリング開始
//================================================================================
void TextureCreator::StartRendering(void)
{
	// デバイスの取得
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// 深度サーフェス設定
	device->SetDepthStencilSurface(my_depth_);
	// ターゲットサーフェス設定
	device->SetRenderTarget(0, tex_buff_suff_);
	// デバイスビューポートを取得
	device->GetViewport(&dev_viewport_);
	// ビューポートを設定
	device->SetViewport(&viewport_);

	// サーフェイスを初期化
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), clear_color_, 1.0f, 0);
}

//================================================================================
// テクスチャへのレンダリング終了
//================================================================================
void TextureCreator::EndRendering(void)
{
	// デバイスの取得
	IDirect3DDevice9* device = DirectX9Holder::device_;

	// ビューポートを戻す
	device->SetViewport(&dev_viewport_);
	// 深度サーフェス設定
	device->SetDepthStencilSurface(dev_depth_);
	// ターゲットサーフェス設定
	device->SetRenderTarget(0, dev_buff_suff_);
}

//================================================================================
// リリース
//================================================================================
void TextureCreator::Release(void)
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(dev_depth_);
	SAFE_RELEASE(my_depth_);
	SAFE_RELEASE(dev_buff_suff_);
	SAFE_RELEASE(tex_buff_suff_);
}
