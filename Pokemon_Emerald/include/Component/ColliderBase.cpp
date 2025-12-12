#include "ColliderBase.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../ProfileManager.h"
#include "../Scene/CameraManager.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderManager.h"
#include "../Shader/ColliderCBuffer.h"
#include "../Shader/TransformCBuffer.h"
#include "../Asset/Mesh/MeshManager.h"
#include "../Asset/Mesh/Mesh.h"



CColliderBase::CColliderBase()
{

#ifdef _DEBUG
	mRenderType = EComponentRender::Render;
	mRenderLayerName = "Object";
#endif



}

CColliderBase::CColliderBase(const CColliderBase& Com)
	: CSceneComponent(Com)
{
}

CColliderBase::CColliderBase(CColliderBase&& Com)
	: CSceneComponent(Com)
{


}

CColliderBase::~CColliderBase()
{
	auto iter = mCollisionObjects.begin();
	auto iterEnd = mCollisionObjects.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->first->CallCollisionEnd(this);
	}



#ifdef _DEBUG
	SAFE_DELETE(mTransformCBuffer);
	SAFE_DELETE(mCBuffer);
#endif // _DEBUG
}

void CColliderBase::SetCollisionProfile(const string& Name)
{
	mProfile = CProfileManager::GetInst()->FindProfile(Name);
}

void CColliderBase::CallCollisionBegin(const FVector3D& HitPoint, CColliderBase* Dest)
{
	// 충돌이 시작된것이므로 충돌목록에 추가해준다. 
	AddCollisionObject(Dest);

	mCollision = true;

	if (mCollisionBeginFunc)
	{
		mCollisionBeginFunc(HitPoint, Dest);
	}
}

void CColliderBase::CallCollisionEnd(CColliderBase* Dest)
{
	// Dest와 충돌이 끝났으므로 목록에서 제거해준다. 
	EraseCollisionObject(Dest);


	// mCollisionObjects 가 empty라면 해당 충돌체는 누구와도 충돌하고 있지 않은것이다.
	if (mCollisionObjects.empty())
	{
		mCollision = false;
	}

	if (mCollisionEndFunc)
	{
		mCollisionEndFunc(Dest);
	}
}

void CColliderBase::AddCollisionObject(CColliderBase* Collider)
{
	// unordered_map은 랜덤액세스를 지원한다.
	// 해당 키값이 없을 경우 노드를 하나 새로 만들고 있을경우 해당 노드를 가져온다.
	// 랜덤액세스 : 임의 접근 
	mCollisionObjects[Collider] = true;
}

// 충돌 리스트에 해당 충돌체가 있는지 확인한다. 
bool CColliderBase::CheckCollisionObject(CColliderBase* Collider)
{
	auto iter =  mCollisionObjects.find(Collider);

	if (iter == mCollisionObjects.end())
	{
		return false;
	}

	return true;
}

void CColliderBase::EraseCollisionObject(CColliderBase* Collider)
{
	mCollisionObjects.erase(Collider);
}

bool CColliderBase::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	SetCollisionProfile("Default");

	mScene->GetCollision()->AddCollider(this);

#ifdef _DEBUG

	mShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

	//Transform 상수버퍼
	mTransformCBuffer = new CTransformCBuffer;
	mTransformCBuffer->Init();

	//Collider 상수버퍼 
	mCBuffer = new CColliderCBuffer;
	mCBuffer->Init();

#endif // _DEBUG



	return true;
}
bool CColliderBase::Init(const char* FileName)
{
	if (!CSceneComponent::Init(FileName))
	{
		return false;
	}

	SetCollisionProfile("Default");

	mScene->GetCollision()->AddCollider(this);

#ifdef _DEBUG

	mShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

	//Transform 상수버퍼
	mTransformCBuffer = new CTransformCBuffer;
	mTransformCBuffer->Init();

	//Collider 상수버퍼 
	mCBuffer = new CColliderCBuffer;
	mCBuffer->Init();

#endif // _DEBUG

	return true;
}
void CColliderBase::PreUpdate(float DeltaTime)
{
	CSceneComponent::PreUpdate(DeltaTime);
}

void CColliderBase::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CColliderBase::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CColliderBase::Collision(float DeltaTime)
{
	CSceneComponent::Collision(DeltaTime);
}
void CColliderBase::PreRender()
{
	CSceneComponent::PreRender();

}
void CColliderBase::Render()
{
	CSceneComponent::Render();

#ifdef _DEBUG

	//Tranform 상수버퍼 데이터 넣어주기 
	//크기, 위치
	FMatrix matScale, matRot ,matTranslate, matWorld;
	matScale.Scaling(mWorldScale);

	if (mEnableRotation)
	{
		matRot.Rotation(mWorldRot);
	}

	matTranslate.Translation(mWorldPos);

	//크자이공부 !!!!!
	matWorld = matScale * matRot * matTranslate;

	FMatrix matView, matProj;
	matView = mScene->GetCameraManager()->GetViewMatrix();
	matProj = mScene->GetCameraManager()->GetProjMatrix();

	mTransformCBuffer->SetWorldMatrix(matWorld);
	mTransformCBuffer->SetViewMatrix(matView);
	mTransformCBuffer->SetProjMatrix(matProj);

	mTransformCBuffer->UpdateBuffer();


	// 상수버퍼들 업데이트
	//Collider 상수버퍼 데이터 넣어주기
	//충돌 중 : 빨강 
	if (mCollision)
	{
		mCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
		
	}
	else
	{
		//충돌 X : 초록 
		mCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);
	}

	mCBuffer->UpdateBuffer();

	//쉐이더 셋팅 
	mShader->SetShader();

	//그려라 
	mMesh->Render();
#endif // _DEBUG


	//mCollision = false;
}
void CColliderBase::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderBase* CColliderBase::Clone()
{
	return nullptr;
}

void CColliderBase::EraseOwner()
{
	CSceneComponent::EraseOwner();

	if (mCollisionBeginFunc && mBeginObj == GetOwner())
	{
		mCollisionBeginFunc = nullptr;
	}

	if (mCollisionEndFunc && mEndObj == GetOwner())
	{
		mCollisionEndFunc = nullptr;
	}

}
