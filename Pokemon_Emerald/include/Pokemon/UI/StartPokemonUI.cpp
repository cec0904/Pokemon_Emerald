#include "StartPokemonUI.h"
#include "../../Device.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneMain.h"
#include "../../Scene/SceneEditor.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../UI/Common/Button.h"
#include "../../UI/Common/Image.h"
#include "../../Share/Log.h"
#include "../../Asset/Asset.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Font/Font.h"
#include "../../Asset/Font/FontManager.h"
#include "../../Asset/Sound/Sound.h"
#include "../../Asset/Sound/SoundManager.h"
#include "../../UI/Common/TextBlock.h"
#include "Inventory.h"
#include "PartyUI.h"
#include "../../Object/PlayerObject.h"
#include "BattleWidget.h"
#include "../../Scene/Input.h"
#include "../../PokemonData.h"
#include "../../Pokemon/Pokemon/PartyData.h"
#include "../../Pokemon/Pokemon/Pokemon.h"
#include "../../Pokemon/Pokemon/PokemonManager.h"
#include <Windows.h>
#include <cmath>

static const int StarterID[3] = { 1, 4, 7 };

static inline void FlushStartKeys()
{
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(VK_DOWN);
	GetAsyncKeyState('D');
	GetAsyncKeyState('S');
}
bool CStartPokemonUI::sBlockWorldInput = false;

void CStartPokemonUI::Confirm(float DeltaTime)
{
	CLog::PrintLog("StartUI PartyDataPtr=" + to_string((uintptr_t)mPartyData));

	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (!mIsConfirming) return;

	if (mPartyData)
	{
		int id = StarterID[mSelectIndex];
		mPartyData->AddPokemon(id, 5);
		const auto& list = mPartyData->GetPartyList();
		CLog::PrintLog( to_string(id));
	}


	SetEnable(false);

	if (mOwner)
		mOwner->OpenPartyUI();
	CLog::PrintLog("StartUI this=" + to_string((uintptr_t)this) +
		" PartyDataPtr=" + to_string((uintptr_t)mPartyData));

}

void CStartPokemonUI::OnOpened()
{
	sBlockWorldInput = true;
	mInputBlockFrame = 2;
	FlushStartKeys();
}

void CStartPokemonUI::OnClosed()
{
	sBlockWorldInput = false;
	FlushStartKeys();
}



static inline wstring Utf8ToWString(const string& s)
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

	wstring ws;
	ws.resize(len);

	MultiByteToWideChar(codepage, 0, s.c_str(), -1, &ws[0], len);

	if (!ws.empty() && ws.back() == L'\0')
		ws.pop_back();

	return ws;
}

CStartPokemonUI::CStartPokemonUI()
{
}

CStartPokemonUI::~CStartPokemonUI()
{
}



void CStartPokemonUI::InputLeft()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");
	if (mIsConfirming) return;
	if (mSelectIndex > 0)
	{
		--mSelectIndex;
		UpdateCursor();
	}
}

void CStartPokemonUI::InputRight()
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");
	if (mIsConfirming) return;
	if (mSelectIndex < mMaxIndex -1)
	{
		++mSelectIndex;
		UpdateCursor();
	}
}

void CStartPokemonUI::ConfirmYes(float DeltaTime)
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");

	if (!mIsConfirming)
		return;

	if (!mPartyData)
	{
		CLog::PrintLog("StartPokemonUI: mPartyData is null");
		return;
	}

	static const int StarterSpeciesID[3] = { 1, 4, 7 };

	int speciesID = StarterSpeciesID[mSelectIndex];

	mPartyData->AddPokemon(speciesID, 5);

	// UI 종료
	mIsConfirming = false;
	if (mConfirmText) mConfirmText->SetEnable(false);
	if (mTextBox)     mTextBox->SetEnable(false);

	SetEnable(false);

	// 키 씹힘 방지
	mInputBlockFrame = 6;
	FlushStartKeys();

	CLog::PrintLog("Starter Added");
}


void CStartPokemonUI::TickCursor(float dt)
{
	if (!mCursor) return;

	float t = 1.f - expf(-mCursorLerpSpeed * dt);
	mCursorCurPos.x += (mCursorTargetPos.x - mCursorCurPos.x) * t;
	mCursorCurPos.y += (mCursorTargetPos.y - mCursorCurPos.y) * t;

	mCursorBobAcc += dt;
	float bob = sinf(mCursorBobAcc * 6.2831853f * mCursorBobFreq) * mCursorBobAmp;

	mCursor->SetPos(mCursorCurPos.x, mCursorCurPos.y + bob);
}


