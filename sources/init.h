#include "debug.h"
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <algorithm>

HRESULT MakeView(
	ID3D11Device* Device,
	ID3D11Texture2D* DepthTexture,
	enum DXGI_FORMAT format,
	ID3D11RenderTargetView* DepthRenderTargetView,
	ID3D11ShaderResourceView* DepthTextureView,
	D3D11_TEXTURE2D_DESC descDepth,
	ID3D11Texture2D* DepthDepthStencil,
	ID3D11DepthStencilView* DepthDepthStencilView,
	const int WIDTH,
	const int HEIGHT
	){
	
	//レンダーターゲット
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = WIDTH;
	tdesc.Height = HEIGHT;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = format;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;

	Device->CreateTexture2D(&tdesc, NULL, &DepthTexture);

	//深度テクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	if (FAILED(Device->CreateRenderTargetView(DepthTexture, &DescRT, &DepthRenderTargetView)))
	{
		DebugWindow("Failed to Create RenderTargetView!!");
		return E_FAIL;
	}

	//シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(Device->CreateShaderResourceView(DepthTexture, &SRVDesc, &DepthTextureView)))
	{
		DebugWindow("Failed to Create ShaderResourceView!!");
		return E_FAIL;
	}

	//深度テクスチャ用　深度ステンシルビューの作成
	descDepth.Width = WIDTH;
	descDepth.Height = HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	Device->CreateTexture2D(&descDepth, NULL, &DepthDepthStencil);
	Device->CreateDepthStencilView(DepthDepthStencil, NULL, &DepthDepthStencilView);

	return S_OK;

}