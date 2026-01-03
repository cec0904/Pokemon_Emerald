#include "Battle.h"
#include "EnemyBattleAI.h"
#include "../Pokemon/PokemonManager.h"
#include <Windows.h>
#include <cstdlib>


CBattle::CBattle()
{
	mAI = new CEnemyBattleAI();
}


CBattle::~CBattle()
{
	delete mAI;
	mAI = nullptr;
}

void CBattle::Reset()
{
	mPlayer = nullptr;
	mEnemy = nullptr;
	mPlayerState = FBattlerState{};
	mEnemyState = FBattlerState{};
	mMsgs.clear();
}

void CBattle::SetCombatants(FPokemonInstance* player, FPokemonInstance* enemy)
{
	mPlayer = player;
	mEnemy = enemy;

	mPlayerState = FBattlerState{};
	mEnemyState = FBattlerState{};
}

bool CBattle::RunTurn(int playerMoveSlot)
{
    mMsgs.clear();

    if (!mPlayer || !mEnemy) return false;
    if (mPlayer->CurrentHP <= 0 || mEnemy->CurrentHP <= 0) return false;

    // 플레이어 기술 유효성
    if (playerMoveSlot < 0 || playerMoveSlot >= (int)mPlayer->Moves.size())
        return false;

    MoveID pMove = mPlayer->Moves[playerMoveSlot];

    // 적 AI 기술 선택
    MoveID eMove = 0;
    if (mAI) eMove = mAI->ChooseMove(*mEnemy);
    int enemySlot = FindMoveSlot(*mEnemy, eMove);

    int pPrio = GetPriority(pMove);
    int ePrio = GetPriority(eMove);

    int pSpd = GetSpeed(*mPlayer);
    int eSpd = GetSpeed(*mEnemy);

    struct Action
    {
        FPokemonInstance* atk;
        FBattlerState* atkSt;
        FPokemonInstance* def;
        FBattlerState* defSt;
        MoveID move;
        int slot;
    };

    bool playerFirst = false;
    if (pPrio != ePrio) playerFirst = (pPrio > ePrio);
    else if (pSpd != eSpd) playerFirst = (pSpd > eSpd);
    else playerFirst = (rand() % 2) == 0;

    Action first{}, second{};

    if (playerFirst)
    {
        first = { mPlayer, &mPlayerState, mEnemy, &mEnemyState, pMove, playerMoveSlot };
        second = { mEnemy,  &mEnemyState,  mPlayer,&mPlayerState, eMove, enemySlot };
    }
    else
    {
        first = { mEnemy,  &mEnemyState,  mPlayer,&mPlayerState, eMove, enemySlot };
        second = { mPlayer, &mPlayerState, mEnemy, &mEnemyState, pMove, playerMoveSlot };
    }

    // 1) 첫 행동
    ApplyMoveOnce(*first.atk, *first.atkSt, *first.def, *first.defSt, first.move, first.slot);

    // 첫 행동 후 기절이면 두 번째 스킵 (너가 말한 “반영된 채로”)
    if (first.def->CurrentHP <= 0)
    {
        // 턴 종료 도트딜은 생존자에게만
        if (mPlayer->CurrentHP > 0) ApplyEndTurnEffects(*mPlayer, mPlayerState);
        if (mEnemy->CurrentHP > 0)  ApplyEndTurnEffects(*mEnemy, mEnemyState);
        return true;
    }

    // 2) 두 번째 행동
    ApplyMoveOnce(*second.atk, *second.atkSt, *second.def, *second.defSt, second.move, second.slot);

    // 턴 종료 도트딜
    if (mPlayer->CurrentHP > 0) ApplyEndTurnEffects(*mPlayer, mPlayerState);
    if (mEnemy->CurrentHP > 0)  ApplyEndTurnEffects(*mEnemy, mEnemyState);

    return true;
}

bool CBattle::IsPlayerFainted() const
{
	return (mPlayer && mPlayer->CurrentHP <= 0);
}

bool CBattle::IsEnemyFainted() const
{
	return (mEnemy && mEnemy->CurrentHP <= 0);
}

