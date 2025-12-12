#include "TileMapCBuffer.h"

CTileMapCBuffer::CTileMapCBuffer()
{
}

CTileMapCBuffer::CTileMapCBuffer(const CTileMapCBuffer& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CTileMapCBuffer::CTileMapCBuffer(CTileMapCBuffer&& Data)
	: CConstantBufferData(Data),
	mData(Data.mData)
{
}

CTileMapCBuffer::~CTileMapCBuffer()
{
}

bool CTileMapCBuffer::Init()
{
	SetConstantBuffer("TileMap");

	return true;
}

void CTileMapCBuffer::UpdateBuffer()
{
	mBuffer->Update(&mData);
}

CTileMapCBuffer* CTileMapCBuffer::Clone()
{
	return new CTileMapCBuffer(*this);
}
