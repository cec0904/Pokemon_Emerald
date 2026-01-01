#include "BattleWithPokemon.h"

#include "../../Scene/SceneUIManager.h"
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

    CBattleWidget* Widget = mUIManager->CreateWidget<CBattleWidget>("BattleWithPokemon");
    mUIManager->AddToViewport(Widget);

    static FPokemonInstance sTestPlayer;


    if (sTestPlayer.SpeciesID == 0)
    {
        sTestPlayer.SpeciesID = 1;
        sTestPlayer.Level = 5;

        const auto& idMap = CPokemonManager::GetInst()->GetIDMap();
        const auto& statMap = CPokemonManager::GetInst()->GetDefaultStatMap();
        const auto& posMap = CPokemonManager::GetInst()->GetPosMap();

        sTestPlayer.Info = idMap.at(1);
        sTestPlayer.ImageInfo = posMap.at(1);

        FBaseStats base = statMap.at(1);
        int lv = sTestPlayer.Level;
        sTestPlayer.CurrentState.HP = ((base.HP * 2) * lv / 100) + 10 + lv;
        sTestPlayer.CurrentHP = sTestPlayer.CurrentState.HP;
        sTestPlayer.CurrentState.Atk = ((base.Atk * 2) * lv / 100) + 5;
        sTestPlayer.CurrentState.Def = ((base.Def * 2) * lv / 100) + 5;
        sTestPlayer.CurrentState.SpAtk = ((base.SpAtk * 2) * lv / 100) + 5;
        sTestPlayer.CurrentState.SpDef = ((base.SpDef * 2) * lv / 100) + 5;
        sTestPlayer.CurrentState.Spd = ((base.Spd * 2) * lv / 100) + 5;

        CPokemonManager::GetInst()->BuildPokemonMoves(sTestPlayer);
    }
    CLog::PrintLog(std::string("Info.Name=") + sTestPlayer.Info.Name + "\n");

    Widget->SetPlayerPokemon(&sTestPlayer); 

    return true;
}
