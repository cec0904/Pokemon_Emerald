#include "ColliderSphere2D.h"
#include "../Collision.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "ColliderAABB2D.h"
#include "ColliderOBB2D.h"
#include "ColliderLine2D.h"

CColliderSphere2D::CColliderSphere2D()
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Sphere2D;
}

CColliderSphere2D::CColliderSphere2D(const CColliderSphere2D& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Sphere2D;

}

CColliderSphere2D::CColliderSphere2D(CColliderSphere2D&& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Sphere2D;

}

CColliderSphere2D::~CColliderSphere2D()
{
}

bool CColliderSphere2D::Init()
{
	if (!CColliderBase::Init())
	{
		return false;
	}

#ifdef _DEBUG

	mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameSphere2D");

#endif // _DEBUG


	return true;
}
bool CColliderSphere2D::Init(const char* FileName)
{
	if (!CColliderBase::Init(FileName))
	{
		return false;
	}

	return true;
}
void CColliderSphere2D::PreUpdate(float DeltaTime)
{
	CColliderBase::PreUpdate(DeltaTime);
}

void CColliderSphere2D::Update(float DeltaTime)
{
	CColliderBase::Update(DeltaTime);

	//충돌체의 좌표값 구하기!
	// 
	mMin.x = mWorldPos.x - mRadius;
	mMin.y = mWorldPos.y - mRadius;

	mMax.x = mWorldPos.x + mRadius;
	mMax.y = mWorldPos.y + mRadius;

	//내접원을 가지는 사각혀의 한변의 길이는 지름이랑 같다. 
	// 지름 = 반지름 * 2
	SetWorldScale(FVector2D(mRadius * 2.f, mRadius * 2.f));
}

void CColliderSphere2D::PostUpdate(float DeltaTime)
{
	CColliderBase::PostUpdate(DeltaTime);
}

void CColliderSphere2D::Collision(float DeltaTime)
{
	CColliderBase::Collision(DeltaTime);
}
void CColliderSphere2D::PreRender()
{
	CColliderBase::PreRender();
}
void CColliderSphere2D::Render()
{
	CColliderBase::Render();
}
void CColliderSphere2D::PostRender()
{
	CColliderBase::PostRender();
}

CColliderSphere2D* CColliderSphere2D::Clone()
{
	return new CColliderSphere2D(*this);
}

bool CColliderSphere2D::Collision(FVector3D& HitPoint, CColliderBase* Dest)
{
	if (Dest->GetColliderType() == EColliderType::Collider3D)
	{
		return false;
	}

	switch (Dest->GetColliderShape())
	{
	case EColliderShape::AABB2D:
		return CCollision::CollisionAABB2DToSphere2D(HitPoint, (CColliderAABB2D*)Dest, this);
		break;
	case EColliderShape::Sphere2D:
		return CCollision::CollisionSphere2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
	case EColliderShape::OBB2D:
		return CCollision::CollisionSphere2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
	case EColliderShape::Line2D:
		return CCollision::CollisionLine2DToSphere2D(HitPoint, (CColliderLine2D*)Dest, this);
	}

	return false;
}
