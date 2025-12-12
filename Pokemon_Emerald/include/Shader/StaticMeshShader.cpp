#include "StaticMeshShader.h"

CStaticMeshShader::CStaticMeshShader()
{
}

CStaticMeshShader::~CStaticMeshShader()
{
}

bool CStaticMeshShader::Init()
{
	//쉐이더 불러오기 
	//정점 쉐이더 
	if (!LoadVertexShader("MeshVS", TEXT("Mesh.fx")))
	{
		return false;
	}
	//픽셀 쉐이더 
	if (!LoadPixelShader("DefaultMaterialPS", TEXT("Mesh.fx")))
	{
		return false;
	}

	//레이아웃 정보 만들어주기
	AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		return false;
	}

	return true;
}
