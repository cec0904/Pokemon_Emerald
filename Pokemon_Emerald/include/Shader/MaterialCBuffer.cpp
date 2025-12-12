#include "MaterialCBuffer.h"

CMaterialCBuffer::CMaterialCBuffer()
{
	mData.Opacity = 1.f;
	mData.BaseColor = FVector4D::White;
}

CMaterialCBuffer::CMaterialCBuffer(const CMaterialCBuffer& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CMaterialCBuffer::CMaterialCBuffer(CMaterialCBuffer&& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CMaterialCBuffer::~CMaterialCBuffer()
{
}

bool CMaterialCBuffer::Init()
{
	//
	SetConstantBuffer("Material");

	return true;
}

void CMaterialCBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CMaterialCBuffer* CMaterialCBuffer::Clone()
{
	return new CMaterialCBuffer(*this);
}
