#pragma once
#include "../Share/Object.h"

//크게 2가지로 쉐이더를 만들수 있다.
// 그래픽스 쉐이더 :: 화면 출력하기 위한 쉐이더  
// 컴퓨트 쉐이더 :: 그래픽 카드에 연산을 시키는 쉐이더 

class CShader abstract : public CObject
{
	friend class CShaderManager;

protected:
	CShader();
	virtual ~CShader();

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;
};

