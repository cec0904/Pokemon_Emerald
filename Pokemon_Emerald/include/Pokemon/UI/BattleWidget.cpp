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
#include "../../PokemonData.h"
#include "../../Pokemon/Pokemon/Pokemon.h"
#include "../../Pokemon/Pokemon/PokemonManager.h"
#include "../../Asset/Asset.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Font/Font.h"
#include "../../Asset/Font/FontManager.h"
#include "../../UI/Common/TextBlock.h"
#include "Inventory.h"
#include "PartyUI.h"
#include <Windows.h>
#include <string>

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
	CSharedPtr<CImage> Cursor = mScene->GetUIManager()->CreateWidget<CImage>("Cursor");

	CSharedPtr<CTextBlock> MsgText = mScene->GetUIManager()->CreateWidget<CTextBlock>("MsgText");



	BattleArena->SetTexture("BattleBack", TEXT("Texture/Pokemon/BackGround/BattleArenas.png"));
	BattleArena->SetBrushAnimation(true);
	// 시작지점x, 시작지점y, 얼마나할지x(이미지가로크기), 얼마나할지(이미지세로크기)y
	BattleArena->AddBrushFrame(2.f, 22.f, 240.f, 112.f);
	BattleArena->SetSize(960.f, 448.f);		// 4배
	BattleArena->SetPivot(FVector2D(0.5f, 0.f));
	BattleArena->SetPos(ScreenW * 0.5f, ScreenH - 448.f);

	// 614*296
	// 297*57
	// 317*239 시작위치
	// 이미지 240*48
	BattleUIBack->SetTexture("BattleUIBack", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	BattleUIBack->SetBrushAnimation(true);
	BattleUIBack->AddBrushFrame(297.f, 56.f, 240.f, 48.f);
	BattleUIBack->SetSize(960.f, 192.f);
	BattleUIBack->SetPivot(FVector2D(0.5f, 0.f));
	BattleUIBack->SetPos(ScreenW * 0.5f, ScreenH - 640.f);

	BehaviorSelect->SetTexture("BehaviorSelect", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	BehaviorSelect->SetBrushAnimation(true);
	BehaviorSelect->AddBrushFrame(146.f, 4.f, 120.f, 48.f);
	BehaviorSelect->SetSize(480.f, 192.f);
	BehaviorSelect->SetPivot(FVector2D(0.5f, 0.f));
	BehaviorSelect->SetPos(ScreenW * 0.5f + 240.f, ScreenH - 640.f);

	// 시작지점297 * 4
	// 240 * 48
	SkillSelect->SetTexture("SkillSelect", TEXT("Texture/Pokemon/BackGround/BattleMenu.png"));
	SkillSelect->SetBrushAnimation(true);
	SkillSelect->AddBrushFrame(297.f, 4.f, 240.f, 48.f);
	SkillSelect->SetSize(960.f, 192.f);
	SkillSelect->SetPivot(FVector2D(0.5f, 0.f));
	SkillSelect->SetPos(ScreenW * 0.5f, ScreenH - 640.f);
	SkillSelect->SetEnable(false);

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

	Cursor->SetTexture("Cursor", TEXT("Texture/Pokemon/Player/Cursor.png"));
	Cursor->SetSize(32.f, 32.f);
	Cursor->SetPivot(FVector2D(0.f, 0.f));
	Cursor->SetPos(ScreenW * 0.5f, ScreenH - 640.f);

	MsgText->SetSize(520.f, 120.f);
	MsgText->SetFont("Default");
	MsgText->SetFontSize(32.f);
	MsgText->SetTextColor(255, 255, 255, 255);
	MsgText->SetPos(120.f, ScreenH - 610.f);
	

	const auto& PosMap = CPokemonManager::GetInst()->GetPosMap();

	PokemonID MyID = (PokemonID)1;	// 나무지기
	PokemonID EnemyID = (PokemonID)4;	// 아차모

	auto itMy = PosMap.find(MyID);
	auto itEnemy = PosMap.find(EnemyID);

	if (itMy != PosMap.end() && itEnemy != PosMap.end())
	{
		// 내 포켓몬 (Back)
		CSharedPtr<CImage> MyPokemon =
			mScene->GetUIManager()->CreateWidget<CImage>("MyPokemon");

		MyPokemon->SetTexture("MyPokemonTex",
			TEXT("Texture/Pokemon/Pokemon/BackSprite.png"));
		MyPokemon->SetBrushAnimation(true);
		MyPokemon->AddBrushFrame(
			itMy->second.Back1.x,
			itMy->second.Back1.y,
			64.f, 64.f
		);
		MyPokemon->SetSize(256.f, 256.f);
		MyPokemon->SetPivot(FVector2D(0.5f, 0.5f));
		MyPokemon->SetPos(410.f, 400.f);
		MyPokemon->SetZOrder(10);
		AddWidget(MyPokemon);

		MyPokemon->SetUseColorKey(true);
		MyPokemon->SetColorKey(FVector3D(255.f / 255.f, 147.f / 255.f, 37.f / 255.f));
		MyPokemon->SetKeyThreshold(0.1f);

		// 적 포켓몬 (Front)
		CSharedPtr<CImage> EnemyPokemon =
			mScene->GetUIManager()->CreateWidget<CImage>("EnemyPokemon");

		EnemyPokemon->SetTexture("EnemyPokemonTex",
			TEXT("Texture/Pokemon/Pokemon/FrontSprite.png"));
		EnemyPokemon->SetBrushAnimation(true);
		EnemyPokemon->AddBrushFrame(
			itEnemy->second.Front1.x,
			itEnemy->second.Front1.y,
			64.f, 64.f
		);
		EnemyPokemon->SetSize(256.f, 256.f);
		EnemyPokemon->SetPivot(FVector2D(0.5f, 1.f));
		EnemyPokemon->SetPos(860.f, 675.f);
		EnemyPokemon->SetZOrder(10);
		AddWidget(EnemyPokemon);

		EnemyPokemon->SetUseColorKey(true);
		EnemyPokemon->SetColorKey(FVector3D(96.f / 255.f, 212.f / 255.f, 255.f / 255.f));
		EnemyPokemon->SetKeyThreshold(0.1f);

	}





	BattleArena->SetZOrder(0);
	BattleUIBack->SetZOrder(0);
	BehaviorSelect->SetZOrder(1);
	SkillSelect->SetZOrder(2);
	Cursor->SetZOrder(5);
	MsgText->SetZOrder(10);

	AddWidget(BattleArena);
	AddWidget(BattleUIBack);
	AddWidget(BehaviorSelect);
	AddWidget(SkillSelect);
	AddWidget(Cursor);
	AddWidget(MsgText);


	// 멤버로 보관
	mBattleUIBack = BattleUIBack;
	mBehaviorSelect = BehaviorSelect;
	mSkillSelect = SkillSelect;
	mCursor = Cursor;
	mMsgText = MsgText;




	mRootText.clear();
	mRootText.reserve(4);

	FVector2D bPos = mBehaviorSelect->GetPos();
	FVector2D bSize = mBehaviorSelect->GetSize();
	FVector2D bPivot = mBehaviorSelect->GetPivot();

	float bLeft = bPos.x - bSize.x * bPivot.x;
	float bBottom = bPos.y - bSize.y * bPivot.y;

	float textBaseX = bLeft + 70.f;
	float textBaseY = bBottom + 108.f;
	float gapX = 240.f;
	float gapY = 62.f;

	for (int i = 0; i < 4; ++i)
	{
		int row = i / 2;
		int col = i % 2;

		std::string name = "RootMenuText_" + std::to_string(i);
		CSharedPtr<CTextBlock> t = mScene->GetUIManager()->CreateWidget<CTextBlock>(name);

		t->SetSize(200.f, 60.f);
		t->SetFont("Default");
		t->SetFontSize(36.f);
		t->SetTextColor(0, 0, 0, 255);

		t->SetText(mRootMenuNames[i].c_str());

		float x = textBaseX + gapX * col;
		float y = textBaseY - gapY * row;
		t->SetPos(x, y);

		t->SetZOrder(2);
		AddWidget(t);

		mRootText.push_back(t);
	}


	OpenRoot();
	UpdateCursorRoot();
	

	return true;
}

void CBattleWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (!IsEnable())
		return;

	// 눌린 순간(토글)만 잡기: & 0x0001
	if (GetAsyncKeyState(VK_UP) & 0x0001)    MoveUp();
	if (GetAsyncKeyState(VK_DOWN) & 0x0001)  MoveDown();
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)  MoveLeft();
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001) MoveRight();

	if (GetAsyncKeyState('D') & 0x0001) Accept();  // 확인
	if (GetAsyncKeyState('S') & 0x0001) Cancel();  // 취소
}


