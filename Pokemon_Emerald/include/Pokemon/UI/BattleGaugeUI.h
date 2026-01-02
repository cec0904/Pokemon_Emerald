#pragma once
#include "../../PokemonData.h"

class CScene;
class CUserWidget;
class CImage;
class CProgressBar;

class CBattleGaugeUI
{

public:
	CBattleGaugeUI() = default;
	~CBattleGaugeUI() = default;

    bool Init(CScene* scene, CUserWidget* owner, CImage* playerHpBar, CImage* enemyHpBar);

    void Update(const FPokemonInstance* player, const FPokemonInstance* enemy, float playerExpRatio = 0.f);

    void SetEnable(bool enable);

private:
    float Clamp01(float v) const
    {
        if (v < 0.f) return 0.f;
        if (v > 1.f) return 1.f;
        return v;
    }

private:
    CScene* mScene = nullptr;
    CUserWidget* mOwner = nullptr;

    CImage* mPlayerHpBar = nullptr;
    CImage* mEnemyHpBar = nullptr;

    // 게이지들
    CSharedPtr<CProgressBar> mPlayerHpGauge;
    CSharedPtr<CProgressBar> mPlayerExpGauge;
    CSharedPtr<CProgressBar> mEnemyHpGauge;
};

