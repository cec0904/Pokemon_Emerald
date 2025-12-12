#pragma once
#include "../../Object/SceneObject.h"

enum class ENpcState
{
	Idle,
	Move,
	Battle,
	End
};

class CNpcBase :
	public CSceneObject
{
	friend class CScene;

protected:
	CNpcBase();
	CNpcBase(const CNpcBase& Obj);
	CNpcBase(CNpcBase&& Obj);
	virtual ~CNpcBase();

protected:
	CSharedPtr<class CSpriteComponent> mRoot;
	CSharedPtr<class CColliderAABB2D> mBody;
	CSharedPtr<class CMovementComponent> mMovement;
	CSharedPtr<CSceneObject> mTarget;
	
	float mDetectDistance = 0.f;

	// 애니메이션
	class CAnimation2D* mAnimation = nullptr;
	// Idle 상태값으로 돌아갈건지 여부
	bool mAutoBasePose = true;

	float mSpeed = 200.f;

	ENpcState mState;

public:
	ENpcState GetState() const
	{
		return mState;
	}

	void SetState(ENpcState State)
	{
		mState = State;
	}

protected:
	virtual void Idle(float DeltaTime);
	virtual void Move(float DeltaTime);
	virtual void Battle(float DeltaTime);

private:
	void CollisionNpc(const FVector3D& HitPont, class CColliderBase* Dest);
	void CollisionNpcEnd(class CColliderBase* Dest);


public:
	void SetTarget(CSceneObject* Target)
	{
		mTarget = Target;
	}

public:
	// 대사
	void NpcScript();


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

