#include "SpriteCBuffer.h"

CSpriteCBuffer::CSpriteCBuffer()
{
	mData.Tint = FVector4D::White;
}

CSpriteCBuffer::CSpriteCBuffer(const CSpriteCBuffer& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CSpriteCBuffer::CSpriteCBuffer(CSpriteCBuffer&& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CSpriteCBuffer::~CSpriteCBuffer()
{
}

bool CSpriteCBuffer::Init()
{
	//
	SetConstantBuffer("Sprite");

	return true;
}

void CSpriteCBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CSpriteCBuffer* CSpriteCBuffer::Clone()
{
	return new CSpriteCBuffer(*this);
}
