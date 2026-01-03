#include "BattleWithPokemon.h"

#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneMain.h"
#include "../../UI/UserWidget/MainWidget.h"
#include "../UI/BattleWidget.h"
#include "../Pokemon/PokemonManager.h"
#include "../../Share/Log.h"

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
    mUIManager->AddToViewport(mBattleWidget.Get());

    static FPokemonInstance sTestPlayer;
    static FPokemonInstance sTestEnemy;

    if (sTestPlayer.SpeciesID == 0)
        CPokemonManager::GetInst()->BuildPokemonInstance(sTestPlayer, 1, 5);

    if (sTestEnemy.SpeciesID == 0)
        CPokemonManager::GetInst()->BuildPokemonInstance(sTestEnemy, 4, 5);

    mBattleWidget->SetPlayerPokemon(&sTestPlayer);
    mBattleWidget->SetEnemyPokemon(&sTestEnemy);

    return true;
}

void CBattleWithPokemon::Update(float DeltaTime)
{
    CScene::Update(DeltaTime);

    if (!mBattleWidget)
        return;

    if (mBattleWidget->IsExitBattleRequested())
    {
        mBattleWidget->ClearExitBattleRequest();

        mBattleWidget->SetEnable(false);
        mBattleWidget = nullptr;

        CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
        return;
    }
}
