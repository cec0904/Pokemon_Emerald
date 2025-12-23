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

	///////////// 사용할 머티리얼 미리 만든다!!
	mAssetManager->CreateMaterial("Monster1");

	CMaterial* material = mAssetManager->FindMaterial("Monster1");

	material->SetPixelShader("DefaultMaterialShader");
	material->SetSamplerType(ETextureSamplerType::Linear);
	material->AddTexture("MonsterTex", TEXT("Texture/Porori.png"), 0);



	///////////// 사용할 사운드도 미리 추가한다.
	mAssetManager->LoadSound("Hit", "Effect", false, "Sound/Fire1.wav");
	mAssetManager->LoadSound("SceneMainBack", "BGM", true, "파일");
	CAssetManager::GetInst()->GetSoundManager()->Play("SceneMainBack");


	///////////////////////////////////////////////
	//백그라운드 오브젝트 
	/*CBackObject* BackGorundObject = CreateObj<CBackObject>("BackGroundObj");
	BackGorundObject->SetWorldScale(3000.f, 1080.f);*/

	//백그라운드 오브젝트 
	/*CBackObject* BackGorundObject = CreateObj<CBackObject>("BackGroundObj");
	BackGorundObject->SetWorldScale(3000.f, 1080.f);
	BackGorundObject->setFlip(true);*/

	CTileMapObj* BackTileObject = CreateObj<CTileMapObj>("BackTileObj");
	BackTileObject->Load("Second2222.tlm");




	CPlayerObject* Player = CreateObj<CPlayerObject>("Player");

	if (Player == nullptr)
		return false;

	

	return true;
}