float CBattle::TypeEffect(EPokemonType atk, EPokemonType def)
{
    if (atk == EPokemonType::None || def == EPokemonType::None)
        return 1.f;

    // 표준 타입 상성(Gen3 기준, Fairy 없음)
    // attack -> defense multiplier
    auto A = atk;
    auto D = def;

    // 편하게 switch로 처리(필요한 것만)
    switch (A)
    {
    case EPokemonType::Normal:
        if (D == EPokemonType::Rock || D == EPokemonType::Steel) return 0.5f;
        if (D == EPokemonType::Ghost) return 0.f;
        return 1.f;

    case EPokemonType::Fight:
        if (D == EPokemonType::Normal || D == EPokemonType::Rock || D == EPokemonType::Steel || D == EPokemonType::Ice || D == EPokemonType::Dark) return 2.f;
        if (D == EPokemonType::Flying || D == EPokemonType::Poison || D == EPokemonType::Bug || D == EPokemonType::Psychc) return 0.5f;
        if (D == EPokemonType::Ghost) return 0.f;
        return 1.f;

    case EPokemonType::Flying:
        if (D == EPokemonType::Fight || D == EPokemonType::Bug || D == EPokemonType::Grass) return 2.f;
        if (D == EPokemonType::Rock || D == EPokemonType::Steel || D == EPokemonType::Electr) return 0.5f;
        return 1.f;

    case EPokemonType::Poison:
        if (D == EPokemonType::Grass) return 2.f;
        if (D == EPokemonType::Poison || D == EPokemonType::Ground || D == EPokemonType::Rock || D == EPokemonType::Ghost) return 0.5f;
        if (D == EPokemonType::Steel) return 0.f;
        return 1.f;

    case EPokemonType::Ground:
        if (D == EPokemonType::Poison || D == EPokemonType::Rock || D == EPokemonType::Steel || D == EPokemonType::Fire || D == EPokemonType::Electr) return 2.f;
        if (D == EPokemonType::Bug || D == EPokemonType::Grass) return 0.5f;
        if (D == EPokemonType::Flying) return 0.f;
        return 1.f;

    case EPokemonType::Rock:
        if (D == EPokemonType::Flying || D == EPokemonType::Bug || D == EPokemonType::Fire || D == EPokemonType::Ice) return 2.f;
        if (D == EPokemonType::Fight || D == EPokemonType::Ground || D == EPokemonType::Steel) return 0.5f;
        return 1.f;

    case EPokemonType::Bug:
        if (D == EPokemonType::Grass || D == EPokemonType::Psychc || D == EPokemonType::Dark) return 2.f;
        if (D == EPokemonType::Fight || D == EPokemonType::Flying || D == EPokemonType::Poison ||
            D == EPokemonType::Ghost || D == EPokemonType::Steel || D == EPokemonType::Fire) return 0.5f;
        return 1.f;

    case EPokemonType::Ghost:
        if (D == EPokemonType::Ghost || D == EPokemonType::Psychc) return 2.f;
        if (D == EPokemonType::Dark) return 0.5f;
        if (D == EPokemonType::Normal) return 0.f;
        return 1.f;

    case EPokemonType::Steel:
        if (D == EPokemonType::Rock || D == EPokemonType::Ice) return 2.f;
        if (D == EPokemonType::Steel || D == EPokemonType::Fire || D == EPokemonType::Water || D == EPokemonType::Electr) return 0.5f;
        return 1.f;

    case EPokemonType::Fire:
        if (D == EPokemonType::Bug || D == EPokemonType::Steel || D == EPokemonType::Grass || D == EPokemonType::Ice) return 2.f;
        if (D == EPokemonType::Rock || D == EPokemonType::Fire || D == EPokemonType::Water || D == EPokemonType::Dragon) return 0.5f;
        return 1.f;

    case EPokemonType::Water:
        if (D == EPokemonType::Ground || D == EPokemonType::Rock || D == EPokemonType::Fire) return 2.f;
        if (D == EPokemonType::Water || D == EPokemonType::Grass || D == EPokemonType::Dragon) return 0.5f;
        return 1.f;

    case EPokemonType::Grass:
        if (D == EPokemonType::Ground || D == EPokemonType::Rock || D == EPokemonType::Water) return 2.f;
        if (D == EPokemonType::Flying || D == EPokemonType::Poison || D == EPokemonType::Bug || D == EPokemonType::Steel ||
            D == EPokemonType::Fire || D == EPokemonType::Grass || D == EPokemonType::Dragon) return 0.5f;
        return 1.f;

    case EPokemonType::Electr:
        if (D == EPokemonType::Flying || D == EPokemonType::Water) return 2.f;
        if (D == EPokemonType::Grass || D == EPokemonType::Electr || D == EPokemonType::Dragon) return 0.5f;
        if (D == EPokemonType::Ground) return 0.f;
        return 1.f;

    case EPokemonType::Psychc:
        if (D == EPokemonType::Fight || D == EPokemonType::Poison) return 2.f;
        if (D == EPokemonType::Steel || D == EPokemonType::Psychc) return 0.5f;
        if (D == EPokemonType::Dark) return 0.f;
        return 1.f;

    case EPokemonType::Ice:
        if (D == EPokemonType::Flying || D == EPokemonType::Ground || D == EPokemonType::Grass || D == EPokemonType::Dragon) return 2.f;
        if (D == EPokemonType::Steel || D == EPokemonType::Fire || D == EPokemonType::Water || D == EPokemonType::Ice) return 0.5f;
        return 1.f;

    case EPokemonType::Dragon:
        if (D == EPokemonType::Dragon) return 2.f;
        if (D == EPokemonType::Steel) return 0.5f;
        return 1.f;

    case EPokemonType::Dark:
        if (D == EPokemonType::Ghost || D == EPokemonType::Psychc) return 2.f;
        if (D == EPokemonType::Fight || D == EPokemonType::Dark || D == EPokemonType::Steel) return 0.5f;
        return 1.f;

    default:
        return 1.f;
    }
}

