#pragma once
#include "ColliderBase.h"

class CColliderLine2D :
    public CColliderBase
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderLine2D();
	CColliderLine2D(const CColliderLine2D& Com);
	CColliderLine2D(CColliderLine2D&& Com);
	virtual ~CColliderLine2D();

protected:
	//진선의 방향 
	EAxis::Type mLineAxis = EAxis::Y;
	float mDist = 100.f;
	FLine2D mLineInfo;

public:
	void SetLineAxis(EAxis::Type Axis);
	void SetLineDistance(float Dist)
	{
		mDist = Dist;
	}
	const FLine2D& GetLine()
	{
		return mLineInfo;
	}

	const EAxis::Type GetLineAxis()
	{
		return mLineAxis;
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
	virtual CColliderLine2D* Clone();

public:
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);

};

