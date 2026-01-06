#pragma once
#include "../../UI/UserWidget/UserWidget.h"
#include "../../PokemonData.h"

class CBattleWidget;

class CPokemonBattleIntro : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CPokemonBattleIntro();
	virtual ~CPokemonBattleIntro();

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	void Start(CBattleWidget* battleWidget, const FPokemonInstance* player, const FPokemonInstance* enemy);
	bool IsFinished() const { return mFinished; }

private:
	enum class EStep
	{
		None,
		Split,
		Done
	};

	void Go(EStep s);

private:
	EStep mStep = EStep::None;
	float mStepTime = 0.f;
	float mSplitDur = 1.f;
	bool  mFinished = false;

	CBattleWidget* mBattleWidget = nullptr;
	const FPokemonInstance* mPlayer = nullptr;
	const FPokemonInstance* mEnemy = nullptr;

private:
	CSharedPtr<class CImage> mMaskTop;
	CSharedPtr<class CImage> mMaskBottom;
};
