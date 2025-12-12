#include "TransformCBuffer.h"

CTransformCBuffer::CTransformCBuffer()
{
}

CTransformCBuffer::CTransformCBuffer(const CTransformCBuffer& Data)
	:CConstantBufferData(Data),
	mData(Data.mData)
{
}

CTransformCBuffer::CTransformCBuffer(CTransformCBuffer&& Data)
	:CConstantBufferData(Data),
	mData(Data.mData)
{
}

CTransformCBuffer::~CTransformCBuffer()
{
}

bool CTransformCBuffer::Init()
{
	SetConstantBuffer("Transform");
	
	return true;
}

void CTransformCBuffer::UpdateBuffer()
{
	mData.matWV = mData.matWorld * mData.matView;
	mData.matWVP = mData.matWorld * mData.matView * mData.matProj;

	//c++에서 Dx 보내줄때 전치 해서 보내줘야한다. 
	//C++에서 우리가 작성한 연산할때 행렬은 가로기준으로 작성했다.
	//쉐이더에서는 행렬을 읽을때 세로로 데이터를 읽는다.
	//그래서 쉐이더에서도 우리가 사용하기 쉽게 전치로 바꿔준다음에 보내준다. 
	mData.matWorld.Transpose();
	mData.matView.Transpose();
	mData.matProj.Transpose();
	mData.matWV.Transpose();
	mData.matWVP.Transpose();

	mBuffer->Update(&mData);
}

CTransformCBuffer* CTransformCBuffer::Clone()
{
	return new CTransformCBuffer(*this);
}
