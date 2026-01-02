#pragma once
#include "../../PokemonData.h"

class CEnemyBattleAI
{
public:
	MoveID ChooseMove(const FPokemonInstance& enemy);

private:
	float GetStatusChanceByHp(float hpPercent) const;
};

