#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../UI/Widget.h"
#include "../../UI/Common/Image.h"
#include "../../UI/Common/TextBlock.h"
#include "../../PokemonData.h"
#include <vector>

class CPlayerObject;
class CPartyData;

class CStartPokemonUI :
	public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CStartPokemonUI();
	virtual ~CStartPokemonUI();

private:
	CPlayerObject* mPlayer = nullptr;

private:
	CPlayerObject* mOwner = nullptr;
	CPartyData* mPartyData = nullptr;

public:
	void SetOwner(CPlayerObject* owner, CPartyData* partyData);

public:

	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;


public:
	vector<wstring> Index =
	{
		L"나무지기",
		L"아차모",
		L"물짱이"
	};


	int mMaxIndex = (int)Index.size();
	int mSelectIndex = 0;



	vector<CSharedPtr<CTextBlock>> mTextList;
	CSharedPtr<CImage> mCursor;
	CSharedPtr<CImage> mBall1;
	CSharedPtr<CImage> mBall2;
	CSharedPtr<CImage> mBall3;

	CSharedPtr<CImage> mBall[3];
	
	CSharedPtr<CTextBlock> mTextBox;
	CSharedPtr<CTextBlock> mConfirmText;

	bool mIsConfirming = false;

	FPokemonInstance* mPlayerPokemon = nullptr;
	CSharedPtr<class CImage> mPlayerSprite;

public:

	void UpdateCursor();
	void Select(float DeltaTime);
	void Cancel(float DeltaTime);
	void SetMessage(const wstring& msg);

public:
	void InputLeft();
	void InputRight();



private:
	void ConfirmYes(float DeltaTime);

private:
	int   mInputBlockFrame = 0;

	// Cursor smooth + bob
	FVector2D mCursorCurPos = FVector2D(0.f, 0.f);
	FVector2D mCursorTargetPos = FVector2D(0.f, 0.f);
	float mCursorLerpSpeed = 18.f;     // 클수록 더 빠르게 붙음
	float mCursorBobAcc = 0.f;
	float mCursorBobAmp = 10.f;        // 위아래 흔들 폭(px)
	float mCursorBobFreq = 2.5f;       // Hz

	// Ball wiggle (0-1-2-1)
	float mBallAnimAcc = 0.f;
	float mBallAnimInterval = 0.12f;   // 프레임 변경 간격
	int   mBallAnimStep = 0;



private:
	void TickCursor(float dt);
	void TickBall(float dt);


public:
	static bool sBlockWorldInput;
	void Confirm(float DeltaTime);

private:
	bool mWasEnabled = false;
	void OnOpened();
	void OnClosed();

};

