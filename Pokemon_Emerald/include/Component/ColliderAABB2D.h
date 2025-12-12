#pragma once
#include "ColliderBase.h"

class CColliderAABB2D :
    public CColliderBase
{
	//CSceneComponent 꺼 기본 복사 
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderAABB2D();
	CColliderAABB2D(const CColliderAABB2D& Com);
	CColliderAABB2D(CColliderAABB2D&& Com);
	virtual ~CColliderAABB2D();

protected:
	FVector2D mBoxSize;
	//최대 최소값 
	FAABB2D mAABB;

public:
	void SetBoxSize(const FVector2D& Size)
	{
		mBoxSize = Size;
	}

	void SetBoxSize(float x, float y)
	{
		mBoxSize.x = x;
		mBoxSize.y = y;
	}

	const FAABB2D& GetBox() const
	{
		return mAABB;
	}
	
	const FVector2D& GetBoxSize() const
	{
		return mBoxSize;
	}


public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderAABB2D* Clone();

public:
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);
};

