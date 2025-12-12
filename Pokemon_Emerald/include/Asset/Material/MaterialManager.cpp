#include "MaterialManager.h"
#include "Material.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
	CMaterial::DestroySampler();
}

bool CMaterialManager::Init()
{
	CMaterial::SetSampler(ETextureSamplerType::Point);
	CMaterial::SetSampler(ETextureSamplerType::Linear);
	CMaterial::SetSampler(ETextureSamplerType::Anisotropic);

	//기본 머티리얼 제공
	CreateMaterial("DefaultMaterial");

	CMaterial* Mtrl = FindMaterial("DefaultMaterial");

	Mtrl->SetPixelShader("DefaultMaterialShader");
	Mtrl->SetSamplerType(ETextureSamplerType::Linear);

	return true;
}

bool CMaterialManager::CreateMaterial(const string& Name)
{
	// 생성해야하는데 같은 이름이 있으면 false
	if (FindMaterial(Name))
	{
		return false;
	}

	CMaterial* Material = new CMaterial;

	Material->SetName(Name);

	mMaterialMap.insert(make_pair(Name, Material));

	return true;
}

class CMaterial* CMaterialManager::FindMaterial(const string& Name)
{
	auto iter = mMaterialMap.find(Name);

	if (iter == mMaterialMap.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CMaterialManager::ReleaseMaterial(class CAsset* Material)
{
	auto iter = mMaterialMap.find(Material->GetName());

	if (iter != mMaterialMap.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMaterialMap.erase(iter);
		}
	}
}