void CBattleWidget::MoveUp()
{
	if (mState == EBattleUIState::Root)
	{
		if (mRootIndex >= 2)
		{
			mRootIndex -= 2;
			UpdateCursorRoot();
		}
	}
	else if (mState == EBattleUIState::Fight)
	{
		if (mSkillIndex >= 1)
		{
			mSkillIndex -= 2;
			UpdateCursorFight();
		}
	}
}

void CBattleWidget::MoveDown()
{
	if (mState == EBattleUIState::Root)
	{
		if (mRootIndex <= 1)
		{
			mRootIndex += 2;
			UpdateCursorRoot();
		}
	}
	else if (mState == EBattleUIState::Fight)
	{
		if (mSkillIndex <= 1)
		{
			mSkillIndex += 2;
			UpdateCursorFight();
		}
	}
}

void CBattleWidget::MoveLeft()
{
	if (mState == EBattleUIState::Root)
	{
		if (mRootIndex % 2 == 1)
		{
			mRootIndex -= 1;
			UpdateCursorRoot();
		}
	}
	else if (mState == EBattleUIState::Fight)
	{
		if (mSkillIndex % 2 == 1)
		{
			mSkillIndex -= 1;
			UpdateCursorFight();
		}
	}
}

void CBattleWidget::MoveRight()
{
	if (mState == EBattleUIState::Root)
	{
		if (mRootIndex % 2 == 0)
		{
			mRootIndex += 1;
			UpdateCursorRoot();
		}
	}
	else if (mState == EBattleUIState::Fight)
	{
		if (mSkillIndex % 2 == 0)
		{
			mSkillIndex += 1;
			UpdateCursorFight();
		}
	}
}

