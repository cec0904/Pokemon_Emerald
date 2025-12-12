#pragma once
#include "ColliderBase.h"

class CColliderSphere2D :
    public CColliderBase
{
	//CSceneComponent 꺼 기본 복사 
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderSphere2D();
	CColliderSphere2D(const CColliderSphere2D& Com);
	CColliderSphere2D(CColliderSphere2D&& Com);
	virtual ~CColliderSphere2D();

protected:
	//반지름 
	float mRadius = 50.f;

public:
	float GetRadius()
	{
		return mRadius;
	}

	void SetRadius(float Radius)
	{
		mRadius = Radius;
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
	virtual CColliderSphere2D* Clone();

public:
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);

};

