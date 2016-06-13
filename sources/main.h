#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <string>
#include <algorithm>
#include "debug.h"
#include "common.h"
//#include "INIT.h"

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�x����\��
#pragma warning(disable : 4305)

//�萔��`
#define WINDOW_WIDTH 800 //�E�B���h�E��
#define WINDOW_HEIGHT 800 //�E�B���h�E����
#define DEPTHTEX_WIDTH 512*2
#define DEPTHTEX_HEIGHT 512*2
#define APP_NAME "Window"
#define SSS_DEPTH 0
#define SSS_IRRADIANCE 1
#define SSS_NORMAL 2
#define SSS_FRESNEL 3
#define SSS_POSITION 4
#define SSS 5
#define DRAW_TEX 6

//random 
#define SAMPLENUM 256
#define RADIUS 1.0f //0.1f

//���S�ɊJ�����邽�߂̃}�N��
#define SAFE_RELEASE(x) if(x){x -> Release(); x = NULL;}	// �|�C���^
#define SAFE_DELETE(x) if(x){delete x; x = 0;}				// ���l
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x = 0;}		// �z��

//�ǂݍ��ރf�[�^
//#define OBJECT "box4.obj"
//#define CSVFILE "box_25.csv"
//#define LUT "box_25.png"
//#define LUT_IN "BRANK.png"

//#define OBJECT "box_sphere.obj"

//#define OBJECT "bottle.obj"
//#define CSVFILE "bottle_60.csv"
//#define LUT "bottle_60.png"
//#define LUT_IN "bottle_inner_60.png"

//#define OBJECT "torus.obj"
//#define CSVFILE "torus_60.csv"
//#define LUT "torus_60.png"
//#define LUT_IN "BRANK.png"

static const std::string OBJECT  = std::string(DATA_DIRECTORY) + "winebottle/wb.obj";
static const std::string CSVFILE = std::string(DATA_DIRECTORY) + "winebottle/wb_60.csv";
static const std::string LUT     = std::string(DATA_DIRECTORY) + "winebottle/wb_60.png";
static const std::string LUT_IN  = std::string(DATA_DIRECTORY) + "winebottle/wb_inner_60.png";
//
//#define OBJECT "chess.obj"
//#define CSVFILE "chess_50.csv"
//#define LUT "chess_50.png"
//#define LUT_IN "chess2_50.png"

//#define OBJECT "head_outer.obj"
//#define CSVFILE "head_60.csv"
//#define LUT "head_60.png"
//#define LUT_IN "head_inner_60.png"

//#define OBJECT "usagi_.obj"
//#define CSVFILE "usagi_40.csv"
//#define LUT "head_60.png"
//#define LUT_IN "head_inner_60.png"


//#define LIGHTTEX "Light.bmp"
static const std::string LIGHTTEX = std::string(DATA_DIRECTORY) + "light.bmp";
//#define LIGHTTEX "Light2.png"


//���_�̍\����
struct VERTEX
{
    D3DXVECTOR4 vPos;		//�ʒu
    D3DXVECTOR4 vNormal;	//�@��
    D3DXVECTOR2 vTex;		// �e�N�X�`�����W
};

//���_�̍\���́iDrawTexture()�p�j
struct TEX_VERTEX
{
    D3DXVECTOR3 vPos; //�ʒu
    D3DXVECTOR2 vTex; //�e�N�X�`���[���W
};

//���_�̍\����(voxel�p)
struct VOXEL
{
    D3DXVECTOR4 vPos; //�ʒu
    D3DXVECTOR2 vTex; //�e�N�X�`���[���W
};

struct MY_VOXEL
{
    DWORD voxelNum;
    ID3D11Buffer* VertexBuffer;
    ID3D11Buffer* IndexBuffer;
    D3DXMATRIX mWorld;
    MY_VOXEL()
    {
        ZeroMemory(this, sizeof(MY_VOXEL));
    }
};

//�I���W�i�����b�V��
struct MY_MESH
{
    DWORD dwNumFace;
    ID3D11Buffer* pVertexBuffer;
    ID3D11Buffer* pIndexBuffer;
    D3DXMATRIX mWorld;
    MY_MESH()
    {
        ZeroMemory(this,sizeof(MY_MESH));
    }
};

#define ALIGN16 _declspec(align(16))
//constant buffer
struct CONSTANT_BUFFER_SSS
{
    ALIGN16 D3DXMATRIX mW;
    ALIGN16 D3DXMATRIX mWVP;
    ALIGN16 D3DXMATRIX mWLPT;
    ALIGN16 D3DXMATRIX mWLP;
    ALIGN16 D3DXVECTOR3 vLightPos;//���C�g�̈ʒu
    //ALIGN16 D3DXVECTOR4 Ambient;//�A���r�G���g��
    ALIGN16 D3DXVECTOR3 g_Scoeff;
    ALIGN16 D3DXVECTOR4 Diffuse; //�g�U����
    ALIGN16 D3DXVECTOR4 Specular;//���ʔ���
    ALIGN16 D3DXVECTOR3 vEye;
    ALIGN16 float g_Transparent;////�ǂ��܂ł̒����Ȃ瓧�߂��邩
    ALIGN16 float g_HalfThicknessPixel;
    ALIGN16 float g_DepthPixel;
    ALIGN16 float g_HalfDepthPixel;
    ALIGN16 float g_random[SAMPLENUM][2];
};

