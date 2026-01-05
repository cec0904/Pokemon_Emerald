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
#include "../../Asset/Sound/Sound.h"
#include "../../Asset/Sound/SoundManager.h"
#include "../../Asset/AssetManager.h"

#include "../../Pokemon/Npc/NpcProfessor.h"
#include "../../Pokemon/Pokemon/PokemonManager.h"

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

	mFlash = mScene->GetUIManager()->CreateWidget<CImage>("MainFlash");
	mFlash->SetTexture("Fade", TEXT("Texture/Pokemon/BackGround/Fade.png"));
	mFlash->SetPos(0.f, 0.f);
	mFlash->SetSize((float)RS.Width, (float)RS.Height);
	mFlash->SetZOrder(9999);
	mFlash->SetUseColorKey(false);
	mFlash->SetTint(1.f, 1.f, 1.f, 0.f);
	mFlash->SetEnable(false);
	AddWidget(mFlash);

	CAssetManager::GetInst()->GetSoundManager()->Play("LittlerootTown");




	return true;
}
void CMainWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (!mFlashPlaying)
		return;

	constexpr float Cycle = 0.7f;
	constexpr int   Count = 3;
	const float Total = Cycle * (float)Count;

	mFlashTime += DeltaTime;

	if (mFlash)
		mFlash->SetEnable(true);

	float tt = fmodf(mFlashTime, Cycle) / Cycle;
	float alpha = (tt < 0.5f) ? (tt * 2.f) : (2.f - tt * 2.f);

	if (mFlash)
		mFlash->SetTint(1.f, 1.f, 1.f, alpha);

	if (mFlashTime >= Total)
	{
		mFlashPlaying = false;

		if (mFlash)
		{
			mFlash->SetTint(1.f, 1.f, 1.f, 0.f);
			mFlash->SetEnable(false);
		}

		if (!mBattlePushed)
		{
			mBattlePushed = true;
			CSceneManager::GetInst()->PushScene<CBattleWithPokemon>();
		}

		if (mStartButton) mStartButton->SetEnable(true);
		if (mBackButton)  mBackButton->SetEnable(true);
	}
}

void CMainWidget::StartButtonClick()
{
	CLog::PrintLog("Battle Button Click");

	CPokemonManager::GetInst()->SetPendingEnemyID((int)mEnemyID);
	if (mEnemyID == 0)
		mEnemyID = rand() % 28 + 1;

	mFlashPlaying = true;
	mBattlePushed = false;
	mFlashTime = 0.f;

	if (mStartButton) mStartButton->SetEnable(false);
	if (mBackButton)  mBackButton->SetEnable(false);


	if (mEnemyID == 28)
		CAssetManager::GetInst()->GetSoundManager()->Play("LegendaryPokemonBattle");
	else
		CAssetManager::GetInst()->GetSoundManager()->Play("WildPokemonBattle");


	CAssetManager::GetInst()->GetSoundManager()->Stop("LittlerootTown");

}

void CMainWidget::BackButtonClick()
{
	CLog::PrintLog("Back Button Click");
	CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();

}