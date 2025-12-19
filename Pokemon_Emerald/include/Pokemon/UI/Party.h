#pragma once
#include "../../UI/UserWidget/UserWidget.h"

class CParty :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CParty();
	virtual ~CParty();


protected:


public:
	virtual bool Init();

private:

};

