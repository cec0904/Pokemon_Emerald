#include "ColliderLine2D.h"

#include "ColliderAABB2D.h"
#include "ColliderOBB2D.h"
#include "../Collision.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "ColliderSphere2D.h"
#include "../Share/Log.h"

CColliderLine2D::CColliderLine2D()
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Line2D;

	mEnableRotation = true;
}

CColliderLine2D::CColliderLine2D(const CColliderLine2D& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Line2D;

}

CColliderLine2D::CColliderLine2D(CColliderLine2D&& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::Line2D;

}

CColliderLine2D::~CColliderLine2D()
{
}

void CColliderLine2D::SetLineAxis(EAxis::Type Axis)
{
	mLineAxis = Axis;
//
#ifdef _DEBUG

	//debug용 메쉬 설정 라인으로
	switch (mLineAxis)
	{
	case EAxis::X:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineRight2D");
		break;
	case EAxis::Y:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineUp2D");
		break;
	}
#endif // _DEBUG


}

bool CColliderLine2D::Init()
{
	if (!CColliderBase::Init())
	{
		return false;
	}

#ifdef _DEBUG

	//debug용 메쉬 설정 라인으로
	switch (mLineAxis)
	{
	case EAxis::X:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineRight2D");
		break;
	case EAxis::Y:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineUp2D");
		break;
	}

#endif // _DEBUG


	return true;
}
bool CColliderLine2D::Init(const char* FileName)
{
	if (!CColliderBase::Init(FileName))
	{
		return false;
	}

#ifdef _DEBUG

	//debug용 메쉬 설정 라인으로
	switch (mLineAxis)
	{
	case EAxis::X:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineRight2D");
		break;
	case EAxis::Y:
		mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("LineUp2D");
		break;
	}

#endif // _DEBUG


	return true;
}
void CColliderLine2D::PreUpdate(float DeltaTime)
{
	CColliderBase::PreUpdate(DeltaTime);
}

void CColliderLine2D::Update(float DeltaTime)
{
	CColliderBase::Update(DeltaTime);

	//시작 위치 지정 
	mLineInfo.Start.x = mWorldPos.x;
	mLineInfo.Start.y = mWorldPos.y;

	//끝 지점 지정
	//시작위치, 방향, 길이
	//  최종 가고싶은 지점 = 시작위치 + 방향 * 길이
	FVector2D Dir;
	Dir.x = mAxis[mLineAxis].x;
	Dir.y = mAxis[mLineAxis].y;

	mLineInfo.End = mLineInfo.Start + Dir * mDist;

	// 쿼드 트리
	mMin.x = mLineInfo.Start.x < mLineInfo.End.x ? mLineInfo.Start.x : mLineInfo.End.x;
	mMin.y = mLineInfo.Start.y < mLineInfo.End.y ? mLineInfo.Start.y : mLineInfo.End.y;

	mMax.x = mLineInfo.Start.x > mLineInfo.End.x ? mLineInfo.Start.x : mLineInfo.End.x;
	mMax.y = mLineInfo.Start.y > mLineInfo.End.y ? mLineInfo.Start.y : mLineInfo.End.y;

	SetWorldScale(mDist, mDist, mDist);
	int i = 10;
}

void CColliderLine2D::PostUpdate(float DeltaTime)
{
	CColliderBase::PostUpdate(DeltaTime);
}

void CColliderLine2D::Collision(float DeltaTime)
{
	CColliderBase::Collision(DeltaTime);
}
void CColliderLine2D::PreRender()
{
	CColliderBase::PreRender();
}
void CColliderLine2D::Render()
{
	CColliderBase::Render();
}
void CColliderLine2D::PostRender()
{
	CColliderBase::PostRender();
}

CColliderLine2D* CColliderLine2D::Clone()
{
	return new CColliderLine2D(*this);
}

bool CColliderLine2D::Collision(FVector3D& HitPoint, CColliderBase* Dest)
{
	if (Dest->GetColliderType() == EColliderType::Collider3D)
	{
		return false;
	}

	switch (Dest->GetColliderShape())
	{
	case EColliderShape::AABB2D:
		return CCollision::CollisionLine2DToAABB2D(HitPoint, this, (CColliderAABB2D*)Dest);
	case EColliderShape::Sphere2D:
		return CCollision::CollisionLine2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
	case EColliderShape::OBB2D:
		return CCollision::CollisionLine2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
	case EColliderShape::Line2D:
		return CCollision::CollisionLine2DToLine2D(HitPoint, this, (CColliderLine2D*)Dest);
	}

	return false;
}
