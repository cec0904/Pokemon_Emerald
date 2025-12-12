#include "GunnerMonster.h"

#include "BulletObject.h"
#include "../Scene/Scene.h"
#include "PlayerObject.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/SpriteComponent.h"

CGunnerMonster::CGunnerMonster()
{
}

CGunnerMonster::CGunnerMonster(const CGunnerMonster& Obj)
	: CMonsterObject(Obj)
{
}

CGunnerMonster::CGunnerMonster(CGunnerMonster&& Obj)
	: CMonsterObject(Obj)

{
}

CGunnerMonster::~CGunnerMonster()
{
}

bool CGunnerMonster::Init()
{
	if (!CMonsterObject::Init())
	{
		return false;
	}

	mRoot->SetTexture("MonsterTex");
	mRoot->SetPivot(0.5f, 0.5f);
	//mRoot->SetOpacity(0, 1.f);

	SetTarget(mScene->FindObjectFromType<CPlayerObject>());

	return true;
}
void CGunnerMonster::Update(float DeltaTime)
{
	CMonsterObject::Update(DeltaTime);

	if (!mTarget->IsActive())
	{
		mTarget = nullptr;
	}
	else if (mTarget->IsEnable())
	{
		//타겟을 바라보는 벡터
		//내가 타겟을 바라보는 방향 벡터는 ?
		// 타겟 위치 - 자기자신 위치 == 방향벡터 
		//FVector3D ViewDir = mTarget->GetWorldPosition() - GetWorldPosition();
		//// 단위벡터로 만들어준다. (단위벡터 : 길이가 1인 벡터 )
		//ViewDir.Normalize();

		//float Angle = FVector3D::Axis[EAxis::Y].GetAngle(ViewDir);

		float Angle = GetWorldPosition().GetViewTargetAngle(mTarget->GetWorldPosition());

		// 출력창 로그찍기 
		//wostringstream oss;
		//oss << L"Angle :" << Angle << L"\n";
		//OutputDebugStringW(oss.str().c_str());

		SetWorldRotationZ(Angle);
	}

	//발사하기 샘플 
	mFireTime -= DeltaTime;
	if (mFireTime <= 0)
	{
		mFireTime = 1.f;
		//총알 발사 카운트 체크 
		++mFireCount;	

		CBulletObject* Bullet = mScene->CreateObj<CBulletObject>("Bullet");
		Bullet->SetBulletCollisionProfile("MonsterAttack");
		Bullet->SetWorldScale(50.f, 50.f);
		Bullet->SetWorldRotation(GetWorldRotation());
		Bullet->SetWorldPos(GetWorldPosition());

		Bullet->SetLifeTime(2.f);

		// 숙제
		// 몬스터가 발사하다가 4번째 탄은 산탄으로 발사하기  45각도 3발 산탄 
		// 지금 총알이 4번재 총알이니?
		if (mFireCount == 4)
		{
			// 4번재 총알이야? 다시 0으로 
			mFireCount = 0;

			Bullet = mScene->CreateObj<CBulletObject>("Bullet");
			Bullet->SetBulletCollisionProfile("MonsterAttack");
			Bullet->SetWorldScale(50.f, 50.f);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->AddWorldRotationZ(45.f);
			Bullet->SetWorldPos(GetWorldPosition());

			Bullet->SetLifeTime(2.f);

			Bullet = mScene->CreateObj<CBulletObject>("Bullet");
			Bullet->SetBulletCollisionProfile("MonsterAttack");
			Bullet->SetWorldScale(50.f, 50.f);
			Bullet->SetWorldRotation(GetWorldRotation());
			Bullet->AddWorldRotationZ(-45.f);
			Bullet->SetWorldPos(GetWorldPosition());

			Bullet->SetLifeTime(2.f);
		}
	}
}

