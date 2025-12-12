#include "Material.h"

#include "../../Scene/Scene.h"
#include "../Texture/Texture.h"
#include "../../Shader/ShaderManager.h"
#include "../AssetManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Shader/MaterialCBuffer.h"
#include "../Texture/TextureManager.h"
#include "../../Device.h"

ID3D11SamplerState* CMaterial::mSampler[ETextureSamplerType::End];

CMaterial::CMaterial()
{
	mCBuffer = new CMaterialCBuffer;
	mCBuffer->Init();
}

CMaterial::CMaterial(const CMaterial& Material)
{
	mCBuffer = new CMaterialCBuffer;
	mCBuffer->Init();

	mTextureList.clear();

	mPS = Material.mPS;
	mBaseColor = Material.mBaseColor;
	mOpacity = Material.mOpacity;
	mSamplerType = Material.mSamplerType;

	mCBuffer->SetBaseColor(mBaseColor);
	mCBuffer->SetOpacity(mOpacity);

	size_t Size = Material.mTextureList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		FMaterialTextureInfo* Info = new FMaterialTextureInfo;
		*Info = *Material.mTextureList[i];
		mTextureList.emplace_back(Info);
	}

	if (!mTextureList.empty())
	{
		//텍스쳐가 사용하는 상수버퍼가 있으면 여기에 작업해준다. 
		mCBuffer->SetTextureSize(
			mTextureList[0]->Texture->GetTexture(0)->Width,
			mTextureList[0]->Texture->GetTexture(0)->Height);
	}

	mRefCount = 0;
}

CMaterial::~CMaterial()
{
	SAFE_DELETE(mCBuffer);

	size_t Size = mTextureList.size();

	for (size_t i = 0; i< Size; ++i)
	{
		SAFE_DELETE(mTextureList[i]);
	}
}

void CMaterial::SetSampler(ETextureSamplerType::Type Type)
{
	D3D11_SAMPLER_DESC Desc = {};

	//MIN : 축소
	//MAG : 확대
	//MIP : MipMap 사용하겠다. 
	switch (Type)
	{
	case ETextureSamplerType::Point:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case ETextureSamplerType::Linear:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case ETextureSamplerType::Anisotropic:
		Desc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}
	/*
	WRAP	: UV 0 ~ 1을 벗어나면 다시 0~ 1 사이로 반복한다. 
	
	: UV 0 ~ 1을 벗어나면 거울에 비친것처럼 반전되어 반복한다. 
	CLAMP	:  UV 0 ~ 1을 벗어나면 가장자리 픽셀 색상으로 고정된다. 
	*/
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//LOD 사용시 필요하다.
	//우리는 사용안할것이기에 필요없다. 
	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	// 샘플링 비교 함수
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &mSampler[Type])))
	{
		return;
	}
}

void CMaterial::DestroySampler()
{
	for (int i = 0; i < ETextureSamplerType::End; ++i)
	{
		SAFE_RELEASE(mSampler[i]);
	}
}

//텍스쳐 
void CMaterial::AddTexture(class CTexture* Texture, int Register, int ShaderBufferType, int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	TexInfo->Texture = Texture;
	TexInfo->Name = Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	//텍스쳐가 사용하는 상수버퍼가 있으면 여기에 작업해준다. 
	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);

	mTextureList.emplace_back(TexInfo);
}

void CMaterial::AddTexture(const string& Name, int Register, int ShaderBufferType, int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	//Texture 정보를 찾아온다.
	//머티리얼 꼭 Scene이 있을때 생성된다고 보장이 없다.
	if (mScene)
	{
		//Scene->SceneAssetManager 통해서
		TexInfo->Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		//AssetManager 에서 TextureManager통해서 가져올수도 있다.
		TexInfo->Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	TexInfo->Name = TexInfo->Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	//텍스쳐가 사용하는 상수버퍼가 있으면 여기에 작업해준다. 
	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);


	mTextureList.emplace_back(TexInfo);

}

void CMaterial::AddTexture(const string& Name, const TCHAR* FileName, int Register, int ShaderBufferType,
	int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	//Texture 정보를 찾아온다.
	//머티리얼 꼭 Scene이 있을때 생성된다고 보장이 없다.
	if (mScene)
	{
		//Scene->SceneAssetManager 통해서
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			SAFE_DELETE(TexInfo);
			return;
		}

		TexInfo->Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			SAFE_DELETE(TexInfo);
			return;
		}
		//AssetManager 에서 TextureManager통해서 가져올수도 있다.
		TexInfo->Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}


	TexInfo->Name = TexInfo->Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	//텍스쳐가 사용하는 상수버퍼가 있으면 여기에 작업해준다. 
	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);

	mTextureList.emplace_back(TexInfo);
}

//픽셀 쉐이더 
void CMaterial::SetPixelShader(const string& Name)
{
	const FMaterialPixelShader* mps = CShaderManager::GetInst()->FindPixelShader(Name);

	if (!mps)
	{
		return;
	}

	mPS = mps->PS;
}

void CMaterial::SetPixelShader(const string& Name, const char* EntryName, const TCHAR* FileName)
{

	if (!CShaderManager::GetInst()->LoadPixelShader(Name, EntryName, FileName))
	{
		return;
	}

	const FMaterialPixelShader* mps = CShaderManager::GetInst()->FindPixelShader(Name);

	if (!mps)
	{
		return;
	}

	mPS = mps->PS;
}

void CMaterial::ClearShader()
{
	mPS = nullptr;
}

void CMaterial::SetBaseColor(const FVector4D& Color)
{
	mBaseColor = Color;
	mCBuffer->SetBaseColor(Color);
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	mBaseColor = FVector4D(r, g, b, a);
	mCBuffer->SetBaseColor(mBaseColor);
}

void CMaterial::SetOpacity(float Opacity)
{
	mOpacity = Opacity;
	mCBuffer->SetOpacity(Opacity);
}

void CMaterial::SetSamplerType(ETextureSamplerType::Type SamplerType)
{
	mSamplerType = SamplerType;
}

//그리기전에 해당 머티리얼 셋팅으로 데이터를 넣어준다. 
void CMaterial::SetMaterial()
{
	//상수버퍼 셋팅
	mCBuffer->UpdateBuffer();

	//샘플러 셋팅
	CDevice::GetInst()->GetContext()->PSSetSamplers(0, 1, &mSampler[mSamplerType]);

	//픽셀쉐이더 셋팅
	if (mPS)
	{
		CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);
	}

	size_t TextCount = mTextureList.size();

	for (size_t i = 0; i < TextCount; ++i)
	{
		mTextureList[i]->Texture->SetShader(mTextureList[i]->Register,
			mTextureList[i]->ShaderBufferType,
			mTextureList[i]->TextureIndex);
	}
}

void CMaterial::ResetMaterial()
{
	size_t TextCount = mTextureList.size();

	for (size_t i = 0; i < TextCount; ++i)
	{
		mTextureList[i]->Texture->ResetShader(mTextureList[i]->Register, mTextureList[i]->ShaderBufferType);
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}
