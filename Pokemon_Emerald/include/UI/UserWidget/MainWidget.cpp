#include "MainWidget.h"

#include "../../Device.h"
#include "../Common/Button.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../Common/TextBlock.h"
#include "../Common/ProgressBar.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../Common/Image.h"
#include "../../Share/Log.h"
#include "../../Pokemon/Scene/BattleWithPokemon.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	mStartButton = mScene->GetUIManager()->CreateWidget<CButton>("StartButton");
	mBackButton = mScene->GetUIManager()->CreateWidget<CButton>("BackButton");

	AddWidget(mStartButton);
	AddWidget(mBackButton);

	mStartButton->SetPos(640.f, 200.f);
	mStartButton->SetSize(100.f, 100.f);
	mStartButton->SetPivot(FVector2D(0.5f, 0.5f));


	mStartButton->SetTexture(EButtonState::Normal, "StartButton", TEXT("Texture/Start.png"));
	mStartButton->SetTexture(EButtonState::Hovered, "StartButton");
	mStartButton->SetTexture(EButtonState::Click, "StartButton", TEXT("Texture/Start.png"));
	mStartButton->SetTexture(EButtonState::Disable, "StartButton", TEXT("Texture/Start.png"));

	mStartButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mStartButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mStartButton->SetTint(EButtonState::Click, 0.5f, 0.5f, 0.5f);
	mStartButton->SetTint(EButtonState::Disable, 0.1f, 0.1f, 0.1f);

	mStartButton->SetEventCallBack(EButtonEventState::Click, this, &CMainWidget::StartButtonClick);


	



	mBackButton->SetPos(640.f, 100.f);
	mBackButton->SetSize(100.f, 100.f);
	mBackButton->SetPivot(FVector2D(0.5f, 0.5f));
	mBackButton->SetTexture(EButtonState::Normal, "EditButton", TEXT("Texture/Edit.png"));
	mBackButton->SetTexture(EButtonState::Hovered, "EditButton");
	mBackButton->SetTexture(EButtonState::Click, "EditButton", TEXT("Texture/Edit.png"));
	mBackButton->SetTexture(EButtonState::Disable, "EditButton", TEXT("Texture/Edit.png"));
	mBackButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mBackButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mBackButton->SetTint(EButtonState::Click, 0.5f, 0.5f, 0.5f);
	mBackButton->SetTint(EButtonState::Disable, 0.1f, 0.1f, 0.1f);

	mBackButton->SetEventCallBack(EButtonEventState::Click, this, &CMainWidget::BackButtonClick);

	return true;
}

void CMainWidget::StartButtonClick()
{
	CLog::PrintLog("Battle Button Click");

	CSceneManager::GetInst()->CreateLoadScene<CBattleWithPokemon>();
}

void CMainWidget::BackButtonClick()
{
	CLog::PrintLog("Back Button Click");

	CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
}