void CStartPokemonUI::TickBall(float dt)
{
	// 0-1-2-1 반복
	static const int Seq[4] = { 0, 1, 2, 1 };

	mBallAnimAcc += dt;
	if (mBallAnimAcc >= mBallAnimInterval)
	{
		mBallAnimAcc -= mBallAnimInterval;
		mBallAnimStep = (mBallAnimStep + 1) % 4;
	}

	for (int i = 0; i < 3; ++i)
	{
		if (!mBall[i]) continue;

		if (i == mSelectIndex)
			mBall[i]->SetCurrentFrame(Seq[mBallAnimStep]);
		else
			mBall[i]->SetCurrentFrame(1); // 비선택은 중간 고정
	}
}


void CStartPokemonUI::UpdateCursor()
{
	if (!mCursor)
		return;

	FResolution RS = CDevice::GetInst()->GetResolution();
	float ScreenH = RS.Height;

	const float BaseX = 160.f;
	const float BaseY = ScreenH - 640.f;

	FVector2D Pos[3] =
	{
		FVector2D(BaseX + 396.f - 220.f, BaseY + 572.f - 80.f),
		FVector2D(BaseX + 640.f - 220.f, BaseY + 488.f - 120.f),
		FVector2D(BaseX + 868.f - 180.f, BaseY + 576.f - 80.f)
	};

	mCursorTargetPos = Pos[mSelectIndex];
}

void CStartPokemonUI::Select(float DeltaTime)
{
	CAssetManager::GetInst()->GetSoundManager()->Play("Button");


	if (mIsConfirming)
	{
		
		SetEnable(false);
		return;
	}
	mIsConfirming = true;

	wstring nameW = Index[mSelectIndex];
	SetMessage(nameW + L"\r\n이 포켓몬으로 결정하겠습니까?");

	mConfirmText->SetEnable(true);
	mTextBox->SetEnable(false);
	
}

void CStartPokemonUI::Cancel(float DeltaTime)
{
	if (!mIsConfirming)
	{
		return;
	}

	CAssetManager::GetInst()->GetSoundManager()->Play("Button");
	if (!mIsConfirming) return;

	mIsConfirming = false;
	mConfirmText->SetEnable(false);
	mTextBox->SetEnable(true);
}

void CStartPokemonUI::SetOwner(CPlayerObject* owner, CPartyData* partyData)
{
	this->mOwner = owner;
	this->mPartyData = partyData;

	CLog::PrintLog("StartUI SetOwner PartyDataPtr=" + to_string((uintptr_t)this->mPartyData));

}

