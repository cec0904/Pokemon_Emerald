#include "StaticMeshComponent.h"

#include"../Asset/Mesh/StaticMesh.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Material/Material.h"
#include "../Asset/Material/MaterialManager.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/Texture/Texture.h"


CStaticMeshComponent::CStaticMeshComponent()
	: CMeshComponent()
{
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& Com)
	: CMeshComponent(Com)
{
}

CStaticMeshComponent::CStaticMeshComponent(CStaticMeshComponent&& Com)
	: CMeshComponent(Com)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::SetShader(const string& Name)
{
	mShader = CShaderManager::GetInst()->FindShader(Name);
}

void CStaticMeshComponent::SetShader(CShader* Shader)
{
	mShader = Shader;
}

void CStaticMeshComponent::SetMesh(const string& Name)
{
	//mMesh = (CStaticMesh*)CAssetManager::GetInst()->GetMeshManager()->FindMesh(Name);

	mMesh = (CStaticMesh*)mScene->GetAssetManager()->FindMesh(Name);

	mMaterialSlots.clear();
	// 메쉬가 가지고 있는 슬롯을 얻어와 채워준다
	if (mMesh)
	{
		int SlotCount = mMesh->GetSlotCount();

		for (int i = 0 ; i < SlotCount; ++i)
		{
			const FMeshSlot*Slot = mMesh->GetSlot(i);
			mMaterialSlots.emplace_back(Slot->Material);
		}
	}

}

void CStaticMeshComponent::SetMesh(CMesh* Mesh)
{
	mMesh = (CStaticMesh*)Mesh;

	mMaterialSlots.clear();
	// 메쉬가 가지고 있는 슬롯을 얻어와 채워준다
	if (mMesh)
	{
		int SlotCount = mMesh->GetSlotCount();

		for (int i = 0; i < SlotCount; ++i)
		{
			const FMeshSlot* Slot = mMesh->GetSlot(i);
			mMaterialSlots.emplace_back(Slot->Material);
		}
	}

}

//머티리얼 변경
void CStaticMeshComponent::SetMaterial(int SlotIndex, const string Name)
{
	CMaterial* Material = nullptr;

	if (mScene)
	{
		Material = mScene->GetAssetManager()->FindMaterial(Name);
	}
	else
	{
		Material = CAssetManager::GetInst()->GetMaterialManager()->FindMaterial(Name);
	}

	if (Material)
	{
		Material = Material->Clone();
	}

	mMaterialSlots[SlotIndex] = Material;
}

void CStaticMeshComponent::SetMaterial(int SlotIndex, class CMaterial* Material)
{
	mMaterialSlots[SlotIndex] = Material;
}

void CStaticMeshComponent::AddTexture(int SlotIndex, class CTexture* Texture, int Register, int ShaderBufferType,
	int TextureIndex)
{
	mMaterialSlots[SlotIndex]->AddTexture(Texture, Register, ShaderBufferType, TextureIndex);
}

void CStaticMeshComponent::AddTexture(int SlotIndex, const string& Name, int Register, int ShaderBufferType,
	int TextureIndex)
{
	mMaterialSlots[SlotIndex]->AddTexture(Name, Register, ShaderBufferType, TextureIndex);
}

void CStaticMeshComponent::AddTexture(int SlotIndex, const string& Name, const TCHAR* FileName, int Register,
	int ShaderBufferType, int TextureIndex)
{
	mMaterialSlots[SlotIndex]->AddTexture(Name, FileName, Register, ShaderBufferType, TextureIndex);
}

void CStaticMeshComponent::SetBaseColor(int SlotIndex, float r, float g, float b, float a)
{
	mMaterialSlots[SlotIndex]->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::SetOpacity(int SlotIndex, float Opacity)
{
	mMaterialSlots[SlotIndex]->SetOpacity(Opacity);
}

bool CStaticMeshComponent::Init()
{	 
	CMeshComponent::Init();

	//쉐이더 설정해주기
	SetShader("StaticMeshShader");

	return true;
}	 
bool CStaticMeshComponent::Init(const char* FileName)
{	 
	CMeshComponent::Init(FileName);

	//쉐이더 설정해주기
	SetShader("StaticMeshShader");

	return true;
}	 
void CStaticMeshComponent::PreUpdate(float DeltaTime)
{	 
	CMeshComponent::PreUpdate(DeltaTime);
}	 
void CStaticMeshComponent::Update(float DeltaTime)
{	 
	CMeshComponent::Update(DeltaTime);
}	 
void CStaticMeshComponent::PostUpdate(float DeltaTime)
{	 
	CMeshComponent::PostUpdate(DeltaTime);
}	 
void CStaticMeshComponent::Collision(float DeltaTime)
{	 
	CMeshComponent::Collision(DeltaTime);
}	 
void CStaticMeshComponent::PreRender()
{	 
	CMeshComponent::PreRender();
}	 
void CStaticMeshComponent::Render()
{

	CMeshComponent::Render();

	mShader->SetShader();

	//MaterialSlots

	int SlotCount = mMesh->GetSlotCount();

	for (int i = 0; i < SlotCount; ++i )
	{
		if (mMaterialSlots[i])
		{
			mMaterialSlots[i]->SetMaterial();
		}
		

		mMesh->Render(i);

		if (mMaterialSlots[i])
		{
			mMaterialSlots[i]->ResetMaterial();
		}
		
	}


	mMesh->Render();

}	 
void CStaticMeshComponent::PostRender()
{
	CMeshComponent::PostRender();
}
CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}
