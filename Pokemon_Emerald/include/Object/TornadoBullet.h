#pragma once
#include "SceneObject.h"

class CTornadoBullet :
    public CSceneObject
{
	friend class CScene;

private:
	float mSpeed = 2.f;

public:
	float GetBulletSpeed() const
	{
		return mSpeed;
	}

	void SetBulletSpeed(const float& Speed)
	{
		mSpeed = Speed;
	}

protected:
	CSharedPtr<class CSceneComponent>		mRoot;
	CSharedPtr<class CSceneComponent>		mPivot;
	CSharedPtr<class CStaticMeshComponent>	mMesh;

protected:
	CTornadoBullet();
	CTornadoBullet(const CTornadoBullet& Obj);
	CTornadoBullet(CTornadoBullet&& Obj);
	virtual ~CTornadoBullet();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

