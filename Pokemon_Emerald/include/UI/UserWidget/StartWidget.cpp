#include "StartWidget.h"

#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../Common/Button.h"
#include "../Common/Image.h"
#include "../../Share/Log.h"
#include "../../Object/SpriteEffect.h"
#include "../../Component/SpriteComponent.h"
#include "../../Asset/Animation/Animation2DData.h"
#include "../../Asset/Animation/Animation2DManager.h"
#include "../../Animation/Animation2D.h"
#include "../../Animation/Animation2DSequence.h"


CStartWidget::CStartWidget()
{
}

CStartWidget::~CStartWidget()
{
}

bool CStartWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	//배경
	//CSharedPtr<CImage> Back = mScene->GetUIManager()->CreateWidget<CImage>("Back");
	CSharedPtr<CImage> Title = mScene->GetUIManager()->CreateWidget<CImage>("Title");
	//버튼을 추가하기 
	mButton = mScene->GetUIManager()->CreateWidget<CButton>("TestButton");
	mEditorButton = mScene->GetUIManager()->CreateWidget<CButton>("SceneEditorButton");




	
	CSpriteEffect* TitleBackGround = mScene->CreateObj<CSpriteEffect>("TitleBackGround");
	TitleBackGround->SetAnimation("TitleBackGround", 8.f, 1.f, true, false);
	TitleBackGround->GetRootComponent()->SetPivot(0.5f, 0.5f);
	TitleBackGround->SetWorldPos(0.f, 0.f);
	TitleBackGround->SetWorldScale(2.f, 2.f);
	



	//Back->SetTexture("StartBakc", TEXT("Texture/Pokemon/Title/TitleBackGround.gif"));
	//Back->SetSize((float)RS.Width, (float)RS.Height);
	//Back->SetBrushAnimation(true);
	
	


	Title->SetTexture("StartTitle", TEXT("Texture/Pokemon/Title/TitleName.png"));
	Title->SetSize(768.f, 192.f);
	Title->SetPivot(FVector2D(0.5f, 0.5f));
	Title->SetPos(RS.Width / 2.f, 500.f);

	//Back->SetZOrder(5);
	Title->SetZOrder(7);
	mButton->SetZOrder(10);
	mEditorButton->SetZOrder(10);

	//AddWidget(Back);
	AddWidget(Title);
	AddWidget(mButton);
	AddWidget(mEditorButton);


	mButton->SetPos(640.f, 200.f);
	mButton->SetSize(100.f, 100.f);
	mButton->SetPivot(FVector2D(0.5f, 0.5f));
	//mButton->SetZOrder(1);

	mButton->SetTexture(EButtonState::Normal, "StartButton", TEXT("Texture/Start.png"));
	mButton->SetTexture(EButtonState::Hovered, "StartButton");
	mButton->SetTexture(EButtonState::Click, "StartButton", TEXT("Texture/Start.png"));
	mButton->SetTexture(EButtonState::Disable, "StartButton", TEXT("Texture/Start.png"));

	mButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mButton->SetTint(EButtonState::Click, 0.5f, 0.5f, 0.5f);
	mButton->SetTint(EButtonState::Disable, 0.1f, 0.1f, 0.1f);

	mButton->SetEventCallBack(EButtonEventState::Click, this, &CStartWidget::StartButtonClick);


	// 에디터 모드 가는 버튼 
	mEditorButton->SetPos(640.f, 100.f);
	mEditorButton->SetSize(100.f, 100.f);
	mEditorButton->SetPivot(FVector2D(0.5f, 0.5f));
	mEditorButton->SetTexture(EButtonState::Normal, "EditButton", TEXT("Texture/Edit.png"));
	mEditorButton->SetTexture(EButtonState::Hovered, "EditButton");
	mEditorButton->SetTexture(EButtonState::Click, "EditButton", TEXT("Texture/Edit.png"));
	mEditorButton->SetTexture(EButtonState::Disable, "EditButton", TEXT("Texture/Edit.png"));
	mEditorButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mEditorButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mEditorButton->SetTint(EButtonState::Click, 0.5f, 0.5f, 0.5f);
	mEditorButton->SetTint(EButtonState::Disable, 0.1f, 0.1f, 0.1f);

	mEditorButton->SetEventCallBack(EButtonEventState::Click, this, &CStartWidget::StartEidtButtonClick);



	return true;
}


void CStartWidget::StartButtonClick()
{
	CLog::PrintLog("Start Button Click");

	CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
}

void CStartWidget::StartEidtButtonClick()
{
	CLog::PrintLog("Start SceneEditor Button Click");

	CSceneManager::GetInst()->CreateLoadScene<CSceneEditor>();
}
