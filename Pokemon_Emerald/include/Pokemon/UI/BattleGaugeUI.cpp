#include "BattleGaugeUI.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../UI/UserWidget/UserWidget.h"
#include "../../UI/Common/Image.h"
#include "../../UI/Common/ProgressBar.h"

static inline int GetCurHP(const FPokemonInstance* p)
{
    if (!p) return 0;
    return p->CurrentHP;
}
static inline int GetMaxHP(const FPokemonInstance* p)
{
    if (!p) return 0;
    return p->CurrentState.HP;
}


static const float HP_START_X = 117.f;
static const float HP_GREEN_Y = 9.f;
static const float HP_YELLOW_Y = 13.f;
static const float HP_RED_Y = 17.f;
static const float HP_BLACK_Y = 21.f;
static const float HP_W = 9.f;
static const float HP_H = 3.f;

static const float EXP_START_X = 129.f;
static const float EXP_BLUE_Y = 7.f;
static const float EXP_BLACK_Y = 12.f;
static const float EXP_W = 7.f;
static const float EXP_H = 2.f;


static inline int GetHpFrameIndex(float ratio)
{
    if (ratio >= 0.6f) return 0;
    if (ratio >= 0.3f) return 1;
    return 2;
}

// ProgressBar에 아틀라스 프레임 세팅(HP)
static inline void SetupHpGaugeAtlas(CProgressBar* bar, const string& texName /*이미 로드된 키*/)
{
    if (!bar) return;

    // Back/Fill 둘 다 같은 아틀라스 텍스처 사용
    bar->SetTexture(EProgressBarImageType::Back, texName);
    bar->SetTexture(EProgressBarImageType::Fill, texName);

    // 틴트는 1,1,1 (원본 색 그대로)
    bar->SetTint(EProgressBarImageType::Back, 1.f, 1.f, 1.f);
    bar->SetTint(EProgressBarImageType::Fill, 1.f, 1.f, 1.f);

    // Back: 검정 프레임 1개
    bar->AddBrushFrame(EProgressBarImageType::Back, HP_START_X, HP_BLACK_Y, HP_W, HP_H);
    bar->SetBrushAnimation(EProgressBarImageType::Back, true);
    bar->SetAnimationPlayRate(EProgressBarImageType::Back, 0.f);
    bar->SetCurrentFrame(EProgressBarImageType::Back, 0);

    // Fill: 초/노/빨 3프레임
    bar->AddBrushFrame(EProgressBarImageType::Fill, HP_START_X, HP_GREEN_Y, HP_W, HP_H);   // 0
    bar->AddBrushFrame(EProgressBarImageType::Fill, HP_START_X, HP_YELLOW_Y, HP_W, HP_H);   // 1
    bar->AddBrushFrame(EProgressBarImageType::Fill, HP_START_X, HP_RED_Y, HP_W, HP_H);   // 2
    bar->SetBrushAnimation(EProgressBarImageType::Fill, true);
    bar->SetAnimationPlayRate(EProgressBarImageType::Fill, 0.f);
    bar->SetCurrentFrame(EProgressBarImageType::Fill, 0);

    bar->SetBarDir(EProgressBarDir::RightToLeft);
}

// ProgressBar에 아틀라스 프레임 세팅(EXP)
static inline void SetupExpGaugeAtlas(CProgressBar* bar, const string& texName)
{
    if (!bar) return;

    bar->SetTexture(EProgressBarImageType::Back, texName);
    bar->SetTexture(EProgressBarImageType::Fill, texName);

    bar->SetTint(EProgressBarImageType::Back, 1.f, 1.f, 1.f);
    bar->SetTint(EProgressBarImageType::Fill, 1.f, 1.f, 1.f);

    // Back: 검정
    bar->AddBrushFrame(EProgressBarImageType::Back, EXP_START_X, EXP_BLACK_Y, EXP_W, EXP_H);
    bar->SetBrushAnimation(EProgressBarImageType::Back, true);
    bar->SetAnimationPlayRate(EProgressBarImageType::Back, 0.f);
    bar->SetCurrentFrame(EProgressBarImageType::Back, 0);

    // Fill: 파랑
    bar->AddBrushFrame(EProgressBarImageType::Fill, EXP_START_X, EXP_BLUE_Y, EXP_W, EXP_H);
    bar->SetBrushAnimation(EProgressBarImageType::Fill, true);
    bar->SetAnimationPlayRate(EProgressBarImageType::Fill, 0.f);
    bar->SetCurrentFrame(EProgressBarImageType::Fill, 0);

    bar->SetBarDir(EProgressBarDir::RightToLeft);
}


