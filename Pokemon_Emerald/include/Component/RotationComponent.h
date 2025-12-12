#pragma once
#include "Component.h"

class CRotationComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CRotationComponent();
	CRotationComponent(const CRotationComponent& Com);
	CRotationComponent(CRotationComponent&& Com);
	virtual ~CRotationComponent();

protected:
	//업데이트할 컴포넌트 
	CSharedPtr<class CSceneComponent> mUpdateComponent;
	EAxis::Type mMoveAxis = EAxis::None;

protected:
	// 회전 방향 
	FVector3D mVelocity;
	// 한 프레임당 이동할 거리 벡터 
	FVector3D mRotationStep;
	//이동 초기화 여부 
	bool mVelocityInit = true;

public:
	const FVector3D& GetRotationStep() const
	{
		return mRotationStep;
	}

	float GetRotationDistance() const
	{
		return mRotationStep.Length();
	}

	void SetUpdateComponent(class CSceneComponent* Target);
	void AddMove(const FVector3D& Rot)
	{
		mVelocity += Rot;
	}

	void AddMoveX(float x)
	{
		mVelocity.x += x;
	}

	void AddMoveY(float y)
	{
		mVelocity.y += y;
	}

	void AddMoveZ(float z)
	{
		mVelocity.z += z;
	}

	void SetMove(const FVector3D& Rot)
	{
		mVelocity = Rot;
	}

	void SetMoveX(float x)
	{
		mVelocity.x = x;
	}

	void SetMoveY(float y)
	{
		mVelocity.y = y;
	}

	void SetMoveZ(float z)
	{
		mVelocity.z = z;
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
	virtual CRotationComponent* Clone();

	virtual void EndFrame() override;
};

