// PartyUI.cpp
#include "PartyUI.h"

#include "../../Device.h"
#include "../../Scene/Input.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../UI/Common/Image.h"
#include "../../UI/Common/TextBlock.h"
#include "../../Share/Log.h"
#include "../../Pokemon/Pokemon/PartyData.h"
#include "BattleWidget.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Sound/Sound.h"
#include "../../Asset/Sound/SoundManager.h"
#include <Windows.h>

static inline void FlushPartyKeys()
{
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(VK_DOWN);
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
	GetAsyncKeyState('D');
	GetAsyncKeyState('S');
}


static inline wstring Utf8ToWString_Party(const string& s)
{
	if (s.empty()) return L"";
	int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	UINT codepage = CP_UTF8;
	if (len <= 0)
	{
		codepage = CP_ACP;
		len = MultiByteToWideChar(codepage, 0, s.c_str(), -1, nullptr, 0);
		if (len <= 0) return L"";
	}

	wstring ws; ws.resize(len);
	MultiByteToWideChar(codepage, 0, s.c_str(), -1, &ws[0], len);
	if (!ws.empty() && ws.back() == L'\0') ws.pop_back();
	return ws;
}

float PartyTL_X(float PartyLeft, float xGBA, float Scale)
{
	return PartyLeft + xGBA * Scale;
}

float PartyTL_Y(float PartyTop, float yGBA, float Scale)
{
	return PartyTop + yGBA * Scale;
}

void GetSlotOverlayTL_GBA(int i, float& outX, float& outY)
{
	if (i == 0)
	{
		outX = 0.f;
		outY = 16.f;
		return;
	}

	outX = 90.f;
	outY = (float)(i - 1) * 24.f;
}

static inline FVector2D PartyBL_FromGBA(float PartyLeft, float PartyTop, float xGBA, float yGBA, float hGBA, float Scale)
{
	const float GBAH = 160.f;
	return FVector2D(PartyLeft + xGBA * Scale, PartyTop + ((GBAH - yGBA - hGBA) * Scale));
}


const FVector2D PokeTL_GBA[6] =
{
	FVector2D(8.f, 26.f),
	FVector2D(96.f, 10.f),
	FVector2D(96.f, 34.f),
	FVector2D(96.f, 58.f),
	FVector2D(96.f, 82.f),
	FVector2D(96.f, 106.f)
};

const FVector2D NameOffset_GBA[6] =
{
	FVector2D(25.f, 8.f),
	FVector2D(25.f, 4.f),
	FVector2D(25.f, 4.f),
	FVector2D(25.f, 4.f),
	FVector2D(25.f, 4.f),
	FVector2D(25.f, 4.f)
};

const FVector2D LvOffset_GBA[6] =
{
	FVector2D(45.f, 20.f),
	FVector2D(45.f, 14.f),
	FVector2D(45.f, 14.f),
	FVector2D(45.f, 14.f),
	FVector2D(45.f, 14.f),
	FVector2D(45.f, 14.f)
};

const FVector2D HpOffset_GBA[6] =
{
	FVector2D(41.f, 37.f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f)
};

const FVector2D HpBarOffset_GBA[6] =
{
	FVector2D(41.f, 37.f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f),
	FVector2D(106.f, 12.5f)
};



CPartyUI::CPartyUI() {}
CPartyUI::~CPartyUI() {}

void CPartyUI::ChangePokemon(int index)
{
	if (!mPartyData) return;
	mPartyData->ChangePokemon(index);
}

const FPokemonInstance& CPartyUI::GetActivePokemon() const
{
	static FPokemonInstance Dummy;
	if (!mPartyData) return Dummy;
	return mPartyData->GetActivePokemon();
}

const vector<FPokemonInstance>& CPartyUI::GetPartyList() const
{
	static vector<FPokemonInstance> Empty;
	if (!mPartyData) return Empty;
	return mPartyData->GetPartyList();
}

