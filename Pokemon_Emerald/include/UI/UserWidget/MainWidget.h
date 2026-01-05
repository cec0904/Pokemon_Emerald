#pragma once
#include "UserWidget.h"
#include "../../Component/AudioComponent.h"
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

	CSharedPtr<class CImage> mNpcProfessor;

	CSharedPtr<class CImage> mFlash;
	bool  mFlashPlaying = false;
	bool  mBattlePushed = false;
	float mFlashTime = 0.f;

public:
	CSharedPtr<class CAudioComponent> mAudio;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:
	void StartButtonClick();
	void BackButtonClick();
private:
	int mEnemyID = 0;

};

