#pragma once
#include "../../UI/UserWidget/UserWidget.h"
class CFadeWidget :
	public CUserWidget
{
	friend class CSceneUIManger;


protected:
	CFadeWidget();
	virtual ~CFadeWidget();

public:
	virtual bool Init();
	void Update(float DeltaTime);
};