bool CPartyUI::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();
	float ScreenW = RS.Width;
	float ScreenH = RS.Height;

	const float GBAW = 240.f;
	const float GBAH = 160.f;
	const float Scale = 4.f;

	const float PartyW = GBAW * Scale;
	const float PartyH = GBAH * Scale;

	const float PartyLeft = (RS.Width * 0.5f) - (120.f * Scale);
	const float PartyTop = (RS.Height * 0.5f) - (80.f * Scale);

	const float UiOffsetX = 0.f;
	const float UiOffsetY = 0.f;



	auto GBA_TL = [&](float x, float y, float /*hGBA*/) -> FVector2D
		{
			float sx = PartyLeft + UiOffsetX + x * Scale;
			float sy = PartyTop + UiOffsetY + y * Scale;
			return FVector2D(sx, sy);
		};

	CSharedPtr<CImage> PartyBG = mScene->GetUIManager()->CreateWidget<CImage>("PartyBG");
	PartyBG->SetTexture("PartyBG", TEXT("Texture/Pokemon/Pokemon/Party.png"));
	PartyBG->SetBrushAnimation(true);
	PartyBG->AddBrushFrame(250.f, 5.f, 240.f, 160.f);
	PartyBG->AddBrushFrame(250.f, 5.f, 240.f, 160.f);
	PartyBG->SetSize(PartyW, PartyH);
	PartyBG->SetPivot(FVector2D(0.f, 0.f));
	PartyBG->SetPos(PartyLeft, PartyTop);
	PartyBG->SetZOrder(200);
	AddWidget(PartyBG);

	mTextBox = mScene->GetUIManager()->CreateWidget<CTextBlock>("PartyText");
	mTextBox->SetFont("Default");
	mTextBox->SetFontSize(64.f);
	mTextBox->SetTextColor(0, 0, 0, 255);
	mTextBox->SetAlignH(ETextAlignH::Left);
	mTextBox->SetAlignV(ETextAlignV::Middle);
	mTextBox->SetSize(180.f * Scale, 28.f * Scale);
	mTextBox->SetPivot(FVector2D(0.f, 0.f));
	{
		float textY = PartyTop + ((GBAH - 138.f - 28.f) * Scale);
		mTextBox->SetPos(200.f, 50.f);
	}
	mTextBox->SetText(TEXT("Choose a POKéMON."));
	mTextBox->SetEnable(true);
	mTextBox->SetZOrder(260);
	AddWidget(mTextBox);

	mHoveredCancel = mScene->GetUIManager()->CreateWidget<CImage>("HoveredCancel");
	mUnHoveredCancel = mScene->GetUIManager()->CreateWidget<CImage>("UnHoveredCancel");

	mHoveredCancel->SetTexture("HoveredCancel", TEXT("Texture/Pokemon/Pokemon/Party.png"));
	mHoveredCancel->SetBrushAnimation(true);
	mHoveredCancel->AddBrushFrame(65.f, 251.f, 54.f, 24.f);
	mHoveredCancel->AddBrushFrame(65.f, 251.f, 54.f, 24.f);
	mHoveredCancel->SetSize(54.f * Scale, 24.f * Scale);
	mHoveredCancel->SetPivot(FVector2D(0.f, 0.f));
	mHoveredCancel->SetZOrder(240);
	AddWidget(mHoveredCancel);

	mUnHoveredCancel->SetTexture("UnHoveredCancel", TEXT("Texture/Pokemon/Pokemon/Party.png"));
	mUnHoveredCancel->SetBrushAnimation(true);
	mUnHoveredCancel->AddBrushFrame(6.f, 253.f, 54.f, 20.f);
	mUnHoveredCancel->AddBrushFrame(6.f, 253.f, 54.f, 20.f);
	mUnHoveredCancel->SetSize(54.f * Scale, 20.f * Scale);
	mUnHoveredCancel->SetPivot(FVector2D(0.f, 0.f));
	mUnHoveredCancel->SetZOrder(240);
	AddWidget(mUnHoveredCancel);

	{
		float cancelY = PartyTop + ((GBAH - 136.f - 24.f) * Scale);
		mHoveredCancel->SetPivot(FVector2D(0.f, 0.f));
		mHoveredCancel->SetPos(PartyLeft + (186.f * Scale) - 8.f, cancelY + 21.f);
		mHoveredCancel->SetUseColorKey(true);
		mHoveredCancel->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
		mHoveredCancel->SetKeyThreshold(0.01f);

		mUnHoveredCancel->SetPivot(FVector2D(0.f, 0.f));
		mUnHoveredCancel->SetPos(PartyLeft + (186.f * Scale) - 8.f, cancelY + 21.f);
		mUnHoveredCancel->SetUseColorKey(true);
		mUnHoveredCancel->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
		mUnHoveredCancel->SetKeyThreshold(0.01f);
	}

	mSlotHovered.resize(MaxParty);
	mSlotUnhovered.resize(MaxParty);
	mSlotSpriteA.resize(MaxParty);
	mSlotSpriteB.resize(MaxParty);
	mSlotSpriteBuilt.assign(MaxParty, false);

	mSlotNameText.resize(MaxParty);
	mSlotLvText.resize(MaxParty);
	mSlotHpText.resize(MaxParty);

	mSlotHpBar.resize(MaxParty);


	static const FVector2D PokeTL_GBA[MaxParty] =
	{
		{  2.f,  43.f },
		{ 88.f,  1.f },
		{ 88.f,  25.f },
		{ 88.f,  49.f },
		{ 88.f,  73.f },
		{ 88.f, 97.f },
	};

	auto SlotOverlayTL = [&](int i)->FVector2D
		{
			if (i == 0) return FVector2D(0.f, 16.f);
			return FVector2D(90.f, (float)(i - 1) * 24.f);
		};

	for (int i = 0; i < MaxParty; ++i)
	{
		mSlotHovered[i] = mScene->GetUIManager()->CreateWidget<CImage>("Party_SlotH_" + to_string(i));
		mSlotUnhovered[i] = mScene->GetUIManager()->CreateWidget<CImage>("Party_SlotN_" + to_string(i));


		const bool isFirst = (i == 0);

		if (isFirst)
		{

			float textY = PartyTop + ((GBAH - 138.f - 28.f) * Scale);

			mSlotHovered[i]->SetTexture("SlotFirstH", TEXT("Texture/Pokemon/Pokemon/Party.png"));
			mSlotHovered[i]->SetBrushAnimation(true);
			mSlotHovered[i]->AddBrushFrame(406.f, 170.f, 84.f, 57.f);
			mSlotHovered[i]->AddBrushFrame(406.f, 170.f, 84.f, 57.f);
			mSlotHovered[i]->SetSize(84.f * Scale, 57.f * Scale);
			mSlotHovered[i]->SetUseColorKey(true);
			mSlotHovered[i]->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
			mSlotHovered[i]->SetKeyThreshold(0.01f);

			mSlotUnhovered[i]->SetTexture("SlotFirstN", TEXT("Texture/Pokemon/Pokemon/Party.png"));
			mSlotUnhovered[i]->SetBrushAnimation(true);
			mSlotUnhovered[i]->AddBrushFrame(317.f, 172.f, 84.f, 55.f);
			mSlotUnhovered[i]->AddBrushFrame(317.f, 172.f, 84.f, 55.f);
			mSlotUnhovered[i]->SetSize(84.f * Scale, 55.f * Scale);
			mSlotUnhovered[i]->SetUseColorKey(true);
			mSlotUnhovered[i]->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
			mSlotUnhovered[i]->SetKeyThreshold(0.01f);

			
		}
		else
		{
			mSlotHovered[i]->SetTexture("SlotRestH", TEXT("Texture/Pokemon/Pokemon/Party.png"));
			mSlotHovered[i]->SetBrushAnimation(true);
			mSlotHovered[i]->AddBrushFrame(162.f, 203.f, 150.f, 24.f);
			mSlotHovered[i]->AddBrushFrame(162.f, 203.f, 150.f, 24.f);
			mSlotHovered[i]->SetSize(150.f * Scale, 24.f * Scale);
			mSlotHovered[i]->SetUseColorKey(true);
			mSlotHovered[i]->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
			mSlotHovered[i]->SetKeyThreshold(0.01f);

			mSlotUnhovered[i]->SetTexture("SlotRestN", TEXT("Texture/Pokemon/Pokemon/Party.png"));
			mSlotUnhovered[i]->SetBrushAnimation(true);
			mSlotUnhovered[i]->AddBrushFrame(162.f, 179.f, 150.f, 22.f);
			mSlotUnhovered[i]->AddBrushFrame(162.f, 179.f, 150.f, 22.f);
			mSlotUnhovered[i]->SetSize(150.f * Scale, 22.f * Scale);
			mSlotUnhovered[i]->SetUseColorKey(true);
			mSlotUnhovered[i]->SetColorKey(FVector3D(0.f / 255.f, 55.f / 255.f, 0.f / 255.f));
			mSlotUnhovered[i]->SetKeyThreshold(0.01f);
		}

		mSlotHovered[i]->SetPivot(FVector2D(0.f, 0.f));
		mSlotUnhovered[i]->SetPivot(FVector2D(0.f, 0.f));



		mSlotHovered[i]->SetZOrder(210);
		mSlotUnhovered[i]->SetZOrder(210);


		FVector2D tl = SlotOverlayTL(i);
		FVector2D pos = GBA_TL(tl.x, tl.y, 0.f);

		float finalX = PartyLeft + (PokeTL_GBA[i].x * Scale);
		float finalY = PartyTop + ((GBAH - PokeTL_GBA[i].y - 32.f) * Scale);



		mSlotHovered[i]->SetPos(finalX, finalY);
		mSlotUnhovered[i]->SetPos(finalX, finalY);



		AddWidget(mSlotUnhovered[i]);
		AddWidget(mSlotHovered[i]);


		const TCHAR* MenuSheet = TEXT("Texture/Pokemon/Pokemon/BackSprite.png");

		mSlotSpriteA[i] = mScene->GetUIManager()->CreateWidget<CImage>("Party_SprA_" + to_string(i));
		mSlotSpriteB[i] = mScene->GetUIManager()->CreateWidget<CImage>("Party_SprB_" + to_string(i));

		if (i == 0)
		{
			mSlotSpriteA[i]->SetTexture("MenuA", MenuSheet);
			mSlotSpriteA[i]->SetBrushAnimation(true);
			mSlotSpriteA[i]->SetSize(32.f * Scale, 32.f * Scale);
			mSlotSpriteA[i]->SetPivot(FVector2D(0.f, 0.f));
			mSlotSpriteA[i]->SetZOrder(220);
			mSlotSpriteA[i]->SetEnable(false);
			mSlotSpriteA[i]->SetUseColorKey(true);
			mSlotSpriteA[i]->SetColorKey(FVector3D(255.f / 255.f, 147.f / 255.f, 37.f / 255.f));
			mSlotSpriteA[i]->SetKeyThreshold(0.01f);
			AddWidget(mSlotSpriteA[i]);

			mSlotSpriteB[i]->SetTexture("MenuB", MenuSheet);
			mSlotSpriteB[i]->SetBrushAnimation(true);
			mSlotSpriteB[i]->SetSize(32.f * Scale, 32.f * Scale);
			mSlotSpriteB[i]->SetPivot(FVector2D(0.f, 0.f));
			mSlotSpriteB[i]->SetZOrder(220);
			mSlotSpriteB[i]->SetEnable(false);
			mSlotSpriteB[i]->SetUseColorKey(true);
			mSlotSpriteB[i]->SetColorKey(FVector3D(255.f / 255.f, 147.f / 255.f, 37.f / 255.f));
			mSlotSpriteB[i]->SetKeyThreshold(0.01f);
			AddWidget(mSlotSpriteB[i]);
			{
				FVector2D sp = GBA_TL(PokeTL_GBA[i].x, PokeTL_GBA[i].y, 32.f);
				mSlotSpriteA[i]->SetPos(finalX, finalY + 70.f);
				mSlotSpriteB[i]->SetPos(finalX, finalY + 70.f);
			}
		}

		else
		{
			mSlotSpriteA[i]->SetTexture("MenuA", MenuSheet);
			mSlotSpriteA[i]->SetBrushAnimation(true);
			mSlotSpriteA[i]->SetSize(32.f * Scale, 32.f * Scale);
			mSlotSpriteA[i]->SetPivot(FVector2D(0.f, 0.f));
			mSlotSpriteA[i]->SetZOrder(220);
			mSlotSpriteA[i]->SetEnable(false);
			mSlotSpriteA[i]->SetUseColorKey(true);
			mSlotSpriteA[i]->SetColorKey(FVector3D(255.f / 255.f, 147.f / 255.f, 37.f / 255.f));
			mSlotSpriteA[i]->SetKeyThreshold(0.01f);
			AddWidget(mSlotSpriteA[i]);

			mSlotSpriteB[i]->SetTexture("MenuB", MenuSheet);
			mSlotSpriteB[i]->SetBrushAnimation(true);
			mSlotSpriteB[i]->SetSize(32.f * Scale, 32.f * Scale);
			mSlotSpriteB[i]->SetPivot(FVector2D(0.f, 0.f));
			mSlotSpriteB[i]->SetZOrder(220);
			mSlotSpriteB[i]->SetEnable(false);
			mSlotSpriteB[i]->SetUseColorKey(true);
			mSlotSpriteB[i]->SetColorKey(FVector3D(255.f / 255.f, 147.f / 255.f, 37.f / 255.f));
			mSlotSpriteB[i]->SetKeyThreshold(0.01f);
			AddWidget(mSlotSpriteB[i]);

			{
				FVector2D sp = GBA_TL(PokeTL_GBA[i].x, PokeTL_GBA[i].y, 32.f);
				mSlotSpriteA[i]->SetPos(finalX, finalY - 15.f);
				mSlotSpriteB[i]->SetPos(finalX, finalY - 15.f);
			}
		}

		// ---------------- Slot Text (Name / Lv / HP) ----------------
		{
			const float SlotFont = 53.f;
			const float LineH_GBA = 10.f;

			const float NameW_GBA = 90.f;
			const float LvW_GBA = 45.f;
			const float HpW_GBA = 70.f;

			float nameX = ::PokeTL_GBA[i].x + ::NameOffset_GBA[i].x;
			float nameY = ::PokeTL_GBA[i].y + ::NameOffset_GBA[i].y;

			float lvX = ::PokeTL_GBA[i].x + ::LvOffset_GBA[i].x;
			float lvY = ::PokeTL_GBA[i].y + ::LvOffset_GBA[i].y;

			float hpX = ::PokeTL_GBA[i].x + ::HpOffset_GBA[i].x;
			float hpY = ::PokeTL_GBA[i].y + ::HpOffset_GBA[i].y;

			mSlotNameText[i] = mScene->GetUIManager()->CreateWidget<CTextBlock>("Party_Name_" + to_string(i));
			mSlotLvText[i] = mScene->GetUIManager()->CreateWidget<CTextBlock>("Party_Lv_" + to_string(i));
			mSlotHpText[i] = mScene->GetUIManager()->CreateWidget<CTextBlock>("Party_HP_" + to_string(i));

			const TCHAR* HpBar = TEXT("Texture/Pokemon/Pokemon/Party.png");
			mSlotHpBar[i] = mScene->GetUIManager()->CreateWidget<CProgressBar>("Party_HP_Bar_" + to_string(i));
			mSlotHpBar[i]->SetTexture(EProgressBarImageType::Back, "SlotHpBar", HpBar);
			mSlotHpBar[i]->SetTexture(EProgressBarImageType::Fill, "SlotHpBar");

			auto SlotHpBar = [&](CSharedPtr<CProgressBar>& b, float wGBA)
				{
					b->SetBrushAnimation(EProgressBarImageType::Back, true);
					b->AddBrushFrame(EProgressBarImageType::Back, 345, 250, 65, 7);
					b->SetSize(260, 28);
					b->SetAnimationPlayRate(EProgressBarImageType::Back, 0.f);
					b->SetCurrentFrame(EProgressBarImageType::Back, 0);
					b->SetBarDir(EProgressBarDir::RightToLeft);
				};

			auto Setup = [&](CSharedPtr<CTextBlock>& t, float wGBA)
				{
					t->SetFont("Default");
					t->SetFontSize(SlotFont);
					t->SetTextColor(255, 255, 255, 255);
					t->SetAlignH(ETextAlignH::Left);
					t->SetAlignV(ETextAlignV::Middle);
					t->SetSize(wGBA * Scale, LineH_GBA * Scale);
					t->SetPivot(FVector2D(0.f, 0.f));
					t->SetZOrder(235);
					t->SetEnable(false);
					AddWidget(t);
				};

			Setup(mSlotNameText[i], NameW_GBA);
			Setup(mSlotLvText[i], LvW_GBA);
			Setup(mSlotHpText[i], HpW_GBA);

			FVector2D pName = PartyBL_FromGBA(PartyLeft, PartyTop, nameX, nameY, LineH_GBA, Scale);
			FVector2D pLv = PartyBL_FromGBA(PartyLeft, PartyTop, lvX, lvY, LineH_GBA, Scale);
			FVector2D pHp = PartyBL_FromGBA(PartyLeft, PartyTop, hpX, hpY, LineH_GBA, Scale);

			mSlotNameText[i]->SetPos(pName.x, pName.y);
			mSlotLvText[i]->SetPos(pLv.x, pLv.y);
			mSlotHpText[i]->SetPos(pHp.x, pHp.y);
		}

	}

	mSelectIndex = 0;
	UpdateHoverVisual();

	mInputBlockFrame = 2;
	FlushPartyKeys();

	return true;
}