bool CBattleGaugeUI::Init(CScene* scene, CUserWidget* owner, CImage* playerHpBar, CImage* enemyHpBar)
{
    mScene = scene;
    mOwner = owner;
    mPlayerHpBar = playerHpBar;
    mEnemyHpBar = enemyHpBar;

    if (!mScene || !mOwner || !mPlayerHpBar || !mEnemyHpBar)
        return false;


    mPlayerHpGauge = mScene->GetUIManager()->CreateWidget<CProgressBar>("PlayerHpGauge");
    mPlayerExpGauge = mScene->GetUIManager()->CreateWidget<CProgressBar>("PlayerExpGauge");
    mEnemyHpGauge = mScene->GetUIManager()->CreateWidget<CProgressBar>("EnemyHpGauge");

    mPlayerHpGauge->SetTexture(EProgressBarImageType::Back, "BattleGaugeAtlas", TEXT("Texture\\Pokemon\\BackGround\\BattleMenu.png"));
    mPlayerHpGauge->SetTexture(EProgressBarImageType::Fill, "BattleGaugeAtlas");

    mPlayerExpGauge->SetTexture(EProgressBarImageType::Back, "BattleGaugeAtlas", TEXT("Texture\\Pokemon\\BackGround\\BattleMenu.png"));
    mPlayerExpGauge->SetTexture(EProgressBarImageType::Fill, "BattleGaugeAtlas");

    mEnemyHpGauge->SetTexture(EProgressBarImageType::Back, "BattleGaugeAtlas", TEXT("Texture\\Pokemon\\BackGround\\BattleMenu.png"));
    mEnemyHpGauge->SetTexture(EProgressBarImageType::Fill, "BattleGaugeAtlas");



    {
        FVector2D pos = mPlayerHpBar->GetPos();
        FVector2D size = mPlayerHpBar->GetSize();
        FVector2D pivot = mPlayerHpBar->GetPivot();

        float left = pos.x - size.x * pivot.x;
        float top = pos.y - size.y * pivot.y;


        mPlayerHpGauge->SetSize(192.f, 12.f);
        mPlayerHpGauge->SetPos(left + 192.f, top + 68.f);

        mPlayerExpGauge->SetSize(256.f, 8.f);
        mPlayerExpGauge->SetPos(left + 128.f, top + 8.f);
    }


    {
        FVector2D pos = mEnemyHpBar->GetPos();
        FVector2D size = mEnemyHpBar->GetSize();
        FVector2D pivot = mEnemyHpBar->GetPivot();

        float left = pos.x - size.x * pivot.x;
        float top = pos.y - size.y * pivot.y;

        mEnemyHpGauge->SetSize(192.f, 12.f);
        mEnemyHpGauge->SetPos(left + 156.f, top + 36.f);
    }

    const string GaugeAtlasTexName = "BattleGaugeAtlas";

    SetupHpGaugeAtlas(mPlayerHpGauge.Get(), GaugeAtlasTexName);
    SetupHpGaugeAtlas(mEnemyHpGauge.Get(), GaugeAtlasTexName);
    SetupExpGaugeAtlas(mPlayerExpGauge.Get(), GaugeAtlasTexName);

    mPlayerHpGauge->SetZOrder(11);
    mPlayerExpGauge->SetZOrder(11);
    mEnemyHpGauge->SetZOrder(11);

    mPlayerHpGauge->SetEnable(true);
    mPlayerExpGauge->SetEnable(true);
    mEnemyHpGauge->SetEnable(true);

    mOwner->AddWidget(mPlayerHpGauge);
    mOwner->AddWidget(mPlayerExpGauge);
    mOwner->AddWidget(mEnemyHpGauge);

    return true;
}

void CBattleGaugeUI::SetEnable(bool enable)
{
    if (mPlayerHpGauge)  mPlayerHpGauge->SetEnable(enable);
    if (mPlayerExpGauge) mPlayerExpGauge->SetEnable(enable);
    if (mEnemyHpGauge)   mEnemyHpGauge->SetEnable(enable);
}

void CBattleGaugeUI::Update(const FPokemonInstance* player, const FPokemonInstance* enemy, float playerExpRatio)
{
    // HP ratio
    float pRatio = 0.f;
    {
        int cur = GetCurHP(player);
        int mx = GetMaxHP(player);
        if (mx > 0) pRatio = (float)cur / (float)mx;
        pRatio = Clamp01(pRatio);
    }

    float eRatio = 0.f;
    {
        int cur = GetCurHP(enemy);
        int mx = GetMaxHP(enemy);
        if (mx > 0) eRatio = (float)cur / (float)mx;
        eRatio = Clamp01(eRatio);
    }

    float exp = Clamp01(playerExpRatio);

    if (mPlayerHpGauge)
    {
        mPlayerHpGauge->SetPercent(pRatio);
        mPlayerHpGauge->SetCurrentFrame(EProgressBarImageType::Fill, GetHpFrameIndex(pRatio));
    }

    if (mEnemyHpGauge)
    {
        mEnemyHpGauge->SetPercent(eRatio);
        mEnemyHpGauge->SetCurrentFrame(EProgressBarImageType::Fill, GetHpFrameIndex(eRatio));
    }

    if (mPlayerExpGauge)
    {
        mPlayerExpGauge->SetPercent(exp);
    }

}
