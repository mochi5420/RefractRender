#include "main.h"

#include <ctime>
#include <string>

#include "common.h"

//グローバル変数
MAIN* m_pMain = NULL;
float g_Angle_x = 0;
float g_Angle_y = 0;
float g_ModelAngle_x = 0;
float g_ModelAngle_y = 0;
float g_Scale = 1.0;
float g_Thickness_Scale = 5.0;
float g_Tex_Scale = 1.0;

float g_Red = 1.0 - 6.0 / 255.0;
float g_Green = 1.0 - 207.0 / 255.0;
float g_Blue = 1.0 - 66.0 / 255.0;
float var = 0.0025;

float random[SAMPLENUM][2] = {0};

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//アプリケーションのエントリー関数（最初に実行される）
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	m_pMain = new MAIN;
	if (m_pMain != NULL)
	{
		if (SUCCEEDED(m_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))  //creat the window
		{
			if (SUCCEEDED(m_pMain->InitD3D()))  //Initializes Direct3D
			{
				// メッセージループ
				MSG msg = { 0 };
				ZeroMemory(&msg, sizeof(msg));
				while (msg.message != WM_QUIT)
				{
					if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					else
					{
						m_pMain->App(); //メインループへ
					}
				}
			}
		}
		//アプリ終了
		m_pMain->Destroy();

		delete m_pMain;
	}
	return 0;
}

//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

MAIN::MAIN()
{
	ZeroMemory(this, sizeof(MAIN));				  //クラス内のすべての変数をゼロ初期化
	m_vLightPos = D3DXVECTOR3(0.0f, 0.0f, 2.5f);  //light position
}
MAIN::~MAIN()
{
}

//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight,  LPSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);									/* 構造体の大きさ */
	wc.style = CS_HREDRAW | CS_VREDRAW;						/* スタイル */
	wc.lpfnWndProc = WndProc;									/* メッセージ処理関数 */
	wc.hInstance = hInstance;									/* プログラムのハンドル */
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				/* アイコン */
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					/* カーソル */
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	/* ブラシ */
	wc.lpszMenuName = NULL;									/* メニュー */
	wc.lpszClassName = WindowName;								/* クラス名 */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);										/* ウインドウクラスwを登録 */

	//ウィンドウの作成
	m_hWnd = CreateWindow(
		WindowName,			/* ウインドウクラス名 */
		WindowName,			/* ウインドウのタイトル */
		WS_OVERLAPPEDWINDOW,	/* ウインドウスタイル */
		100, 100,				/* ウインドウ表示位置 */
		iWidth, iHeight,		/* ウインドウの大きさ */
		0,						/* 親ウインドウのハンドル */
		0,						/* メニューのハンドル */
		hInstance,				/* インスタンスのハンドル */
		0);					/* 作成時の引数保存用ポインタ */

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//ウィンドウプロシージャー
LRESULT MAIN::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:	//ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

//アプリケーション処理。アプリのメイン関数。
void MAIN::App()
{
	//key 

	//light 
	if (GetKeyState('X') & 0x80)
	{
		g_Angle_x += var;
	}
	if (GetKeyState('W') & 0x80)
	{
		g_Angle_x -= var;
	}
	if (GetKeyState('D') & 0x80)
	{
		g_Angle_y += var;
	}
	if (GetKeyState('A') & 0x80)
	{
		g_Angle_y -= var;
	}

	//model
	if (GetKeyState(VK_UP) & 0x80)
	{
		g_ModelAngle_x += var;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		g_ModelAngle_x -= var;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		if (GetKeyState('R') & 0x80)
		{
			g_Red += var;
		}
		else if (GetKeyState('G') & 0x80)
		{
			g_Green += var;
		}
		else if (GetKeyState('B') & 0x80)
		{
			g_Blue += var;
		}
		else g_ModelAngle_y += var;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		if (GetKeyState('R') & 0x80)
		{
			g_Red -= var;
		}
		else if (GetKeyState('G') & 0x80)
		{
			g_Green -= var;
		}
		else if (GetKeyState('B') & 0x80)
		{
			g_Blue -= var;
		}
		else g_ModelAngle_y -= var;
	}
	if (GetKeyState('O') & 0x80)
	{
		g_Scale -= var;
	}
	if (GetKeyState('I') & 0x80)
	{
		g_Scale += var;
	}
	g_Red = (std::max)(g_Red, (float)0.0);
	g_Red = (std::min)(g_Red, (float)1.0);
	g_Green = (std::max)(g_Green, (float)0.0);
	g_Green = (std::min)(g_Green, (float)1.0);
	g_Blue = (std::max)(g_Blue, (float)0.0);
	g_Blue = (std::min)(g_Blue, (float)1.0);
	g_Scale = (std::max)(g_Scale, (float)0.0);

	//thickness
	if (GetKeyState('L') & 0x80)
	{
		g_Thickness_Scale -= var;
	}
	if (GetKeyState('K') & 0x80)
	{
		g_Thickness_Scale += var;
	}
	g_Thickness_Scale = (std::max)(g_Thickness_Scale, (float)0.0);

	//artifact
	if (GetKeyState('N') & 0x80)
	{
		g_Tex_Scale -= var;
	}
	if (GetKeyState('M') & 0x80)
	{
		g_Tex_Scale += var;
	}
	g_Tex_Scale = (std::max)(g_Tex_Scale, (float)0.0);


	//render
	Render();
	FPS();
}

