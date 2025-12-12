#pragma once
#include "RenderState.h"

class CRasterizerState :
    public CRenderState
{
	friend class CRenderStateManager;

protected:
	CRasterizerState();
	virtual ~CRasterizerState();

protected:
	//ID3D11RasterizerState;
	D3D11_RASTERIZER_DESC mDesc;

public:
	void SetRasterizerDesc(D3D11_CULL_MODE CullMode, bool DepthClipEnable);

	bool CreateRasterizerState();

public:
	// 내 블랜드 스테이트로 설정해줘
	virtual void SetState() override;
	// 이전 스테이트로 복구해줘
	virtual void ResetState() override;

};