struct CONSTANT_BUFFER_DRAWTEX
{
    D3DXMATRIX mWP;
};

//struct SHADER
//{
//	ID3D11VertexShader* m_pVertexShader;	
//};


class MAIN
{
public:
    //data
    HWND m_hWnd;
    ID3D11Device* m_pDevice;				//�쐬�S���̐l
    ID3D11DeviceContext* m_pDeviceContext;	//�`��Ƃ��D���Ȑl
    IDXGISwapChain* m_pSwapChain;			//�o�b�t�@����ւ���̂�炳��Ă�l

    ID3D11RenderTargetView* m_pRenderTargetView;//�ʏ탌���_�����O�p
    ID3D11DepthStencilView* m_pDepthStencilView;//�ʏ탌���_�����O�p
    ID3D11Texture2D* m_pDepthStencil;//�ʏ탌���_�����O�p
    
    ID3D11RenderTargetView* m_pDepthRenderTargetView;//�[�x�e�N�X�`���[�����_�����O�p
    ID3D11DepthStencilView* m_pDepthDepthStencilView;//�[�x�e�N�X�`���[�����_�����O�p
    ID3D11Texture2D* m_pDepthDepthStencil;//�[�x�e�N�X�`���[�����_�����O�p

    ID3D11RenderTargetView* m_pIrradianceRenderTargetView;//Irradiance�e�N�X�`���[�����_�����O�p
    ID3D11DepthStencilView* m_pIrradianceDepthStencilView;//Irradiance�e�N�X�`���[�����_�����O�p
    ID3D11Texture2D* m_pIrradianceDepthStencil;//Irradiance�e�N�X�`���[�����_�����O�p

    ID3D11RenderTargetView* m_pNormalRenderTargetView;//Normal�e�N�X�`���[�����_�����O�p
    ID3D11DepthStencilView* m_pNormalDepthStencilView;//Normal�e�N�X�`���[�����_�����O�p
    ID3D11Texture2D* m_pNormalDepthStencil;//Normal�e�N�X�`���[�����_�����O�p

    ID3D11RenderTargetView* m_pFresnelRenderTargetView;//Fresnel�e�N�X�`���[�����_�����O�p
    ID3D11DepthStencilView* m_pFresnelDepthStencilView;//Fresnel�e�N�X�`���[�����_�����O�p
    ID3D11Texture2D* m_pFresnelDepthStencil;//Fresnel�e�N�X�`���[�����_�����O�p

    ID3D11RenderTargetView* m_pPositionRenderTargetView;//Position�e�N�X�`���[�����_�����O�p
    ID3D11DepthStencilView* m_pPositionDepthStencilView;//Position�e�N�X�`���[�����_�����O�p
    ID3D11Texture2D* m_pPositionDepthStencil;//Position�e�N�X�`���[�����_�����O�p

    ID3D11RasterizerState* m_pRasterizerState;

    D3DXMATRIX m_mView;
    D3DXMATRIX m_mProj;	
    
    ID3D11SamplerState* m_pSamLinear;
    ID3D11SamplerState* m_pSamPoint;
    
    MY_MESH* m_pMesh;
    
    D3DXVECTOR3 m_vLightPos;
    
    ID3D11Texture2D* m_pDepthTexture;
    ID3D11Texture2D* m_pIrradianceTexture;
    ID3D11Texture2D* m_pNormalTexture;
    ID3D11Texture2D* m_pFresnelTexture;
    ID3D11Texture2D* m_pPositionTexture;

    ID3D11ShaderResourceView* m_pDepthTextureView;
    ID3D11ShaderResourceView* m_pIrradianceTextureView;
    ID3D11ShaderResourceView* m_pNormalTextureView;
    ID3D11ShaderResourceView* m_pFresnelTextureView;
    ID3D11ShaderResourceView* m_pPositionTextureView;

    ID3D11InputLayout* m_pVertexLayout[10];//10���g��Ȃ����A�Ƃ肠�����B
    ID3D11VertexShader* m_pVertexShader[10];
    ID3D11PixelShader* m_pPixelShader[10];
    ID3D11Buffer* m_pConstantBuffer[10];

    MY_VOXEL* m_pVoxel;

    ID3D11ShaderResourceView* m_pThicknessTexture;//���݃e�N�X�`���[
    ID3D11ShaderResourceView* m_pNaKaMiTexture;//���g�̃e�N�X�`���[�i�΁j

    ID3D11Buffer* m_pDrawTexVertexBuffer;	//�[�x�e�N�X�`���\���p

    ID3D11Buffer* m_pLightTexVertexBuffer;	//Light�e�N�X�`���\���p
    ID3D11ShaderResourceView* m_pLightTexture;//Light�e�N�X�`���[

    //method
    MAIN();
    ~MAIN();
    HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
    LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
    HRESULT InitD3D();
    void App();
    void Render();
    void Destroy();
    void FPS();
    HRESULT InitShader();
    HRESULT InitStaticMesh(LPCSTR FileName,
                           LPCSTR FileName2, MY_MESH* pMesh);
    HRESULT CreatVoxel(LPCSTR filename, MY_VOXEL* _voxel);
    HRESULT DrawTexture(ID3D11ShaderResourceView* TextureSRV);
    HRESULT DrawLight(ID3D11ShaderResourceView* LightTextureSRV);
};
