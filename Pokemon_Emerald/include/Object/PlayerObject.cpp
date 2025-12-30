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

#include "../Pokemon/Pokemon/PartyData.h"
#include "../Pokemon/Pokemon/Pokemon.h"
#include "../Pokemon/Pokemon/PokemonManager.h"

#include "../Pokemon/Scene/BattleWithPokemon.h"
#include "../Pokemon/UI/BattleWidget.h"
#include "../Pokemon/UI/MenuUI.h"
#include "../Pokemon/UI/Inventory.h"
#include "../Pokemon/UI/PartyUI.h"
#include "../Pokemon/UI/FadeWidget.h"

#include "../Component/TileMapComponent.h"



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

void CPlayerObject::TransPort(int SpawnIndex)
{
	if (!mTileMap) return;

	int CountX = mTileMap->GetTileCountX();
	int X = SpawnIndex % CountX;
	int Y = SpawnIndex / CountX;

	FVector2D Center = mTileMap->GetTileCenter(X, Y);
	if (Center.x < 0.f || Center.y < 0.f)
	{
		return;
	}


	FVector3D CurPos = mRoot->GetWorldPosition();
	mRoot->SetWorldPos(Center.x, Center.y, CurPos.z);

	mTileMoving = false;
}

bool CPlayerObject::Init()
{
	//UI 생성
	class CMainWidget* MainWidget = mScene->GetUIManager()->CreateWidget<CMainWidget>("Main");
	mScene->GetUIManager()->AddToViewport(MainWidget);

	mParty = mScene->GetUIManager()->CreateWidget<CPartyUI>("Party");
	mScene->GetUIManager()->AddToViewport(mParty.Get());
	mParty->SetEnable(false);

	m_pParty = mParty.Get();
	if (m_pParty)
	{

		m_pParty->SetPartyData(&mPartyData);
	}

	mPartyData.AddPokemon(1, 5);
	mPartyData.AddPokemon(4, 5);
	mPartyData.AddPokemon(7, 5);
	mPartyData.AddPokemon(10, 5);
	mPartyData.AddPokemon(13, 5);
	mPartyData.AddPokemon(16, 5);


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
	mRoot->SetPivot(0.5f, 0.25f);
	//mRoot->SetOpacity(1.f);
	//mRoot->SetShader("ColorMeshShader");

	mRoot->SetWorldPos(352.f, 160.f, 0.f);
	mRoot->SetWorldScale(64.f, 128.f, 1.f);
	SetRootComponent(mRoot);
	mRoot->SetUseColorKey(true);
	mRoot->SetColorKey(FVector3D(44.f / 255.f, 142.f / 255.f, 95.f / 255.f));
	mRoot->SetKeyThreshold(0.1f);


	mBody->SetCollisionBeginFunc<CPlayerObject>(this, &CPlayerObject::IsOnCollision);
	mBody->SetCollisionEndFunc<CPlayerObject>(this, &CPlayerObject::IsOffCollision);

	mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
	/*mAnimation->AddSequence("PlayerIdle", 1.f, 1.f, true, false);*/

	mAnimation->AddSequence("PlayerWalkDown", 1.f, 0.08f, true, false);
	mAnimation->AddSequence("PlayerWalkUp", 1.f, 0.08f, true, false);
	mAnimation->AddSequence("PlayerWalkLeft", 1.f, 0.08f, true, false);


	mAnimation->AddSequence("PlayerIdleDown", 1.f, 1.f, true, false);
	mAnimation->AddSequence("PlayerIdleUp", 1.f, 1.f, true, false);
	mAnimation->AddSequence("PlayerIdleLeft", 1.f, 1.f, true, false);

	mRoot->SetFlip(false);

	mRoot->AddChild(mBody);
	mBody->SetBoxSize(64.f, 64.f);
	mBody->SetCollisionProfile("Player");

	mMovement->SetUpdateComponent(mRoot);
	mMovement->SetMoveSpeed(200.f);

	mRotation->SetUpdateComponent(mRoot);


	//카메라 셋팅 
	mCamera->SetProjectionType(ECameraProjectionType::Ortho);
	mRoot->AddChild(mCamera);

	//입력
	mScene->GetInput()->AddBindKey("MoveUp", VK_UP);
	mScene->GetInput()->AddBindFunction("MoveUp", EInputType::Hold, this, &CPlayerObject::MoveUp);

	mScene->GetInput()->AddBindKey("MoveDown", VK_DOWN);
	mScene->GetInput()->AddBindFunction("MoveDown", EInputType::Hold, this, &CPlayerObject::MoveDown);

	mScene->GetInput()->AddBindKey("MoveRight", VK_RIGHT);
	mScene->GetInput()->AddBindFunction("MoveRight", EInputType::Hold, this, &CPlayerObject::MoveRight);

	mScene->GetInput()->AddBindKey("MoveLeft", VK_LEFT);
	mScene->GetInput()->AddBindFunction("MoveLeft", EInputType::Hold, this, &CPlayerObject::MoveLeft);

	mScene->GetInput()->AddBindKey("Menu", VK_TAB);
	mScene->GetInput()->AddBindFunction("Menu", EInputType::Down, this, &CPlayerObject::Menu);

	mScene->GetInput()->AddBindKey("Accept", 'D');
	mScene->GetInput()->AddBindFunction("Accept", EInputType::Down, this, &CPlayerObject::Accept);

	mScene->GetInput()->AddBindKey("Cancel", 'S');
	mScene->GetInput()->AddBindFunction("Cancel", EInputType::Down, this, &CPlayerObject::Cancel);

	mScene->GetInput()->AddBindKey("MenuUp", VK_UP);
	mScene->GetInput()->AddBindFunction("MenuUp", EInputType::Down, this, &CPlayerObject::MenuUp);

	mScene->GetInput()->AddBindKey("MenuDown", VK_DOWN);
	mScene->GetInput()->AddBindFunction("MenuDown", EInputType::Down, this, &CPlayerObject::MenuDown);

	mAnimation->ChangeAnimation("PlayerIdleDown");
	mRoot->SetFlip(false);


	mPortals.clear();

	// 포탈 위치, 포탈 타고 왔을 때
	// 집 거실
	mPortals.push_back({ 1069, 911 });
	mPortals.push_back({ 1089, 891 });

	// 거실 밖
	mPortals.push_back({ 20, 4769 });
	mPortals.push_back({ 4947, 199 });

	// 포켓몬 센터
	mPortals.push_back({ 10991, 25028 });
	mPortals.push_back({ 24850, 10813 });

	mPortals.push_back({ 8569, 25028 });
	mPortals.push_back({ 24850, 8391 });

	mPortals.push_back({ 12705, 25028 });
	mPortals.push_back({ 24850, 12527 });

	mPortals.push_back({ 25379, 25534 });
	mPortals.push_back({ 25533, 25380 });


	// 포켓스토어
	mPortals.push_back({12779, 25227});
	mPortals.push_back({25049, 12601});
	
	mPortals.push_back({ 7323, 25227 });
	mPortals.push_back({ 25049, 7145 });

	mPortals.push_back({ 13422, 25227 });
	mPortals.push_back({ 25049, 13244 });




	return true;
}

void CPlayerObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (mTileMoving)
	{
		FVector3D CurPos = mRoot->GetWorldPosition();
		FVector3D Dir = mTileTargetPos - CurPos;

		float Dist = Dir.Length();
		float Step = mTileMoveSpeed * DeltaTime;

		if (Dist <= Step)
		{
			// 정확히 타일 중앙에 스냅
			mRoot->SetWorldPos(mTileTargetPos);
			mTileMoving = false;

			int CurrentIndex = mTileMap->GetTileIndex(FVector2D(mTileTargetPos.x, mTileTargetPos.y));

			for (const FPortal& P : mPortals)
			{
				if (CurrentIndex == P.PortalIndex)
				{
					TransPort(P.SpawnIndex);

					break;
				}
			}

			/*for (int i = 0; i < mPortals.size(); ++i)
			{
				const FPortal& P = mPortals[i];

				if (CurrentIndex == P.PortalIndex)
				{
					TransPort(P.SpawnIndex);
					break;
				}
			}*/


			if (mAutoBasePose)
			{
				switch (mMoveState)
				{
				case MoveState::Up:
					mAnimation->ChangeAnimation("PlayerIdleUp");
					mRoot->SetFlip(false);
					break;
				case MoveState::Down:
					mAnimation->ChangeAnimation("PlayerIdleDown");
					mRoot->SetFlip(false);
					break;
				case MoveState::Left:
					mAnimation->ChangeAnimation("PlayerIdleLeft");
					mRoot->SetFlip(false);
					break;
				case MoveState::Right:
					mAnimation->ChangeAnimation("PlayerIdleLeft");
					mRoot->SetFlip(true);
					break;
				}
			}
		}
		else
		{
			Dir.Normalize();
			mRoot->SetWorldPos(CurPos + Dir * Step);
		}


		return;
	}
}






