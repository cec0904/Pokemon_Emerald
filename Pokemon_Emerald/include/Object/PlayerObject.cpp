#include "PlayerObject.h"

#include "../Component/StaticMeshComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/RotationComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/ColliderSphere2D.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/ColliderLine2D.h"

#include "../Animation/Animation2D.h"
#include "../Asset/Animation/Animation2DData.h"
#include "../Asset/Texture/Texture.h"

#include "../Scene/Scene.h"
#include "../Scene/Input.h"

#include "BulletObject.h"
#include "TornadoBullet.h"
#include "TalonR.h"
#include "GravityBullet.h"

#include "../Component/SpriteComponent.h"
#include "../Share/Log.h"

#include "../UI/UserWidget/MainWidget.h"
#include "../Scene/SceneUIManager.h"
#include "../UI/Common/ProgressBar.h"

#include "../Pokemon/Scene/BattleWithPokemon.h"
#include "../Pokemon/UI/BattleWidget.h"



CPlayerObject::CPlayerObject()
	: CSceneObject()
{
}

CPlayerObject::CPlayerObject(const CPlayerObject& Obj)
	: CSceneObject(Obj)
{

}

CPlayerObject::CPlayerObject(CPlayerObject&& Obj)
	: CSceneObject(Obj)
{
}

CPlayerObject::~CPlayerObject()
{
}

bool CPlayerObject::Init()
{
	//UI 생성
	class CMainWidget* MainWidget = mScene->GetUIManager()->CreateWidget<CMainWidget>("Main");
	mScene->GetUIManager()->AddToViewport(MainWidget);

	


	// 게임 매니져 -> 씬매니져를 통해 -> 현재 씬을 실행시키고
	// 씬에서는 씬에 포함된 오브젝트들을 전부 순회하면서 시점 함수들을 호출해준다. 
	// 오브젝트들은 본인의 루트 컴포넌트를 호출해주면 
	// 루트 컴포넌트는 자식 컴포넌트들을 호출해준다. 

	// 컴포넌트 하나 등록해줄거다 .
	//mRoot = CreateComponent<CStaticMeshComponent>();
	mRoot = CreateComponent<CSpriteComponent>();
	mBody = CreateComponent<CColliderAABB2D>();
	//mBody = CreateComponent<CColliderSphere2D>();
	//mBody = CreateComponent<CColliderOBB2D>();
	//mLine = CreateComponent<CColliderLine2D>();

	mMovement = CreateComponent<CMovementComponent>();
	mRotation = CreateComponent<CRotationComponent>();
	mCamera = CreateComponent<CCameraComponent>();

	//mRoot->SetMesh("CenterTexRect");
	mRoot->SetTexture("BonoBono", TEXT("Texture/BonoBono.png"), 0);
	//mRoot->SetTint(0.1f, 0.1f, 0.8f);
	mRoot->SetPivot(0.5f, 0.5f);
	//mRoot->SetOpacity(1.f);
	//mRoot->SetShader("ColorMeshShader");

	mRoot->SetWorldPos(0.f, 0.f, 0.f);
	mRoot->SetWorldScale(100.f, 100.f, 1.f);
	SetRootComponent(mRoot);

	mBody->SetCollisionBeginFunc<CPlayerObject>(this, &CPlayerObject::IsOnCollision);
	mBody->SetCollisionEndFunc<CPlayerObject>(this, &CPlayerObject::IsOffCollision);

	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
	/*mAnimation->AddSequence("PlayerIdle", 1.f, 1.f, true, false);*/

	mAnimation->AddSequence("PlayerWalkDown", 1.f, 1.f, true, false);
	mAnimation->AddSequence("PlayerWalkUp", 1.f, 1.f, true, false);
	mAnimation->AddSequence("PlayerWalkRight", 1.f, 1.f, true, false);
	mAnimation->AddSequence("PlayerWalkLeft", 1.f, 1.f, true, false);

	mAnimation->AddSequence("PlayerAttack", 1.f, 0.6f, true, false);

	mAnimation->SetEndFunction("PlayerAttack", this, &CPlayerObject::AttackEnd);
	mAnimation->AddNotify("PlayerAttack", 1, this, &CPlayerObject::AttackNotify);

	mRoot->SetFlip(false);

	mRoot->AddChild(mBody);
	mBody->SetBoxSize(100.f, 100.f);
	mBody->SetCollisionProfile("Player");
	//mBody->SetRadius(50.f);

	//mBody->AddChild(mLine);
	//mLine->SetCollisionProfile("Player");
	//mLine->SetLineDistance(300.f);
	//mLine->SetRelativePos(0.f, 50.f);





	mMovement->SetUpdateComponent(mRoot);
	mMovement->SetMoveSpeed(200.f);

	mRotation->SetUpdateComponent(mRoot);

	//카메라 셋팅 
	mCamera->SetProjectionType(ECameraProjectionType::Ortho);
	mRoot->AddChild(mCamera);

	//입력
	mScene->GetInput()->AddBindKey("MoveUp", 'W');
	mScene->GetInput()->AddBindFunction("MoveUp", EInputType::Hold, this, &CPlayerObject::MoveUp);

	mScene->GetInput()->AddBindKey("MoveDown", 'S');
	mScene->GetInput()->AddBindFunction("MoveDown", EInputType::Hold, this, &CPlayerObject::MoveDown);

	mScene->GetInput()->AddBindKey("MoveRight", 'D');
	mScene->GetInput()->AddBindFunction("MoveRight", EInputType::Hold, this, &CPlayerObject::MoveRight);

	mScene->GetInput()->AddBindKey("MoveLeft", 'A');
	mScene->GetInput()->AddBindFunction("MoveLeft", EInputType::Hold, this, &CPlayerObject::MoveLeft);



	return true;
}

void CPlayerObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);



	if (mMovement->GetVelocityLength() == 0.f && mAutoBasePose)
	{
		mAnimation->ChangeAnimation("PlayerIdle");
	}

}



void CPlayerObject::MoveUp(float DeltaTime)
{
	//FVector3D Pos = mRootComponent->GetWorldPosition();
	////FVector3D Dir = { 0.f, 1.f, 0.f };
	//FVector3D Dir = mRootComponent->GetAxis(EAxis::Y);

	//mRootComponent->SetWorldPos(Pos + Dir*DeltaTime * 3.f);

	mAnimation->ChangeAnimation("PlayerWalkUp");
	mRoot->SetFlip(false);

	if (Block)
	{
		return;
	}

	mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y));
}

void CPlayerObject::MoveDown(float DeltaTime)
{
	/*FVector3D Pos = mRootComponent->GetWorldPosition();
	FVector3D Dir = mRootComponent->GetAxis(EAxis::Y);
	mRootComponent->SetWorldPos(Pos + Dir * DeltaTime * -3.f);*/
	mAnimation->ChangeAnimation("PlayerWalkDown");
	mRoot->SetFlip(false);

	if (Block)
	{
		return;
	}

	mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y) * -1);
}

void CPlayerObject::MoveRight(float DeltaTime)
{
	mAnimation->ChangeAnimation("PlayerWalkRight");
	mRoot->SetFlip(false);
	if (Block)
	{
		return;
	}

	mMovement->AddMove(mRootComponent->GetAxis(EAxis::X) * 1);
}

void CPlayerObject::MoveLeft(float DeltaTime)
{
	mAnimation->ChangeAnimation("PlayerWalkLeft");
	mRoot->SetFlip(false);
	if (Block)
	{
		return;
	}
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::X) * -1);
}

void CPlayerObject::IsOnCollision(const FVector3D& HitPoint, CColliderBase* Dest)
{
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::None));
	Block = true;
}

void CPlayerObject::IsOffCollision(CColliderBase* Dest)
{
	//mMovement->AddMove(mRootComponent->GetAxis(EAxis::None));
	Block = false;
}



void CPlayerObject::rotationZ(float DeltaTime)
{
	//FVector3D Rot = mRootComponent->GetWorldRotation();
	//mRootComponent->SetWorldRotationZ(Rot.z + 90.f * DeltaTime);

	mRotation->AddMoveZ(-90.f);
}

void CPlayerObject::rotationZInv(float DeltaTime)
{
	/*FVector3D Rot = mRootComponent->GetWorldRotation();
	mRootComponent->SetWorldRotationZ(Rot.z + -90.f * DeltaTime);*/

	mRotation->AddMoveZ(90.f);
}



void CPlayerObject::AttackEnd()
{
	mAutoBasePose = true;
}

void CPlayerObject::AttackNotify()
{
}

