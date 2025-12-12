#pragma once
#include "SceneObject.h"

enum class EMonsterState
{
	Idle,
	Recone,
	Research,
	Move,
	Attack,
	End
};

class CMonsterObject :
	public CSceneObject
{
	friend class CScene;

protected:
	CMonsterObject();
	CMonsterObject(const CMonsterObject& Obj);
	CMonsterObject(CMonsterObject&& Obj);
	virtual ~CMonsterObject();

protected:
	//CSharedPtr<class CStaticMeshComponent> mRoot;
	CSharedPtr<class CSpriteComponent> mRoot;
	//CSharedPtr<class CColliderAABB2D> mBody;
	CSharedPtr<class CColliderSphere2D> mBody;
	//CSharedPtr<class CColliderOBB2D> mBody;

	//Widget Component
	CSharedPtr<class CWidgetComponent> mWidgetCoponent;

	CSharedPtr<CSceneObject> mTarget;

	EMonsterState mState;

	float mDetectDistance = 0.f;

	//몬스터 체력 
	int mHP = 5;

public:
	EMonsterState GetState() const
	{
		return mState;
	}

	void SetState(EMonsterState State)
	{
		mState = State;
	}

protected:
	virtual void Idle(float DeltaTime);
	virtual void Recone(float DeltaTime);
	virtual void Research(float DeltaTime);
	virtual void Move(float DeltaTime);
	virtual void Attack(float DeltaTime);



private:
	void CollisionMonster(const FVector3D& HitPont, class CColliderBase* Dest);
	void CollisionMonsterEnd(class CColliderBase* Dest);

public:
	virtual float Damage(float Attack, CSceneObject* Obj);

public:
	void SetTarget(CSceneObject* Target)
	{
		mTarget = Target;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

