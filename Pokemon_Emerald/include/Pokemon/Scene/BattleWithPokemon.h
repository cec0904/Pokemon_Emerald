#pragma once
#include "../../Scene/Scene.h"

class CBattleWidget;
class CMainWidget;

class CBattleWithPokemon :
	public CScene
{
	friend class CSceneManager;

private:
	CSharedPtr<CBattleWidget> mBattleWidget;
	CSharedPtr<CMainWidget> mMainWidget;


protected:
	CBattleWithPokemon();
	virtual ~CBattleWithPokemon();

public:


public:
	virtual bool Init();
	virtual void Update(float DeltaTime) override;
};

