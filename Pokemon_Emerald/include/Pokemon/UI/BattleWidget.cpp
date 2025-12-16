#include "BattleWidget.h"
#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../../UI/Common/Button.h"
#include "../../UI/Common/Image.h"
#include "../../Share/Log.h"

CBattleWidget::CBattleWidget()
{
}

CBattleWidget::~CBattleWidget()
{
}

bool CBattleWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	float ScreenW = RS.Width;	// 1280
	float ScreenH = RS.Height;	// 720


	// 기본 배경
	CSharedPtr<CImage> BattleArena = mScene->GetUIManager()->CreateWidget<CImage>("BattleArena");
	CSharedPtr<CImage> BattleUIBack = mScene->GetUIManager()->CreateWidget<CImage>("BattleUIBack");
	CSharedPtr<CImage> BehaviorSelect = mScene->GetUIManager()->CreateWidget<CImage>("BehaviorSelect");
	CSharedPtr<CImage> SkillSelect = mScene->GetUIManager()->CreateWidget<CImage>("SkillSelect");
	CSharedPtr<CImage> HoveredUI = mScene->GetUIManager()->CreateWidget<CImage>("HoveredUI");
	CSharedPtr<CImage> PlayerHpBar = mScene->GetUIManager()->CreateWidget<CImage>("PlayerHpBar");
	CSharedPtr<CImage> EnemyHpBar = mScene->GetUIManager()->CreateWidget<CImage>("EnemyHpBar");

	CSharedPtr<CImage> HpGreen = mScene->GetUIManager()->CreateWidget<CImage>("HpGreen");
	CSharedPtr<CImage> HpYellow = mScene->GetUIManager()->CreateWidget<CImage>("HpYellow");
	CSharedPtr<CImage> HpRed = mScene->GetUIManager()->CreateWidget<CImage>("HpRed");
	CSharedPtr<CImage> HpBlack = mScene->GetUIManager()->CreateWidget<CImage>("HpBlack");
	CSharedPtr<CImage> ExpBlue = mScene->GetUIManager()->CreateWidget<CImage>("ExpBlue");
	CSharedPtr<CImage> ExpBlack = mScene->GetUIManager()->CreateWidget<CImage>("ExpBlack");
	CSharedPtr<CImage> PlayerPokemonParty = mScene->GetUIManager()->CreateWidget<CImage>("PlayerPokemonParty");
	CSharedPtr<CImage> EnemyPokemonParty = mScene->GetUIManager()->CreateWidget<CImage>("EnemyPokemonParty");
	CSharedPtr<CImage> PokemonPartyUI = mScene->GetUIManager()->CreateWidget<CImage>("PokemonPartyUI");



	BattleArena->SetTexture("BattleBack", TEXT("Texture/Pokemon/BackGround/BattleArenas.png"));
	BattleArena->SetBrushAnimation(true);
	// 시작지점x, 시작지점y, 얼마나할지x(이미지가로크기), 얼마나할지(이미지세로크기)y
	BattleArena->AddBurshFrame(2.f, 22.f, 240.f, 112.f);
	BattleArena->SetSize(960.f, 448.f);		// 4배
	BattleArena->SetPivot(FVector2D(0.5f, 0.f));
	BattleArena->SetPos(ScreenW * 0.5f, ScreenH - 448.f);

	// 614*296
	// 297*57
	// 317*239 시작위치
	// 이미지 240*48
	BattleUIBack->SetTexture("BattleUIBack", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	BattleUIBack->SetBrushAnimation(true);
	BattleUIBack->AddBurshFrame(297.f, 56.f, 240.f, 48.f);
	BattleUIBack->SetSize(960.f, 192.f);
	BattleUIBack->SetPivot(FVector2D(0.5f, 0.f));
	BattleUIBack->SetPos(ScreenW * 0.5f, ScreenH - 640.f);

	BehaviorSelect->SetTexture("BehaviorSelect", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	BehaviorSelect->SetBrushAnimation(true);
	BehaviorSelect->AddBurshFrame(146.f, 4.f, 120.f, 48.f);
	BehaviorSelect->SetSize(480.f, 192.f);
	BehaviorSelect->SetPivot(FVector2D(0.5f, 0.f));
	BehaviorSelect->SetPos(ScreenW * 0.5f + 240.f, ScreenH - 640.f);

	SkillSelect->SetTexture("SkillSelect", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	SkillSelect->SetSize(960.f, 448.f);
	SkillSelect->SetPivot(FVector2D(0.5f, 0.f));

	HoveredUI->SetTexture("HoveredUI", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	HoveredUI->SetSize(960.f, 448.f);
	HoveredUI->SetPivot(FVector2D(0.5f, 0.f));

	PlayerHpBar->SetTexture("BattleBack", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	PlayerHpBar->SetSize(960.f, 448.f);
	PlayerHpBar->SetPivot(FVector2D(0.5f, 0.f));

	EnemyHpBar->SetTexture("EnemyHpBar", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	EnemyHpBar->SetSize(960.f, 448.f);
	EnemyHpBar->SetPivot(FVector2D(0.5f, 0.f));

	HpGreen->SetTexture("HpGreen", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	HpGreen->SetSize(960.f, 448.f);
	HpGreen->SetPivot(FVector2D(0.5f, 0.f));

	HpYellow->SetTexture("HpYellow", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	HpYellow->SetSize(960.f, 448.f);
	HpYellow->SetPivot(FVector2D(0.5f, 0.f));

	HpRed->SetTexture("HpRed", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	HpRed->SetSize(960.f, 448.f);
	HpRed->SetPivot(FVector2D(0.5f, 0.f));

	HpBlack->SetTexture("HpBlack", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	HpBlack->SetSize(960.f, 448.f);
	HpBlack->SetPivot(FVector2D(0.5f, 0.f));

	ExpBlue->SetTexture("ExpBlue", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	ExpBlue->SetSize(960.f, 448.f);
	ExpBlue->SetPivot(FVector2D(0.5f, 0.f));

	ExpBlack->SetTexture("ExpBlack", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	ExpBlack->SetSize(960.f, 448.f);
	ExpBlack->SetPivot(FVector2D(0.5f, 0.f));

	PlayerPokemonParty->SetTexture("PlayerPokemonParty", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	PlayerPokemonParty->SetSize(960.f, 448.f);
	PlayerPokemonParty->SetPivot(FVector2D(0.5f, 0.f));

	EnemyPokemonParty->SetTexture("EnemyPokemonParty", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	EnemyPokemonParty->SetSize(960.f, 448.f);
	EnemyPokemonParty->SetPivot(FVector2D(0.5f, 0.f));

	PokemonPartyUI->SetTexture("PokemonPartyUI", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	PokemonPartyUI->SetSize(960.f, 448.f);
	PokemonPartyUI->SetPivot(FVector2D(0.5f, 0.f));



	

	BattleArena->SetZOrder(0);
	BattleUIBack->SetZOrder(0);
	BehaviorSelect->SetZOrder(1);

	AddWidget(BattleArena);
	AddWidget(BattleUIBack);
	AddWidget(BattleUIBack);
	AddWidget(BehaviorSelect);

	return true;
}

