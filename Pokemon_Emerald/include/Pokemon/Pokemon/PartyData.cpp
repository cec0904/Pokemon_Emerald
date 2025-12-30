#include "PartyData.h"
#include "PokemonManager.h"

bool CPartyData::AddPokemon(int id, int level)
{
	if (mPokemonParty.size() >= 6)
		return false;

	auto& IDMap = CPokemonManager::GetInst()->GetIDMap();
	auto& StatMap = CPokemonManager::GetInst()->GetDefaultStatMap();
	auto& PosMap = CPokemonManager::GetInst()->GetPosMap();

	if (IDMap.find(id) == IDMap.end())
		return false;

	// (안전) Stat/Pos도 혹시 없으면 방지
	if (StatMap.find(id) == StatMap.end()) return false;
	if (PosMap.find(id) == PosMap.end())  return false;

	FPokemonInstance newInstance;
	newInstance.SpeciesID = id;
	newInstance.Info = IDMap.at(id);
	newInstance.Level = level;
	newInstance.ImageInfo = PosMap.at(id);

	FBaseStats base = StatMap.at(id);

	// hp = ((종족값 * 2) * 레벨 / 100) + 10 + 레벨
	newInstance.CurrentState.HP = ((base.HP * 2) * level / 100) + 10 + level;
	newInstance.CurrentHP = newInstance.CurrentState.HP;

	// 나머지 능력치 = (종족값 * 2) * 레벨 / 100 + 5
	newInstance.CurrentState.Atk = ((base.Atk * 2) * level / 100) + 5;
	newInstance.CurrentState.Def = ((base.Def * 2) * level / 100) + 5;
	newInstance.CurrentState.SpAtk = ((base.SpAtk * 2) * level / 100) + 5;
	newInstance.CurrentState.SpDef = ((base.SpDef * 2) * level / 100) + 5;
	newInstance.CurrentState.Spd = ((base.Spd * 2) * level / 100) + 5;

	// 기술/PP 채우기
	CPokemonManager::GetInst()->BuildPokemonMoves(newInstance);

	mPokemonParty.push_back(newInstance);
	return true;
}
