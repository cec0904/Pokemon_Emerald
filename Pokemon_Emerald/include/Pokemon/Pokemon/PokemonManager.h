#pragma once
#include "../../Gameinfo.h"
#include "../../PokemonData.h"

class CPartyData;

class CPokemonManager 
{


private:
	//포켓몬 도감 
	map<PokemonID, FPokemonDefaultInfo> PokemonIDMap;
	map<PokemonID, FBaseStats> PokemonDefaultStateMap;
	map<PokemonID, FPokemonSpritePos> PokemonSpritePosInfoMap;

	// 포켓몬 기술
	map<MoveID, FMoveData> MoveDataMap;
	map<PokemonID, vector<pair<int, MoveID>>> PokemonSkillSet;

public:
	bool Init();
	CPartyData* GetPlayerPartyData();

public:
	void LoadSpecies();
	void LoadSkill();
	void BuildSkillSetTable();
	void Evolve(FPokemonInstance& inst, PokemonID nextID) const;
	void LevelChange(FPokemonInstance& inst) const;
	
	const map<int, FPokemonDefaultInfo>& GetIDMap() const
	{
		return PokemonIDMap;
	}

	const map<int, FBaseStats>& GetDefaultStatMap() const
	{
		return PokemonDefaultStateMap;
	}

	const map<int, FPokemonSpritePos>& GetPosMap() const
	{
		return PokemonSpritePosInfoMap;
	}


	EPokemonType GetTypeFromString(const string& _typeStr);

	const map<MoveID, FMoveData>& GetMoveDB() const
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


	void RecalcCurrentStateForLevel(FPokemonInstance& p);

private:
	int mPendingEnemyID = 0; // 0이면 미설정

public:
	void SetPendingEnemyID(int id) { mPendingEnemyID = id; }
	int  GetPendingEnemyID() const { return mPendingEnemyID; }

public:
	DECLARE_SINGLETON(CPokemonManager)
};