float CBattle::TypeEffectBoth(EPokemonType atk, EPokemonType def1, EPokemonType def2)
{
    float m1 = TypeEffect(atk, def1);
    float m2 = TypeEffect(atk, def2);
    return m1 * m2;
}


wstring CBattle::Utf8ToWString(const string& s)
{
	if (s.empty())
	{
		return L"";
	}
	int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);

	UINT codepage = CP_UTF8;
	if (len <= 0)
	{
		codepage = CP_ACP;
		len = MultiByteToWideChar(codepage, 0, s.c_str(), -1, nullptr, 0);
		if (len <= 0 )
		{
			return L"";
		}

	}

	wstring ws;
	ws.resize(len);
	MultiByteToWideChar(codepage, 0, s.c_str(), -1, &ws[0], len);

	if (!ws.empty() && ws.back() == L'\0')
	{
		ws.pop_back();
	}


	return ws;
}

int CBattle::ClampInt(int v, int mn, int mx)
{
	if (v < mn) return mn;
	if (v > mx) return mx;
	return v;
}

const FMoveData* CBattle::FindMove(MoveID id) const
{
	const auto& db = CPokemonManager::GetInst()->GetMoveDB();
	auto it = db.find(id);
	if (it == db.end()) return nullptr;
	return &it->second;
}

int CBattle::GetPriority(MoveID id) const
{
	const FMoveData* mv = FindMove(id);
	if (!mv) return 0;
	return mv->Priority;
}

int CBattle::GetSpeed(const FPokemonInstance& p) const
{
	return p.CurrentState.Spd;
}

int CBattle::FindMoveSlot(const FPokemonInstance& p, MoveID id) const
{
	for (int i = 0; i < (int)p.Moves.size() && i < 4; ++i)
		if (p.Moves[i] == id) return i;
	return -1;
}

bool CBattle::HasPP(const FPokemonInstance& p, int slot) const
{
	if (slot < 0) return false;
	if (slot >= (int)p.CurrentPP.size()) return true; // PP 배열이 없으면 무한 취급
	return p.CurrentPP[slot] > 0;
}

void CBattle::ConsumePP(FPokemonInstance& p, int slot)
{
	if (slot < 0) return;
	if (slot >= (int)p.CurrentPP.size()) return;
	if (p.CurrentPP[slot] > 0) --p.CurrentPP[slot];

}

