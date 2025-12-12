#include "EditorPlayer.h"
#include "../Component/CameraComponent.h"
#include "../Component/MovementComponent.h"
#include "../Scene/Input.h"
#include "../Scene/Scene.h"
#include "../Share/Log.h"

CEditorPlayer::CEditorPlayer()
{
}

CEditorPlayer::CEditorPlayer(const CEditorPlayer& Obj)
	:	CSceneObject(Obj)
{
}

CEditorPlayer::CEditorPlayer(CEditorPlayer&& Obj)
	: CSceneObject(Obj)
{
}

CEditorPlayer::~CEditorPlayer()
{
}

bool CEditorPlayer::Init()
{
	CSceneObject::Init();

	mCamera = CreateComponent<CCameraComponent>();
	mMovement = CreateComponent<CMovementComponent>();

	//카메라 타입을 직교로 변경해준다. 
	mCamera->SetProjectionType(ECameraProjectionType::Ortho);

	SetRootComponent(mCamera);

	mMovement->SetUpdateComponent(mCamera);
	mMovement->SetMoveSpeed(500.f);

	mScene->GetInput()->AddBindKey("MoveUp", 'W');
	mScene->GetInput()->AddBindKey("MoveDown", 'S');
	mScene->GetInput()->AddBindKey("MoveLeft", 'A');
	mScene->GetInput()->AddBindKey("MoveRight", 'D');

	mScene->GetInput()->AddBindFunction("MoveUp", EInputType::Hold, this, &CEditorPlayer::MoveUp);
	mScene->GetInput()->AddBindFunction("MoveDown", EInputType::Hold, this, &CEditorPlayer::MoveDown);
	mScene->GetInput()->AddBindFunction("MoveLeft", EInputType::Hold, this, &CEditorPlayer::MoveLeft);
	mScene->GetInput()->AddBindFunction("MoveRight", EInputType::Hold, this, &CEditorPlayer::MoveRight);

	return true;
}

void CEditorPlayer::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	string PosLog = "X : " + to_string(GetWorldPosition().x) + "    Y : " + to_string(GetWorldPosition().y);
	//CLog::PrintLog(PosLog);
}

void CEditorPlayer::MoveUp(float DeltaTime)
{
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y));
}

void CEditorPlayer::MoveDown(float DeltaTime)
{
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y) * -1.f);
}

void CEditorPlayer::MoveLeft(float DeltaTime)
{
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::X) * -1.f);
}

void CEditorPlayer::MoveRight(float DeltaTime)
{
	mMovement->AddMove(mRootComponent->GetAxis(EAxis::X));
}
