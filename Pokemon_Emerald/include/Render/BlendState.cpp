#include "BlendState.h"
#include "../Device.h"



CBlendState::CBlendState()
{
	mStateType = ERenderStateType::Blend;
}

CBlendState::~CBlendState()
{
}


void CBlendState::SetBlendFactor(float r, float g, float b, float a)
{
	mBlendFactor[0] = r;
	mBlendFactor[1] = g;
	mBlendFactor[2] = b;
	mBlendFactor[3] = a;
}

void CBlendState::SetSampleMarks(UINT Mask)
{
	mSampleMask = Mask;
}

void CBlendState::AddBlendDesc(bool Enable, D3D11_BLEND Src, D3D11_BLEND Dest, D3D11_BLEND_OP BlendOp,
                               D3D11_BLEND SrcAlpha, D3D11_BLEND DestAlpha, D3D11_BLEND_OP AlphaOp, UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC Desc = {};

	Desc.BlendEnable = Enable;
	Desc.SrcBlend = Src;
	Desc.DestBlend = Dest;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcAlpha;
	Desc.DestBlendAlpha = DestAlpha;
	Desc.BlendOpAlpha = AlphaOp;
	// 렌더 타겟에서 쓸수 있는 색갈 지정.
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	mDesc.emplace_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (mDesc.empty())
	{
		return false;
	}

	D3D11_BLEND_DESC Desc = {};

	//알파 겹침 사용 여부 : 덩쿨 같은거 만들때 사용한다. 
	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// 독립적 블렌드 사용 여부
	// 1개를 지정하면 나머지 7개는 동일하게 적용할지 여부를 물어보는 것이다. 
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	for(int i = 0 ; i < 8; ++i)
	{
		Desc.RenderTarget[i].BlendEnable = false;
		Desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
		Desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		Desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	//우리가 설정한 배열만큼 복사한다. 
	memcpy(Desc.RenderTarget, &mDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * mDesc.size());

	//블랜드 스테이트 생성
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&mState)))
	{
		return false;
	}

	return true;
}

void CBlendState::SetState()
{
	//현재 디바이스에 지정된 블렌드 스테이트를 얻어온다.
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&mPrevState, mPrevBlendFactor, &mPrevSampleMask);

	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)mState, mBlendFactor, mSampleMask);
}

void CBlendState::ResetState()
{
	// 사용할 블랜드 스테이트를 설정해준다.
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)mPrevState, mPrevBlendFactor, mPrevSampleMask);

	//SetState 할때 Get을 이용해서 사용했기 때문에
	// 레퍼런스 카운트를 1 낮춰줘야한다. 
	SAFE_RELEASE(mPrevState);
}

