#pragma once
#include "ColliderBase.h"

class CColliderOBB2D :
    public CColliderBase
{
	//CSceneComponent 꺼 기본 복사 
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderOBB2D();
	CColliderOBB2D(const CColliderOBB2D& Com);
	CColliderOBB2D(CColliderOBB2D&& Com);
	virtual ~CColliderOBB2D();

protected:
	//충돌체의 크기 
	FVector2D mBoxSize;
	//OBB 충돌체 정보 
	FOBB2D mBoxInfo;

public:
	const FOBB2D& GetBox() const
	{
		return mBoxInfo;
	}

	const FVector2D& GetBoxSize() const
	{
		return mBoxSize;
	}

	void SetBoxSize(const FVector2D& Size)
	{
		mBoxSize = Size;
	}

	void SetBoxSize(float x, float y)
	{
		mBoxSize.x = x;
		mBoxSize.y = y;
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
	virtual CColliderOBB2D* Clone();

public:
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);

};

