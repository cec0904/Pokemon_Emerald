#pragma once
#include "../../Gameinfo.h"
#include "../../PokemonData.h"

class CPokemonManager
{


private:
	CPokemonManager();
	~CPokemonManager();

private:
	//포켓몬 도감 
	unordered_map<PokemonID, FPokemonDefaultInfo> PokemonIDMap;
	unordered_map<PokemonID, FBaseStats> PokemonDefaultStateMap;
	unordered_map<PokemonID, FPokemonSpritePos> PokemonSpritePosInfoMap;

	//스킬 리스트

	/*unordered_map<PokemonID, FPokemonAilment> PokemonAilMap;
	unordered_map<PokemonID, FPokemonInstance> PokemonInstMap;
	unordered_map<PokemonID, FPokemonSpeciesData> PokemonSpecMap;
	unordered_map<PokemonID, FPokemonSpriteKey> PokemonSprMap;
	unordered_map<string, EPokemonType> MapTypeTable;*/
public:
	bool Init();

private:
	void LoadFile();
	void InitTypeTable();

	EPokemonType GetTypeFromString(const string& _typeStr);
};

