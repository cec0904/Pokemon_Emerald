#pragma once
#include "UserWidget.h"
class CStartWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CStartWidget();
	virtual ~CStartWidget();

protected:
	CSharedPtr<class CButton> mButton;
	CSharedPtr<class CButton> mEditorButton;

public:
	virtual bool Init();

private:
	void StartButtonClick();
	void StartEidtButtonClick();

};

