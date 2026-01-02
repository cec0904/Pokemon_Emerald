#include "EnemyBattleAI.h"
#include "../Pokemon/PokemonManager.h"
#include <cstdlib>

float CEnemyBattleAI::GetStatusChanceByHp(float hpPercent) const
{

    if (hpPercent <= 0.30f) return 0.10f;
    if (hpPercent <= 0.50f) return 0.20f;
    if (hpPercent <= 0.70f) return 0.50f;
    return 0.50f;
}

MoveID CEnemyBattleAI::ChooseMove(const FPokemonInstance& enemy)
{
    int maxHP = enemy.CurrentState.HP;
    int curHP = enemy.CurrentHP;

    float hpPercent = 0.f;
    if (maxHP > 0) hpPercent = (float)curHP / (float)maxHP;

    float statusChance = GetStatusChanceByHp(hpPercent);
    float r = (float)(rand() % 1000) / 1000.f;
    bool wantStatus = (r < statusChance);

    const auto& db = CPokemonManager::GetInst()->GetMoveDB();

    MoveID candidates[4] = { 0,0,0,0 };
    int count = 0;


    for (int i = 0; i < (int)enemy.Moves.size() && i < 4; ++i)
    {
        MoveID id = enemy.Moves[i];
        auto it = db.find(id);
        if (it == db.end()) continue;


        bool isStatus = (it->second.Category == EMoveCategory::Status);

        if (wantStatus == isStatus)
            candidates[count++] = id;
    }


    if (count == 0)
    {
        for (int i = 0; i < (int)enemy.Moves.size() && i < 4; ++i)
        {
            MoveID id = enemy.Moves[i];
            if (db.find(id) != db.end())
                candidates[count++] = id;
        }
    }

    if (count == 0) return 0;
    return candidates[rand() % count];
}
