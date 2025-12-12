#include "BulletObject.h"

#include "SpriteEffect.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/ColliderAABB2D.h"
#include "../Share/Log.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "SpriteEffect.h"
#include "../Scene/Scene.h"


CBulletObject::CBulletObject()
	:CSceneObject()
{
}

CBulletObject::CBulletObject(const CBulletObject& Obj)
	:CSceneObject(Obj)
{
}

CBulletObject::CBulletObject(CBulletObject&& Obj)
	:CSceneObject(Obj)
{
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::SetBulletCollisionProfile(const string& Name)
{
	mBody->SetCollisionProfile(Name);
}


bool CBulletObject::Init()
{
	CSceneObject::Init();

	mRoot = CreateComponent<CSpriteComponent>();
	mBody = CreateComponent<CColliderAABB2D>();
	mMovement = CreateComponent<CMovementComponent>();


	mRoot->SetTexture("Wallnut");
	mRoot->SetPivot(0.5f, 0.5f);
	mRoot->SetWorldScale(0.5f, 0.5f, 1.f);

	SetRootComponent(mRoot);

	mRoot->AddChild(mBody);
	mBody->SetBoxSize(50.f, 50.f);
	mBody->SetCollisionProfile("PlayerAttack");
	mBody->SetCollisionBeginFunc<CBulletObject>(this, &CBulletObject::CollisionBullet);

	//Movement 셋팅 
	mMovement->SetUpdateComponent(mRoot);
	mMovement->SetMoveAxis(EAxis::Y);
	mMovement->SetMoveSpeed(500.f);


	return true;
}

void CBulletObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	//FVector3D Pos = mRoot->GetWorldPosition();
	////이동할 위치의 새로운 위치값 = 내 위치 + 내Y축 * 속도 * DeltaTime 
	//mRoot->SetWorldPos(Pos + mRootComponent->GetAxis(EAxis::Y) * mSpeed * DeltaTime);
}

void CBulletObject::CollisionBullet(const FVector3D& HitPoint, CColliderBase* Dest)
{
	//CLog::PrintLog("Bullet Hit Collision", ELogPrintType::All);

	Dest->GetOwner()->Damage(1.f, this);

	CSpriteEffect* Effect = mScene->CreateObj<CSpriteEffect>("HitEffect");

	Effect->SetAnimation("Explosion");
	Effect->SetWorldPos(HitPoint);
	Effect->SetWorldScale(300.f, 300.f);


	Destroy();
}

