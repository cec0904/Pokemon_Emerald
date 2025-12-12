#pragma once
#include "RenderState.h"
class CDepthStencilState :
	public CRenderState
{
	friend class CRenderStateManager;

protected:
	CDepthStencilState();
	virtual ~CDepthStencilState();

protected:
	//기본값
	unsigned int	mStencilRef = 0xffffffff;
	unsigned int	mPrevStencilRef = 0xffffffff;

public:
	//D3D11_DEPTH_STENCIL_DESC
	bool CreateState(bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC BackFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });



	//BOOL DepthEnable;
	//D3D11_DEPTH_WRITE_MASK DepthWriteMask;    // D3D11_DEPTH_WRITE_MASK 용도 : 깊이 버퍼에 쓰기 허용 여부
	//D3D11_COMPARISON_FUNC DepthFunc;          // D3D11_COMPARISON_FUNC 용도 : 깊이 비교 함수 설정
	//BOOL StencilEnable;						// 스텐실 테스트 사용 여부
	//UINT8 StencilReadMask;					// 
	//UINT8 StencilWriteMask;
	//D3D11_DEPTH_STENCILOP_DESC FrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
	//D3D11_DEPTH_STENCILOP_DESC BackFace;

public:
	virtual void SetState();
	virtual void ResetState();
};

