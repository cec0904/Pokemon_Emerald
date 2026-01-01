#pragma once
#include "../../Scene/Scene.h"

class CBattleWithPokemon :
	public CScene
{
	friend class CSceneManager;

protected:
	CBattleWithPokemon();
	virtual ~CBattleWithPokemon();

public:


public:
	virtual bool Init();
};

