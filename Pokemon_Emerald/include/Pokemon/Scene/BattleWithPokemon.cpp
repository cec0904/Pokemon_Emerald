#include "BattleWithPokemon.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneMain.h"
#include "../../UI/UserWidget/MainWidget.h"
#include "../UI/BattleWidget.h"
#include "../Pokemon/PokemonManager.h"
#include "../../Share/Log.h"
#include "../Pokemon/PartyData.h"
#include "../UI/PartyUI.h"
#include "../UI/PokemonBattleIntro.h"


CBattleWithPokemon::CBattleWithPokemon()
{
}

CBattleWithPokemon::~CBattleWithPokemon()
{
}

bool CBattleWithPokemon::Init()
{
	if (!CScene::Init())
		return false;

	mBattleWidget = mUIManager->CreateWidget<CBattleWidget>("BattleWithPokemon");
	if (!mBattleWidget)
	{
		CLog::PrintLog("CreateWidget<CBattleWidget> failed");
		return false;
	}
	mUIManager->AddToViewport(mBattleWidget.Get());


	mBattleWidget->SetEnable(false);

	static CPartyData sPlayerParty;
	static bool sInited = false;

	if (!sInited)
	{
		sInited = true;
		sPlayerParty.AddPokemon(1, 2);
		sPlayerParty.AddPokemon(2, 5);
		sPlayerParty.AddPokemon(3, 5);
		sPlayerParty.AddPokemon(4, 5);
		sPlayerParty.AddPokemon(7, 5);
	}

	srand((unsigned int)time(NULL));

	int enemyId = CPokemonManager::GetInst()->GetPendingEnemyID();
	
	enemyId = (rand() % 28 + 1);
	//enemyId = 15;
	//enemyId = 12;

	static FPokemonInstance sTestEnemy;
	CPokemonManager::GetInst()->BuildPokemonInstance(sTestEnemy, enemyId, 3);

	CSharedPtr<CPartyUI> partyUI = mUIManager->CreateWidget<CPartyUI>("PartyUI");
	mUIManager->AddToViewport(partyUI.Get());
	partyUI->SetEnable(false);

	partyUI->SetPartyData(&sPlayerParty);
	partyUI->SetBattleWidget(mBattleWidget.Get());
	mBattleWidget->SetPartyUI(partyUI.Get());

	mBattleWidget->SetPlayerPokemon(sPlayerParty.GetActivePokemonPtr());
	mBattleWidget->SetEnemyPokemon(&sTestEnemy);


	mIntroWidget = mUIManager->CreateWidget<CPokemonBattleIntro>("BattleIntro");
	if (!mIntroWidget)
	{
		CLog::PrintLog("CreateWidget<CPokemonBattleIntro> failed");
		return false;
	}
	mUIManager->AddToViewport(mIntroWidget.Get());

	mIntroWidget->Start(mBattleWidget.Get(), sPlayerParty.GetActivePokemonPtr(), &sTestEnemy);


	mBattleWidget->SetEnable(true);


	return true;
}

void CBattleWithPokemon::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);


	if (mIntroWidget)
	{
		if (mIntroWidget->IsFinished())
		{
			mIntroWidget->SetEnable(false);
			mIntroWidget = nullptr;

			if (mBattleWidget)
			{
				mBattleWidget->SetEnable(true);
				mBattleWidget->OnOpenedFromIntro();
			}
		}
		return;
	}

	if (!mBattleWidget)
		return;

	if (mBattleWidget->IsExitBattleRequested())
	{
		mBattleWidget->ClearExitBattleRequest();
		mBattleWidget->SetEnable(false);
		mBattleWidget = nullptr;

		CSceneManager::GetInst()->PopScene();
		return;
	}

}