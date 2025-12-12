#pragma once
#include "SceneObject.h"

enum class EGravityType
{
	Pull,
	Push
};

class CGravityBullet :
    public CSceneObject
{
	friend class CScene;
protected:
	CGravityBullet();
	CGravityBullet(const CGravityBullet& Obj);
	CGravityBullet(CGravityBullet&& Obj);
	virtual ~CGravityBullet();



protected:
	CSharedPtr<class CStaticMeshComponent> mRoot;
	CSharedPtr<class CMovementComponent> mMovement;
	EGravityType mGravityType = EGravityType::Pull;
	float mSpeed = 200.f;
	float mDistance = 150.f;
	float mDuration = 1.f;
	float mRange = 500.f;
	float mGravitySpeed = 400.f;


public:
	float GetBulletSpeed() const
	{
		return mSpeed;
	}

	void SetBulletSpeed(const float& Speed)
	{
		mSpeed = Speed;
	}

	EGravityType GetGravityType()
	{
		return mGravityType;
	}

	void SetGravityType(EGravityType Type)
	{
		mGravityType = Type;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

};

