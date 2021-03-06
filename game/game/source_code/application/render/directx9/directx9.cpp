//=========================================================
// directx9.cpp
// author;ryuya nakamura
//=========================================================

//-------------------------------------
// include
//-------------------------------------
#include "../../../common/common.h"
#include "../renderer.h"
#include "directx9.h"
#include "directx9_holder.h"
#include "../../window/window_holder.h"



//-------------------------------------
// DirectX9()
//-------------------------------------
DirectX9::DirectX9()
{
	D3DPRESENT_PARAMETERS parameters;
	D3DDISPLAYMODE display;
	LPDIRECT3D9 directx9;
	LPDIRECT3DDEVICE9 device;
	DWORD multisample_quality;

	directx9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!directx9)
	{
		ASSERT_ERROR("DirectX9の生成に失敗");
		return;
	}

	if (FAILED(directx9->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&display)))
	{
		ASSERT_ERROR("ディスプレイ状態の取得に失敗");
		return;
	}

	ZeroMemory(&parameters, sizeof(parameters));
	parameters.BackBufferCount = 1;
	parameters.BackBufferWidth =
		static_cast<UINT>(SCREEN_WIDTH);
	parameters.BackBufferHeight =
		static_cast<UINT>(SCREEN_HEIGHT);
	parameters.BackBufferFormat = display.Format;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.Windowed = TRUE;
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;

	directx9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		display.Format,
		TRUE,
		D3DMULTISAMPLE_4_SAMPLES,
		&multisample_quality);

	parameters.MultiSampleQuality = multisample_quality - 1;

	if (parameters.Windowed)
	{
		parameters.FullScreen_RefreshRateInHz = 0;
		parameters.PresentationInterval =
			D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		parameters.BackBufferFormat = D3DFMT_R5G6B5;
		parameters.FullScreen_RefreshRateInHz =
			D3DPRESENT_RATE_DEFAULT;
		parameters.PresentationInterval =
			D3DPRESENT_INTERVAL_DEFAULT;
	}


	if (FAILED(directx9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		WindowHolder::handle_,
		D3DCREATE_HARDWARE_VERTEXPROCESSING |
		D3DCREATE_MULTITHREADED,
		&parameters,
		&device)))
	{
		if (FAILED(directx9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			WindowHolder::handle_,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING |
			D3DCREATE_MULTITHREADED,
			&parameters,
			&device)))
		{
			if (FAILED(directx9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				WindowHolder::handle_,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING |
				D3DCREATE_MULTITHREADED,
				&parameters,
				&device)))
			{
				ASSERT_ERROR("描画デバイスの生成に失敗");
				return;
			}
		}
	}

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);

	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, 1);

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	D3DVERTEXELEMENT9 velement2d[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END(),
	};

	if (FAILED(device->CreateVertexDeclaration(
		velement2d,
		&DirectX9Holder::vertex_declaration_2d_)))
	{
		ASSERT_ERROR("2D用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velement3d[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END(),
	};

	if (FAILED(device->CreateVertexDeclaration(
		velement3d,
		&DirectX9Holder::vertex_declaration_3d_)))
	{
		ASSERT_ERROR("3D用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementx[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END(),
	};

	if (FAILED(device->CreateVertexDeclaration(
		velementx,
		&DirectX9Holder::vertex_declaration_x_)))
	{
		ASSERT_ERROR("Xモデル用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementfbx[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	if (FAILED(device->CreateVertexDeclaration(
		velementfbx,
		&DirectX9Holder::vertex_declaration_fbx_)))
	{
		ASSERT_ERROR("fbx用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementins[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		D3DDECL_END(),
	};

	if(FAILED(device->CreateVertexDeclaration(
		velementins,
		&DirectX9Holder::vertex_declaration_instancing_)))
	{
		ASSERT_ERROR("インスタンシング用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementbench[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		D3DDECL_END(),
	};

	if (FAILED(device->CreateVertexDeclaration(
		velementbench,
		&DirectX9Holder::vertex_declaration_instancing_bench_)))
	{
		ASSERT_ERROR("インスタンシングベンチ用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementfield[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		D3DDECL_END(),
	};

	if (FAILED(device->CreateVertexDeclaration(
		velementfield,
		&DirectX9Holder::vertex_declaration_field_)))
	{
		ASSERT_ERROR("地形用頂点宣言生成に失敗");
	}

	D3DVERTEXELEMENT9 velementshadow[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END(),
	};

	if(FAILED(device->CreateVertexDeclaration(
		velementshadow,
		&DirectX9Holder::vertex_declaration_shadow_)))
	{
		ASSERT_ERROR("地形用頂点宣言生成に失敗");
	}

	DirectX9Holder::directx9_ = directx9;
	DirectX9Holder::device_ = device;
}


//-------------------------------------
// ~DirectX9()
//-------------------------------------
DirectX9::~DirectX9()
{
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_2d_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_3d_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_field_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_shadow_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_x_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_fbx_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_instancing_);
	SAFE_RELEASE(DirectX9Holder::vertex_declaration_instancing_bench_);
	SAFE_RELEASE(DirectX9Holder::device_);
	SAFE_RELEASE(DirectX9Holder::directx9_);
}


//-------------------------------------
// end of file
//-------------------------------------
