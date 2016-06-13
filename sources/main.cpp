#include "main.h"

#include <ctime>
#include <string>

#include "common.h"

//�O���[�o���ϐ�
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

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�A�v���P�[�V�����̃G���g���[�֐��i�ŏ��Ɏ��s�����j
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
				// ���b�Z�[�W���[�v
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
						m_pMain->App(); //���C�����[�v��
					}
				}
			}
		}
		//�A�v���I��
		m_pMain->Destroy();

		delete m_pMain;
	}
	return 0;
}

//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

MAIN::MAIN()
{
	ZeroMemory(this, sizeof(MAIN));				  //�N���X���̂��ׂĂ̕ϐ����[��������
	m_vLightPos = D3DXVECTOR3(0.0f, 0.0f, 2.5f);  //light position
}
MAIN::~MAIN()
{
}

//�E�B���h�E�쐬
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight,  LPSTR WindowName)
{
	// �E�B���h�E�̒�`
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);									/* �\���̂̑傫�� */
	wc.style = CS_HREDRAW | CS_VREDRAW;						/* �X�^�C�� */
	wc.lpfnWndProc = WndProc;									/* ���b�Z�[�W�����֐� */
	wc.hInstance = hInstance;									/* �v���O�����̃n���h�� */
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				/* �A�C�R�� */
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);					/* �J�[�\�� */
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	/* �u���V */
	wc.lpszMenuName = NULL;									/* ���j���[ */
	wc.lpszClassName = WindowName;								/* �N���X�� */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);										/* �E�C���h�E�N���Xw��o�^ */

	//�E�B���h�E�̍쐬
	m_hWnd = CreateWindow(
		WindowName,			/* �E�C���h�E�N���X�� */
		WindowName,			/* �E�C���h�E�̃^�C�g�� */
		WS_OVERLAPPEDWINDOW,	/* �E�C���h�E�X�^�C�� */
		100, 100,				/* �E�C���h�E�\���ʒu */
		iWidth, iHeight,		/* �E�C���h�E�̑傫�� */
		0,						/* �e�E�C���h�E�̃n���h�� */
		0,						/* ���j���[�̃n���h�� */
		hInstance,				/* �C���X�^���X�̃n���h�� */
		0);					/* �쐬���̈����ۑ��p�|�C���^ */

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	//�E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//�E�B���h�E�v���V�[�W���[
LRESULT MAIN::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:	//ESC�L�[�ŏC��
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

