#pragma once
#include "../../Gameinfo.h"
#include "../../PokemonData.h"

class CPokemonManager 
{


private:
	//포켓몬 도감 
	unordered_map<PokemonID, FPokemonDefaultInfo> PokemonIDMap;
	unordered_map<PokemonID, FBaseStats> PokemonDefaultStateMap;
	unordered_map<PokemonID, FPokemonSpritePos> PokemonSpritePosInfoMap;

	// 포켓몬 기술
	unordered_map<MoveID, FMoveData> MoveDataMap;
	unordered_map<PokemonID, vector<pair<int, MoveID>>> PokemonSkillSet;

public:
	bool Init();

public:
	void LoadSpecies();
	void LoadSkill();
	void BuildSkillSetTable();
	void Evolve(FPokemonInstance& inst, PokemonID nextID) const;
	void LevelChange(FPokemonInstance& inst) const;
	
	const unordered_map<int, FPokemonDefaultInfo>& GetIDMap() const
	{
		return PokemonIDMap;
	}

	const unordered_map<int, FBaseStats>& GetDefaultStatMap() const
	{
		return PokemonDefaultStateMap;
	}

	const unordered_map<int, FPokemonSpritePos>& GetPosMap() const
	{
		return PokemonSpritePosInfoMap;
	}


	EPokemonType GetTypeFromString(const string& _typeStr);

	const unordered_map<MoveID, FMoveData>& GetMoveDB() const
	{
		return MoveDataMap;
	}

	const vector<pair<int, MoveID>>& GetSkillSet(PokemonID id) const
	{
		static vector<pair<int, MoveID>> Empty;

		auto iter = PokemonSkillSet.find(id);
		return iter == PokemonSkillSet.end() ? Empty : iter->second;
	}


public:
	bool BuildPokemonInstance(FPokemonInstance& out, int id, int level);

	void BuildPokemonMoves(FPokemonInstance& inst) const;

	void AddExpAndLevelUp(FPokemonInstance& inst, int gain);


public:
	DECLARE_SINGLETON(CPokemonManager)
};

