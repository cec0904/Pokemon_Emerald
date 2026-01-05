#include "SceneMain.h"

#include "SceneAssetManager.h"
#include "SceneUIManager.h"
#include "../Asset/Material/Material.h"
#include "../Object/PlayerObject.h"
#include "../Object/MonsterObject.h"
#include "../Object/GunnerMonster.h"
#include "../Object/NearingMonster.h"
#include "../Object/BackObject.h"
#include "../Object/ObjectSpawnPoint.h"
#include "../Object/TileMapObj.h"
#include "../UI/UserWidget/MainWidget.h"
#include "../Asset/Sound/SoundManager.h"
#include "../Asset/AssetManager.h"
#include "../Component/TileMapComponent.h"
#include "../Pokemon/Npc/NpcBase.h"
#include "../Pokemon/Npc/NpcProfessor.h"

CSceneMain::CSceneMain()
{
}

CSceneMain::~CSceneMain()
{
	CAssetManager::GetInst()->GetSoundManager()->Stop("SceneMainBack");
}

bool CSceneMain::Init()
{
	if (!CScene::Init())
	{
		return false;
	}


	///////////////////////////////////////////////
	//백그라운드 오브젝트 
	/*CBackObject* BackGorundObject = CreateObj<CBackObject>("BackGroundObj");
	BackGorundObject->SetWorldScale(3000.f, 1080.f);*/

	//백그라운드 오브젝트 
	/*CBackObject* BackGorundObject = CreateObj<CBackObject>("BackGroundObj");
	BackGorundObject->SetWorldScale(3000.f, 1080.f);
	BackGorundObject->setFlip(true);*/

	CTileMapObj* BackTileObject = CreateObj<CTileMapObj>("BackTileObj");
	BackTileObject->Load("WorldMap.tlm");




	CPlayerObject* Player = CreateObj<CPlayerObject>("Player");

	if (Player == nullptr)
		return false;

	mPlayer = Player;
	Player->SetTileMap(BackTileObject->GetTileMapComponent());

	CSceneAssetManager* AM = GetAssetManager();

	AM->LoadTexture("ProfessorTex", TEXT("Texture/Pokemon/Npc/NpcRealWorld.png"));

	AM->CreateAnimation("ProfessorIdle");
	AM->SetAnimationTextureType("ProfessorIdle", EAnimationTextureType::SpriteSheet);
	AM->SetAnimationTexture("ProfessorIdle", "ProfessorTex");
	AM->AddAnimationFrame("ProfessorIdle", 0.f, 428.f, 18.f, 20.f);

	CNpcProfessor* Professor = CreateObj<CNpcProfessor>("Professor");
	mProfessor = Professor;

	Professor->SetWorldScale(72.f, 80.f);


	const float Cell = 16.f * 4.f;


	Professor->SetWorldPos((138.f + 0.5f) * Cell, (41.f + 0.5f) * Cell);


	CSpriteComponent* Spr = Professor->GetRootSprite();
	if (Spr)
	{

		Spr->SetTexture("NpcProfessor");
		Spr->SetUseColorKey(true);
		Spr->SetColorKey(FVector3D(44.f / 255.f, 142.f / 255.f, 96.f / 255.f));
		Spr->SetKeyThreshold(0.01f);
		Spr->SetPivot(0.5f, 0.25f);


		CAnimation2D* Anim = Spr->CreateAnimation2D<CAnimation2D>(); 
		if (Anim)
		{
			Anim->AddSequence("ProfessorIdle", 1.f, 1.f, true, false);
			Anim->ChangeAnimation("ProfessorIdle");
		}
	}



	return true;
}