void CPartyUI::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (!IsEnable())
	{
		mWasEnabled = false;
		return;
	}

	if (!mWasEnabled)
	{
		mWasEnabled = true;
		OnOpened();
	}

	if (mInputBlockFrame > 0)
	{
		FlushPartyKeys();
		--mInputBlockFrame;
		return;
	}

	RefreshSlotsRuntime();
	UpdateSlotSpriteAnim(DeltaTime);

	if (GetAsyncKeyState(VK_UP) & 0x0001)    MoveUp();
	if (GetAsyncKeyState(VK_DOWN) & 0x0001)  MoveDown();
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)  MoveLeft();
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001) MoveRight();

	if (GetAsyncKeyState('D') & 0x0001) Accept();
	if (GetAsyncKeyState('S') & 0x0001) Cancel();
}

void CPartyUI::OnOpened()
{
	CLog::PrintLog("PartyUI OnOpened PartyDataPtr=" + to_string((uintptr_t)mPartyData) +
		" size=" + to_string(mPartyData ? (int)mPartyData->GetPartyList().size() : -1));

	fill(mSlotSpriteBuilt.begin(), mSlotSpriteBuilt.end(), false);

	if (mPartyData) mSelectIndex = mPartyData->GetCurrentIndex();
	else mSelectIndex = 0;

	mHoverAnimAcc = 0.f;
	mHoverAnimToggle = false;

	for (int i = 0; i < MaxParty; ++i)
	{
		mSlotSpriteBuilt[i] = false;

		if (mSlotSpriteA[i])
		{
			mSlotSpriteA[i]->ClearBrushFrames();
			mSlotSpriteA[i]->SetCurrentFrame(0);
		}

		if (mSlotSpriteB[i])
		{
			mSlotSpriteB[i]->ClearBrushFrames();
			mSlotSpriteB[i]->SetCurrentFrame(0);
		}
	}

	if (mPartyData)
	{
		const auto& list = mPartyData->GetPartyList();
		for (int i = 0; i < MaxParty; ++i)
		{
			if (i >= (int)list.size()) continue;
			if (mSlotSpriteBuilt[i]) continue;

			const auto& inst = list[i];

			float x = inst.ImageInfo.MenuSprite1.x;
			float y = inst.ImageInfo.MenuSprite1.y;

			mSlotSpriteA[i]->AddBrushFrame(x, y, 32.f, 32.f);
			mSlotSpriteA[i]->AddBrushFrame(x, y, 32.f, 32.f);

			mSlotSpriteB[i]->AddBrushFrame(x, y + 32.f, 32.f, 32.f);
			mSlotSpriteB[i]->AddBrushFrame(x, y + 32.f, 32.f, 32.f);

			mSlotSpriteBuilt[i] = true;
		}
	}

	UpdateHoverVisual();
	mInputBlockFrame = 2;
	FlushPartyKeys();
}