//Initializes Direct3D
HRESULT MAIN::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd)); //初期化
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;					//サイズ指定
	sd.BufferDesc.Height = WINDOW_HEIGHT;				//サイズ指定
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//32bit color
	sd.BufferDesc.RefreshRate.Numerator = 60;			//リフレッシュレート
	sd.BufferDesc.RefreshRate.Denominator = 1;			//バックバッファの数
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;									//Fullscreen表示の指定

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;	//受け皿
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;		//裏面も塗りつぶすぜ！！	　
	rdc.FillMode = D3D11_FILL_SOLID;	//ポリゴン内部も塗りつぶすぜ！！
	rdc.FrontCounterClockwise = TRUE;	//時計回りに頂点を結ぶぜ！！（三角形メッシュ）

	m_pDevice->CreateRasterizerState(&rdc, &m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);

	//
	//
	//深度テクスチャ （レンダーターゲット）
	
	//MakeView(m_pDevice, m_pDepthTexture, DXGI_FORMAT_R32_FLOAT, m_pDepthRenderTargetView, m_pDepthTextureView,
	//	descDepth, m_pDepthDepthStencil, m_pDepthDepthStencilView, WINDOW_WIDTH, WINDOW_HEIGHT);

	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = DEPTHTEX_WIDTH;
	tdesc.Height = DEPTHTEX_HEIGHT;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&tdesc, NULL, &m_pDepthTexture);

	//深度テクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pDepthTexture, &DescRT, &m_pDepthRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//深度テクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = tdesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDepthTexture, &SRVDesc, &m_pDepthTextureView)))
	{
		MessageBox(0, "SRV fail", "", MB_OK);
		return E_FAIL;
	}

	//深度テクスチャ用　深度ステンシルビューの作成
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthDepthStencil, NULL, &m_pDepthDepthStencilView);

	////
	////
	////Irradianceテクスチャ （レンダーターゲット）
	//D3D11_TEXTURE2D_DESC Irradiancetdesc;
	//ZeroMemory(&Irradiancetdesc, sizeof(D3D11_TEXTURE2D_DESC));
	//Irradiancetdesc.Width = DEPTHTEX_WIDTH;
	//Irradiancetdesc.Height = DEPTHTEX_HEIGHT;
	//Irradiancetdesc.MipLevels = 1;
	//Irradiancetdesc.ArraySize = 1;
	//Irradiancetdesc.MiscFlags = 0;
	//Irradiancetdesc.Format = DXGI_FORMAT_R32_FLOAT;
	//Irradiancetdesc.SampleDesc.Count = 1;
	//Irradiancetdesc.SampleDesc.Quality = 0;
	//Irradiancetdesc.Usage = D3D11_USAGE_DEFAULT;
	//Irradiancetdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//Irradiancetdesc.CPUAccessFlags = 0;

	//m_pDevice->CreateTexture2D(&Irradiancetdesc, NULL, &m_pIrradianceTexture);

	////Irradianceテクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	//D3D11_RENDER_TARGET_VIEW_DESC IrradianceDescRT;
	//IrradianceDescRT.Format = Irradiancetdesc.Format;
	//IrradianceDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//IrradianceDescRT.Texture2D.MipSlice = 0;

	//if (FAILED(m_pDevice->CreateRenderTargetView(m_pIrradianceTexture, &IrradianceDescRT, &m_pIrradianceRenderTargetView)))
	//{
	//	MessageBox(0, "RTV fail", "", MB_OK);
	//	return E_FAIL;
	//}

	////Irradianceテクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	//D3D11_SHADER_RESOURCE_VIEW_DESC IrradianceSRVDesc;
	//ZeroMemory(&IrradianceSRVDesc, sizeof(IrradianceSRVDesc));
	//IrradianceSRVDesc.Format = Irradiancetdesc.Format;
	//IrradianceSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//IrradianceSRVDesc.Texture2D.MipLevels = 1;

	//if (FAILED(m_pDevice->CreateShaderResourceView(m_pIrradianceTexture, &IrradianceSRVDesc, &m_pIrradianceTextureView)))
	//{
	//	MessageBox(0, "SRV fail", "", MB_OK);
	//	return E_FAIL;
	//}

	////Irradianceテクスチャ用　深度ステンシルビューの作成
	//descDepth.Width = DEPTHTEX_WIDTH;
	//descDepth.Height = DEPTHTEX_HEIGHT;
	//descDepth.MipLevels = 1;
	//descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	//descDepth.SampleDesc.Count = 1;
	//descDepth.SampleDesc.Quality = 0;
	//descDepth.Usage = D3D11_USAGE_DEFAULT;
	//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//descDepth.CPUAccessFlags = 0;
	//descDepth.MiscFlags = 0;

	//m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pIrradianceDepthStencil);
	//m_pDevice->CreateDepthStencilView(m_pIrradianceDepthStencil, NULL, &m_pIrradianceDepthStencilView);

	//
	//
	//Normalテクスチャ （レンダーターゲット）
	D3D11_TEXTURE2D_DESC Normaltdesc;
	ZeroMemory(&Normaltdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Normaltdesc.Width = DEPTHTEX_WIDTH;
	Normaltdesc.Height = DEPTHTEX_HEIGHT;
	Normaltdesc.MipLevels = 1;
	Normaltdesc.ArraySize = 1;
	Normaltdesc.MiscFlags = 0;
	Normaltdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //要検証
	Normaltdesc.SampleDesc.Count = 1;
	Normaltdesc.SampleDesc.Quality = 0;
	Normaltdesc.Usage = D3D11_USAGE_DEFAULT;
	Normaltdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Normaltdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Normaltdesc, NULL, &m_pNormalTexture);

	//Normalテクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC NormalDescRT;
	NormalDescRT.Format = Normaltdesc.Format;
	NormalDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	NormalDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pNormalTexture, &NormalDescRT, &m_pNormalRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Normalテクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC NormalSRVDesc;
	ZeroMemory(&NormalSRVDesc, sizeof(NormalSRVDesc));
	NormalSRVDesc.Format = Normaltdesc.Format;
	NormalSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	NormalSRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pNormalTexture, &NormalSRVDesc, &m_pNormalTextureView)))
	{
		MessageBox(0, "SRV fail", "", MB_OK);
		return E_FAIL;
	}

	//Normalテクスチャ用　深度ステンシルビューの作成
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pNormalDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pNormalDepthStencil, NULL, &m_pNormalDepthStencilView);
	
	//
	//
	//Fresnelテクスチャ （レンダーターゲット）
	D3D11_TEXTURE2D_DESC Fresneltdesc;
	ZeroMemory(&Fresneltdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Fresneltdesc.Width = DEPTHTEX_WIDTH;
	Fresneltdesc.Height = DEPTHTEX_HEIGHT;
	Fresneltdesc.MipLevels = 1;
	Fresneltdesc.ArraySize = 1;
	Fresneltdesc.MiscFlags = 0;
	Fresneltdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //要検証
	Fresneltdesc.SampleDesc.Count = 1;
	Fresneltdesc.SampleDesc.Quality = 0;
	Fresneltdesc.Usage = D3D11_USAGE_DEFAULT;
	Fresneltdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Fresneltdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Fresneltdesc, NULL, &m_pFresnelTexture);

	//Fresnelテクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC FresnelDescRT;
	FresnelDescRT.Format = Fresneltdesc.Format;
	FresnelDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	FresnelDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pFresnelTexture, &FresnelDescRT, &m_pFresnelRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Fresnelテクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC FresnelSRVDesc;
	ZeroMemory(&FresnelSRVDesc, sizeof(FresnelSRVDesc));
	FresnelSRVDesc.Format = Fresneltdesc.Format;
	FresnelSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	FresnelSRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pFresnelTexture, &FresnelSRVDesc, &m_pFresnelTextureView)))
	{
		MessageBox(0, "SRV fail", "", MB_OK);
		return E_FAIL;
	}

	//Fresnelテクスチャ用　深度ステンシルビューの作成
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pFresnelDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pFresnelDepthStencil, NULL, &m_pFresnelDepthStencilView);

	//
	//
	//Positionテクスチャ （レンダーターゲット）
	D3D11_TEXTURE2D_DESC Positiontdesc;
	ZeroMemory(&Positiontdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Positiontdesc.Width = DEPTHTEX_WIDTH;
	Positiontdesc.Height = DEPTHTEX_HEIGHT;
	Positiontdesc.MipLevels = 1;
	Positiontdesc.ArraySize = 1;
	Positiontdesc.MiscFlags = 0;
	Positiontdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //要検証
	Positiontdesc.SampleDesc.Count = 1;
	Positiontdesc.SampleDesc.Quality = 0;
	Positiontdesc.Usage = D3D11_USAGE_DEFAULT;
	Positiontdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Positiontdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Positiontdesc, NULL, &m_pPositionTexture);

	//Positionテクスチャ用　レンダーターゲットビュー作成（レンダリング時用）
	D3D11_RENDER_TARGET_VIEW_DESC PositionDescRT;
	PositionDescRT.Format = Positiontdesc.Format;
	PositionDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	PositionDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pPositionTexture, &PositionDescRT, &m_pPositionRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Positionテクスチャ用　シェーダーリソースビュー作成（テクスチャー確認時用）	
	D3D11_SHADER_RESOURCE_VIEW_DESC PositionSRVDesc;
	ZeroMemory(&PositionSRVDesc, sizeof(PositionSRVDesc));
	PositionSRVDesc.Format = Positiontdesc.Format;
	PositionSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	PositionSRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pPositionTexture, &PositionSRVDesc, &m_pPositionTextureView)))
	{
		MessageBox(0, "SRV fail", "", MB_OK);
		return E_FAIL;
	}

	//Positionテクスチャ用　深度ステンシルビューの作成
	descDepth.Width = DEPTHTEX_WIDTH;
	descDepth.Height = DEPTHTEX_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pPositionDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pPositionDepthStencil, NULL, &m_pPositionDepthStencilView);


	//
	//
	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	m_pMesh = new MY_MESH;
	if (FAILED(InitStaticMesh(OBJECT.c_str(), CSVFILE.c_str(), m_pMesh)))
	{
		return E_FAIL;
	}

	//ボクセル作成
	m_pVoxel = new MY_VOXEL;
	if (FAILED(CreatVoxel(CSVFILE.c_str(), m_pVoxel)))
	{
		return E_FAIL;
	}

	//テクスチャ作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, LUT.c_str(), NULL, NULL, &m_pThicknessTexture, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, LIGHTTEX.c_str(), NULL, NULL, &m_pLightTexture, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, LUT_IN.c_str(), NULL, NULL, &m_pNaKaMiTexture, NULL)))
	{
		return E_FAIL;
	}

	//generate randam sample point
	srand((unsigned long)time(0));
	for (int i = 0; i < SAMPLENUM; ++i) {
		float r = RADIUS *(float)rand() / (float)RAND_MAX;
		float t = 6.2831853f * (float)rand() / ((float)RAND_MAX);
		float ct = cos(t), st = sin(t);
		random[i][0] = r * ct;
		random[i][1] = r * st;
	}

	return S_OK;
}

