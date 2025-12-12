#include "TileMapRenderComponent.h"


#include "TileMapComponent.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderManager.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/Texture/TextureManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Shader/SpriteCBuffer.h"
#include "../Shader/TransformCBuffer.h"
#include "../Component/CameraComponent.h"
#include "../Scene/CameraManager.h"


CTileMapRenderComponent::CTileMapRenderComponent()
{
	mRenderType = EComponentRender::Render;
	mRenderLayerName = "BackGround";
}

CTileMapRenderComponent::CTileMapRenderComponent(const CTileMapRenderComponent& Com)
	:	CSceneComponent(Com)
{
}

CTileMapRenderComponent::CTileMapRenderComponent(CTileMapRenderComponent&& Com)
	: CSceneComponent(Com)
{
}

CTileMapRenderComponent::~CTileMapRenderComponent()
{
}

void CTileMapRenderComponent::SetTileMapComponent(CTileMapComponent* TileMap)
{
	mTileMap = TileMap;

	if (mTileMap)
	{
		FVector2D Scale;
		Scale.x = mTileMap->GetTileSize().x * mTileMap->GetTileCountX();
		Scale.y = mTileMap->GetTileSize().y * mTileMap->GetTileCountY();
		SetWorldScale(Scale);
	}
}

void CTileMapRenderComponent::SetShader(const string& Name)
{
	mShader = CShaderManager::GetInst()->FindShader(Name);
}

void CTileMapRenderComponent::SetShader(class CShader* Shader)
{
	mShader = Shader;
}

void CTileMapRenderComponent::SetBackTexture(const string& Name)
{
	if (mScene)
	{
		mBackTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		mBackTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

}

void CTileMapRenderComponent::SetBackTexture(const string& Name, const TCHAR* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mBackTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mBackTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CTileMapRenderComponent::SetBackTexture(class CTexture* Texture)
{
	mBackTexture = Texture;
}

void CTileMapRenderComponent::SetTileTexture(const string& Name)
{
	if (mScene)
	{
		mTileTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		mTileTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	if (mTileMap && mTileTexture)
	{
		mTileMap->SetTileTextureSize(mTileTexture->GetTexture(0)->Width, mTileTexture->GetTexture(0)->Height);
	}


}

void CTileMapRenderComponent::SetTileTexture(const string& Name, const TCHAR* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mTileTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mTileTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	if (mTileMap && mTileTexture)
	{
		mTileMap->SetTileTextureSize(mTileTexture->GetTexture(0)->Width, mTileTexture->GetTexture(0)->Height);
	}
}

void CTileMapRenderComponent::SetTileTexture(class CTexture* Texture)
{
	mTileTexture = Texture;

	if (mTileMap && mTileTexture)
	{
		mTileMap->SetTileTextureSize(mTileTexture->GetTexture(0)->Width, mTileTexture->GetTexture(0)->Height);
	}
}

bool CTileMapRenderComponent::Init()
{
	CSceneComponent::Init();

	mTileMap = mOwnerObject->FindNonSceneComponent<CTileMapComponent>();

	if (mTileMap)
	{
		FVector2D Scale;
		Scale.x = mTileMap->GetTileSize().x * mTileMap->GetTileCountX();
		Scale.y = mTileMap->GetTileSize().y * mTileMap->GetTileCountY();
		SetWorldScale(Scale);
	}


	SetShader("TileMapShader");

	if (mScene)
	{
		mMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");
	}
	else
	{
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");
	}

	return true;
}

bool CTileMapRenderComponent::Init(const char* FileName)
{
	CSceneComponent::Init(FileName);

	mTileMap = mOwnerObject->FindNonSceneComponent<CTileMapComponent>();

	if (mTileMap)
	{
		FVector2D Scale;
		Scale.x = mTileMap->GetTileSize().x * mTileMap->GetTileCountX();
		Scale.y = mTileMap->GetTileSize().y * mTileMap->GetTileCountY();
		SetWorldScale(Scale);
	}


	SetShader("TileMapShader");

	if (mScene)
	{
		mMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");
	}
	else
	{
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");
	}

	return true;
}

void CTileMapRenderComponent::PreUpdate(float DeltaTime)
{
	CSceneComponent::PreUpdate(DeltaTime);
}

void CTileMapRenderComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileMapRenderComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CTileMapRenderComponent::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CTileMapRenderComponent::PreRender()
{
	CSceneComponent::PreRender();
}

void CTileMapRenderComponent::Render()
{
	CSceneComponent::Render();

	//배경이미지가 있는가? 
	if (mBackTexture)
	{
		//Trnasform;
		mTransformCBuffer->SetWorldMatrix(mmatWorld);
		FMatrix matView, matProj;
		matView = mScene->GetCameraManager()->GetViewMatrix();
		matProj = mScene->GetCameraManager()->GetProjMatrix();

		mTransformCBuffer->SetViewMatrix(matView);
		mTransformCBuffer->SetProjMatrix(matProj);
		mTransformCBuffer->SetPivot(mPivot);

		mTransformCBuffer->UpdateBuffer();

		mShader->SetShader();

		if (mBackTexture)
		{
			mBackTexture->SetShader(0, EShaderBufferType::Pixel, 0);
		}

		mMesh->Render();
	}


	// 그려줄 타일이 있는가? 
	if (mTileMap)
	{
		//그려줄 타일 텍스쳐가 있는가 
		if (mTileTexture)
		{
			mTileTexture->SetShader(0, EShaderBufferType::Pixel, 0);
			mTileMap->RenderTile();
		}

		mTileMap->RenderTileOutLine();
	}


}

void CTileMapRenderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileMapRenderComponent* CTileMapRenderComponent::Clone()
{
	return new CTileMapRenderComponent(*this);
}