void CPartyUI::RefreshSlotsRuntime()
{
	if (!mPartyData) return;

	const auto& list = mPartyData->GetPartyList();

	for (int i = 0; i < MaxParty; ++i)
	{
		bool has = (i < (int)list.size());

		if (!has)
		{
			if (mSlotSpriteA[i]) mSlotSpriteA[i]->SetEnable(false);
			if (mSlotSpriteB[i]) mSlotSpriteB[i]->SetEnable(false);

			if (mSlotNameText[i]) mSlotNameText[i]->SetEnable(false);
			if (mSlotLvText[i])   mSlotLvText[i]->SetEnable(false);
			if (mSlotHpText[i])   mSlotHpText[i]->SetEnable(false);

			continue;
		}

		if (!mSlotSpriteBuilt[i])
		{
			const auto& inst = list[i];

			float x = inst.ImageInfo.MenuSprite1.x;
			float y = inst.ImageInfo.MenuSprite1.y;

			if (mSlotSpriteA[i])
			{
				mSlotSpriteA[i]->ClearBrushFrames();
				mSlotSpriteA[i]->AddBrushFrame(x, y, 32.f, 32.f);
				mSlotSpriteA[i]->AddBrushFrame(x, y, 32.f, 32.f);
				mSlotSpriteA[i]->SetCurrentFrame(0);
			}

			if (mSlotSpriteB[i])
			{
				mSlotSpriteB[i]->ClearBrushFrames();
				mSlotSpriteB[i]->AddBrushFrame(x, y + 32.f, 32.f, 32.f);
				mSlotSpriteB[i]->AddBrushFrame(x, y + 32.f, 32.f, 32.f);
				mSlotSpriteB[i]->SetCurrentFrame(0);
			}

			mSlotSpriteBuilt[i] = true;
			// 여기서 continue 하지 말고 그대로 아래 표시 로직으로 내려가게 한다
		}

		if (mSlotSpriteA[i]) mSlotSpriteA[i]->SetEnable(true);
		if (mSlotSpriteB[i]) mSlotSpriteB[i]->SetEnable(false);

		// ---- Slot Text Update ----
		if (mSlotNameText[i]) mSlotNameText[i]->SetEnable(true);
		if (mSlotLvText[i])   mSlotLvText[i]->SetEnable(true);
		if (mSlotHpText[i])   mSlotHpText[i]->SetEnable(true);

		const auto& inst = list[i];

		// 이름
		if (mSlotNameText[i])
		{
			wstring n = Utf8ToWString_Party(inst.Info.Name);
			mSlotNameText[i]->SetText(n.c_str());
		}

		// 레벨
		if (mSlotLvText[i])
		{
			wstring lv = to_wstring(inst.Level);
			mSlotLvText[i]->SetText(lv.c_str());
		}

		// 현재 HP (cur/max 형태로 표시: CurrentHP / CurrentState.HP)
		if (mSlotHpText[i])
		{
			int maxHP = inst.CurrentState.HP;
			if (maxHP < 1) maxHP = 1;

			wstring hp = to_wstring(inst.CurrentHP) + L"  " + to_wstring(maxHP);
			mSlotHpText[i]->SetText(hp.c_str());
		}

	}
}

