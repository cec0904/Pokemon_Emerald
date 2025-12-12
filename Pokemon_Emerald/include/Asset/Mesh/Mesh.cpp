#include "Mesh.h"

#include "../AssetManager.h"
#include "../../Device.h"

#include "../Material/Material.h"
#include "../Material/MaterialManager.h"

CMesh::CMesh()
{
	mAssetType = EAssetType::Mesh;
}

CMesh::~CMesh()
{
	size_t Size = mMeshSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mMeshSlot[i]);
	}

}

bool CMesh::CreateMesh(void* VertexData, int Size, int Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitiv, void* IndexData, int IndexSize, int IndexCount, DXGI_FORMAT Fmt, D3D11_USAGE IndexUsage)
{
	mVertexBuffer.Size = Size;
	mVertexBuffer.Count = Count;
	mVertexBuffer.Data = new char[Size * Count];
	memcpy(mVertexBuffer.Data, VertexData, Size * Count);

	//정점 버퍼 만들기 
	if (!CreateBuffer(&mVertexBuffer.Buffer, D3D11_BIND_VERTEX_BUFFER, VertexData, Size, Count, VertexUsage))
	{
		return false;
	}
	mPrimitive = Primitiv;

	//인덱스 버퍼 만들기 
	if (IndexData)
	{
		FMeshSlot* Slot = new FMeshSlot;
		Slot->IndexBuffer.Size = IndexSize;
		Slot->IndexBuffer.Count = IndexCount;
		Slot->IndexBuffer.Fmt = Fmt;
		Slot->IndexBuffer.Data = new char[IndexSize * IndexCount];
		memcpy(Slot->IndexBuffer.Data, IndexData, IndexSize * IndexCount);

		if (!CreateBuffer(&Slot->IndexBuffer.Buffer, D3D11_BIND_INDEX_BUFFER, IndexData, IndexSize, IndexCount, IndexUsage))
		{
			SAFE_DELETE(Slot);
			return false;
		}

		Slot->Material = CAssetManager::GetInst()->GetMaterialManager()->FindMaterial("DefaultMaterial");


		mMeshSlot.push_back(Slot);
	}

	return true;
}

bool CMesh::CreateBuffer(ID3D11Buffer** Buffer, D3D11_BIND_FLAG Flag, void* Data, int Size, int Count, D3D11_USAGE Usage)
{
	//버퍼 사용 설명서 만들어서 
	//설명서 대로 만들어주세요 
	// 버퍼 만드는 용도 
	D3D11_BUFFER_DESC BufferDesc = {};

	BufferDesc.ByteWidth = Size * Count;

	if (Usage == D3D11_USAGE_DYNAMIC)
	{
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (Usage == D3D11_USAGE_STAGING)
	{
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	//2025.10.20 버그 수정 . 누락 
	BufferDesc.BindFlags = Flag;
	BufferDesc.Usage = Usage;

	// 버퍼를 생성한다. 
	D3D11_SUBRESOURCE_DATA BufferData = {};
	BufferData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&BufferDesc, &BufferData, Buffer)))
	{
		return false;
	}
	return true;
}

void CMesh::Render()
{

	// 그려줄 도형 타입을 지정해준다. 
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(mPrimitive);

	//버텍스 버퍼 셋팅 해준다. 
	UINT stride = mVertexBuffer.Size;
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer.Buffer, &stride, &Offset);

	//인덱스 버퍼 유무 판단 
	size_t SlotSize = mMeshSlot.size();

	if (SlotSize > 0)
	{
		for (size_t i = 0; i < SlotSize; ++i)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(mMeshSlot[i]->IndexBuffer.Buffer, mMeshSlot[i]->IndexBuffer.Fmt, 0);

			//인덱스 참고하여 화면에 도형을 그린다. 
			// 인덱스 갯수, 인덱스 위치, 버텍스의 시작 위치 
			CDevice::GetInst()->GetContext()->DrawIndexed(mMeshSlot[i]->IndexBuffer.Count, 0, 0);
		}
	}
	else
	{
		// 인덱스 버퍼가 없으므로 그냥 그려줄것이다. 
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		//정점만 출력해라!
		CDevice::GetInst()->GetContext()->Draw(mVertexBuffer.Count, 0);
	}
}

void CMesh::Render(int SlotIndex)
{
	// 그려줄 도형 타입을 지정해준다. 
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(mPrimitive);

	//버텍스 버퍼 셋팅 해준다. 
	UINT stride = mVertexBuffer.Size;
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer.Buffer, &stride, &Offset);

	//인덱스 버퍼 유무 판단 
	size_t SlotSize = mMeshSlot.size();

	if (SlotSize > 0)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(mMeshSlot[SlotIndex]->IndexBuffer.Buffer, mMeshSlot[SlotIndex]->IndexBuffer.Fmt, 0);

		//인덱스 참고하여 화면에 도형을 그린다. 
		// 인덱스 갯수, 인덱스 위치, 버텍스의 시작 위치 
		CDevice::GetInst()->GetContext()->DrawIndexed(mMeshSlot[SlotIndex]->IndexBuffer.Count, 0, 0);

	}
	else
	{
		// 인덱스 버퍼가 없으므로 그냥 그려줄것이다. 
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		//정점만 출력해라!
		CDevice::GetInst()->GetContext()->Draw(mVertexBuffer.Count, 0);
	}

}
