#include "FrameMeshShader.h"


CFrameMeshShader::CFrameMeshShader()
{
}

CFrameMeshShader::~CFrameMeshShader()
{

}

bool CFrameMeshShader::Init()
{
	//쉐이더 불러오기 
	//정점 쉐이더 
	if (!LoadVertexShader("FrameMeshVS", TEXT("ColorMesh.fx")))
	{
		return false;
	}
	//픽셀 쉐이더 
	if (!LoadPixelShader("FrameMeshPS", TEXT("ColorMesh.fx")))
	{
		return false;
	}

	//레이아웃 정보 만들어주기
	AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		return false;
	}

	return true;
}