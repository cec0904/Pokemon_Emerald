#include "NearingMonster.h"
#include "PlayerObject.h"
#include "../Scene/Scene.h"
#include "../Component/ColliderLine2D.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/SpriteComponent.h"

CNearingMonster::CNearingMonster()
{
}

CNearingMonster::CNearingMonster(const CNearingMonster& Obj)
	: CMonsterObject(Obj)
{
}

CNearingMonster::CNearingMonster(CNearingMonster&& Obj)
	: CMonsterObject(Obj)
{
}

CNearingMonster::~CNearingMonster()
{
}

bool CNearingMonster::Init()
{
	if (!CMonsterObject::Init())
	{
		return false;
	}


	mRoot->SetTexture("Monster2Tex", TEXT("Texture/nuguri.png"));
	mRoot->SetPivot(0.5f, 0.5f);


	CPlayerObject* pPlayer = mScene->FindObjectFromName<CPlayerObject>("Player");
	if (pPlayer)
	{
		SetTarget(pPlayer);
	}


	mDetectDistance = 400.f;

	return true;
}
void CNearingMonster::Update(float DeltaTime)
{
	CMonsterObject::Update(DeltaTime);

	//타겟이 있니?
	if(mTarget)
	{
		//나랑 타겟의 거리를 측정한다. 
		float Distance = GetWorldPosition().Distance(mTarget->GetWorldPosition());


		//측정한 거리가 내 사정거리 안이야?
		if(Distance <= mDetectDistance)	// 탐지 되었어!
		{
			//타겟쪽으로 회전시켜줘 
			float RotAngle = GetWorldPosition().GetViewTargetAngle(mTarget->GetWorldPosition());

			SetWorldRotationZ(RotAngle);


			//타겟과의 각도를 계산한다. 
			//타겟을 바라보는 방향벡터를 구한다. 
			FVector3D View = mTarget->GetWorldPosition() - GetWorldPosition();
			View.Normalize();

			float Angle = GetAxis(EAxis::Y).GetAngle(View);
			// 내가 타겟을 바라보는 각도 < 내 공격 각도 안이야?
			if(Angle < mDetectAngle)
			{
				//공격
				//Attack time 주기마다. 
				mAttackTime -= DeltaTime;

				if (mAttackTime <= 0.f)
				{
					mAttackTime += 1.f;
					//공격을 할것이다. 
					CPlayerObject* Player = dynamic_cast<CPlayerObject*>(mTarget.Get());

					
				}
			}
			else
			{
				mAttackTime = 1.f;
			}
		}
		else
		{
			mAttackTime = 1.f;
		}

	}



}

