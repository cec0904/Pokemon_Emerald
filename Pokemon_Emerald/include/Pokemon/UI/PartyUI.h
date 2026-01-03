#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"

class CPartyData; // 전방 선언

class CPartyUI : public CUserWidget
{
	friend class CSceneUIManager;

private:
	CPartyData* mPartyData = nullptr;

public:
	CPartyUI();
	~CPartyUI();

	// 데이터 연결
	void SetPartyData(CPartyData* data) 
	{
		mPartyData = data;
	}

	void ChangePokemon(int index);
	const FPokemonInstance& GetActivePokemon() const;
	const vector<FPokemonInstance>& GetPartyList() const;

public:
	virtual bool Init();

private:
};
