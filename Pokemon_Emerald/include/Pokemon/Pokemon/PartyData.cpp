#include "PartyData.h"
#include "PokemonManager.h"

bool CPartyData::AddPokemon(int id, int level)
{
    if (mPokemonParty.size() >= 6) return false;

    FPokemonInstance inst;
    if (!CPokemonManager::GetInst()->BuildPokemonInstance(inst, id, level))
        return false;

    mPokemonParty.push_back(inst);
    return true;
}

bool CPartyData::AddPokemonInstance(const FPokemonInstance& inst)
{
    if (mPokemonParty.size() >= 6)
    {
        return false;
    }
    mPokemonParty.push_back(inst);
    return true;
    
}
