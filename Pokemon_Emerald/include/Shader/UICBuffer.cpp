#include "UICBuffer.h"
#include "../Share/Log.h"

CUICBuffer::CUICBuffer()
{
	mData.BrushTint = FVector4D::White;
	mData.BrushAnimEnable = 0;

	mData.UseColorKey = 0;
	mData.KeyThreshold = 0.10f;
	mData.ColorKey = FVector3D(165.f / 255.f, 235.f / 255.f, 255.f / 255.f);
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
