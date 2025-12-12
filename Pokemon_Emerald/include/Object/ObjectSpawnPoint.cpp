#include "ObjectSpawnPoint.h"

#include "../Component/SceneComponent.h"
#include "../Scene/Scene.h"
#include "PlayerObject.h"
#include "GunnerMonster.h"
#include "NearingMonster.h"
#include "BulletObject.h"
#include "GravityBullet.h"
#include "TalonR.h"
#include "TornadoBullet.h"

CObjectSpawnPoint::CObjectSpawnPoint()
{
}

CObjectSpawnPoint::CObjectSpawnPoint(const CObjectSpawnPoint& Obj)
	: CSceneObject(Obj)
{
}

CObjectSpawnPoint::CObjectSpawnPoint(CObjectSpawnPoint&& Obj)
	: CSceneObject(Obj)
{
}

CObjectSpawnPoint::~CObjectSpawnPoint()
{
}

bool CObjectSpawnPoint::Init()
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	mRoot = CreateComponent<CSceneComponent>();
	SetRootComponent(mRoot);

	return true;
}
void CObjectSpawnPoint::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (mImmediateSpawn)
	{
		mImmediateSpawn = false;
		Spawn();

	}
	else
	{
		switch (mSpawnLoopType)
		{
		case EObjectSpawnLoopType::Once:	//한번만 스폰 
			if (!mSpawnObject && !mOnceSpawn)
			{
				mOnceSpawn = ComputeSpawnTime(DeltaTime);
			}
			break;
		case EObjectSpawnLoopType::Loop:	//반복해서 스폰 

			//시간계산 
			if (!mSpawnObject)
			{
				ComputeSpawnTime(DeltaTime);
			}

			break;

		}
	}

	
}

void CObjectSpawnPoint::Spawn()
{
	switch (mSpawnType)
	{
	case EObjectSpawnType::Player:
		mSpawnObject = mScene->CreateObj<CPlayerObject>("Player");
		break;
	case EObjectSpawnType::Bullet:
		mSpawnObject = mScene->CreateObj<CBulletObject>("Bullet");
		break;
	case EObjectSpawnType::GravityBullet:
		mSpawnObject = mScene->CreateObj<CGravityBullet>("GravitBullet");
		break;
	case EObjectSpawnType::TalonR:
		mSpawnObject = mScene->CreateObj <CTalonR>("TalonR");
		break;
	case EObjectSpawnType::TornadoBullet:
		mSpawnObject = mScene->CreateObj <CTornadoBullet>("TornadoBullet");
		break;
	case EObjectSpawnType::GunnerMonster:
		mSpawnObject = mScene->CreateObj <CGunnerMonster>("GunnerMonster");
		break;
	case EObjectSpawnType::NearingMonster:
		mSpawnObject = mScene->CreateObj <CNearingMonster>("NearingMonster");
		break;
	}

	mSpawnObject->SetWorldPos(GetWorldPosition());
	mSpawnObject->SetWorldRotation(GetWorldRotation());
	mSpawnObject->SetSpawnPoint(this);

	//만약 내가 EObjectSpawnCountType 이 Loop 가 아니라면 
	// 횟수를 내려주고 여차하면 SpawnPoint도 삭제해줘야한다. 
	ComputeSpawnCountType();
}

void CObjectSpawnPoint::ComputeSpawnCountType()
{
	switch (mSpawnCountType)
	{
	case EObjectSpawnCountType::OnceDestory:
		Destroy();
		break;
	case EObjectSpawnCountType::CountDestroy:
		--mDestroySpawnCount;

		if (mDestroySpawnCount <= 0)
		{
			Destroy();
		}
		break;
	}
}

bool CObjectSpawnPoint::ComputeSpawnTime(float DeltaTime)
{
	if (mSpawnTime == 0.f)
	{
		Spawn();
		return true;
	}

	//시간이 있다? 
	mTime += DeltaTime;

	if (mTime >= mSpawnTime)
	{
		mTime -= mSpawnTime;

		Spawn();

		return true;
	}

	return false;
}
