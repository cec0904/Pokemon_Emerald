#include "BattleWithPokemon.h"

#include "../../Scene/SceneUIManager.h"
#include "../../UI/UserWidget/MainWidget.h"
#include "../UI/BattleWidget.h"

CBattleWithPokemon::CBattleWithPokemon()
{
}

CBattleWithPokemon::~CBattleWithPokemon()
{
}

bool CBattleWithPokemon::Init()
{
	if (!CScene::Init())
	{
		return false;
	}

	// ui
	CBattleWidget* Widget = mUIManager->CreateWidget<CBattleWidget>("BattleWithPokemon");

	mUIManager->AddToViewport(Widget);








	return true;
}
