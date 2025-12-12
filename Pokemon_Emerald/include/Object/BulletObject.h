#pragma once
#include "SceneObject.h"

class CBulletObject :
    public CSceneObject
{
	friend class CScene;

private:
	float mSpeed = 200.f;

public:
	float GetBulletSpeed() const
	{
		return mSpeed;
	}

	void SetBulletSpeed(const float& Speed)
	{
		mSpeed = Speed;
	}

	void  SetBulletCollisionProfile(const string& Name);


protected:
	CSharedPtr<class CSpriteComponent> mRoot;
	CSharedPtr<class CColliderAABB2D> mBody;
	CSharedPtr<class CMovementComponent> mMovement;

	//20251105  할거
	//총알 오브젝트의 충돌체의 profile을 설정해주는 만들고 
	//각각의 총알타입에 맞게 변경해주기 

protected:
	CBulletObject();
	CBulletObject(const CBulletObject& Obj);
	CBulletObject(CBulletObject&& Obj);
	virtual ~CBulletObject();
	

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void CollisionBullet(const FVector3D& HitPoint, class CColliderBase* Dest);

};

