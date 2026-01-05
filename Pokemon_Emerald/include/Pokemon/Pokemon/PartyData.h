// PartyData.h
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
			if (index != 0)
			{
				swap(mPokemonParty[0], mPokemonParty[index]);
			}
			CurrentIndex = 0;

		}
	}

	const FPokemonInstance& GetActivePokemon() const
	{
		static FPokemonInstance Dummy;

		if (mPokemonParty.empty())
			return Dummy;

		int idx = CurrentIndex;
		if (idx < 0) idx = 0;
		if (idx >= (int)mPokemonParty.size()) idx = (int)mPokemonParty.size() - 1;

		return mPokemonParty[idx];
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
		if (mPokemonParty.empty())
			return nullptr;

		if (CurrentIndex < 0) CurrentIndex = 0;
		if (CurrentIndex >= (int)mPokemonParty.size()) CurrentIndex = (int)mPokemonParty.size() - 1;

		return &mPokemonParty[CurrentIndex];
	}
};
