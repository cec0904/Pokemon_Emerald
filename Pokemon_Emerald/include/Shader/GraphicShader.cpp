#include "GraphicShader.h"
#include "../Device.h"

CGraphicShader::CGraphicShader()
{
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(mInputLayout);

	SAFE_RELEASE(mVS);
	SAFE_RELEASE(mVSBlob);

	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mPSBlob);

	SAFE_RELEASE(mHS);
	SAFE_RELEASE(mHSBlob);

	SAFE_RELEASE(mDS);
	SAFE_RELEASE(mDSBlob);

	SAFE_RELEASE(mGS);
	SAFE_RELEASE(mGSBlob);
	
}

bool CGraphicShader::Init()
{

	return true;
}

void CGraphicShader::SetShader()
{
	//내가 출력에 사용하고자 하는 쉐이더를 지정할려고 만들어 뒀다. 
	//나는 이렇게 쉐이더를 사용할거다 지정
	CDevice::GetInst()->GetContext()->VSSetShader(mVS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(mHS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(mDS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(mGS, nullptr, 0);

	//정점은 이렇게 생겼다고 
	//사용할 입력 레이아웃 지정해줘야한다 
	CDevice::GetInst()->GetContext()->IASetInputLayout(mInputLayout);

}

void CGraphicShader::AddInputLayoutDesc(const char* Semantic, unsigned int SemanticIndex, DXGI_FORMAT Fmt, unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InPutSlotClass, unsigned int InstanceDataStepRate)
{
	//초기화
	D3D11_INPUT_ELEMENT_DESC Desc = {};

	// 시멘틱 이름
	Desc.SemanticName = Semantic;
	// 시멘틱 인덱스
	Desc.SemanticIndex = SemanticIndex;
	
	Desc.Format = Fmt;
	
	//인스턴싱을 사용하게 된다면  
	Desc.InputSlot = InputSlot;
	Desc.InputSlotClass = InPutSlotClass;
	Desc.InstanceDataStepRate = InstanceDataStepRate;

	Desc.AlignedByteOffset = mInputLayerSize;
	mInputLayerSize += Size;

	mvecDesc.emplace_back(Desc);
}

// 입력 레이아웃 생성
bool CGraphicShader::CreateInputLayout()
{
	// pShaderBytecodeWithInputSignature ??
	// dx11 에서 제공하는 컴파일러로 컴파일 해야한다. 
	// 컴파일하는 함수 만들어 줘야한다. 
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(&mvecDesc[0], (UINT)mvecDesc.size(), mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadVertexShader(const char* EntryName, const TCHAR* FileName)
{
	//최종 경로가 나온다.
	TCHAR FullPath[MAX_PATH] = {};
	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);	
	
	//컴파일 했는데 에러가나면 여기에 들어올것이다. 
	ID3DBlob* ErrorBlob = nullptr;

	unsigned int Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "vs_5_0", Flag, 0, &mVSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	//컴파일이 성공했다. 
	//성공했다면 쉐이더 객체 만들어야한다. 
	// mVSBlob->GetBufferPointer() : 컴파일 된 코드 
	// mVSBlob->GetBufferSize : 컴파일 된 코드의 크기 
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mVS)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* EntryName, const TCHAR* FileName)
{
	//최종 경로가 나온다.
	TCHAR FullPath[MAX_PATH] = {};
	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);

	//컴파일 했는데 에러가나면 여기에 들어올것이다. 
	ID3DBlob* ErrorBlob = nullptr;

	unsigned int Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	if (FAILED(D3DCompileFromFile(FullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "ps_5_0", Flag, 0, &mPSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	//컴파일이 성공했다. 
	//성공했다면 쉐이더 객체 만들어야한다. 
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mPS)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadHullShader(const char* EntryName, const TCHAR* FileName)
{
	TCHAR   FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);

	unsigned int    Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	ID3DBlob* ErrorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"hs_5_0", Flag, 0, &mHSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char    ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// mVSBlob->GetBufferPointer() : 컴파일된 코드
	// mVSBlob->GetBufferSize() : 컴파일된 코드의 크기
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(
		mHSBlob->GetBufferPointer(), mHSBlob->GetBufferSize(),
		nullptr, &mHS)))
		return false;

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* EntryName, const TCHAR* FileName)
{
	TCHAR   FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);

	unsigned int    Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	ID3DBlob* ErrorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"ds_5_0", Flag, 0, &mDSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char    ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// mVSBlob->GetBufferPointer() : 컴파일된 코드
	// mVSBlob->GetBufferSize() : 컴파일된 코드의 크기
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(
		mDSBlob->GetBufferPointer(), mDSBlob->GetBufferSize(),
		nullptr, &mDS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* EntryName, const TCHAR* FileName)
{
	TCHAR   FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);

	unsigned int    Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG


	ID3DBlob* ErrorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"gs_5_0", Flag, 0, &mGSBlob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char    ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// mVSBlob->GetBufferPointer() : 컴파일된 코드
	// mVSBlob->GetBufferSize() : 컴파일된 코드의 크기
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(
		mGSBlob->GetBufferPointer(), mGSBlob->GetBufferSize(),
		nullptr, &mGS)))
		return false;

	return true;
}
