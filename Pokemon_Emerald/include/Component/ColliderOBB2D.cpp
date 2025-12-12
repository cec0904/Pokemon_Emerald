#include "ColliderOBB2D.h"

#include "ColliderAABB2D.h"
#include "../Collision.h"
#include "../Asset/AssetManager.h"
#include "../Asset/Mesh/Mesh.h"
#include "../Asset/Mesh/MeshManager.h"
#include "ColliderSphere2D.h"
#include "ColliderLine2D.h"

CColliderOBB2D::CColliderOBB2D()
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::OBB2D;
	mBoxSize.x = 100.f;
	mBoxSize.y = 100.f;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::OBB2D;

	mBoxSize = Com.mBoxSize;
}

CColliderOBB2D::CColliderOBB2D(CColliderOBB2D&& Com)
	: CColliderBase(Com)
{
	mColliderType = EColliderType::Collider2D;
	mColliderShape = EColliderShape::OBB2D;

	mBoxSize = Com.mBoxSize;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

bool CColliderOBB2D::Init()
{
	if (!CColliderBase::Init())
	{
		return false;
	}

#ifdef _DEBUG

	mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameCenterRect");
	mEnableRotation = true;
#endif // _DEBUG


	return true;
}
bool CColliderOBB2D::Init(const char* FileName)
{
	if (!CColliderBase::Init(FileName))
	{
		return false;
	}

	return true;
}
void CColliderOBB2D::PreUpdate(float DeltaTime)
{
	CColliderBase::PreUpdate(DeltaTime);
}

void CColliderOBB2D::Update(float DeltaTime)
{
	CColliderBase::Update(DeltaTime);

	//정보 넣기
	mBoxInfo.Center.x = mWorldPos.x;
	mBoxInfo.Center.y = mWorldPos.y;

	mBoxInfo.Axis[0].x = mAxis[EAxis::X].x;
	mBoxInfo.Axis[0].y = mAxis[EAxis::X].y;

	mBoxInfo.Axis[1].x = mAxis[EAxis::Y].x;
	mBoxInfo.Axis[1].y = mAxis[EAxis::Y].y;

	mBoxInfo.HalfSize = mBoxSize * 0.5;

	// 사각형을 구상하는 꼭지점 구하기
	FVector2D Pos[4];
	//좌상
	Pos[0] = mBoxInfo.Center - mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x + mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;
	//좌하
	Pos[1] = mBoxInfo.Center - mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x - mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;
	//우상
	Pos[2] = mBoxInfo.Center + mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x + mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;
	//우하
	Pos[3] = mBoxInfo.Center + mBoxInfo.Axis[0] * mBoxInfo.HalfSize.x - mBoxInfo.Axis[1] * mBoxInfo.HalfSize.y;

	//4개의 꼭지점에서 가장 작은 X,Y 값을 Min, 가장 큰 X, Y값을 Max로 넣으면 된다. 

	//초기화 
	mMin.x = Pos[0].x;
	mMin.y = Pos[0].y;

	mMax.x = Pos[0].x;
	mMax.y = Pos[0].y;

	for (int i = 0; i< 4; ++i)
	{
		mMin.x = mMin.x > Pos[i].x ? Pos[i].x : mMin.x;
		mMin.y = mMin.y > Pos[i].y ? Pos[i].y : mMin.y;

		mMax.x = mMax.x < Pos[i].x ? Pos[i].x : mMax.x;
		mMax.y = mMax.y < Pos[i].y ? Pos[i].y : mMax.y;
	}

	//박스사이즈가 곳 ColliderBase 의 크기이다. 
	SetWorldScale(mBoxSize);
}

void CColliderOBB2D::PostUpdate(float DeltaTime)
{
	CColliderBase::PostUpdate(DeltaTime);
}

void CColliderOBB2D::Collision(float DeltaTime)
{
	CColliderBase::Collision(DeltaTime);
}
void CColliderOBB2D::PreRender()
{
	CColliderBase::PreRender();
}
void CColliderOBB2D::Render()
{
	CColliderBase::Render();
}
void CColliderOBB2D::PostRender()
{
	CColliderBase::PostRender();
}

CColliderOBB2D* CColliderOBB2D::Clone()
{
	return new CColliderOBB2D(*this);
}

bool CColliderOBB2D::Collision(FVector3D& HitPoint, CColliderBase* Dest)
{
	if (Dest->GetColliderType() == EColliderType::Collider3D)
	{
		return false;
	}

	switch (Dest->GetColliderShape())
	{
	case EColliderShape::AABB2D:
		return CCollision::CollisionAABB2DToOBB2D(HitPoint, (CColliderAABB2D*)Dest, this);
	case EColliderShape::Sphere2D:
		return CCollision::CollisionSphere2DToOBB2D(HitPoint, (CColliderSphere2D*)Dest, this);
	case EColliderShape::OBB2D:
		return CCollision::CollisionOBB2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
	case EColliderShape::Line2D:
		return CCollision::CollisionLine2DToOBB2D(HitPoint, (CColliderLine2D*)Dest, this);
	}

	return false;
}