//�A�v���P�[�V���������B�A�v���̃��C���֐��B
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
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd)); //������
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;					//�T�C�Y�w��
	sd.BufferDesc.Height = WINDOW_HEIGHT;				//�T�C�Y�w��
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//32bit color
	sd.BufferDesc.RefreshRate.Numerator = 60;			//���t���b�V�����[�g
	sd.BufferDesc.RefreshRate.Denominator = 1;			//�o�b�N�o�b�t�@�̐�
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;									//Fullscreen�\���̎w��

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;	//�󂯎M
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	//�[�x�X�e���V���r���[�̍쐬
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

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;		//���ʂ��h��Ԃ����I�I	�@
	rdc.FillMode = D3D11_FILL_SOLID;	//�|���S���������h��Ԃ����I�I
	rdc.FrontCounterClockwise = TRUE;	//���v���ɒ��_�����Ԃ��I�I�i�O�p�`���b�V���j

	m_pDevice->CreateRasterizerState(&rdc, &m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);

	//
	//
	//�[�x�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
	
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

	//�[�x�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = tdesc.Format;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pDepthTexture, &DescRT, &m_pDepthRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//�[�x�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
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

	//�[�x�e�N�X�`���p�@�[�x�X�e���V���r���[�̍쐬
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
	////Irradiance�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
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

	////Irradiance�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	//D3D11_RENDER_TARGET_VIEW_DESC IrradianceDescRT;
	//IrradianceDescRT.Format = Irradiancetdesc.Format;
	//IrradianceDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//IrradianceDescRT.Texture2D.MipSlice = 0;

	//if (FAILED(m_pDevice->CreateRenderTargetView(m_pIrradianceTexture, &IrradianceDescRT, &m_pIrradianceRenderTargetView)))
	//{
	//	MessageBox(0, "RTV fail", "", MB_OK);
	//	return E_FAIL;
	//}

	////Irradiance�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
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

	////Irradiance�e�N�X�`���p�@�[�x�X�e���V���r���[�̍쐬
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
	//Normal�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
	D3D11_TEXTURE2D_DESC Normaltdesc;
	ZeroMemory(&Normaltdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Normaltdesc.Width = DEPTHTEX_WIDTH;
	Normaltdesc.Height = DEPTHTEX_HEIGHT;
	Normaltdesc.MipLevels = 1;
	Normaltdesc.ArraySize = 1;
	Normaltdesc.MiscFlags = 0;
	Normaltdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //�v����
	Normaltdesc.SampleDesc.Count = 1;
	Normaltdesc.SampleDesc.Quality = 0;
	Normaltdesc.Usage = D3D11_USAGE_DEFAULT;
	Normaltdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Normaltdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Normaltdesc, NULL, &m_pNormalTexture);

	//Normal�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	D3D11_RENDER_TARGET_VIEW_DESC NormalDescRT;
	NormalDescRT.Format = Normaltdesc.Format;
	NormalDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	NormalDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pNormalTexture, &NormalDescRT, &m_pNormalRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Normal�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
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

	//Normal�e�N�X�`���p�@�[�x�X�e���V���r���[�̍쐬
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
	//Fresnel�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
	D3D11_TEXTURE2D_DESC Fresneltdesc;
	ZeroMemory(&Fresneltdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Fresneltdesc.Width = DEPTHTEX_WIDTH;
	Fresneltdesc.Height = DEPTHTEX_HEIGHT;
	Fresneltdesc.MipLevels = 1;
	Fresneltdesc.ArraySize = 1;
	Fresneltdesc.MiscFlags = 0;
	Fresneltdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //�v����
	Fresneltdesc.SampleDesc.Count = 1;
	Fresneltdesc.SampleDesc.Quality = 0;
	Fresneltdesc.Usage = D3D11_USAGE_DEFAULT;
	Fresneltdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Fresneltdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Fresneltdesc, NULL, &m_pFresnelTexture);

	//Fresnel�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	D3D11_RENDER_TARGET_VIEW_DESC FresnelDescRT;
	FresnelDescRT.Format = Fresneltdesc.Format;
	FresnelDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	FresnelDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pFresnelTexture, &FresnelDescRT, &m_pFresnelRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Fresnel�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
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

	//Fresnel�e�N�X�`���p�@�[�x�X�e���V���r���[�̍쐬
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
	//Position�e�N�X�`�� �i�����_�[�^�[�Q�b�g�j
	D3D11_TEXTURE2D_DESC Positiontdesc;
	ZeroMemory(&Positiontdesc, sizeof(D3D11_TEXTURE2D_DESC));
	Positiontdesc.Width = DEPTHTEX_WIDTH;
	Positiontdesc.Height = DEPTHTEX_HEIGHT;
	Positiontdesc.MipLevels = 1;
	Positiontdesc.ArraySize = 1;
	Positiontdesc.MiscFlags = 0;
	Positiontdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //�v����
	Positiontdesc.SampleDesc.Count = 1;
	Positiontdesc.SampleDesc.Quality = 0;
	Positiontdesc.Usage = D3D11_USAGE_DEFAULT;
	Positiontdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Positiontdesc.CPUAccessFlags = 0;

	m_pDevice->CreateTexture2D(&Positiontdesc, NULL, &m_pPositionTexture);

	//Position�e�N�X�`���p�@�����_�[�^�[�Q�b�g�r���[�쐬�i�����_�����O���p�j
	D3D11_RENDER_TARGET_VIEW_DESC PositionDescRT;
	PositionDescRT.Format = Positiontdesc.Format;
	PositionDescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	PositionDescRT.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pPositionTexture, &PositionDescRT, &m_pPositionRenderTargetView)))
	{
		MessageBox(0, "RTV fail", "", MB_OK);
		return E_FAIL;
	}

	//Position�e�N�X�`���p�@�V�F�[�_�[���\�[�X�r���[�쐬�i�e�N�X�`���[�m�F���p�j	
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

	//Position�e�N�X�`���p�@�[�x�X�e���V���r���[�̍쐬
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
	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	m_pMesh = new MY_MESH;
	if (FAILED(InitStaticMesh(OBJECT.c_str(), CSVFILE.c_str(), m_pMesh)))
	{
		return E_FAIL;
	}

	//�{�N�Z���쐬
	m_pVoxel = new MY_VOXEL;
	if (FAILED(CreatVoxel(CSVFILE.c_str(), m_pVoxel)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���쐬
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

//�V�F�[�_�[���쐬�@���_���C�A�E�g���`
HRESULT MAIN::InitShader()
{
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;

	//SSS�V�F�[�_�[ ������
	const std::string sssShaderFile = std::string(SOURCE_DIRECTORY) + "sss.hlsl";
	
	//�u���u����SSS�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//SSS�V�F�[�_�[ ���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS])))
	{
		return FALSE;
	}

	//�u���u����SSS�s�N�Z���V�F�[�_�[�쐬
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
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//
	//
	//�u���u����SSS�[�x�e�N�X�`���[�쐬�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//SSS�V�F�[�_�[ ���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements2 = sizeof(layout2) / sizeof(layout2[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout2, numElements2, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_DEPTH])))
	{
		return FALSE;
	}

	//�u���u����SSS�[�x�e�N�X�`���[�쐬�s�N�Z���V�F�[�_�[�쐬
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
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	////
	////
	////�u���u����SSS�Ɠx�e�N�X�`���[�쐬�o�[�e�b�N�X�V�F�[�_�[�쐬
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
	//	MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
	//	return E_FAIL;
	//}

	////SSS�V�F�[�_�[ ���_�C���v�b�g���C�A�E�g���`	
	//D3D11_INPUT_ELEMENT_DESC layout3[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//UINT numElements3 = sizeof(layout3) / sizeof(layout3[0]);

	////���_�C���v�b�g���C�A�E�g���쐬
	//if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(), 
	//										pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_IRRADIANCE])))
	//{
	//	return FALSE;
	//}

	////�u���u����SSS�Ɠx�e�N�X�`���[�쐬�s�N�Z���V�F�[�_�[�쐬
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
	//	MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
	//	return E_FAIL;
	//}

	//SAFE_RELEASE(pCompiledShader);
	//SAFE_RELEASE(pErrors);

	//
	//
	//�u���u����SSS�@���e�N�X�`���[�쐬�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//SSS�V�F�[�_�[ ���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout3[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements3 = sizeof(layout3) / sizeof(layout3[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(), 
											pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_NORMAL])))
	{
		return FALSE;
	}

	//�u���u����SSS�@���e�N�X�`���[�쐬�s�N�Z���V�F�[�_�[�쐬
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
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//�u���u����SSSFresnel�e�N�X�`���[�쐬�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_FRESNEL])))
	{
		return FALSE;
	}

	//�u���u����SSSFresnel�e�N�X�`���[�쐬�s�N�Z���V�F�[�_�[�쐬
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
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//�u���u����SSSPosition�e�N�X�`���[�쐬�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout3, numElements3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout[SSS_POSITION])))
	{
		return FALSE;
	}

	//�u���u����SSS�@���e�N�X�`���[�쐬�s�N�Z���V�F�[�_�[�쐬
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
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//
	//
	//SSS�V�F�[�_�[�@�[�x�e�N�X�`���[�p�T���v���[�쐬
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
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_SSS);	//�\���̂̃T�C�Y��n���Ă���
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
	//�r���[�s��
	D3DXVECTOR3 vEye(0.0f, 0.0f, -3.0f);	//�J�����ʒu
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);	//��������
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);	//����ʒu
	D3DXMatrixLookAtLH(&m_mView, &vEye, &vAt, &vUp);

	//�v���W�F�N�V�����s��i�ˉe�ϊ��j
	D3DXMatrixPerspectiveFovLH(&m_mProj,
		(float)D3DX_PI / 4.0,							//����p
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,		//�A�X�y�N�g��
		0.1f,											//near clip
		100.0f);										//far clip

	float ClearColor[4] = { 0.178, 0.178, 0.178, 1 };// �N���A�F�쐬�@RGBA�̏�
	float ClearColor2[4] = { 0.0, 0.0, 0.0, 1 };

	//���s���e�p
	D3DXMATRIX mOrtho;
	D3DXMatrixOrthoLH(&mOrtho, 2.5f, 2.5f, 0.1f, 100.0f);

	//
	//
	//1�p�X��
	//�[�x�E�@���EFresnel�EPosition�e�N�X�`���쐬

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp); //�r���[�|�[�g�̃Z�b�g

	//
	//�[�x�e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pDepthRenderTargetView, m_pDepthDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pDepthRenderTargetView, ClearColor2);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	//���f�����[���h�s�� 
	D3DXMATRIX mR_x, mR_y, mS;
	D3DXMatrixRotationX(&mR_x, g_ModelAngle_x);
	D3DXMatrixRotationY(&mR_y, g_ModelAngle_y);
	D3DXMatrixScaling(&mS, g_Scale, g_Scale, g_Scale);
	m_pMesh->mWorld = mS * mR_x * mR_y;
	m_pVoxel->mWorld = mS * mR_x * mR_y;

	// �r���[�g�����X�t�H�[�� �����ł̓��C�g����̎��E
	//���C�g����]������
	D3DXMATRIX mLight;
	D3DXVECTOR3 vLight = m_vLightPos;
	D3DXMatrixRotationX(&mR_x, g_Angle_x);
	D3DXMatrixRotationY(&mR_y, g_Angle_y);
	D3DXMATRIX Rot_light = mR_x * mR_y;
	D3DXVec3TransformCoord(&vLight, &vLight, &Rot_light);

	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mLight, &vLight, &vLookatPt, &vUpVec);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_SSS cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E���C�g�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		D3DXMATRIX s;//�d�v�I�@�ۂߌ덷�ɂ��e�N�Z���̒[�ł̃A�[�e�B�t�@�N�g�h�~
		D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
		//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
		//cb.mWLP = s * m_pVoxel->mWorld * mLight * m_mProj;

		cb.mWLP = s * m_pVoxel->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//�]�u����i�V�F�[�_�ł̍s��͗�D��ł���j

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//���_�C���v�b�g���C�A�E�g���Z�b�g(�o�[�e�b�N�X�V�F�[�_�������Ɏ���v�f)�@IA��Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_DEPTH]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g�i�O�p�`���b�V���Ȃ烊�X�g���X�g���b�v���j
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(VOXEL);
	//UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVoxel->VertexBuffer, &stride, &offset);
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pVoxel->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_DEPTH], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_DEPTH], NULL, 0);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(m_pVoxel->voxelNum * 36, 0, 0);
	//m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	////
	////�Ɠx�e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	//m_pDeviceContext->OMSetRenderTargets(1, &m_pIrradianceRenderTargetView, m_pIrradianceDepthStencilView);

	//m_pDeviceContext->ClearRenderTargetView(m_pIrradianceRenderTargetView, ClearColor2);//��ʃN���A
	//m_pDeviceContext->ClearDepthStencilView(m_pIrradianceDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	////�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	//if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	//{
	//	//���[���h�E���C�g�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
	//	D3DXMATRIX s;//�d�v�I�@�ۂߌ덷�ɂ��e�N�Z���̒[�ł̃A�[�e�B�t�@�N�g�h�~
	//	D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
	//	//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
	//	cb.mWLP = s * m_pMesh->mWorld * mLight * mOrtho;
	//	D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//�]�u����i�V�F�[�_�ł̍s��͗�D��ł���j
	//	
	//	//���[���h�s����V�F�[�_�[�ɓn��
	//	cb.mW = m_pMesh->mWorld;
	//	D3DXMatrixTranspose(&cb.mW, &cb.mW);
	//	
	//	////���[���h�E���C�g�r���[�s��̋t�s��̓]�u�s��
	//	//cb.mWLP_t = s * m_pMesh->mWorld * mLight;
	//	//D3DXMatrixInverse(&cb.mWLP_t, NULL, &cb.mWLP_t);
	//	//D3DXMatrixTranspose(&cb.mWLP_t, &cb.mWLP_t);

	//	//���C�g�̈ʒu���V�F�[�_�[�ɓn��
	//	cb.vLightPos = vLight;//vLight�͏�ŁA��]���������Ƃ̃��C�g���݈ʒu

	//	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	//	m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	//}

	////���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	////���_�C���v�b�g���C�A�E�g���Z�b�g(�o�[�e�b�N�X�V�F�[�_�������Ɏ���v�f)�@IA��Input Assembler
	//m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_IRRADIANCE]);
	////�v���~�e�B�u�E�g�|���W�[���Z�b�g�i�O�p�`���b�V���Ȃ烊�X�g���X�g���b�v���j
	//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	//stride = sizeof(VERTEX);
	//offset = 0;
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	////�C���f�b�N�X�o�b�t�@�[���Z�b�g
	//stride = sizeof(int);
	//offset = 0;
	//m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////�g�p����V�F�[�_�[�̓o�^	
	//m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_IRRADIANCE], NULL, 0);
	//m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_IRRADIANCE], NULL, 0);
	////�v���~�e�B�u�������_�����O
	//m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//�@���e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pNormalRenderTargetView, m_pNormalDepthStencilView);

	float yyyy[4] = { 0, 0, 0, 1 };
	m_pDeviceContext->ClearRenderTargetView(m_pNormalRenderTargetView, yyyy);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pNormalDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E���C�g�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		D3DXMATRIX s;//�d�v�I�@�ۂߌ덷�ɂ��e�N�Z���̒[�ł̃A�[�e�B�t�@�N�g�h�~
		D3DXMatrixScaling(&s, g_Tex_Scale, g_Tex_Scale, g_Tex_Scale);
		//cb.mWLP = s * m_pMesh->mWorld * mLight * m_mProj;
		cb.mWLP = s * m_pMesh->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);	//�]�u����i�V�F�[�_�ł̍s��͗�D��ł���j

		//���[���h�s����V�F�[�_�[�ɓn��
		cb.mW = m_pMesh->mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		////���[���h�E���C�g�r���[�s��̋t�s��̓]�u�s��
		//cb.mWLP_t = s * m_pMesh->mWorld * mLight;
		//D3DXMatrixInverse(&cb.mWLP_t, NULL, &cb.mWLP_t);
		//D3DXMatrixTranspose(&cb.mWLP_t, &cb.mWLP_t);

		//���C�g�̈ʒu���V�F�[�_�[�ɓn��
		cb.vLightPos = vLight;//vLight�͏�ŁA��]���������Ƃ̃��C�g���݈ʒu

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[SSS], 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//���_�C���v�b�g���C�A�E�g���Z�b�g(�o�[�e�b�N�X�V�F�[�_�������Ɏ���v�f)�@IA��Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_NORMAL]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g�i�O�p�`���b�V���Ȃ烊�X�g���X�g���b�v���j
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_NORMAL], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_NORMAL], NULL, 0);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//Fresnel�e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pFresnelRenderTargetView, m_pFresnelDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pFresnelRenderTargetView, yyyy);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pFresnelDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	//���_�C���v�b�g���C�A�E�g���Z�b�g(�o�[�e�b�N�X�V�F�[�_�������Ɏ���v�f)�@IA��Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_FRESNEL]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g�i�O�p�`���b�V���Ȃ烊�X�g���X�g���b�v���j
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_FRESNEL], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_FRESNEL], NULL, 0);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);
	

	//
	//Position�e�N�X�`���p�̃����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pPositionRenderTargetView, m_pPositionDepthStencilView);

	m_pDeviceContext->ClearRenderTargetView(m_pPositionRenderTargetView, yyyy);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pPositionDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

																									//���_�C���v�b�g���C�A�E�g���Z�b�g(�o�[�e�b�N�X�V�F�[�_�������Ɏ���v�f)�@IA��Input Assembler
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS_POSITION]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g�i�O�p�`���b�V���Ȃ烊�X�g���X�g���b�v���j
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS_POSITION], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS_POSITION], NULL, 0);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//�����_�[�^�[�Q�b�g��߂�
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//
	//
	//2�p�X��
	//SSS�����_�[

	//�r���[�|�[�g�̐ݒ�
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[SSS], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		cb.mWVP = m_pMesh->mWorld * m_mView * m_mProj;
		//cb.mWVP = m_pMesh->mWorld * m_mView * mOrtho;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		
		//���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
		//cb.mWLP = m_pMesh->mWorld * mLight * m_mProj;
		cb.mWLP = m_pMesh->mWorld * mLight * mOrtho;
		D3DXMatrixTranspose(&cb.mWLP, &cb.mWLP);

		//�ˉe��Ԃ���e�N�X�`�����W��Ԃ֕ϊ�����s���n��
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
		
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3DXVECTOR4 mat(0, 0, 0, 0);
		//mat = D3DXVECTOR4(0.0, 0.0, 0.0, 1);
		//cb.Ambient = mat;
		//mat = D3DXVECTOR4(0.3, 0.3, 1.0, 1);
		mat = D3DXVECTOR4(1.0, 1.0, 1.0, 1);
		cb.Diffuse = mat;
		mat = D3DXVECTOR4(0.3, 0.3, 0.3, 1);
		cb.Specular = mat;

		//�U���W��
		cb.g_Scoeff = D3DXVECTOR3(g_Red, g_Green, g_Blue);

		//�����x�N�g�����G�t�F�N�g�i�V�F�[�_�[�j�ɒʒm�E�K�p
		cb.vEye = vEye;

		//�ǂ��܂œ��߂��邩�̋���
		cb.g_Transparent = g_Thickness_Scale;

		//�{�N�Z����
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
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[SSS]);
	//�P�p�X�ڂō�����[�x�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(0, 1, &m_pDepthTextureView);
	//���݃e�N�X�`����VS�ɓn��
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(1, 1, &m_pThicknessTexture);
	//���g�e�N�X�`���i�΁j��n��
	m_pDeviceContext->VSSetSamplers(0, 1, &m_pSamPoint);
	m_pDeviceContext->VSSetShaderResources(2, 1, &m_pNaKaMiTexture);
	////�Ɠx�e�N�X�`����n��
	//m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	//m_pDeviceContext->VSSetShaderResources(3, 1, &m_pIrradianceTextureView);
	//�@���e�N�X�`����n��
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(3, 1, &m_pNormalTextureView);
	//Fresnel�e�N�X�`����n��
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(4, 1, &m_pFresnelTextureView);
	//Position�e�N�X�`����n��
	m_pDeviceContext->VSSetSamplers(1, 1, &m_pSamLinear);
	m_pDeviceContext->VSSetShaderResources(5, 1, &m_pPositionTextureView);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(VERTEX);
	offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMesh->pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	m_pDeviceContext->IASetIndexBuffer(m_pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[SSS]);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[SSS], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[SSS], NULL, 0);
	//�v���~�e�B�u�������_�����O	
	m_pDeviceContext->DrawIndexed(m_pMesh->dwNumFace * 3, 0, 0);


	//
	//�[�x�e�N�X�`���m�F
	//DrawTexture(m_pDepthTextureView);
	//DrawTexture(m_pIrradianceTextureView);
	//DrawTexture(m_pNormalTextureView);
	//DrawTexture(m_pFresnelTextureView);
	DrawTexture(m_pPositionTextureView);

	//
	DrawLight(m_pLightTexture);

	m_pSwapChain->Present(0, 0);//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj	
}
//
//
//FPS�̌v�Z�E�\��
void MAIN::FPS()
{
	//FPS�v�Z�\��
	static DWORD time = 0;
	static int frame = 0;
	static int av_frame = 0;
	static int fps[3600] = { 0 };//�v����1���Ԉȓ��ɂ��邱��
	static int avg = 0;
	static int first2 = 0;
	frame++;
	char str[60];
	sprintf(str, "fps=%d avg_fps=%d  R=%f G=%f B=%f", frame, avg, g_Red, g_Green, g_Blue); //���ł�RGB���\�����Ƃ�
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
//��ʂɐ[�x�e�N�X�`����`�悷��
HRESULT MAIN::DrawTexture(ID3D11ShaderResourceView* TextureSRV)
{
	const std::string dtexShaderFile = std::string(SOURCE_DIRECTORY) + "drawtex.hlsl";
	static bool boOnce = false;
	if (!boOnce)
	{
		boOnce = true;
		//Tex�V�F�[�_�[ ������
		//�u���u����DRAW_TEX�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		//Tex�V�F�[�_�[�p�@���_�C���v�b�g���C�A�E�g���쐬
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
		//�u���u����DRAW_TEX�s�N�Z���V�F�[�_�[�쐬
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
		//Tex�V�F�[�_�[�p�@�|���S���o�[�e�b�N�X�o�b�t�@�[�쐬
		TEX_VERTEX vertices[] =
		{
			D3DXVECTOR3(-0.2, -0.2, 0), D3DXVECTOR2(0, 1),//���_1,
			D3DXVECTOR3(-0.2, 0.2, 0), D3DXVECTOR2(0, 0),//���_2
			D3DXVECTOR3(0.2, -0.2, 0), D3DXVECTOR2(1, 1), //���_3
			D3DXVECTOR3(0.2, 0.2, 0), D3DXVECTOR2(1, 0), //���_4

			//�S��ʂŌ�������
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
		//�R���X�^���g�o�b�t�@�[�쐬
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
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(TEX_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pDrawTexVertexBuffer, &stride, &offset);
	//���[���h�s��
	D3DXMATRIX mWorld;
	D3DXMATRIX mPosition;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mPosition, -0.8, -0.8, 2.5);
	//D3DXMatrixTranslation(&mPosition, 0, 0, 2.4); //�S��ʂŌ�������
	mWorld = mPosition;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	CONSTANT_BUFFER_DRAWTEX cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[DRAW_TEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn���i�Œ�ʒu�\���Ȃ̂Ńr���[�ϊ��͂����Ȃ��j
		cb.mWP = mWorld*m_mProj;
		D3DXMatrixTranspose(&cb.mWP, &cb.mWP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[DRAW_TEX], 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[DRAW_TEX]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &TextureSRV);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[DRAW_TEX], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[DRAW_TEX], NULL, 0);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->Draw(4, 0);

	return S_OK;
}
//
//
//�����ʒu��\������
HRESULT MAIN::DrawLight(ID3D11ShaderResourceView* LightTextureSRV)
{
	const std::string dtexShaderFile = std::string(SOURCE_DIRECTORY) + "drawtex.hlsl";
	static bool boOnce = false;
	if (!boOnce)
	{
		boOnce = true;
		//Tex�V�F�[�_�[ ������
		//�u���u����DRAW_TEX�o�[�e�b�N�X�V�F�[�_�[�쐬
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
		//Tex�V�F�[�_�[�p�@���_�C���v�b�g���C�A�E�g���쐬
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
		//�u���u����DRAW_TEX�s�N�Z���V�F�[�_�[�쐬
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
		//Tex�V�F�[�_�[�p�@�|���S���o�[�e�b�N�X�o�b�t�@�[�쐬
		TEX_VERTEX vertices[] =
		{
			D3DXVECTOR3(-0.3, -0.3, 2) + m_vLightPos, D3DXVECTOR2(0, 1),//���_1,
			D3DXVECTOR3(-0.3, 0.3, 2) + m_vLightPos, D3DXVECTOR2(0, 0),//���_2
			D3DXVECTOR3(0.3, -0.3, 2) + m_vLightPos, D3DXVECTOR2(1, 1), //���_3
			D3DXVECTOR3(0.3, 0.3, 2) + m_vLightPos, D3DXVECTOR2(1, 0), //���_4
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
		//�R���X�^���g�o�b�t�@�[�쐬
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
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(TEX_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pLightTexVertexBuffer, &stride, &offset);

	//���f�����[���h�s�� 
	D3DXMATRIX mWorld;
	D3DXMATRIX mR_x, mR_y, mS;
	//D3DXMatrixRotationX(&mR_x, g_ModelAngle_x);
	D3DXMatrixRotationX(&mR_x, g_Angle_x);
	//D3DXMatrixRotationY(&mR_y, g_ModelAngle_y);
	D3DXMatrixRotationY(&mR_y, g_Angle_y);
	D3DXMatrixScaling(&mS, g_Scale, g_Scale, g_Scale);
	mWorld = mS * mR_x * mR_y;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	CONSTANT_BUFFER_DRAWTEX cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer[DRAW_TEX], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��(�����̖ʓ|������mWP�̂܂�)
		cb.mWP = mWorld*m_mView*m_mProj;
		D3DXMatrixTranspose(&cb.mWP, &cb.mWP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer[DRAW_TEX], 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer[DRAW_TEX]);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout[DRAW_TEX]);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &LightTextureSRV);
	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader[DRAW_TEX], NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader[DRAW_TEX], NULL, 0);
	//�v���~�e�B�u�������_�����O
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