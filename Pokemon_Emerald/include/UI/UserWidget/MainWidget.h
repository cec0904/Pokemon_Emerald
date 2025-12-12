#pragma once
#include "WindowWidget.h"

class CMainWidget :
    public CWindowWidget
{
	friend class CSceneUIManager;

protected:
	CMainWidget();
	virtual ~CMainWidget();

public:
	CSharedPtr<class CButton> mButton;
	CSharedPtr<class CProgressBar> HPBar;

public:


public:
	virtual bool Init();




};

