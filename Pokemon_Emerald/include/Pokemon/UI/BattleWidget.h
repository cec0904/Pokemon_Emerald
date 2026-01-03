#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"
#include "../../UI/Common/ProgressBar.h"
#include "../../UI/Common/TextBlock.h"
#include "BattleGaugeUI.h"
#include "../Pokemon/Battle.h"
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

enum class ETurnPhase
{
	None,
	ShowText,
	HpAnim,
	FaintAnim,
	NextStep,
	Done
};

struct FTurnStep
{
	enum class EType
	{
		Text,
		Hp,
		Exp,
		Faint
	}Type;

	wstring Text;
	bool bTargetEnemy = true;
	int TargetHP = 0;

	int TargetExp = 0;

	bool bFaintEnemy = true;

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

	FPokemonInstance* mPlayerPokemon = nullptr;
	FPokemonInstance* mEnemyPokemon = nullptr;
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


	vector<FTurnStep> mTurnSteps;
	int mTurnStepIndex = 0;

	ETurnPhase mTurnPhase = ETurnPhase::None;

	bool mTyping = false;
	wstring mTypingFull;
	int mTypingIndex = 0;
	float mTypingAcc = 0.f;
	float mTypingInterval = 0.03f;

	bool mHpAnimating = false;
	bool mHpAnimTargetEnemy = true;
	int mHpAnimTargetHP = 0;

	bool mFaintAnimating = false;
	bool mFaintTargetEnemy = true;
	float mFaintElapsed = 0.f;
	float mFaintDuration = 0.45f;
	float mFaintSinkPx = 90.f;
	FVector2D mFaintStartPos{};
	FVector2D mFaintStartSize{};

	
	CSharedPtr<class CImage> mPlayerSprite;
	CSharedPtr<class CImage> mEnemySprite;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) override;


	void SetPlayerPokemon(FPokemonInstance* p);
	void SetEnemyPokemon(FPokemonInstance* p);


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

private:
	vector<wstring> mTurnMsgs;
	int mTurnMsgIndex = 0;
	bool mTurnBusy = false;
	CBattle mBattle;

	void AdvanceTurnMessage();
	void StartNextTurnStep();
	void BeginTyping(const wstring& msg);
	bool UpdateTyping(float dt);

public:
	bool IsHpAnimFinished() const;
	void BeginHpAnim(bool targetEnemy, int targetHP);
	bool UpdateHpAnim(float dt);
	void BeginFaintAnim(bool enemy);
	bool UpdateFaintAnim(float dt);
	float mHpAnimElapsed = 0.f;
	float mHpAnimMinTime = 0.35f;

	// 경험치
	bool mExpAnimating = false;
	int mExpAnimTargetExp = 0;
	int mExpAnimStartExp = 0;
	float mExpAnimElapsed = 0.f;
	float mExpAnimMinTime = 0.35f;

	bool  mExitAfterTurn = false;

	void BeginExpAnim(int targetExp);
	bool UpdateExpAnim(float dt);
};

