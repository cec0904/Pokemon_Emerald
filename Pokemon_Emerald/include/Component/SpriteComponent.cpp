#include "SpriteComponent.h"

#include "../Share/Timer.h"
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

#include "../Animation/Animation2D.h"
#include "../Asset/Animation/Animation2DData.h"

CSpriteComponent::CSpriteComponent()
{
	mRenderType = EComponentRender::Render;
	mRenderLayerName = "Object";
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& Com)
	: CSceneComponent(Com)
{
}

CSpriteComponent::CSpriteComponent(CSpriteComponent&& Com)
	: CSceneComponent(Com)
{
}

CSpriteComponent::~CSpriteComponent()
{
	SAFE_DELETE(mAnimation);

	SAFE_DELETE(mSpriteCBuffer);
}

void CSpriteComponent::SetShader(const string& Name)
{
	mShader = CShaderManager::GetInst()->FindShader(Name);
}

void CSpriteComponent::SetShader(class CShader* Shader)
{
	mShader = Shader;
}

void CSpriteComponent::SetTexture(const string& Name, int TextureIndex)
{
	if (mScene)
	{
		//Scene->SceneAssetManager 통해서
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		//AssetManager 에서 TextureManager통해서 가져올수도 있다.
		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	mTextureIndex = TextureIndex;

}

void CSpriteComponent::SetTexture(const string& Name, const TCHAR* FileName, int TextureIndex)
{
	if (mScene)
	{
		//Scene->SceneAssetManager 통해서
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	mTextureIndex = TextureIndex;
}

void CSpriteComponent::SetTexture(class CTexture* Texture, int TextureIndex)
{
	mTexture = Texture;
	mTextureIndex = TextureIndex;
}

void CSpriteComponent::SetTextureIndex(int Index)
{
	mTextureIndex = Index;
}

void CSpriteComponent::SetTint(float r, float g, float b)
{
	mTint.x = r;
	mTint.y = g;
	mTint.z = b;
}

void CSpriteComponent::SetOpacity(float Opacity)
{
	mTint.w = Opacity;
}

void CSpriteComponent::SetFlip(bool Flip)
{
	mIsFlip = Flip;
}

const CAnimation2DData* CSpriteComponent::GetCurrentAnimationData()
{
	if (mAnimation)
	{
		return mAnimation->mCurrentSequence->GetAnimationAsset();
	}

	return nullptr;
}


bool CSpriteComponent::Init()
{
	CSceneComponent::Init();

	mSpriteCBuffer = new CSpriteCBuffer;
	mSpriteCBuffer->Init();

	SetShader("SpriteShader");

	//SpriteRect
	// Sprite이미지는 Mesh 고정해서 사용할것이다. 
	if (mScene)
	{
		mMesh= mScene->GetAssetManager()->FindMesh("SpriteRect");
	}
	else
	{
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");
	}


	return true;
}

bool CSpriteComponent::Init(const char* FileName)
{
	CSceneComponent::Init(FileName);

	mSpriteCBuffer = new CSpriteCBuffer;
	mSpriteCBuffer->Init();

	SetShader("SpriteShader");

	//SpriteRect
	// Sprite이미지는 Mesh 고정해서 사용할것이다. 
	if (mScene)
	{
		mMesh= mScene->GetAssetManager()->FindMesh("SpriteRect");
	}
	else
	{
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");
	}

	return true;
}

void CSpriteComponent::PreUpdate(float DeltaTime)
{
	CSceneComponent::PreUpdate(DeltaTime);
}

void CSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (mAnimation)
	{

		mAnimation->Update(DeltaTime);

	}

}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}

void CSpriteComponent::PreRender()
{
	CSceneComponent::PreRender();
}

void CSpriteComponent::Render()
{
	CSceneComponent::Render();

	//애니메이션 상수버퍼 셋팅
	if (mAnimation)
	{
		//애니메이션 셋팅
		mAnimation->SetShader();
	}
	else
	{
		CAnimation2D::DisableAnimation();
	}

	//반전여부 
	CAnimation2D::SetAnimFlip(mIsFlip);

	//스프라이트 상수버퍼 셋팅 해주기
	mSpriteCBuffer->SetTint(mTint);
	mSpriteCBuffer->SetUseTime(mUseTime);


	if (mUseTime)
	{
		static float TotalTime = 0.f;
		TotalTime += CTimer::GetDeltaTime() * 0.2f;
		mSpriteCBuffer->SetTime(TotalTime);
	}

	
	mSpriteCBuffer->UpdateBuffer();

	//Trnasform;
	mTransformCBuffer->SetWorldMatrix(mmatWorld);
	FMatrix matView, matProj;
	matView = mScene->GetCameraManager()->GetViewMatrix();
	matProj = mScene->GetCameraManager()->GetProjMatrix();

	mTransformCBuffer->SetViewMatrix(matView);
	mTransformCBuffer->SetProjMatrix(matProj);
	mTransformCBuffer->SetPivot(mPivot);
	//FMatrix matProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(90.f), 1280.f / 720.f, 0.5f, 1000.f);
	//mTransformCBuffer->SetProjMatrix(matProj);

	mTransformCBuffer->UpdateBuffer();

	mShader->SetShader();

	if (mTexture)
	{
		mTexture->SetShader(0, EShaderBufferType::Pixel, mTextureIndex);
	}

	mMesh->Render();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}