void CPlayerObject::MoveUp(float DeltaTime)
{
	//FVector3D Pos = mRootComponent->GetWorldPosition();
	////FVector3D Dir = { 0.f, 1.f, 0.f };
	//FVector3D Dir = mRootComponent->GetAxis(EAxis::Y);

	//mRootComponent->SetWorldPos(Pos + Dir*DeltaTime * 3.f);
	if (IsMenuOpen || IsPartyOpen)
	{
		return;
	}
	if (Block)
	{
		return;
	}
	if (mTileMoving) return;
	if (!mTileMap) return;


	FVector3D CurPos = mRoot->GetWorldPosition();

	int CurIndex = mTileMap->GetTileIndex(FVector2D(CurPos.x, CurPos.y));
	if (CurIndex == -1) return;

	int CountX = mTileMap->GetTileCountX();
	int CurX = CurIndex % CountX;
	int CurY = CurIndex / CountX;

	int NextX = CurX;
	int NextY = CurY + 1;


	int NextIndex = NextY * CountX + NextX;

	if (mTileMap->GetTileType(NextIndex) == ETileType::UnableToMove)
	{
		return;
	}

	FVector2D Center = mTileMap->GetTileCenter(NextX, NextY);
	if (Center.x < 0.f || Center.y < 0.f)
	{
		return;
	}
	mTileTargetPos = FVector3D(Center.x, Center.y, CurPos.z);
	mTileMoving = true;

	//CLog::PrintLog("Change Walk");
	mAnimation->ChangeAnimation("PlayerWalkUp");
	mRoot->SetFlip(false);
	mMoveState = MoveState::Up;
}

void CPlayerObject::MoveDown(float DeltaTime)
{
	if (IsMenuOpen || IsPartyOpen) return;
	if (mTileMoving) return;
	if (!mTileMap) return;

	FVector3D CurPos = mRoot->GetWorldPosition();

	int CurIndex = mTileMap->GetTileIndex(FVector2D(CurPos.x, CurPos.y));
	if (CurIndex == -1)
	{
		return;
	}
	int CountX = mTileMap->GetTileCountX();
	int CurX = CurIndex % CountX;
	int CurY = CurIndex / CountX;

	int NextX = CurX;
	int NextY = CurY - 1;

	if (NextY < 0)
	{
		return;
	}
	int NextIndex = NextY * CountX + NextX;

	if (mTileMap->GetTileType(NextIndex) == ETileType::UnableToMove)
	{
		return;
	}

	FVector2D Center = mTileMap->GetTileCenter(NextX, NextY);
	if (Center.x < 0.f || Center.y < 0.f)
	{
		return;
	}
	mTileTargetPos = FVector3D(Center.x, Center.y, CurPos.z);
	mTileMoving = true;


	mAnimation->ChangeAnimation("PlayerWalkDown");
	mRoot->SetFlip(false);
	mMoveState = MoveState::Down;

}


