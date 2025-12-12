#pragma once

#include "../Gameinfo.h"

// 랜더 스테이트란?
// 백 버퍼에 최종적으로 "어떻게" 그려질지 결정하는 설정이다.

// 스테이트는 크게 3가지가 있다.
// 레스터라이즈 스테이트
// 뎁스스텐실 스테이트
// 블렌드 스테이트
//ID3D11RasterizerState;
//ID3D11DepthStencilState;
//ID3D11BlendState;


class CRenderState
{
	friend class CRenderStateManager;

protected:
	// 내가 사용할 블렌드 스테이트 설정값
	// mState는 내가 셋팅할 스테이트
	// mPrevState는 내가 사용하기 이전에 셋팅되어있던 스테이트
	// 사용하고나 PrevState 셋팅으로 다시 되돌려 줄것이다. 
	ID3D11DeviceChild* mState = nullptr;
	ID3D11DeviceChild* mPrevState = nullptr;
	ERenderStateType::Type mStateType;

public:
	// 내 블랜드 스테이트로 설정해줘
	virtual void SetState() = 0;
	// 이전 스테이트로 복구해줘
	virtual void ResetState() = 0;

protected:
	CRenderState();
	virtual ~CRenderState();

};