//シェーダーを作成　頂点レイアウトを定義
HRESULT MAIN::InitShader()
{
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	//SSSシェーダー 初期化
	const std::string sssShaderFile = std::string(SOURCE_DIRECTORY) + "sss.hlsl";
	
	//ブロブからSSSバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS", "vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		//MessageBoxA(m_hWnd, (LPSTR)pErrors->GetBufferPointer(), NULL, MB_OK);
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), 
											 pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//SSSシェーダー 頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS])))
	{
		return FALSE;
	}

	//ブロブからSSSピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS", "ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//
	//
	//ブロブからSSS深度テクスチャー作成バーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS_Depth", "vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), 
												pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS_DEPTH])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//SSSシェーダー 頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements2 = sizeof(layout2) / sizeof(layout2[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout2, numElements2, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_DEPTH])))
	{
		return FALSE;
	}

	//ブロブからSSS深度テクスチャー作成ピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS_Depth", "ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS_DEPTH])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	////
	////
	////ブロブからSSS照度テクスチャー作成バーテックスシェーダー作成
	//if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS_Irradiance", "vs_5_0",
	//	D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
	//	return E_FAIL;
	//}
	//if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
	//										 pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS_IRRADIANCE])))
	//{
	//	SAFE_RELEASE(pCompiledShader);
	//	MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}

	////SSSシェーダー 頂点インプットレイアウトを定義	
	//D3D11_INPUT_ELEMENT_DESC layout3[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//UINT numElements3 = sizeof(layout3) / sizeof(layout3[0]);

	////頂点インプットレイアウトを作成
	//if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(), 
	//										pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_IRRADIANCE])))
	//{
	//	return FALSE;
	//}

	////ブロブからSSS照度テクスチャー作成ピクセルシェーダー作成
	//if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS_Irradiance", "ps_5_0",
	//	D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	//{
	//	MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
	//	return E_FAIL;
	//}
	//if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), 
	//										pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS_IRRADIANCE])))
	//{
	//	SAFE_RELEASE(pCompiledShader);
	//	MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
	//	return E_FAIL;
	//}

	//SAFE_RELEASE(pCompiledShader);
	//SAFE_RELEASE(pErrors);

	//
	//
	//ブロブからSSS法線テクスチャー作成バーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS_Normal", "vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), 
											 pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS_NORMAL])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//SSSシェーダー 頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout3[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements3 = sizeof(layout3) / sizeof(layout3[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_NORMAL])))
	{
		return FALSE;
	}

	//ブロブからSSS法線テクスチャー作成ピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS_Normal", "ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS_NORMAL])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//ブロブからSSSFresnelテクスチャー作成バーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS_Fresnel", "vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS_FRESNEL])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_FRESNEL])))
	{
		return FALSE;
	}

	//ブロブからSSSFresnelテクスチャー作成ピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS_Fresnel", "ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS_FRESNEL])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//ブロブからSSSPositionテクスチャー作成バーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "VS_Position", "vs_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[SSS_POSITION])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_POSITION])))
	{
		return FALSE;
	}

	//ブロブからSSS法線テクスチャー作成ピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(sssShaderFile.c_str(), NULL, NULL, "PS_Position", "ps_5_0",
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[SSS_POSITION])))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//SSSシェーダー　深度テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSamLinear);


	D3D11_SAMPLER_DESC SamDesc2;
	ZeroMemory(&SamDesc2, sizeof(D3D11_SAMPLER_DESC));
	SamDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pDevice->CreateSamplerState(&SamDesc2, &m_pSamPoint);

	//
	//
	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_SSS);	//構造体のサイズを渡している
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer[SSS])))
	{
		return E_FAIL;
	}

	return S_OK;
}

