#pragma once
#include "Shader.h"
class CComputeShader :
    public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader();

public:
	virtual bool Init() override;

	//final : 이 함수는 더이상 오버라이드 할 수 없다.
	virtual void SetShader() final;
};