void CPartyUI::UpdateSlotSpriteAnim(float dt)
{
	mHoverAnimAcc += dt;
	if (mHoverAnimAcc >= mHoverAnimInterval)
	{
		mHoverAnimAcc -= mHoverAnimInterval;
		mHoverAnimToggle = !mHoverAnimToggle;
	}

	if (mSelectIndex == CancelIndex)
	{
		for (int i = 0; i < MaxParty; ++i)
		{
			if (mSlotSpriteA[i]) mSlotSpriteA[i]->SetEnable(mSlotSpriteBuilt[i]);
			if (mSlotSpriteB[i]) mSlotSpriteB[i]->SetEnable(false);
		}
		return;
	}

	for (int i = 0; i < MaxParty; ++i)
	{
		bool hovered = (i == mSelectIndex);

		if (!mSlotSpriteBuilt[i]) continue;
		if (!mSlotSpriteA[i] || !mSlotSpriteB[i]) continue;

		if (!hovered)
		{
			mSlotSpriteA[i]->SetEnable(true);
			mSlotSpriteB[i]->SetEnable(false);
		}
		else
		{
			mSlotSpriteA[i]->SetEnable(!mHoverAnimToggle);
			mSlotSpriteB[i]->SetEnable(mHoverAnimToggle);
		}
	}
}

