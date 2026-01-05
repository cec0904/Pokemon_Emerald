// PartyUI.h
#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"
#include <vector>

class CPartyData;
class CBattleWidget;

class CPartyUI : public CUserWidget
{
	friend class CSceneUIManager;

private:
	CPartyData* mPartyData = nullptr;
	CBattleWidget* mBattleWidget = nullptr;

	int mSelectIndex = 0;
	int mInputBlockFrame = 0;

public:
	CPartyUI();
	~CPartyUI();

	void SetPartyData(CPartyData* data)
	{
		mPartyData = data;
	}

	void SetBattleWidget(CBattleWidget* w)
	{
		mBattleWidget = w;
	}

	void ChangePokemon(int index);
	const FPokemonInstance& GetActivePokemon() const;
	const vector<FPokemonInstance>& GetPartyList() const;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) override;

private:
	static constexpr int MaxParty = 6;
	static constexpr int CancelIndex = 6;

	CSharedPtr<class CTextBlock> mTextBox;

	CSharedPtr<class CImage> mHoveredCancel;
	CSharedPtr<class CImage> mUnHoveredCancel;

	vector<CSharedPtr<class CImage>> mSlotHovered;
	vector<CSharedPtr<class CImage>> mSlotUnhovered;

	vector<CSharedPtr<class CImage>> mSlotSpriteA;
	vector<CSharedPtr<class CImage>> mSlotSpriteB;


	vector<CSharedPtr<class CTextBlock>> mSlotNameText;
	vector<CSharedPtr<class CTextBlock>> mSlotLvText;
	vector<CSharedPtr<class CTextBlock>> mSlotHpText;

	vector<CSharedPtr<class CProgressBar>> mSlotHpBar;

	vector<bool> mSlotSpriteBuilt;

	float mHoverAnimAcc = 0.f;
	float mHoverAnimInterval = 0.22f;
	bool  mHoverAnimToggle = false;

	bool mWasEnabled = false;

private:
	void OnOpened();
	void RefreshSlotsRuntime();
	void UpdateSlotSpriteAnim(float dt);
	void UpdateHoverVisual();



	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Accept();
	void Cancel();
};
