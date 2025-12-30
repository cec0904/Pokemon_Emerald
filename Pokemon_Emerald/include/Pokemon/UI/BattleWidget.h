#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"
#include <vector>

class CPartyUI;

enum class EBattleUIState
{
	Root,	// fight/bag/pokemon/run
	Fight	// 기술 4개 선택
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
	

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) override;


	void SetPlayerPokemon(const FPokemonInstance* p)
	{
		mPlayerPokemon = p;
	}
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

	wstring GetMoveName(MoveID id);

};

