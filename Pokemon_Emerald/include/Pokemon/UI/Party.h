#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"

class CParty :
	public CUserWidget
{
	friend class CSceneUIManager;

private:
	vector<FPokemonInstance> mPokemonParty;
	int CurrentIndex = 0;	// 현재 선택된(배틀중인) 포켓몬 인덱스

public:
	CParty();
	~CParty();

	bool AddPokemon(int id, int level);
	void ChangePokemon(int index)
	{
		if (index < mPokemonParty.size())
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

public:
	virtual bool Init();

private:

};

