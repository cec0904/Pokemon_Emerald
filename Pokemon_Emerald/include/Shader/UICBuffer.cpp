#include "UICBuffer.h"
#include "../Share/Log.h"

CUICBuffer::CUICBuffer()
{
	mData.BrushTint = FVector4D::White;
	mData.BrushAnimEnable = 0;
}

CUICBuffer::CUICBuffer(const CUICBuffer& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CUICBuffer::CUICBuffer(CUICBuffer&& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CUICBuffer::~CUICBuffer()
{
}

bool CUICBuffer::Init()
{
	SetConstantBuffer("UI");

	return true;
}

void CUICBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CUICBuffer* CUICBuffer::Clone()
{
	return new CUICBuffer(*this);
}
