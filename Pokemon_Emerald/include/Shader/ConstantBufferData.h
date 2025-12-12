#pragma once

#include "ConstantBuffer.h"

class CConstantBufferData abstract
{
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& Data);
	CConstantBufferData(CConstantBufferData&& Data);
	virtual ~CConstantBufferData();

protected:
	//버퍼는 공유해서 사용한다. 오브젝트별로 같은걸 만들 필요가 없다.
	// 왜냐면 캐릭터가 100명이라고 같은 용도의 메모리 공간을 100개 만들 필요가 없기 대문이다. 
	CSharedPtr<CConstantBuffer> mBuffer;	//공유 포인터로 버퍼를 들고있는 이유!

protected:
	void SetConstantBuffer(const string& Name);

public:
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;

	virtual CConstantBufferData* Clone() = 0;

};

