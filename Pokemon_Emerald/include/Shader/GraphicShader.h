#pragma once
#include "Shader.h"

class CGraphicShader abstract:
    public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	virtual ~CGraphicShader();


private:
	//쉐이더에는 
	//입력 레이아웃 
	// 입력 레이아웃은 정점 구조체가 어떻게 구성되어있는지를 정의한다. 
	// gameinfo 의 FVertexColor 와 fx의 VS_Input_Color 매칭해주는 것이다. 

	//레지스터 번호 이름을 지정하는 구조체가있다.
	// 해당 구조체 하나당 하나의 레지스터이다. 
	ID3D11InputLayout* mInputLayout = nullptr;

	//레지스터 지정을 위한 구조체
	//동적 배열로 만드는 이유는 입력 레지스터가 몇개가 될지 모르기 때문이다. 
	vector<D3D11_INPUT_ELEMENT_DESC> mvecDesc;

	unsigned int mInputLayerSize = 0;


	//사용할 쉐이더
	ID3D11VertexShader* mVS = nullptr;	// 쉐이더 객체
	//쉐이더를 컴파일해서 들고있을 버퍼가 필요하다.
	ID3DBlob* mVSBlob = nullptr;			// 컴파일된 쉐이더를 담고 있는 버퍼

	// PixelShader 객체.
	ID3D11PixelShader* mPS = nullptr;

	// PixelShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
	ID3DBlob* mPSBlob = nullptr;

	// HullShader 객체.
	ID3D11HullShader* mHS = nullptr;

	// HullShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
	ID3DBlob* mHSBlob = nullptr;

	// DomainShader 객체.
	ID3D11DomainShader* mDS = nullptr;

	// DomainShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
	ID3DBlob* mDSBlob = nullptr;

	// GeometryShader 객체.
	ID3D11GeometryShader* mGS = nullptr;

	// GeometryShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
	ID3DBlob* mGSBlob = nullptr;



public:
	virtual bool Init();

	//final : 이 함수는 더이상 오버라이드 할 수 없다.
	virtual void SetShader() final;

public:
	void AddInputLayoutDesc(const char* Semantic, unsigned int SemanticIndex, DXGI_FORMAT Fmt, unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InPutSlotClass, unsigned int InstanceDataStepRate);
	bool CreateInputLayout();

	/*
	EntryName	: 쉐이더 함수명 
	FileName	: Bin/Shader/폴더에 있는 파일명을 넣을것이다. 
	*/
	bool LoadVertexShader(const char* EntryName, const TCHAR* FileName);
	bool LoadPixelShader(const char* EntryName, const TCHAR* FileName);
	bool LoadHullShader(const char* EntryName, const TCHAR* FileName);
	bool LoadDomainShader(const char* EntryName, const TCHAR* FileName);
	bool LoadGeometryShader(const char* EntryName, const TCHAR* FileName);
};

