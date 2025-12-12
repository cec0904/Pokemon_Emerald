#include "Animation2DCBuffer.h"
#include "../Share/Log.h"

CAnimation2DCBuffer::CAnimation2DCBuffer()
{
	mData.AnimEnable = 0;
	mData.AnimFlip = 0;
}

CAnimation2DCBuffer::CAnimation2DCBuffer(const CAnimation2DCBuffer& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CAnimation2DCBuffer::CAnimation2DCBuffer(CAnimation2DCBuffer&& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CAnimation2DCBuffer::~CAnimation2DCBuffer()
{
}

bool CAnimation2DCBuffer::Init()
{
	SetConstantBuffer("Animation2D");

	return true;
}

void CAnimation2DCBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CAnimation2DCBuffer* CAnimation2DCBuffer::Clone()
{
	return new CAnimation2DCBuffer(*this);
}
