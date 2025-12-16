#pragma once
#include "../../UI/UserWidget/UserWidget.h"

class CBattleWidget :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CBattleWidget();
	virtual ~CBattleWidget();


protected:


public:
	virtual bool Init();

private:

};