bool CBattle::CheckCanAct(FPokemonInstance& atk, FBattlerState& atkState)
{
    wstring atkName = Utf8ToWString(atk.Info.Name);

    // 수면
    if (atkState.MajorAilment == FPokemonAilment::Sleep)
    {
        if (atkState.SleepTurns > 0)
        {
            atkState.SleepTurns--;
            mMsgs.push_back(atkName + L"은(는)\r\n쿨쿨 잠들어 있다!");
            return false;
        }
        // 수면 종료
        atkState.MajorAilment = FPokemonAilment::None;
        mMsgs.push_back(atkName + L"은(는)\r\n잠에서 깼다!");
    }

    // 빙결: 20%로 해동(임시 룰)
    if (atkState.MajorAilment == FPokemonAilment::Freeze)
    {
        if (Rand100() < 20)
        {
            atkState.MajorAilment = FPokemonAilment::None;
            mMsgs.push_back(atkName + L"의\r\n얼음이 녹았다!");
        }
        else
        {
            mMsgs.push_back(atkName + L"은(는)\r\n얼어붙어 움직일 수 없다!");
            return false;
        }
    }

    // 마비: 25% 행동 불가(임시 룰)
    if (atkState.MajorAilment == FPokemonAilment::Paralysis)
    {
        if (Rand100() < 25)
        {
            mMsgs.push_back(atkName + L"은(는)\r\n몸이 저려 움직일 수 없다!");
            return false;
        }
    }

    // 혼란: 50% 자해(임시 룰)
    if (atkState.ConfusionTurns > 0)
    {
        atkState.ConfusionTurns--;
        mMsgs.push_back(atkName + L"은(는)\r\n혼란에 빠져 있다!");

        if (Rand100() < 50)
        {
            // 자해 데미지: power 40 물리 취급(임시)
            // damage = 간단식
            int power = 40;
            int A = atk.CurrentState.Atk;
            int D = atk.CurrentState.Def;
            if (D <= 0) D = 1;

            int base = (2 * atk.Level) / 5 + 2;
            int dmg = (base * power * A) / D;
            dmg = dmg / 50 + 2;
            if (dmg < 1) dmg = 1;

            atk.CurrentHP -= dmg;
            if (atk.CurrentHP < 0) atk.CurrentHP = 0;

            mMsgs.push_back(L"혼란해서 자신을 공격했다!\r\n데미지 " + to_wstring(dmg) + L"!");
            return false; // 기술 사용 실패(자해로 턴 소모)
        }

        if (atkState.ConfusionTurns == 0)
        {
            mMsgs.push_back(atkName + L"의\r\n혼란이 풀렸다!");
        }
    }

    return true;
}

bool CBattle::CheckHit(const FPokemonInstance& atk, const FBattlerState& atkState, const FPokemonInstance& def, const FBattlerState& defState, const FMoveData& mv) const
{
    int acc = mv.Accuracy;
    if (acc <= 0) return true;

    int r = rand() % 100; // 0~99
    return (r < acc);
}

int CBattle::CalcDamageSimple(const FPokemonInstance& atk, const FPokemonInstance& def, const FMoveData& mv) const
{
    if (mv.Power <= 0) return 0;
    if (mv.Category == EMoveCategory::Status) return 0;

    int atkStat = 1, defStat = 1;

    if (mv.Category == EMoveCategory::Physical)
    {
        atkStat = atk.CurrentState.Atk;
        defStat = def.CurrentState.Def;
    }
    else
    {
        atkStat = atk.CurrentState.SpAtk;
        defStat = def.CurrentState.SpDef;
    }

    if (defStat <= 0) defStat = 1;

    int base = (2 * atk.Level) / 5 + 2;
    int dmg = (base * mv.Power * atkStat) / defStat;
    dmg = dmg / 50 + 2;

    float stab = 1.f;
    if (mv.Type == atk.Info.Type1 || mv.Type == atk.Info.Type2)
    {
        stab = 1.5;
    }

    float eff = TypeEffectBoth(mv.Type, def.Info.Type1, def.Info.Type2);

    float finalMul = stab * eff;
    dmg = (int)(dmg * finalMul);


    if (dmg < 1) dmg = 1;
    return dmg;
}

