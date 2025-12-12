#include "ConstantBuffer.h"
#include "../Device.h"

CConstantBuffer::CConstantBuffer()
{
}

CConstantBuffer::~CConstantBuffer()
{
	//20251024 추가.
	SAFE_RELEASE(mBuffer);
}

bool CConstantBuffer::Init(int Size, int Register, int ShaderBufferType)
{
	mSize = Size;
	mRegister = Register;
	mShaderBufferType = ShaderBufferType;

	//버퍼 만들기
	D3D11_BUFFER_DESC Desc = {};

	// 상수버퍼는 정보갱신이 많기 때문에 Dynamic으로 만들어준다. 
	// 예를들어서 플레이어 이동 할때마다 갱신이 일어나기 때문에...
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	//버퍼의 크기 
	Desc.ByteWidth = Size;
	//상수 버퍼로 사용하겠다고 바인딩 한다. 
	//D3D11_BIND_FLAG
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//D3D11_CPU_ACCESS_FLAG
	// CPU에서 상수버퍼에 값을 쓰겠다고 정의한다. 
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &mBuffer)))
	{
		return false;
	}


	return true;
}

//상수버퍼에 들어갈 데이터 정보가 들어올것이다. 
void CConstantBuffer::Update(void* Data)
{
	// Buffer 안에 있는 데이터를 저장하기위한 주소를 가져온다. 
	D3D11_MAPPED_SUBRESOURCE Map = {};
	//Map : CPU가 GPU를 사용하기 위해서 잠금처리하는것이다. 
	//D3D11_MAP_WRITE_DISCARD : 기존의 데이터를 무시하고 새로 쓴다. 
	CDevice::GetInst()->GetContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map);

	memcpy(Map.pData, Data, mSize);

	//Map을 사용했으면 반드시 Unmap을 해줘야한다. 
	CDevice::GetInst()->GetContext()->Unmap(mBuffer, 0);

	//위에서는 GPU 메모리공간에 우리가 계산한 데이터를 넣어준것이다. 
	//지정된 Shader에 상수버퍼의 데이터를 넘겨줘야한다. 
	//정점
	if (mShaderBufferType & EShaderBufferType::Vertex)
	{
		// 1인자 : 레지스터 번호 
		// 2인자 : 몇개의 버퍼가 셋팅되어있는지
		// 3인자 : 셋팅할 버퍼의 주소 
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
	//픽셀
	if (mShaderBufferType & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
	//HULL
	if (mShaderBufferType & EShaderBufferType::Hull)
	{

		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
	//Domain
	if (mShaderBufferType & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
	//Geometry
	if (mShaderBufferType & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	//Comput
	if (mShaderBufferType & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
}
