#pragma once
#include "../../UI/UserWidget/UserWidget.h"

class CBattleWidget :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CBattleWidget();
	virtual ~CBattleWidget();


private:
	

public:
	virtual bool Init();
	//virtual void SetPlayerPokemonUI(const FPokemonInstance& data);
private:

};

