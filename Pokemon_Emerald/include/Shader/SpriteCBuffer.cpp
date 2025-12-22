#include "SpriteCBuffer.h"

CSpriteCBuffer::CSpriteCBuffer()
{
	mData.Tint = FVector4D::White;

	// 추가: 기본값 (컬러키 기본 OFF)
	mData.UseColorKey = 0;
	mData.KeyThreshold = 0.08f; // 기본값(원하면 조절)
	mData.ColorKey = FVector3D(0.f, 0.f, 0.f);
	mData.Pad = FVector3D(0.f, 0.f, 0.f);
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
