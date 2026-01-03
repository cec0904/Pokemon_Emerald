#pragma once
#include "../../PokemonData.h"
#include <cmath>

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

    void UpdateSmooth(const FPokemonInstance* player, const FPokemonInstance* enemy, float dt, float playerExpRatio = 0.f);
    bool IsPlayerHpSettled(const FPokemonInstance* player, float eps = 0.005f) const;
    bool IsEnemyHpSettled(const FPokemonInstance* enemy, float eps = 0.005f) const;


    void SetEnable(bool enable);

private:
    float Clamp01(float v) const
    {
        if (v < 0.f) return 0.f;
        if (v > 1.f) return 1.f;
        return v;
    }

    static float MoveTowards(float cur, float target, float maxDelta)
    {
        float d = target - cur;
        if (std::fabs(d) <= maxDelta) return target;
        return cur + (d > 0.f ? maxDelta : -maxDelta);
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

    bool  mShownInit = false;
    float mShownPlayerHp = 0.f;
    float mShownEnemyHp = 0.f;
    float mShownExp = 0.f;

};

