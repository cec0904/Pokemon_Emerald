#include "RenderStateManager.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
	auto iter = mRenderStateMap.begin();
	auto iterEnd = mRenderStateMap.end();

	for (;iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

}

bool CRenderStateManager::Init()
{
	AddBlendDesc("AlphaBlend", true, D3D11_BLEND_SRC_ALPHA, 
		D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);

	CreateBlendState("AlphaBlend", true, false);


	AddRasterizerDesc("Rasterizer", D3D11_CULL_NONE, true);
	CreateRasterizerState("Rasterizer");

	CreateDepthStencilState("DepthDisable", false);

	return true;
}

void CRenderStateManager::SetBlendFactor(const string& Name, float r, float g, float b, float a)
{
	CBlendState* State = (CBlendState*)FindState(Name);

	if (!State)
	{
		State = new CBlendState;

		mRenderStateMap.insert(make_pair(Name, State));
	}

	State->SetBlendFactor(r, g, b, a);

}

void CRenderStateManager::SetSampleMarks(const string& Name, UINT Mask)
{
	CBlendState* State = (CBlendState*)FindState(Name);

	if (!State)
	{
		State = new CBlendState;

		mRenderStateMap.insert(make_pair(Name, State));
	}

	State->SetSampleMarks(Mask);
}

void CRenderStateManager::AddBlendDesc(const string& Name, bool Enable, D3D11_BLEND Src, D3D11_BLEND Dest,
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcAlpha, D3D11_BLEND DestAlpha, D3D11_BLEND_OP AlphaOp,
	UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindState(Name);

	if (!State)
	{
		State = new CBlendState;

		mRenderStateMap.insert(make_pair(Name, State));
	}

	State->AddBlendDesc(Enable,
		Src, Dest, BlendOp,
		SrcAlpha, DestAlpha, AlphaOp,
		RenderTargetWriteMask);
}

bool CRenderStateManager::CreateBlendState(const string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindState(Name);

	if (!State)
	{
		return false;
	}

	if (FAILED(State->CreateState(AlphaToCoverageEnable, IndependentBlendEnable)))
	{
		SAFE_DELETE(State);
		return false;
	}

	return true;
}

void CRenderStateManager::AddRasterizerDesc(const string& Name, D3D11_CULL_MODE CullBackEnable,
	bool DepthClipEnable)
{
	CRasterizerState* State = (CRasterizerState*)FindState(Name);

	if (!State)
	{
		State = new CRasterizerState;

		mRenderStateMap.insert(make_pair(Name, State));
	}

	State->SetRasterizerDesc(CullBackEnable, DepthClipEnable);
}

bool CRenderStateManager::CreateRasterizerState(const string& Name)
{
	CRasterizerState* State = (CRasterizerState*)FindState(Name);

	if (!State)
	{
		return false;
	}

	if (State->mState)
	{
		return false;
	}

	if (!State->CreateRasterizerState())
	{
		return false;
	}

	return true;

}

bool CRenderStateManager::CreateDepthStencilState(const string& Name, bool DepthEnable,
	D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, BOOL StencilEnable, UINT8 StencilReadMask,
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	 CDepthStencilState* State =  (CDepthStencilState*)FindState(Name);

	 if (State)
	 {
		 return true;
	 }

	 State = new CDepthStencilState;

	 if (!State->CreateState(DepthEnable, DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask, FrontFace, BackFace))
	 {
		 SAFE_DELETE(State);
		 return false;
	 }

	 mRenderStateMap.insert(make_pair(Name, State));

	 return true;
}

class CRenderState* CRenderStateManager::FindState(const string& Name)
{
	auto iter = mRenderStateMap.find(Name);

	if (iter == mRenderStateMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

