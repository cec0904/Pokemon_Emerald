#include "ColliderCBuffer.h"


CColliderCBuffer::CColliderCBuffer()
{
}
CColliderCBuffer::CColliderCBuffer(const CColliderCBuffer& Data)
	:CConstantBufferData(Data),
	mData(Data.mData)
{
}
CColliderCBuffer::CColliderCBuffer(CColliderCBuffer&& Data)
	:CConstantBufferData(Data),
	mData(Data.mData)
{
}
CColliderCBuffer::~CColliderCBuffer()
{

}

bool CColliderCBuffer::Init()
{
	SetConstantBuffer("Collider");

	return true;
}
void CColliderCBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CColliderCBuffer* CColliderCBuffer::Clone()
{
	return new CColliderCBuffer(*this);
}