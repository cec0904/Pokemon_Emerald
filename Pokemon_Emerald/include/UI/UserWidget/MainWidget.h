#pragma once
#include "UserWidget.h"

class CMainWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CMainWidget();
	virtual ~CMainWidget();

protected:
	CSharedPtr<class CButton> mStartButton;
	CSharedPtr<class CButton> mBackButton;

public:


public:
	virtual bool Init();


private:
	void StartButtonClick();
	void BackButtonClick();

};