void CBattleWidget::Accept()
{
	if (mState == EBattleUIState::Root)
	{
		switch (mRootIndex)
		{
		case 0:
			CLog::PrintLog("싸운다 선택\n");
			OpenFight(); 
			return;

		case 1:
			CLog::PrintLog("가방 선택\n");
			SetMessage(L"가방 미구현");
			return;

		case 2:
			CLog::PrintLog("포켓몬 선택\n");

			if (mPartyUI) { mPartyUI->SetEnable(true); SetEnable(false); }
			return;

		case 3:
			CLog::PrintLog("도망감 선택\n");
			mRequestExitBattle = true;
			return;
		}
	}
	else if (mState == EBattleUIState::Fight)
	{

		CLog::PrintLog("기술 선택 idx=%d\n");
	}
}


void CBattleWidget::Cancel()
{
	if (mState == EBattleUIState::Fight)
	{
		OpenRoot();
		SetMessage(L"무엇을 할까?");
	}
	else
	{
		SetMessage(L"...");
	}
}


void CBattleWidget::SetMessage(const wstring& msg)
{
	if (!mMsgText)
	{
		return;
	}
	mMsgText->SetText(msg.c_str());
}

void CBattleWidget::OpenRoot()
{
	mState = EBattleUIState::Root;
	mRootIndex = 0;

	if (mBattleUIBack)   mBattleUIBack->SetEnable(true);
	if (mBehaviorSelect) mBehaviorSelect->SetEnable(true);
	if (mSkillSelect)    mSkillSelect->SetEnable(false);
	if (mMsgText)        mMsgText->SetEnable(true);

	for (auto& t : mRootText) if (t) t->SetEnable(true);
	for (auto& t : mMoveText) if (t) t->SetEnable(false);
	

	wstring nameW = L"포켓몬";
	if (mPlayerPokemon) nameW = Utf8ToWString(mPlayerPokemon->Info.Name);

	SetMessage(nameW + L"은[는]\r\n무엇을 할까?");
	UpdateCursorRoot();
}


void CBattleWidget::OpenFight()
{
	mState = EBattleUIState::Fight;
	mSkillIndex = 0;


	if (mBattleUIBack)
	{
		mBattleUIBack->SetEnable(false);
	}
	if (mBehaviorSelect)
	{
		mBehaviorSelect->SetEnable(false);
	}
	if (mSkillSelect)
	{
		mSkillSelect->SetEnable(true);
	}
	if (mMsgText)
	{
		mMsgText->SetEnable(false);
	}

	for (auto& t : mRootText)
		if (t) t->SetEnable(false);

	for (auto& t : mMoveText)
		if (t) t->SetEnable(true);


	UpdateCursorFight();
}



void CBattleWidget::UpdateCursorRoot()
{
	if (!mCursor || !mBehaviorSelect) return;

	// BehaviorSelect 정보로 박스 좌하단 계산
	FVector2D pos = mBehaviorSelect->GetPos();
	FVector2D size = mBehaviorSelect->GetSize();
	FVector2D pivot = mBehaviorSelect->GetPivot();

	float left = pos.x - size.x * pivot.x;
	float bottom = pos.y - size.y * pivot.y;

	int row = mRootIndex / 2;
	int col = mRootIndex % 2;

	float baseX = left + 26.f;
	float baseY = bottom + 118.f;

	float gapX = 240.f;
	float gapY = 60.f;

	mCursor->SetPos(baseX + gapX * col, baseY - gapY * row);
}



void CBattleWidget::UpdateCursorFight()
{
	if (!mCursor || !mSkillSelect)
		return;

	FResolution RS = CDevice::GetInst()->GetResolution();
	float ScreenW = RS.Width;
	float ScreenH = RS.Height;

	// SkillSelect는:
	// size 960x192, pivot (0.5,0), pos (ScreenW*0.5, ScreenH-640)
	float panelW = 960.f;
	float panelH = 192.f;

	float panelCenterX = ScreenW * 0.5f;
	float panelLeft = panelCenterX - panelW * 0.5f;
	float panelBottom = ScreenH - 640.f;

	// BattleMenu.png (297,4,240,48) 슬라이스를 분석하면
	// 큰 기술 박스가 대략 "왼쪽 160px(원본)" = 640px(4배) 영역임.
	// => 2열이면 col 간격은 320px이 맞음.
	int row = mSkillIndex / 2;
	int col = mSkillIndex % 2;

	float baseX = panelLeft + 26.f;  // 큰 박스 안쪽 여백
	float baseY = panelBottom + 118.f;

	float gapX = 320.f;  // 큰 박스 640 / 2
	float gapY = 60.f;

	float x = baseX + gapX * col;
	float y = baseY - gapY * row;

	mCursor->SetPos(x, y);
}


wstring CBattleWidget::GetMoveName(MoveID id)
{
	return L"Move " + to_wstring((int)id);
}
