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

public:
	bool Init();

public:
	void LoadFile();
	
	
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
public:
	DECLARE_SINGLETON(CPokemonManager)
};