bool CStartPokemonUI::Init()
{
	CUserWidget::Init();
	



	FResolution RS = CDevice::GetInst()->GetResolution();

	float ScreenW = RS.Width;	// 1280
	float ScreenH = RS.Height;	// 720

	CSharedPtr<CImage> PokemonSelect = mScene->GetUIManager()->CreateWidget<CImage>("PokemonSelect");
	PokemonSelect->SetTexture("PokemonSelect", TEXT("Texture/Pokemon/Pokemon/PokemonSelect.png"));
	PokemonSelect->SetBrushAnimation(true);
	PokemonSelect->AddBrushFrame(1.f, 1.f, 240.f, 160.f);
	PokemonSelect->SetSize(960.f, 640.f);
	PokemonSelect->SetPivot(FVector2D(0.f, 0.f));
	PokemonSelect->SetPos(160.f, ScreenH - 640.f);
	PokemonSelect->SetZOrder(200);
	AddWidget(PokemonSelect);

	mCursor = mScene->GetUIManager()->CreateWidget<CImage>("StartPokemonCursor");
	mCursor->SetTexture("StartPokemonCursor", TEXT("Texture/Pokemon/Pokemon/PokemonSelect.png"));
	mCursor->SetBrushAnimation(true);
	mCursor->AddBrushFrame(1.f, 161.f, 25.f, 27.f);
	mCursor->SetSize(100.f, 108.f);
	mCursor->SetPivot(FVector2D(0.f, 0.f));
	AddWidget(mCursor);


	float BaseX = 160.f;
	float BaseY = 720.f - 640.f;
	FVector2D BallPositions[3] = {
		FVector2D(BaseX + 180.f, BaseY + 380.f),
		FVector2D(BaseX + 420.f, BaseY + 250.f),
		FVector2D(BaseX + 680.f, BaseY + 380.f)
	};


	//for (int i = 0; i < 3; ++i)
	//{
	//	string Name = "Ball" + to_string(i);
	//	mBall[i] = mScene->GetUIManager()->CreateWidget<CImage>(Name);
	//	mBall[i]->SetTexture("StartPokemonCursor", TEXT("Texture/Pokemon/Pokemon/PokemonSelect.png"));
	//	mBall[i]->SetBrushAnimation(true);

	//	// 볼이 가만히 있을 때의 프레임 (스프라이트 시트 내 볼 이미지 영역)
	//	mBall[i]->AddBrushFrame(53.f, 162.f, 22.f, 20.f);
	//	mBall[i]->SetSize(88.f, 80.f); // 원본 크기에 맞춰 조정
	//	mBall[i]->SetPos(BallPositions[i].x, BallPositions[i].y);
	//	mBall[i]->SetZOrder(300);
	//	AddWidget(mBall[i]);
	//}

	for (int i = 0; i < 3; ++i)
	{
		string Name = "Ball" + to_string(i);
		mBall[i] = mScene->GetUIManager()->CreateWidget<CImage>(Name);
		mBall[i]->SetTexture("StartPokemonCursor", TEXT("Texture/Pokemon/Pokemon/PokemonSelect.png"));
		mBall[i]->SetBrushAnimation(true);

		mBall[i]->ClearBrushFrames();
		mBall[i]->AddBrushFrame(29.f, 162.f, 23.f, 20.f); // 0: left
		mBall[i]->AddBrushFrame(53.f, 162.f, 22.f, 20.f); // 1: middle(idle)
		mBall[i]->AddBrushFrame(76.f, 162.f, 21.f, 20.f); // 2: right

		mBall[i]->SetCurrentFrame(1); // 기본 중간볼
		mBall[i]->SetSize(88.f, 80.f);
		mBall[i]->SetPos(BallPositions[i].x, BallPositions[i].y);
		mBall[i]->SetZOrder(300);
		AddWidget(mBall[i]);
	}










	mTextBox = mScene->GetUIManager()->CreateWidget<CTextBlock>("SelectText");
	mTextBox->SetFont("Default");
	mTextBox->SetFontSize(64.f);
	mTextBox->SetTextColor(0, 0, 0, 255);
	mTextBox->SetAlignH(ETextAlignH::Left);
	mTextBox->SetAlignV(ETextAlignV::Middle);
	mTextBox->SetSize(180.f * 4.f, 28.f * 4.f);
	mTextBox->SetPivot(FVector2D(0.f, 0.f));
	mTextBox->SetPos(ScreenW * 0.5f - 350.f, ScreenH * 0.5f - 250.f);
	mTextBox->SetText(TEXT("Choose a POKéMON."));
	mTextBox->SetEnable(true);
	mTextBox->SetZOrder(260);
	AddWidget(mTextBox);

	mConfirmText = mScene->GetUIManager()->CreateWidget<CTextBlock>("SelectConfrimText");
	mConfirmText->SetFont("Default");
	mConfirmText->SetFontSize(64.f);
	mConfirmText->SetTextColor(0, 0, 0, 255);
	mConfirmText->SetAlignH(ETextAlignH::Left);
	mConfirmText->SetAlignV(ETextAlignV::Middle);
	mConfirmText->SetSize(180.f * 4.f, 28.f * 4.f);
	mConfirmText->SetPivot(FVector2D(0.f, 0.f));
	mConfirmText->SetPos(ScreenW * 0.5f - 350.f, ScreenH * 0.5f - 250.f);
	mConfirmText->SetEnable(true);
	mConfirmText->SetZOrder(400);

	mConfirmText->SetEnable(false);
	AddWidget(mConfirmText);

	mCursor->SetZOrder(300);

	


	UpdateCursor();
	mCursorCurPos = mCursorTargetPos;
	mCursor->SetPos(mCursorCurPos.x, mCursorCurPos.y);

	mInputBlockFrame = 2;
	FlushStartKeys();
	return true;
}

void CStartPokemonUI::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (!IsEnable())
	{
		if (mWasEnabled)
		{
			mWasEnabled = false;
			FlushStartKeys();
			mInputBlockFrame = 2;
		}
		return;
	}

	if (!mWasEnabled)
	{
		mWasEnabled = true;
		FlushStartKeys();
		mInputBlockFrame = 2;
	}

	if (mInputBlockFrame > 0)
	{
		FlushStartKeys();
		--mInputBlockFrame;
		return;
	}

	if (!mIsConfirming)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x0001) InputLeft();
		if (GetAsyncKeyState(VK_RIGHT) & 0x0001) InputRight();
		if (GetAsyncKeyState('D') & 0x0001) Select(DeltaTime);
	}
	else
	{
		if (GetAsyncKeyState('D') & 0x0001) Confirm(DeltaTime);
		if (GetAsyncKeyState('S') & 0x0001) Cancel(DeltaTime);
	}

	// 애니메이션
	TickCursor(DeltaTime);
	TickBall(DeltaTime);
}

void CStartPokemonUI::SetMessage(const wstring& msg)
{
	if (!mConfirmText)
	{
		return;
	}
	mConfirmText->SetText(msg.c_str());
}
