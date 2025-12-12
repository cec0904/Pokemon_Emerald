#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{
	mStateType = ERenderStateType::DepthStencil;
}

CDepthStencilState::~CDepthStencilState()
{
}


bool CDepthStencilState::CreateState(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask,
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask, UINT8 StencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC Desc = {};
	;
	Desc.DepthEnable = DepthEnable;
	Desc.DepthWriteMask = DepthWriteMask;
	Desc.DepthFunc = DepthFunc;
	Desc.StencilEnable = StencilEnable;
	Desc.StencilReadMask = StencilReadMask;
	Desc.StencilWriteMask = StencilWriteMask;
	Desc.FrontFace = FrontFace;
	Desc.BackFace = BackFace;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc, (ID3D11DepthStencilState**)&mState)))
	{
		return false;
	}

	return true;
}

void CDepthStencilState::SetState()
{
	//현재 디바이스에 지정된 블렌드 스테이트를 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)&mPrevState, &mPrevStencilRef);

	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)mState, mStencilRef);
}

void CDepthStencilState::ResetState()
{
	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)mPrevState, mPrevStencilRef);

	//SetState 할때 Get을 이용해서 사용했기 때문에
	// 레퍼런스 카운트를 1 낮춰줘야한다. 
	SAFE_RELEASE(mPrevState);
}
