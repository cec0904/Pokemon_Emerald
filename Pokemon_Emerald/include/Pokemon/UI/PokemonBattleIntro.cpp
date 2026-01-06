#include "PokemonBattleIntro.h"
#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../UI/Common/Image.h"
#include "../UI/BattleWidget.h"
#include <cmath>

static inline float Clamp01(float v)
{
	if (v < 0.f) return 0.f;
	if (v > 1.f) return 1.f;
	return v;
}

static inline float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

static inline float Smooth01(float t)
{
	t = Clamp01(t);
	return t * t * (3.f - 2.f * t);
}

CPokemonBattleIntro::CPokemonBattleIntro() {}
CPokemonBattleIntro::~CPokemonBattleIntro() {}

bool CPokemonBattleIntro::Init()
{
	if (!CUserWidget::Init())
		return false;

	FResolution RS = CDevice::GetInst()->GetResolution();

	mMaskTop = mScene->GetUIManager()->CreateWidget<CImage>("IntroMaskTop");
	mMaskTop->SetTexture("MaskTop", TEXT("Texture/Pokemon/BackGround/Fade.png"));
	mMaskTop->SetSize((float)RS.Width, (float)RS.Height * 0.5f);
	mMaskTop->SetPos(0.f, 0.f);
	mMaskTop->SetZOrder(9500);
	mMaskTop->SetUseColorKey(false);
	mMaskTop->SetTint(1.f, 1.f, 1.f, 1.f);
	mMaskTop->SetEnable(false);
	AddWidget(mMaskTop);

	mMaskBottom = mScene->GetUIManager()->CreateWidget<CImage>("IntroMaskBottom");
	mMaskBottom->SetTexture("MaskBottom", TEXT("Texture/Pokemon/BackGround/Fade.png"));
	mMaskBottom->SetSize((float)RS.Width, (float)RS.Height * 0.5f);
	mMaskBottom->SetPos(0.f, (float)RS.Height * 0.5f);
	mMaskBottom->SetZOrder(9500);
	mMaskBottom->SetUseColorKey(false);
	mMaskBottom->SetTint(1.f, 1.f, 1.f, 1.f);
	mMaskBottom->SetEnable(false);
	AddWidget(mMaskBottom);

	mStep = EStep::None;
	mFinished = false;

	SetEnable(false);
	return true;
}

void CPokemonBattleIntro::Start(CBattleWidget* battleWidget, const FPokemonInstance* player, const FPokemonInstance* enemy)
{
	mBattleWidget = battleWidget;
	mPlayer = player;
	mEnemy = enemy;

	mFinished = false;

	// ✅ 인트로 동안 배틀 입력 막기(방어)
	if (mBattleWidget)
		mBattleWidget->SetEnable(false);

	SetEnable(true);
	Go(EStep::Split);
}

void CPokemonBattleIntro::Go(EStep s)
{
	mStep = s;
	mStepTime = 0.f;

	if (s == EStep::Split)
	{
		FResolution RS = CDevice::GetInst()->GetResolution();

		if (mMaskTop)
		{
			mMaskTop->SetEnable(true);
			mMaskTop->SetPos(0.f, 0.f);
		}
		if (mMaskBottom)
		{
			mMaskBottom->SetEnable(true);
			mMaskBottom->SetPos(0.f, (float)RS.Height * 0.5f);
		}
	}
}

void CPokemonBattleIntro::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (!IsEnable() || mFinished)
		return;

	if (mStep != EStep::Split)
		return;

	FResolution RS = CDevice::GetInst()->GetResolution();

	mStepTime += DeltaTime;

	float t = Smooth01(mStepTime / mSplitDur);

	float splitY = (float)RS.Height * 0.5f;

	float topY = Lerp(0.f, -splitY, t);
	float botY = Lerp(splitY, (float)RS.Height, t);

	if (mMaskTop)    mMaskTop->SetPos(0.f, topY);
	if (mMaskBottom) mMaskBottom->SetPos(0.f, botY);

	if (t >= 1.f)
	{
		if (mMaskTop)    mMaskTop->SetEnable(false);
		if (mMaskBottom) mMaskBottom->SetEnable(false);

		mFinished = true;
		mStep = EStep::Done;

		SetEnable(false);
		return;
	}
}