#pragma once
#include "../../PokemonData.h"
#include <vector>

class CPartyData
{
private:
	vector<FPokemonInstance> mPokemonParty;
	int CurrentIndex = 0;

public:
	bool AddPokemon(int id, int level);
	bool AddPokemonInstance(const FPokemonInstance& inst);


	void ChangePokemon(int index)
	{
		if (index >= 0 && index < (int)mPokemonParty.size())
		{
			CurrentIndex = index;
		}
	}

	const FPokemonInstance& GetActivePokemon() const
	{
		return mPokemonParty[CurrentIndex];
	}

	const vector<FPokemonInstance>& GetPartyList() const
	{
		return mPokemonParty;
	}

	int GetCurrentIndex() const
	{
		return CurrentIndex;
	}

	FPokemonInstance* GetActivePokemonPtr()
	{
		if (mPokemonParty.empty()) return nullptr;
		return &mPokemonParty[CurrentIndex];
	}

};