void CBattle::ApplyMoveOnce(FPokemonInstance& atk, FBattlerState& atkState, FPokemonInstance& def, FBattlerState& defState, MoveID moveId, int moveSlot)
{
    const FMoveData* mv = FindMove(moveId);
    wstring atkName = Utf8ToWString(atk.Info.Name);

    if (!mv)
    {
        mMsgs.push_back(atkName + L"은(는)\r\n알 수 없는 기술을 썼다!");
        return;
    }

    // PP 체크
    if (!HasPP(atk, moveSlot))
    {
        mMsgs.push_back(atkName + L"은(는)\r\nPP가 없다!");
        return;
    }

    // 행동 불가 체크(수면/빙결/마비/혼란 자해)
    if (!CheckCanAct(atk, atkState))
        return;

    // PP 소모
    ConsumePP(atk, moveSlot);

    // 기술명
    wstring moveName = Utf8ToWString(mv->Name);
    mMsgs.push_back(atkName + L"의\r\n" + moveName + L"!");

    // 명중 체크
    if (!CheckHit(atk, atkState, def, defState, *mv))
    {
        mMsgs.push_back(L"하지만 빗나갔다!");
        return;
    }

    // 변화기
    if (mv->Category == EMoveCategory::Status)
    {
        // 지금은 확률 필드만 있으니 "상태 확률"만 처리(필요하면 스탯 상승/하락 필드 추가 후 여기서 적용)
        TryApplyAilmentsByMove(def, defState, *mv);
        return;
    }

    // 데미지
    int dmg = CalcDamageSimple(atk, def, *mv);
    def.CurrentHP -= dmg;
    if (def.CurrentHP < 0) def.CurrentHP = 0;

    mMsgs.push_back(L"데미지 " + to_wstring(dmg) + L"!");

    // 맞고 나서 상태이상 확률 적용
    TryApplyAilmentsByMove(def, defState, *mv);

    // 기절
    if (def.CurrentHP <= 0)
    {
        mMsgs.push_back(Utf8ToWString(def.Info.Name) + L"은(는)\r\n쓰러졌다!");
    }
}

void CBattle::TryApplyAilmentsByMove(FPokemonInstance& def, FBattlerState& defState, const FMoveData& mv)
{
    if (mv.ConfusionChancePercent > 0 && defState.ConfusionTurns == 0)
    {
        if (Rand100() < mv.ConfusionChancePercent)
        {
            defState.ConfusionTurns = 1 + (rand() % 4); // 1~4턴(임시)
            mMsgs.push_back(Utf8ToWString(def.Info.Name) + L"은(는)\r\n혼란에 빠졌다!");
        }
    }

    if (defState.MajorAilment != FPokemonAilment::None)
        return;

    auto applyMajor = [&](FPokemonAilment a, int chance, const wchar_t* msg)
        {
            if (chance <= 0) return false;
            if (Rand100() < chance)
            {
                defState.MajorAilment = a;
                mMsgs.push_back(Utf8ToWString(def.Info.Name) + L"은(는)\r\n" + wstring(msg));
                return true;
            }
            return false;
        };

    if (applyMajor(FPokemonAilment::Poison, mv.PoisonChancePercent, L"독에 걸렸다!")) return;
    if (applyMajor(FPokemonAilment::Burn, mv.BurnChancePercent, L"화상을 입었다!")) return;
    if (applyMajor(FPokemonAilment::Paralysis, mv.ParalysisChancePercent, L"마비되었다!")) return;

    if (mv.SleepChancePercent > 0)
    {
        if (Rand100() < mv.SleepChancePercent)
        {
            defState.MajorAilment = FPokemonAilment::Sleep;
            defState.SleepTurns = 1 + (rand() % 3); // 1~3턴(임시)
            mMsgs.push_back(Utf8ToWString(def.Info.Name) + L"은(는)\r\n잠들었다!");
            return;
        }
    }

    if (applyMajor(FPokemonAilment::Freeze, mv.FreezeChancePercent, L"얼어붙었다!")) return;
}


void CBattle::ApplyEndTurnEffects(FPokemonInstance& p, FBattlerState& st)
{
    if (p.CurrentHP <= 0) return;

    wstring nameW = Utf8ToWString(p.Info.Name);

    // 독/화상 도트딜: 최대HP의 1/8(임시)
    int maxHP = p.CurrentState.HP;
    if (maxHP <= 0) maxHP = 1;

    if (st.MajorAilment == FPokemonAilment::Poison)
    {
        int dmg = maxHP / 8;
        if (dmg < 1) dmg = 1;
        p.CurrentHP -= dmg;
        if (p.CurrentHP < 0) p.CurrentHP = 0;
        mMsgs.push_back(nameW + L"은(는)\r\n독 데미지를 입었다!");
    }
    else if (st.MajorAilment == FPokemonAilment::Burn)
    {
        int dmg = maxHP / 8;
        if (dmg < 1) dmg = 1;
        p.CurrentHP -= dmg;
        if (p.CurrentHP < 0) p.CurrentHP = 0;
        mMsgs.push_back(nameW + L"은(는)\r\n화상 데미지를 입었다!");
    }

    if (p.CurrentHP <= 0)
    {
        mMsgs.push_back(nameW + L"은(는)\r\n쓰러졌다!");
    }
}

int CBattle::Rand100() const
{
	return rand() % 100;
}
