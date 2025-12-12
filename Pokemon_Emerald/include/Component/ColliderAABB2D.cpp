#include "ColliderAABB2D.h"

#include "ColliderLine2D.h"
#include "ColliderOBB2D.h"
#include "../Collision.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "ColliderSphere2D.h"
#include "ColliderLine2D.h"

CColliderAABB2D::CColliderAABB2D()
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::AABB2D;
	mBoxSize.x = 100.f;
	mBoxSize.y = 100.f;
}

CColliderAABB2D::CColliderAABB2D(const CColliderAABB2D& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::AABB2D;

	mBoxSize = Com.mBoxSize;
}

CColliderAABB2D::CColliderAABB2D(CColliderAABB2D&& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::AABB2D;

	mBoxSize = Com.mBoxSize;
}

CColliderAABB2D::~CColliderAABB2D()
{
}

bool CColliderAABB2D::Init()
{
	if (!CColliderBase::Init())
	{
		return false;
	}

#ifdef _DEBUG
	
	mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameCenterRect");

#endif // _DEBUG


	return true;
}
bool CColliderAABB2D::Init(const char* FileName)
{
	if (!CColliderBase::Init(FileName))
	{
		return false;
	}

	return true;
}
void CColliderAABB2D::PreUpdate(float DeltaTime)
{
	CColliderBase::PreUpdate(DeltaTime);
}

void CColliderAABB2D::Update(float DeltaTime)
{
	CColliderBase::Update(DeltaTime);

	//충돌체의 좌표값 구하기!
	mAABB.Min.x = mWorldPos.x - mBoxSize.x * 0.5f;
	mAABB.Min.y = mWorldPos.y - mBoxSize.y * 0.5f;

	mAABB.Max.x = mWorldPos.x + mBoxSize.x * 0.5f;
	mAABB.Max.y = mWorldPos.y + mBoxSize.y * 0.5f;

	mMin.x = mAABB.Min.x;
	mMin.y = mAABB.Min.y;
	mMax.x = mAABB.Max.x;
	mMax.y = mAABB.Max.y;

	//박스사이즈가 곳 ColliderBase 의 크기이다. 
	SetWorldScale(mBoxSize);
}

void CColliderAABB2D::PostUpdate(float DeltaTime)
{
	CColliderBase::PostUpdate(DeltaTime);
}

void CColliderAABB2D::Collision(float DeltaTime)
{
	CColliderBase::Collision(DeltaTime);
}
void CColliderAABB2D::PreRender()
{
	CColliderBase::PreRender();
}
void CColliderAABB2D::Render()
{
	CColliderBase::Render();
}
void CColliderAABB2D::PostRender()
{
	CColliderBase::PostRender();
}

CColliderAABB2D* CColliderAABB2D::Clone()
{
	return new CColliderAABB2D(*this);
}

bool CColliderAABB2D::Collision(FVector3D& HitPoint, CColliderBase* Dest)
{
	if (Dest->GetColliderType() == EColliderType::Collider3D)
	{
		return false;
	}

	switch (Dest->GetColliderShape())
	{
	case EColliderShape::AABB2D:
		return CCollision::CollisionAABB2DToAABB2D(HitPoint, this, (CColliderAABB2D*)Dest);
	case EColliderShape::Sphere2D:
		return CCollision::CollisionAABB2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
	case EColliderShape::OBB2D:
		return CCollision::CollisionAABB2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
	case EColliderShape::Line2D:
		return CCollision::CollisionLine2DToAABB2D(HitPoint, (CColliderLine2D*)Dest,this);
	}

	return false;
}