void MAIN::Render()
{
	//ビュー行列
	D3DXVECTOR3 vEye(0.0f, 0.0f, -3.0f);	//カメラ位置
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);	//注視方向
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);	//上方位置
	D3DXMatrixLookAtLH(&m_mView, &vEye, &vAt, &vUp);

	//プロジェクション行列（射影変換）
	D3DXMatrixPerspectiveFovLH(&m_mProj,
		(float)D3DX_PI / 4.0,							//視野角
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,		//アスペクト比
		0.1f,											//near clip
		100.0f);										//far clip

	float ClearColor[4] = { 0.178, 0.178, 0.178, 1 };// クリア色作成　RGBAの順
	float ClearColor2[4] = { 0.0, 0.0, 0.0, 1 };

	//平行投影用
	D3DXMATRIX mOrtho;
	D3DXMatrixOrthoLH(&mOrtho, 2.5f, 2.5f, 0.1f, 100.0f);

	//
	//
	//1パス目
	//深度・法線・Fresnel・Positionテクスチャ作成

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp); //ビューポートのセット

	//
	//深度テクスチャ用のレンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pDepthRenderTargetView, m_pDepthDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pDepthRenderTargetView, ClearColor2);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	//モデルワールド行列 
	D3DXMATRIX mR_x, mR_y, mS;
	D3DXMatrixRotationX(&mR_x, g_ModelAngle_x);
	D3DXMatrixRotationY(&mR_y, g_ModelAngle_y);
	D3DXMatrixScaling(&mS, g_Scale, g_Scale, g_Scale);
	m_pMesh->mWorld = mS * mR_x * mR_y;
	m_pVoxel->mWorld = mS * mR_x * mR_y;

	// ビュートランスフォーム ここではライトからの視界
	//ライトを回転させる
	D3DXMATRIX mLight;
	D3DXVECTOR3 vLight = m_vLightPos;
	D3DXMatrixRotationX(&mR_x, g_Angle_x);
	D3DXMatrixRotationY(&mR_y, g_Angle_y);
	D3DXMATRIX Rot_light = mR_x * mR_y;
	D3DXVec3TransformCoord(&vLight, &vLight, &Rot_light);

	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・ライトビュー・プロジェクション行列をシェーダーに渡す
		D3DXMATRIX s;//重要！　丸め誤差によるテクセルの端でのアーティファクト防止
		D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
		//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
		//cb.mWLP = s * m_pVoxel->mWorld * mLight * m_mProj;

		cb.mWLP = s * m_pVoxel->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//転置する（シェーダでの行列は列優先ですよ）

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//頂点インプットレイアウトをセット(バーテックスシェーダが引数に取れる要素)　IA＝Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_DEPTH]);
	//プリミティブ・トポロジーをセット（三角形メッシュならリストかストリップか）
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	UINT stride = sizeof(VOXEL);
	//UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVoxel->VertexBuffer, &stride, &offset);
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pVoxel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_DEPTH], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_DEPTH], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(m_pVoxel->voxelNum * 36, 0, 0);
	//m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	////
	////照度テクスチャ用のレンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	//m_pDeviceContext->OMSetRenderTargets(1, &m_pIrradianceRenderTargetView, m_pIrradianceDepthStencilView);

	//m_pDeviceContext->ClearRenderTargetView(m_pIrradianceRenderTargetView, ClearColor2);//画面クリア
	//m_pDeviceContext->ClearDepthStencilView(m_pIrradianceDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	////シェーダーのコンスタントバッファーに各種データを渡す
	//if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	//{
	//	//ワールド・ライトビュー・プロジェクション行列をシェーダーに渡す
	//	D3DXMATRIX s;//重要！　丸め誤差によるテクセルの端でのアーティファクト防止
	//	D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
	//	//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
	//	cb.mWLP = s * m_pMesh->mWorld * mLight * mOrtho;
	//	D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//転置する（シェーダでの行列は列優先ですよ）
	//	
	//	//ワールド行列をシェーダーに渡す
	//	cb.mW = m_pMesh->mWorld;
	//	D3DXMatrixTranspose(&cb.mW, &cb.mW);
	//	
	//	////ワールド・ライトビュー行列の逆行列の転置行列
	//	//cb.mWLP_t = s * m_pMesh->mWorld * mLight;
	//	//D3DXMatrixInverse(&cb.mWLP_t, NULL, &cb.mWLP_t);
	//	//D3DXMatrixTranspose(&cb.mWLP_t, &cb.mWLP_t);

	//	//ライトの位置をシェーダーに渡す
	//	cb.vLightPos = vLight;//vLightは上で、回転させたあとのライト現在位置

	//	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	//	m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	//}

	////このコンスタントバッファーを使うシェーダーの登録
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	////頂点インプットレイアウトをセット(バーテックスシェーダが引数に取れる要素)　IA＝Input Assembler
	//m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_IRRADIANCE]);
	////プリミティブ・トポロジーをセット（三角形メッシュならリストかストリップか）
	//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////バーテックスバッファーをセット
	//stride = sizeof(VERTEX);
	//offset = 0;
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	////インデックスバッファーをセット
	//stride = sizeof(int);
	//offset = 0;
	//m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////使用するシェーダーの登録	
	//m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_IRRADIANCE], NULL, 0);
	//m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_IRRADIANCE], NULL, 0);
	////プリミティブをレンダリング
	//m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//法線テクスチャ用のレンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pNormalRenderTargetView, m_pNormalDepthStencilView);

	float yyyy[4] = { 0, 0, 0, 1 };
	m_pDeviceContext->ClearRenderTargetView(m_pNormalRenderTargetView, yyyy);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pNormalDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	//シェーダーのコンスタントバッファーに各種データを渡す
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・ライトビュー・プロジェクション行列をシェーダーに渡す
		D3DXMATRIX s;//重要！　丸め誤差によるテクセルの端でのアーティファクト防止
		D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
		//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
		cb.mWLP = s * m_pMesh->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//転置する（シェーダでの行列は列優先ですよ）

		//ワールド行列をシェーダーに渡す
		cb.mW = m_pMesh->mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		////ワールド・ライトビュー行列の逆行列の転置行列
		//cb.mWLP_t = s * m_pMesh->mWorld * mLight;
		//D3DXMatrixInverse(&cb.mWLP_t, NULL, &cb.mWLP_t);
		//D3DXMatrixTranspose(&cb.mWLP_t, &cb.mWLP_t);

		//ライトの位置をシェーダーに渡す
		cb.vLightPos = vLight;//vLightは上で、回転させたあとのライト現在位置

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//頂点インプットレイアウトをセット(バーテックスシェーダが引数に取れる要素)　IA＝Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_NORMAL]);
	//プリミティブ・トポロジーをセット（三角形メッシュならリストかストリップか）
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_NORMAL], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_NORMAL], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//Fresnelテクスチャ用のレンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pFresnelRenderTargetView, m_pFresnelDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pFresnelRenderTargetView, yyyy);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pFresnelDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	//頂点インプットレイアウトをセット(バーテックスシェーダが引数に取れる要素)　IA＝Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_FRESNEL]);
	//プリミティブ・トポロジーをセット（三角形メッシュならリストかストリップか）
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_FRESNEL], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_FRESNEL], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);
	

	//
	//Positionテクスチャ用のレンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pPositionRenderTargetView, m_pPositionDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pPositionRenderTargetView, yyyy);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pPositionDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

																									//頂点インプットレイアウトをセット(バーテックスシェーダが引数に取れる要素)　IA＝Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_POSITION]);
	//プリミティブ・トポロジーをセット（三角形メッシュならリストかストリップか）
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バーテックスバッファーをセット
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_POSITION], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_POSITION], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//レンダーターゲットを戻す
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//
	//
	//2パス目
	//SSSレンダー

	//ビューポートの設定
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	//シェーダーのコンスタントバッファーに各種データを渡す
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・ビュー・プロジェクション行列をシェーダーに渡す
		cb.mWVP = m_pMesh->mWorld * m_mView * m_mProj;
		//cb.mWVP = m_pMesh->mWorld * m_mView * mOrtho;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		
		//ワールド・”ライトビュー”・プロジェクション行列をシェーダーに渡す
		//cb.mWLP = m_pMesh->mWorld * mLight * m_mProj;
		cb.mWLP = m_pMesh->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);

		//射影空間からテクスチャ座標空間へ変換する行列を渡す
		D3DXMATRIX m;
		ZeroMemory(&m, sizeof(D3DXMATRIX));
		m._11 = 0.5f;
		m._22 = -0.5f;
		m._33 = 1.0f;
		m._41 = 0.5f;
		m._42 = 0.5f;
		m._44 = 1.0f;
		
		//cb.mWLPT = m_pMesh->mWorld * mLight * m_mProj * m;
		cb.mWLPT = m_pMesh->mWorld * mLight * mOrtho * m;
		D3DXMatrixTranspose(&cb.mWLPT, &cb.mWLPT);
		
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3DXVECTOR4 mat(0, 0, 0, 0);
		//mat = D3DXVECTOR4(0.0, 0.0, 0.0, 1);
		//cb.Ambient = mat;
		//mat = D3DXVECTOR4(0.3, 0.3, 1.0, 1);
		mat = D3DXVECTOR4(1.0, 1.0, 1.0, 1);
		cb.Diffuse = mat;
		mat = D3DXVECTOR4(0.3, 0.3, 0.3, 1);
		cb.Specular = mat;

		//散乱係数
		cb.g_Scoeff = D3DXVECTOR3(g_Red, g_Green, g_Blue);

		//視線ベクトルをエフェクト（シェーダー）に通知・適用
		cb.vEye = vEye;

		//どこまで透過するかの距離
		cb.g_Transparent = g_Thickness_Scale;

		//ボクセル数
		cb.g_HalfThicknessPixel = 1.0f / (float)m_pVoxel->voxelNum * 0.5f;

		cb.g_DepthPixel = 1.0f / (float)DEPTHTEX_WIDTH;
		cb.g_HalfDepthPixel = 1.0f / (float)DEPTHTEX_WIDTH * 0.5f;

		for (int i = 0; i < SAMPLENUM; ++i) {
			cb.g_random[i][0] = random[i][0];
			cb.g_random[i][1] = random[i][1];
		}
			
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//１パス目で作った深度テクスチャーをシェーダーに渡す
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(0, 1, &m_pDepthTextureView);
	//厚みテクスチャをVSに渡す
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(1, 1, &m_pThicknessTexture);
	//中身テクスチャ（笑）を渡す
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(2, 1, &m_pNaKaMiTexture);
	////照度テクスチャを渡す
	//m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	//m_pDeviceContext->VSSetShaderResources(3, 1, &m_pIrradianceTextureView);
	//法線テクスチャを渡す
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(3, 1, &m_pNormalTextureView);
	//Fresnelテクスチャを渡す
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(4, 1, &m_pFresnelTextureView);
	//Positionテクスチャを渡す
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(5, 1, &m_pPositionTextureView);
	//バーテックスバッファーをセット
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS]);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS], NULL, 0);
	//プリミティブをレンダリング	
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//深度テクスチャ確認
	//DrawTexture(m_pDepthTextureView);
	//DrawTexture(m_pIrradianceTextureView);
	//DrawTexture(m_pNormalTextureView);
	//DrawTexture(m_pFresnelTextureView);
	DrawTexture(m_pPositionTextureView);

	//
	DrawLight(m_pLightTexture);

	m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに）	
}
//
//
//FPSの計算・表示
void MAIN::FPS()
{
	//FPS計算表示
	static DWORD time = 0;
	static int frame = 0;
	static int av_frame = 0;
	static int fps[3600] = { 0 };//計測は1時間以内にすること
	static int avg = 0;
	static int first2 = 0;
	frame++;
	char str[60];
	sprintf(str, "fps=%d avg_fps=%d  R=%f G=%f B=%f", frame, avg, g_Red, g_Green, g_Blue); //ついでにRGBも表示しとく
	if (timeGetTime() - time>1000)
	{
		first2++;
		fps[av_frame] = frame;
		if (first2>2)
		{
			av_frame++;
			avg = 0;
			for (int i = 0; i<av_frame; i++)
			{
				avg += fps[i];
			}
			avg = avg / av_frame;
		}
		time = timeGetTime();
		frame = 0;
		SetWindowTextA(m_hWnd, str);
	}
}
//
//
//画面に深度テクスチャを描画する
HRESULT MAIN::DrawTexture(ID3D11ShaderResourceView* TextureSRV)
{
	const std::string dtexShaderFile = std::string(SOURCE_DIRECTORY) + "drawtex.hlsl";
	static bool boOnce = false;
	if (!boOnce)
	{
		boOnce = true;
		//Texシェーダー 初期化
		//ブロブからDRAW_TEXバーテックスシェーダー作成
		ID3DBlob *pCompiledShader = NULL;
		ID3DBlob *pErrors = NULL;
		if (FAILED(D3DX11CompileFromFile(dtexShaderFile.c_str(), NULL, NULL, "VS", "vs_5_0",
			D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(m_hWnd, (LPSTR)pErrors->GetBufferPointer(), NULL, MB_OK);
			return E_FAIL;
		}

		if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[DRAW_TEX])))
		{
			SAFE_RELEASE(pCompiledShader);
			return E_FAIL;
		}
		//Texシェーダー用　頂点インプットレイアウトを作成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		int numElements = sizeof(layout) / sizeof(layout[0]);
		if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout[DRAW_TEX])))
		{
			return FALSE;
		}
		//ブロブからDRAW_TEXピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(dtexShaderFile.c_str(), NULL, NULL, "PS", "ps_5_0",
			D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(m_hWnd, (LPSTR)pErrors->GetBufferPointer(), NULL, MB_OK);
			return E_FAIL;
		}
		if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[DRAW_TEX])))
		{
			SAFE_RELEASE(pCompiledShader);
			return E_FAIL;
		}

		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		//Texシェーダー用　ポリゴンバーテックスバッファー作成
		TEX_VERTEX vertices[] =
		{
			D3DXVECTOR3(-0.2, -0.2, 0), D3DXVECTOR2(0, 1),//頂点1,
			D3DXVECTOR3(-0.2, 0.2, 0), D3DXVECTOR2(0, 0),//頂点2
			D3DXVECTOR3(0.2, -0.2, 0), D3DXVECTOR2(1, 1), //頂点3
			D3DXVECTOR3(0.2, 0.2, 0), D3DXVECTOR2(1, 0), //頂点4

			//全画面で見たい時
			/*D3DXVECTOR3(-1.0, -1.0, 0), D3DXVECTOR2(0, 1),
			D3DXVECTOR3(-1.0, 1.0, 0), D3DXVECTOR2(0, 0),
			D3DXVECTOR3(1.0, -1.0, 0), D3DXVECTOR2(1, 1),
			D3DXVECTOR3(1.0, 1.0, 0), D3DXVECTOR2(1, 0), */

		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TEX_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;
		if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pDrawTexVertexBuffer)))
		{
			return E_FAIL;
		}
		//コンスタントバッファー作成
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CONSTANT_BUFFER_DRAWTEX);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer[DRAW_TEX])))
		{
			return E_FAIL;
		}
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(TEX_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pDrawTexVertexBuffer, &stride, &offset);
	//ワールド行列
	D3DXMATRIX mWorld;
	D3DXMATRIX mPosition;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mPosition, -0.8, -0.8, 2.5);
	//D3DXMatrixTranslation(&mPosition, 0, 0, 2.4); //全画面で見たい時
	mWorld = mPosition;

	//シェーダーのコンスタントバッファーに各種データを渡す
	CONSTANT_BUFFER_DRAWTEX cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[DRAW_TEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・プロジェクション行列をシェーダーに渡す（固定位置表示なのでビュー変換はかけない）
		cb.mWP = mWorld*m_mProj;
		D3DXMatrixTranspose(&cb.mWP, &cb.mWP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[DRAW_TEX], 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[DRAW_TEX]);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &TextureSRV);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[DRAW_TEX], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[DRAW_TEX], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->Draw(4, 0);

	return S_OK;
}
//
//
//光源位置を表示する
HRESULT MAIN::DrawLight(ID3D11ShaderResourceView* LightTextureSRV)
{
	const std::string dtexShaderFile = std::string(SOURCE_DIRECTORY) + "drawtex.hlsl";
	static bool boOnce = false;
	if (!boOnce)
	{
		boOnce = true;
		//Texシェーダー 初期化
		//ブロブからDRAW_TEXバーテックスシェーダー作成
		ID3DBlob *pCompiledShader = NULL;
		ID3DBlob *pErrors = NULL;
		if (FAILED(D3DX11CompileFromFile(dtexShaderFile.c_str(), NULL, NULL, "VS", "vs_5_0",
			D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(m_hWnd, (LPSTR)pErrors->GetBufferPointer(), NULL, MB_OK);
			return E_FAIL;
		}
		if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[DRAW_TEX])))
		{
			SAFE_RELEASE(pCompiledShader);
			return E_FAIL;
		}
		//Texシェーダー用　頂点インプットレイアウトを作成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		int numElements = sizeof(layout) / sizeof(layout[0]);
		if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout[DRAW_TEX])))
		{
			return FALSE;
		}
		//ブロブからDRAW_TEXピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(dtexShaderFile.c_str(), NULL, NULL, "PS", "ps_5_0",
			D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(m_hWnd, (LPSTR)pErrors->GetBufferPointer(), NULL, MB_OK);
			return E_FAIL;
		}
		if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader[DRAW_TEX])))
		{
			SAFE_RELEASE(pCompiledShader);
			return E_FAIL;
		}

		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		//Texシェーダー用　ポリゴンバーテックスバッファー作成
		TEX_VERTEX vertices[] =
		{
			D3DXVECTOR3(-0.3, -0.3, 2) + m_vLightPos, D3DXVECTOR2(0, 1),//頂点1,
			D3DXVECTOR3(-0.3, 0.3, 2) + m_vLightPos, D3DXVECTOR2(0, 0),//頂点2
			D3DXVECTOR3(0.3, -0.3, 2) + m_vLightPos, D3DXVECTOR2(1, 1), //頂点3
			D3DXVECTOR3(0.3, 0.3, 2) + m_vLightPos, D3DXVECTOR2(1, 0), //頂点4
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TEX_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;
		if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pLightTexVertexBuffer)))
		{
			return E_FAIL;
		}
		//コンスタントバッファー作成
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CONSTANT_BUFFER_DRAWTEX);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer[DRAW_TEX])))
		{
			return E_FAIL;
		}
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(TEX_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pLightTexVertexBuffer, &stride, &offset);

	//モデルワールド行列 
	D3DXMATRIX mWorld;
	D3DXMATRIX mR_x, mR_y, mS;
	//D3DXMatrixRotationX(&mR_x, g_ModelAngle_x);
	D3DXMatrixRotationX(&mR_x, g_Angle_x);
	//D3DXMatrixRotationY(&mR_y, g_ModelAngle_y);
	D3DXMatrixRotationY(&mR_y, g_Angle_y);
	D3DXMatrixScaling(&mS, g_Scale, g_Scale, g_Scale);
	mWorld = mS * mR_x * mR_y;

	//シェーダーのコンスタントバッファーに各種データを渡す
	CONSTANT_BUFFER_DRAWTEX cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[DRAW_TEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド・プロジェクション行列をシェーダーに渡す(直すの面倒だからmWPのまま)
		cb.mWP = mWorld*m_mView*m_mProj;
		D3DXMatrixTranspose(&cb.mWP, &cb.mWP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[DRAW_TEX], 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[DRAW_TEX]);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &LightTextureSRV);
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader[DRAW_TEX], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[DRAW_TEX], NULL, 0);
	//プリミティブをレンダリング
	m_pDeviceContext->Draw(4, 0);

	return S_OK;
}
//
//
//
void MAIN::Destroy()
{
	SAFE_RELEASE(m_pDrawTexVertexBuffer);
	SAFE_RELEASE(m_pLightTexVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer[SSS]);
	
	SAFE_RELEASE(m_pPixelShader[SSS_DEPTH]);
	SAFE_RELEASE(m_pPixelShader[SSS]);
	SAFE_RELEASE(m_pPixelShader[SSS_IRRADIANCE]);
	SAFE_RELEASE(m_pPixelShader[SSS_NORMAL]);
	SAFE_RELEASE(m_pPixelShader[SSS_FRESNEL]);
	SAFE_RELEASE(m_pPixelShader[SSS_POSITION]);
	
	SAFE_RELEASE(m_pVertexShader[SSS_DEPTH]);
	SAFE_RELEASE(m_pVertexShader[SSS]);
	SAFE_RELEASE(m_pVertexShader[SSS_IRRADIANCE]);
	SAFE_RELEASE(m_pVertexShader[SSS_NORMAL]);
	SAFE_RELEASE(m_pVertexShader[SSS_FRESNEL]);
	SAFE_RELEASE(m_pVertexShader[SSS_POSITION]);
	
	SAFE_RELEASE(m_pVertexLayout[SSS]);
	SAFE_RELEASE(m_pVertexLayout[SSS_DEPTH]);
	SAFE_RELEASE(m_pVertexLayout[SSS_IRRADIANCE]);
	SAFE_RELEASE(m_pVertexLayout[SSS_NORMAL]);
	SAFE_RELEASE(m_pVertexLayout[SSS_FRESNEL]);
	SAFE_RELEASE(m_pVertexLayout[SSS_POSITION]);

	SAFE_RELEASE(m_pDepthDepthStencilView);
	SAFE_RELEASE(m_pDepthRenderTargetView);
	
	SAFE_RELEASE(m_pIrradianceDepthStencilView);
	SAFE_RELEASE(m_pIrradianceRenderTargetView);
	
	SAFE_RELEASE(m_pNormalDepthStencilView);
	SAFE_RELEASE(m_pNormalRenderTargetView);
	
	SAFE_RELEASE(m_pFresnelDepthStencilView);
	SAFE_RELEASE(m_pFresnelRenderTargetView);

	SAFE_RELEASE(m_pPositionDepthStencilView);
	SAFE_RELEASE(m_pPositionRenderTargetView);
	
	SAFE_RELEASE(m_pThicknessTexture);
	SAFE_RELEASE(m_pNaKaMiTexture);
	SAFE_RELEASE(m_pLightTexture);
	
	SAFE_RELEASE(m_pDepthDepthStencil);
	SAFE_RELEASE(m_pDepthTextureView);
	
	SAFE_RELEASE(m_pIrradianceDepthStencil);
	SAFE_RELEASE(m_pIrradianceTextureView);
	
	SAFE_RELEASE(m_pNormalDepthStencil);
	SAFE_RELEASE(m_pNormalTextureView);
	
	SAFE_RELEASE(m_pFresnelDepthStencil);
	SAFE_RELEASE(m_pFresnelTextureView);
	
	SAFE_RELEASE(m_pPositionDepthStencil);
	SAFE_RELEASE(m_pPositionTextureView);

	SAFE_RELEASE(m_pSamLinear);
	SAFE_RELEASE(m_pSamPoint);
	
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pSwapChain);
	
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDevice);
}