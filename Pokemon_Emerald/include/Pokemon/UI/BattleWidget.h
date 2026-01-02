#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"
#include "../../UI/Common/ProgressBar.h"
#include "../../UI/Common/TextBlock.h"
#include "BattleGaugeUI.h"
#include <vector>

class CPartyUI;
class CBattleGaugeUI;

enum class EBattleUIState
{
	Root,	// fight/bag/pokemon/run
	Fight	// 기술 4개 선택
};

enum class EBattleIntroPhase
{
	None,
	Flash,
	Grass,
	Reveal,
	EnemyEnter,
	EnemyCry,
	MsgWild,
	MsgGo,
	PlayerEnter,
	PlayerCry,
	Done
};

class CBattleWidget :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CBattleWidget();
	virtual ~CBattleWidget();

public:
	vector<wstring> mRootMenuNames =
	{
		L"싸운다",
		L"가방",
		L"포켓몬",
		L"도망감"
	};

	EBattleUIState mState = EBattleUIState::Root;
	int mRootIndex = 0;
	int mSkillIndex = 0;

	const FPokemonInstance* mPlayerPokemon = nullptr;
	const FPokemonInstance* mEnemyPokemon = nullptr;
	CPartyUI* mPartyUI = nullptr;

	bool mRequestExitBattle = false;


	// 실제 기술 담길 곳
	vector<CSharedPtr<class CTextBlock>> mRootText;
	vector<CSharedPtr<class CTextBlock>> mMoveText;

	CSharedPtr<class CImage> mBattleUIBack;
	CSharedPtr<class CImage> mBehaviorSelect;
	CSharedPtr<class CImage> mSkillSelect;
	CSharedPtr<class CImage> mCursor;
	CSharedPtr<class CTextBlock> mMsgText;

	CSharedPtr<class CTextBlock> mEnemyNameText;
	CSharedPtr<class CTextBlock> mEnemyLvText;
	CSharedPtr<class CProgressBar> mEnemyHpGauge;

	CSharedPtr<class CTextBlock> mPlayerNameText;
	CSharedPtr<class CTextBlock> mPlayerLvText;
	CSharedPtr<class CTextBlock> mPlayerHpText;
	CSharedPtr<class CProgressBar> mPlayerHpGauge;
	CSharedPtr<class CProgressBar> mPlayerExpGauge;

	CBattleGaugeUI mGaugeUI;


	CSharedPtr<class CTextBlock> mMoveTypeText;
	CSharedPtr<class CTextBlock> mMovePPText;


	int mInputBlockFrame = 0;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) override;


	void SetPlayerPokemon(const FPokemonInstance* p);
	void SetEnemyPokemon(const FPokemonInstance* p);


	void SetPartyUI(CPartyUI* ui)
	{
		mPartyUI = ui;
	}

	bool IsExitBattleRequested() const
	{ 
		return mRequestExitBattle;
	}
	void ClearExitBattleRequest()
	{
		mRequestExitBattle = false;
	}

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Accept(); // D
	void Cancel(); // S

private:
	void SetMessage(const wstring& msg);

	void OpenRoot();
	void OpenFight();


	void UpdateCursorRoot();
	void UpdateCursorFight();
	void RefreshMoveText();

	wstring GetMoveName(MoveID id);

	void UpdateStatusUI();
	void UpdateMoveInfoUI();

	wstring GetTypeName(EPokemonType type);

	/////////////////////////////// 배틀 인트로
private:
	EBattleIntroPhase mIntro= EBattleIntroPhase::None;
	bool mInroPlayer = true;
	float mIntroTime = 0.f;

	float mScreenW = 0.f;
	float mScreenH = 0.f;

	bool mIntroStarted = false;

	CSharedPtr<class CImage> mCurtainTop;
	CSharedPtr<class CImage> mCurtainBottom;

	vector<CSharedPtr<class CImage>> mGrassRows;
	vector<float> mGrassDelay;
	vector<float> mGrassBaseY;

	CSharedPtr<class CImage> mMyPokemonSprite;
	CSharedPtr<class CImage> mEnemyPokemonSprite;

	FVector2D mMyStartPos, mMyTargetPos;
	FVector2D mEnemyStartPos, mEnemyTargetPos;

private:
	void BeginIntro();
	void UpdateIntro(float dt);
	void EnterIntro(EBattleIntroPhase next);
	void SetBattleUIEnable(bool enable);
	void TryStartIntro();
	void CreateIntroWidgets();
	void EnsurePokemonSprites();
};

