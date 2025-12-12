#include "GravityBullet.h"

#include "../Component/MovementComponent.h"
#include "../Component/StaticMeshComponent.h"

#include "MonsterObject.h"
#include "../Scene/Scene.h"

CGravityBullet::CGravityBullet()
{
}

CGravityBullet::CGravityBullet(const CGravityBullet& Obj)
{
}

CGravityBullet::CGravityBullet(CGravityBullet&& Obj)
{
}

CGravityBullet::~CGravityBullet()
{
}

bool CGravityBullet::Init()
{
	mRoot = CreateComponent<CStaticMeshComponent>();
	mMovement = CreateComponent<CMovementComponent>();

	mRoot->SetMesh("CenterRect");
	mRoot->SetShader("ColorMeshShader");
	mRoot->SetWorldScale(50.f, 50.f);
	SetRootComponent(mRoot);

	mMovement->SetUpdateComponent(mRoot);
	mMovement->SetMoveAxis(EAxis::Y);
	mMovement->SetMoveSpeed(500.f);

	return true;
}
void CGravityBullet::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	mDistance -= mMovement->GetMoveDistance();

	if (mDistance <= 0.f)
	{
		mDuration -= DeltaTime;

		if (mDuration <= 0)
		{
			//총알 삭제 
			Destroy();
		}

		//총알이동 스탑 
		mMovement->SetEnable(false);

		list<CSharedPtr<CMonsterObject>> MonsterList;

		mScene->FindObjectsFromType<CMonsterObject>(MonsterList);

		auto iter = MonsterList.begin();
		auto iterEnd = MonsterList.end();

		for (; iter != iterEnd; ++iter) 
		{
			//몬스터 위치
			FVector3D Pos = (*iter)->GetWorldPosition();
			//몬스터랑 내 위치 거리
			float dist = Pos.Distance(GetWorldPosition());

			if (dist <= mRange)
			{
				FVector3D MoveDir;

				switch (mGravityType)
				{
				case EGravityType::Pull:
					// 몬스터 -> 총알 : 방향벡터
					MoveDir = GetWorldPosition() - Pos;
					break;
				case EGravityType::Push:
					// 총알 -> 몬스터 : 방향벡터 
					MoveDir = Pos - GetWorldPosition();
					break;
				}

				MoveDir.Normalize();

				//(*iter) 반복자가 가리키는 데이터 
				(*iter)->AddWorldPos(MoveDir * mGravitySpeed * DeltaTime);
			}
		}
	}


}