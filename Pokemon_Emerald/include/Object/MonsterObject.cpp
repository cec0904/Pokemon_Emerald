#include "MonsterObject.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/SpriteComponent.h"

#include "BulletObject.h"
#include "../Scene/Scene.h"
#include "../Share/Log.h"
#include "ObjectSpawnPoint.h"

//문자열 
#include <sstream>

#include "../Component/WidgetComponent.h"
#include "../Scene/SceneUIManager.h"
#include "../UI/UserWidget/HeadInfo.h"

CMonsterObject::CMonsterObject()
{
	mState = EMonsterState::Idle;
}

CMonsterObject::CMonsterObject(const CMonsterObject& Obj)
	: CSceneObject(Obj)
{
	mState = EMonsterState::Idle;
}

CMonsterObject::CMonsterObject(CMonsterObject&& Obj)
	: CSceneObject(Obj)
{
	mState = EMonsterState::Idle;
}

CMonsterObject::~CMonsterObject()
{
}


void CMonsterObject::Idle(float DeltaTime)
{
}

void CMonsterObject::Recone(float DeltaTime)
{
}

void CMonsterObject::Research(float DeltaTime)
{
}

void CMonsterObject::Move(float DeltaTime)
{
}

void CMonsterObject::Attack(float DeltaTime)
{
}

void CMonsterObject::CollisionMonster(const FVector3D& HitPont, CColliderBase* Dest)
{

	//Dest->GetProfile()->Channel == ECollisionChannel::PlayerAttack();
}

void CMonsterObject::CollisionMonsterEnd(class CColliderBase* Dest)
{
}

float CMonsterObject::Damage(float Attack, CSceneObject* Obj)
{
	float Dmg = CSceneObject::Damage(Attack, Obj);

	mHP -= (int)Dmg;

	if (mHP <= 0)
	{
		if (mSpawnPoint != nullptr)
		{
			mSpawnPoint->ClearObject();
		}

		Destroy();
	}

	return Dmg;
}

bool CMonsterObject::Init()
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	//mRoot = CreateComponent<CStaticMeshComponent>();
	mRoot = CreateComponent<CSpriteComponent>();
	//mBody = CreateComponent<CColliderAABB2D>();
	mBody = CreateComponent<CColliderSphere2D>();
	//mBody = CreateComponent<CColliderOBB2D>();

	CHeadInfo* Widget = mScene->GetUIManager()->CreateWidget<CHeadInfo>("HeadInfo");

	mWidgetCoponent = CreateComponent<CWidgetComponent>();
	mWidgetCoponent->SetWidget(Widget);

	mRoot->AddChild(mWidgetCoponent);

	/*mRoot->SetMesh("CenterRect");
	mRoot->SetShader("ColorMeshShader");*/

	mRoot->SetTexture("Monster1");
	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetWorldScale(100.f, 100.f);

	SetRootComponent(mRoot);

	mRoot->AddChild(mBody);
	//mBody->SetBoxSize(100.f, 100.f);
	mBody->SetRadius(50.f);
	mBody->SetCollisionProfile("Monster");
	mBody->SetCollisionBeginFunc<CMonsterObject>(this, &CMonsterObject::CollisionMonster);
	mBody->SetCollisionEndFunc<CMonsterObject>(this, &CMonsterObject::CollisionMonsterEnd);

	return true;
}
void CMonsterObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (!mTarget->IsActive())
	{
		mTarget = nullptr;
		return;
	}
	else if (!mTarget->IsEnable())
	{
		return;
	}


	//FSM
	//유한 상태 기계
	switch (mState)
	{
	case EMonsterState::Idle:
		Idle(DeltaTime);
		break;
	case EMonsterState::Recone:
		Recone(DeltaTime);
		break;
	case EMonsterState::Research:
		Research(DeltaTime);
		break;
	case EMonsterState::Move:
		Move(DeltaTime);
		break;
	case EMonsterState::Attack:
		Attack(DeltaTime);
		break;
	}
}