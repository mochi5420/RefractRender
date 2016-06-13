#include "MAIN.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "DEBUG.h"

struct Triplet {
	Triplet(unsigned int a_ = 0u, unsigned int b_ = 0u, unsigned int c_ = 0u) 
		: a(a_), b(b_), c(c_) {
	}
	unsigned int a, b, c;
};

HRESULT MAIN::InitStaticMesh(LPCSTR FileName, LPCSTR FileName2, MY_MESH* pMesh)
{
	// ---------------------------------------------------------------------
	//  VertexとNormalを.objファイルから読み込む処理 
	// ---------------------------------------------------------------------
	std::ifstream ifs(FileName, std::ios::in);
	if (!ifs.is_open()) {
		DebugWindow("Failed to load file: %s", FileName);
		std::exit(1);
	}

	std::vector<D3DXVECTOR4> pvCoord;
	std::vector<D3DXVECTOR4> pvNormal;
	std::vector<Triplet> vIndices;
	std::vector<Triplet> nIndices;

	std::string line;
	while(!ifs.eof())
	{
		std::getline(ifs, line);

		std::stringstream ss;
		ss << line;

		std::string identifier;
		ss >> identifier;

		if (identifier == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			pvCoord.push_back(D3DXVECTOR4(x, y, z, 1.0f));
		} else if (identifier == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			pvNormal.push_back(D3DXVECTOR4(x, y, z, 1.0f));
		} else if (identifier == "f") {
			std::string s1, s2, s3;
			ss >> s1 >> s2 >> s3;

			int v1, v2, v3, vn1, vn2, vn3;

			if (sscanf(s1.c_str(), "%d//%d", &v1, &vn1) != 2 ||
			    sscanf(s2.c_str(), "%d//%d", &v2, &vn2) != 2 ||
				sscanf(s3.c_str(), "%d//%d", &v3, &vn3) != 2) {
				DebugWindow("Failed to parse file!!");
				std::exit(1);
			}
			vIndices.push_back(Triplet(v1 - 1, v2 - 1, v3 - 1));
			nIndices.push_back(Triplet(vn1 - 1, vn2 - 1, vn3 - 1));
		}	
	}

	std::vector<unsigned int> piFaceBuffer;
	std::vector<VERTEX> pvVertexBuffer(pvCoord.size());
	for (int i = 0; i < vIndices.size(); i++) {
		Triplet vt = vIndices[i];
		Triplet nt = nIndices[i];

		pvVertexBuffer[vt.a].vPos    = pvCoord[vt.a];
		pvVertexBuffer[vt.a].vNormal = pvNormal[nt.a];
		pvVertexBuffer[vt.b].vPos    = pvCoord[vt.b];
		pvVertexBuffer[vt.b].vNormal = pvNormal[nt.b];
		pvVertexBuffer[vt.c].vPos    = pvCoord[vt.c];
		pvVertexBuffer[vt.c].vNormal = pvNormal[nt.c];

		piFaceBuffer.push_back(vt.a);
		piFaceBuffer.push_back(vt.b);
		piFaceBuffer.push_back(vt.c);
	}
	ifs.close();	

	// ---------------------------------------------------------------------
	//  ボクセルの中心位置をCSVファイルから読み込む
	// ---------------------------------------------------------------------
	ifs.open(FileName2, std::ios::in);
	if (!ifs.is_open()) {
		DebugWindow("Failed to load file: %S", FileName2);
		std::exit(1);
	}

	std::vector<D3DXVECTOR4> texCoord3d;
	while (!ifs.eof()) {
		std::getline(ifs, line);

		float x, y, z;
		if (sscanf(line.c_str(), "%f,%f,%f", &x, &y, &z) != 3) {
			continue;
		}

		texCoord3d.push_back(D3DXVECTOR4(x, y, z, 1.0f));
	}
	ifs.close();

	// ---------------------------------------------------------------------
	//  各Vertexの属するボクセルを探す
	// ---------------------------------------------------------------------
	for (int i = 0; i < pvVertexBuffer.size(); i++)
	{
		double minDist = 1.0e20;
		int    minID   = -1;
		for (int j = 0; j < texCoord3d.size(); j++)
		{
			double dist = D3DXVec4LengthSq(&D3DXVECTOR4(pvVertexBuffer[i].vPos - texCoord3d[j]));
			if (dist < minDist)
			{
				minID = j;
				minDist = dist;
			}
		}

		//UV座標
		pvVertexBuffer[i].vTex.x = (float)minID / (float)texCoord3d.size();
		pvVertexBuffer[i].vTex.y = (float)minID / (float)texCoord3d.size();
	}

	// ---------------------------------------------------------------------
	//  描画用のVertex Bufferを作成
	// ---------------------------------------------------------------------
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX ) * pvVertexBuffer.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)&pvVertexBuffer[0];
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &pMesh->pVertexBuffer ) ) ) {
		DebugWindow("Failed to create vertex buffer!!");
		return FALSE;
	}

	// ---------------------------------------------------------------------
	//  描画用のIndex Bufferを作成
	// ---------------------------------------------------------------------	
	pMesh->dwNumFace = vIndices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( int ) * pMesh->dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = (void*)&piFaceBuffer[0];
	InitData.SysMemPitch=0;
	InitData.SysMemSlicePitch=0;
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &pMesh->pIndexBuffer ) ) ) {
		DebugWindow("Failed to create index buffer!!");
		return FALSE;
	}

	return S_OK;
}