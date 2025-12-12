#pragma once

#include "../Gameinfo.h"

class CRenderStateManager
{
	friend class CRenderManager;

private:
	CRenderStateManager();
	~CRenderStateManager();

private:
	unordered_map<string, class CRenderState*> mRenderStateMap;

public:
	bool Init();

public:

	void SetBlendFactor(const string& Name, float r, float g, float b, float a);
	void SetSampleMarks(const string& Name, UINT Mask);
	//Src : 출력물체 대상
	//Dest : 백버퍼 
	void AddBlendDesc(const string& Name, bool Enable,
		D3D11_BLEND Src, D3D11_BLEND Dest, D3D11_BLEND_OP BlendOp,
		D3D11_BLEND SrcAlpha, D3D11_BLEND DestAlpha, D3D11_BLEND_OP AlphaOp,
		UINT8 RenderTargetWriteMask);

	bool CreateBlendState(const string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);

	//레스터라이즈
	void AddRasterizerDesc(const string& Name, D3D11_CULL_MODE CullBackEnable, bool DepthClipEnable);
	bool CreateRasterizerState(const string& Name);

	//뎁스 스탠실
	bool CreateDepthStencilState(const string& Name, bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		BOOL StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC BackFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS });

	class CRenderState* FindState(const string& Name);

};

