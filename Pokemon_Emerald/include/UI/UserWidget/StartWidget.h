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
	
	CSharedPtr<class CTextBlock> mPushText;

	float mBlinkTimer = 0.f;
	bool mBlinkOn = true;

public:
	virtual bool Init();

private:
	void StartButtonClick();
	void StartEidtButtonClick();

	void Update(float DeltaTime);

	void SelectKey(float DeltaTime);
	void Select();
};

