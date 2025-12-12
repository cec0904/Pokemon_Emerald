#include "NpcBase.h"
#include "../../Object/PlayerObject.h"
#include "../../Object/SceneObject.h"
#include "../../Component/StaticMeshComponent.h"
#include "../../Component/MovementComponent.h"
#include "../../Component/ColliderAABB2D.h"
#include "../../Component/ColliderSphere2D.h"
#include "../../Component/ColliderOBB2D.h"
#include "../../Component/ColliderLine2D.h"
#include "../../Component/SpriteComponent.h"
#include "../../Animation/Animation2D.h"
#include "../../Asset/Animation/Animation2DData.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Scene/Scene.h"
#include "../../Share/Log.h"

//문자열 
#include <sstream>

CNpcBase::CNpcBase()
{
	mState = ENpcState::Idle;
}

CNpcBase::CNpcBase(const CNpcBase& Obj)
{
	mState = ENpcState::Idle;
}

CNpcBase::CNpcBase(CNpcBase&& Obj)
{
	mState = ENpcState::Idle;
}

CNpcBase::~CNpcBase()
{
}

void CNpcBase::Idle(float DeltaTime)
{
}

void CNpcBase::Move(float DeltaTime)
{
}

void CNpcBase::Battle(float DeltaTime)
{
}

void CNpcBase::CollisionNpc(const FVector3D& HitPont, CColliderBase* Dest)
{
	
}

void CNpcBase::CollisionNpcEnd(CColliderBase* Dest)
{

}

void CNpcBase::NpcScript()
{
	// 플레이어가 Npc 에게 상호작용하면 대사던지기

	// 1. 플레이어가 상호작용 했니 ?

	// 2. 상호작용 키, 키바인드, 함수, 함수 바인드 만들기


}

bool CNpcBase::Init()
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	mRoot = CreateComponent<CSpriteComponent>();
	mBody = CreateComponent<CColliderAABB2D>();

	mRoot->SetTexture("Npc");
	mRoot->SetPivot(0.5f, 0.1f);
	mRoot->SetWorldScale(100.f, 100.f);

	SetRootComponent(mRoot);

	mRoot->AddChild(mBody);
	mBody->SetBoxSize(50.f, 50.f);
	mBody->SetCollisionProfile("Npc");
	mBody->SetCollisionBeginFunc<CNpcBase>(this, &CNpcBase::CollisionNpc);
	mBody->SetCollisionEndFunc<CNpcBase>(this, &CNpcBase::CollisionNpcEnd);


	return true;
}

void CNpcBase::Update(float DeltaTime)
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

	switch (mState)
	{
	case ENpcState::Idle:
		Idle(DeltaTime);
		break;
	case ENpcState::Move:
		Move(DeltaTime);
		break;
	case ENpcState::Battle:
		Battle(DeltaTime);
		break;
	}
}
