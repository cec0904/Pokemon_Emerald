#pragma once
#include "Component.h"

class CMovementComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CMovementComponent();
	CMovementComponent(const CMovementComponent& Com);
	CMovementComponent(CMovementComponent&& Com);
	virtual ~CMovementComponent();
	
protected:
	//업데이트할 컴포넌트 
	CSharedPtr<class CSceneComponent> mUpdateComponent;
	EAxis::Type mMoveAxis = EAxis::None;

protected:
	// 이동 방향 벡터 
	FVector3D mVelocity;
	// 한 프레임당 이동할 거리 벡터 
	FVector3D mMoveStep;
	float mSpeed = 1.f;
	//이동 초기화 여부 
	bool mVelocityInit = true;

public:
	const FVector3D& GetMoveStep() const
	{
		return mMoveStep;
	}

	float GetVelocityLength() const
	{
		return mVelocity.Length();
	}

	float GetMoveDistance() const
	{
		return mMoveStep.Length();
	}

	void SetUpdateComponent(class CSceneComponent* Target);
	void SetMoveSpeed(float Speed)
	{
		mSpeed = Speed;
	}

	void AddMove(const FVector3D& Dir)
	{
		mVelocity += Dir;
	}

	void SetMove(const FVector3D& Dir)
	{
		mVelocity = Dir;
	}

	void SetMoveAxis(EAxis::Type Axis)
	{
		mMoveAxis = Axis;
	}

	void SetVelocityInit(bool VelocityInit)
	{
		mVelocityInit = VelocityInit;
	}



	//시점 함수 
public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PostRender();
	virtual CMovementComponent* Clone();
	virtual void EndFrame();
};

