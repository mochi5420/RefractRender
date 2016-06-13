#include "MAIN.h"

#define MAX 200

using namespace std;

HRESULT MAIN::CreatVoxel(LPCSTR filename, MY_VOXEL* _voxel)
{
	FILE *fp = NULL;
	char buf[MAX] = { 0 };
	char *ary[3] = {0};
	double data[3] = { 0 };
	float uv;

	_voxel->voxelNum = 0;

	if ((fp = fopen(filename, "r")) != NULL){

		//ボクセル数を把握する
		while (fgets(buf, MAX, fp) != NULL)
		{
			_voxel->voxelNum++;
		}

		//メモリ確保
		VOXEL* vertexBuffer = new VOXEL[_voxel->voxelNum * 8];
		D3DXVECTOR4* vertex = new D3DXVECTOR4[_voxel->voxelNum];
		int* indexBuffer = new int[_voxel->voxelNum * 36];


		//本読み込み
		fseek(fp, SEEK_SET, 0);
		int num = 0;
		/*ファイルの終わりまで繰り返し読み込む*/
		while (fgets(buf, MAX, fp) != NULL){
			/*文字列(char配列)をカンマで分割する*/
			ary[0] = strtok(buf, ",");
			ary[1] = strtok(NULL, ",");
			ary[2] = strtok(NULL, ",");
			/*文字列(char配列)をdoubleに変換する*/
			data[0] = atof(ary[0]);
			data[1] = atof(ary[1]);
			data[2] = atof(ary[2]);

			vertex[num] = D3DXVECTOR4(data[0], data[1], data[2], 1.0);
			num++;
		}

		//読み込んだ座標から８つの頂点を作成
		float half_length = 0.5 * D3DXVec3Length(&D3DXVECTOR3(vertex[0] - vertex[1])); //とりあえず（ダメなときもあるよな）
		
		for (int i = 0; i < _voxel->voxelNum; i++){

			uv = (float)i / (float)_voxel->voxelNum;

			vertexBuffer[i * 8].vPos = vertex[i] + D3DXVECTOR4(-half_length, half_length, half_length,0);
			vertexBuffer[i * 8].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 1].vPos = vertex[i] + D3DXVECTOR4(-half_length, -half_length, half_length,0);
			vertexBuffer[i * 8 + 1].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 2].vPos = vertex[i] + D3DXVECTOR4(half_length, -half_length, half_length,0);
			vertexBuffer[i * 8 + 2].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 3].vPos = vertex[i] + D3DXVECTOR4(half_length, half_length, half_length,0);
			vertexBuffer[i * 8 + 3].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 4].vPos = vertex[i] + D3DXVECTOR4(half_length, half_length, -half_length,0);
			vertexBuffer[i * 8 + 4].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 5].vPos = vertex[i] + D3DXVECTOR4(half_length, -half_length, -half_length,0);
			vertexBuffer[i * 8 + 5].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 6].vPos = vertex[i] + D3DXVECTOR4(-half_length, -half_length, -half_length,0);
			vertexBuffer[i * 8 + 6].vTex = D3DXVECTOR2(uv, uv);
			vertexBuffer[i * 8 + 7].vPos = vertex[i] + D3DXVECTOR4(-half_length, half_length, -half_length,0);
			vertexBuffer[i * 8 + 7].vTex = D3DXVECTOR2(uv, uv);

			indexBuffer[i * 36] = i * 8;
			indexBuffer[i * 36 + 1] = i * 8 + 2;
			indexBuffer[i * 36 + 2] = i * 8 + 1;

			indexBuffer[i * 36 + 3] = i * 8;
			indexBuffer[i * 36 + 4] = i * 8 + 3;
			indexBuffer[i * 36 + 5] = i * 8 + 2;

			indexBuffer[i * 36 + 6] = i * 8 + 3;
			indexBuffer[i * 36 + 7] = i * 8 + 5;
			indexBuffer[i * 36 + 8] = i * 8 + 2;

			indexBuffer[i * 36 + 9] = i * 8 + 3;
			indexBuffer[i * 36 + 10] = i * 8 + 4;
			indexBuffer[i * 36 + 11] = i * 8 + 5;

			indexBuffer[i * 36 + 12] = i * 8 + 4;
			indexBuffer[i * 36 + 13] = i * 8 + 6;
			indexBuffer[i * 36 + 14] = i * 8 + 5;

			indexBuffer[i * 36 + 15] = i * 8 + 4;
			indexBuffer[i * 36 + 16] = i * 8 + 7;
			indexBuffer[i * 36 + 17] = i * 8 + 6;

			indexBuffer[i * 36 + 18] = i * 8 + 7;
			indexBuffer[i * 36 + 19] = i * 8 + 1;
			indexBuffer[i * 36 + 20] = i * 8 + 6;

			indexBuffer[i * 36 + 21] = i * 8 + 7;
			indexBuffer[i * 36 + 22] = i * 8;
			indexBuffer[i * 36 + 23] = i * 8 + 1;

			indexBuffer[i * 36 + 24] = i * 8 + 7;
			indexBuffer[i * 36 + 25] = i * 8 + 3;
			indexBuffer[i * 36 + 26] = i * 8;

			indexBuffer[i * 36 + 27] = i * 8 + 7;
			indexBuffer[i * 36 + 28] = i * 8 + 4;
			indexBuffer[i * 36 + 29] = i * 8 + 3;

			indexBuffer[i * 36 + 30] = i * 8 + 1;
			indexBuffer[i * 36 + 31] = i * 8 + 5;
			indexBuffer[i * 36 + 32] = i * 8 + 3;

			indexBuffer[i * 36 + 33] = i * 8 + 1;
			indexBuffer[i * 36 + 34] = i * 8 + 2;
			indexBuffer[i * 36 + 35] = i * 8 + 5;

		}

		//creat the vertexbuffer
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VOXEL) *_voxel->voxelNum * 8;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertexBuffer;
		if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &_voxel->VertexBuffer)))
			return FALSE;

		//インデックスバッファーを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * _voxel->voxelNum * 36;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = indexBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &_voxel->IndexBuffer)))
			return FALSE;


		delete[] vertex;
		delete[] vertexBuffer;
		delete[] indexBuffer;
	}

	return S_OK;
}