void CPlayerObject::MoveRight(float DeltaTime)
{
	if (IsMenuOpen || IsPartyOpen) return;
	if (mTileMoving) return;
	if (!mTileMap) return;

	FVector3D CurPos = mRoot->GetWorldPosition();

	int CurIndex = mTileMap->GetTileIndex(FVector2D(CurPos.x, CurPos.y));
	if (CurIndex == -1)
	{
		return;
	}
	int CountX = mTileMap->GetTileCountX();
	int CurX = CurIndex % CountX;
	int CurY = CurIndex / CountX;

	int NextX = CurX + 1;
	int NextY = CurY;

	if (NextX >= mTileMap->GetTileCountX())
	{
		return;
	}
	int NextIndex = NextY * CountX + NextX;

	if (mTileMap->GetTileType(NextIndex) == ETileType::UnableToMove)
	{
		return;
	}

	FVector2D Center = mTileMap->GetTileCenter(NextX, NextY);
	if (Center.x < 0.f || Center.y < 0.f)
	{
		return;
	}
	mTileTargetPos = FVector3D(Center.x, Center.y, CurPos.z);
	mTileMoving = true;

	mAnimation->ChangeAnimation("PlayerWalkLeft");
	mRoot->SetFlip(true);

	mMoveState = MoveState::Right;
}


void CPlayerObject::MoveLeft(float DeltaTime)
{
	if (IsMenuOpen || IsPartyOpen) return;
	if (mTileMoving) return;
	if (!mTileMap) return;

	FVector3D CurPos = mRoot->GetWorldPosition();

	int CurIndex = mTileMap->GetTileIndex(FVector2D(CurPos.x, CurPos.y));
	if (CurIndex == -1)
	{
		return;
	}
	int CountX = mTileMap->GetTileCountX();
	int CurX = CurIndex % CountX;
	int CurY = CurIndex / CountX;

	int NextX = CurX - 1;
	int NextY = CurY;

	if (NextX < 0)
	{
		return;
	}
	int NextIndex = NextY * CountX + NextX;

	if (mTileMap->GetTileType(NextIndex) == ETileType::UnableToMove)
	{
		return;
	}
	FVector2D Center = mTileMap->GetTileCenter(NextX, NextY);
	if (Center.x < 0.f || Center.y < 0.f)
	{
		return;
	}
	mTileTargetPos = FVector3D(Center.x, Center.y, CurPos.z);
	mTileMoving = true;

	mAnimation->ChangeAnimation("PlayerWalkLeft");
	mRoot->SetFlip(false);

	mMoveState = MoveState::Left;
}


void CPlayerObject::Menu(float DeltaTime)
{
	if (!mMenuUI)
	{
		mMenuUI = mScene->GetUIManager()->CreateWidget<CMenuUI>("FieldMenu");
		mScene->GetUIManager()->AddToViewport(mMenuUI.Get());
		mMenuUI->SetEnable(false);
	}

	IsMenuOpen = !IsMenuOpen;
	mMenuUI->SetEnable(IsMenuOpen);
}

void CPlayerObject::Party(float DeltaTime)
{
	if (IsPartyOpen)
	{
		// 닫기
		if (mParty) mParty->SetEnable(false);
		IsPartyOpen = false;
		return;
	}

	
}

void CPlayerObject::Accept(float DeltaTime)
{
	if (!IsMenuOpen)
		return;

	

	mMenuUI->Select();
}

void CPlayerObject::Cancel(float DeltaTime)
{
	if (!IsMenuOpen)
		return;

	mMenuUI->SetEnable(false);
	IsMenuOpen = false;

	if (mParty)
	{
		mParty->SetEnable(false);
	}
}

void CPlayerObject::MenuUp(float DeltaTime)
{
	if (IsMenuOpen && mMenuUI)
	{
		mMenuUI->MoveUp();
	}
}

void CPlayerObject::MenuDown(float DeltaTime)
{
	if (IsMenuOpen && mMenuUI)
	{
		mMenuUI->MoveDown();
	}
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