void CPartyUI::UpdateHoverVisual()
{
	bool cancelSel = (mSelectIndex == CancelIndex);

	if (mHoveredCancel)   mHoveredCancel->SetEnable(cancelSel);
	if (mUnHoveredCancel) mUnHoveredCancel->SetEnable(!cancelSel);

	for (int i = 0; i < MaxParty; ++i)
	{
		bool sel = (!cancelSel && i == mSelectIndex);
		if (mSlotHovered[i])   mSlotHovered[i]->SetEnable(sel);
		if (mSlotUnhovered[i]) mSlotUnhovered[i]->SetEnable(!sel);
	}

	if (!mTextBox) return;

	if (cancelSel)
	{
		mTextBox->SetText(TEXT("Cancel"));
		return;
	}

	if (!mPartyData)
	{
		mTextBox->SetText(TEXT("Choose a POKéMON."));
		return;
	}

	const auto& list = mPartyData->GetPartyList();
	if (mSelectIndex >= 0 && mSelectIndex < (int)list.size())
	{
		wstring n = Utf8ToWString_Party(list[mSelectIndex].Info.Name);
		mTextBox->SetText((n + L"을(를) 선택").c_str());
	}
	else
	{
		mTextBox->SetText(TEXT("Choose a POKéMON."));
	}
}


