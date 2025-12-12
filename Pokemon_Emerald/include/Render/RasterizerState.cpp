#include "RasterizerState.h"
#include "../Device.h"

CRasterizerState::CRasterizerState()
{
}

CRasterizerState::~CRasterizerState()
{
}

void CRasterizerState::SetRasterizerDesc(D3D11_CULL_MODE CullMode, bool DepthClipEnable)
{
	// 1. 구조체 선언 및 초기화
	D3D11_RASTERIZER_DESC RasterDesc = {};

	// 2. 주요 속성 설정
	// D3D11_FILL_SOLID: 내부를 채움 (와이어프레임은 D3D11_FILL_WIREFRAME)
	RasterDesc.FillMode = D3D11_FILL_SOLID;

	// [핵심] D3D11_CULL_NONE: 앞/뒷면 구분 없이 모두 그리기
	// (기본값인 D3D11_CULL_BACK을 쓰면 뒤집힌 스프라이트가 안 보임)
	RasterDesc.CullMode = CullMode;

	// 깊이 클리핑 켜기 (화면 밖 물체 잘라내기)
	RasterDesc.DepthClipEnable = DepthClipEnable;

	mDesc = RasterDesc;
}

bool CRasterizerState::CreateRasterizerState()
{
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRasterizerState(&mDesc, (ID3D11RasterizerState**)&mState)))
	{
		return false;
	}

	return true;
}

void CRasterizerState::SetState()
{
	//현재 디바이스에 지정된 블렌드 스테이트를 얻어온다.
	CDevice::GetInst()->GetContext()->RSGetState((ID3D11RasterizerState**)&mPrevState);

	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->RSSetState((ID3D11RasterizerState*)mState);
}

void CRasterizerState::ResetState()
{
	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->RSSetState((ID3D11RasterizerState*)mPrevState);

	//SetState 할때 Get을 이용해서 사용했기 때문에
	// 레퍼런스 카운트를 1 낮춰줘야한다. 
	SAFE_RELEASE(mPrevState);
}