void CPartyUI::MoveUp()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mSelectIndex == 0) { UpdateHoverVisual(); return; }
	if (mSelectIndex == CancelIndex) mSelectIndex = 5;
	else mSelectIndex -= 1;

	UpdateHoverVisual();
}

void CPartyUI::MoveDown()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mSelectIndex == CancelIndex) { UpdateHoverVisual(); return; }
	if (mSelectIndex == 5) mSelectIndex = CancelIndex;
	else mSelectIndex += 1;

	UpdateHoverVisual();
}

void CPartyUI::MoveLeft()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mSelectIndex >= 1 && mSelectIndex <= 5)
		mSelectIndex = 0;
	UpdateHoverVisual();
}

void CPartyUI::MoveRight()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mSelectIndex == 0)
		mSelectIndex = 1;
	UpdateHoverVisual();
}

void CPartyUI::Accept()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mSelectIndex == CancelIndex)
	{
		Cancel();
		return;
	}

	if (!mPartyData) return;

	const auto& list = mPartyData->GetPartyList();
	if (mSelectIndex < 0 || mSelectIndex >= (int)list.size()) return;

	mPartyData->ChangePokemon(mSelectIndex);

	if (mBattleWidget)
	{
		mBattleWidget->SetPlayerPokemon(mPartyData->GetActivePokemonPtr());
		mBattleWidget->SetEnable(true);

		mBattleWidget->OnOpenedFromIntro();
	}
	if (mBattleWidget)
	{
		mBattleWidget->SetEnable(true);
		mBattleWidget->OnOpenedFromIntro();
	}
	mWasEnabled = false;
	FlushPartyKeys();
	mInputBlockFrame = 2;
	SetEnable(false);
}

void CPartyUI::Cancel()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (mBattleWidget)
	{
		mBattleWidget->SetEnable(true);
		mBattleWidget->OnOpenedFromIntro();
	}
		

	mWasEnabled = false;
	FlushPartyKeys();
	mInputBlockFrame = 2;
	SetEnable(false);


}
