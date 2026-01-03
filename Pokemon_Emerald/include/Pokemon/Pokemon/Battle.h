#pragma once
#include "../../PokemonData.h"
#include "../../Gameinfo.h"

class CEnemyBattleAI;

class CBattle
{

public:
	CBattle();
	~CBattle();

	void Reset();

	void SetCombatants(FPokemonInstance* player, FPokemonInstance* enemy);

	bool RunTurn(int playerMoveSlot);

	const vector<wstring>& GetMessages() const
	{
		return mMsgs;
	}

	bool IsPlayerFainted() const;
	bool IsEnemyFainted() const;


private:
	struct FBattlerState
	{
		FPokemonAilment MajorAilment = FPokemonAilment::None;

		int ConfusionTurns = 0;
		int SleepTurns = 0;
		FBattleStatStage Stage;
	};

	static float TypeEffect(EPokemonType atk, EPokemonType def);
	static float TypeEffectBoth(EPokemonType atk, EPokemonType def1, EPokemonType def2);


private:
	FPokemonInstance* mPlayer = nullptr;
	FPokemonInstance* mEnemy = nullptr;

	FBattlerState mPlayerState;
	FBattlerState mEnemyState;

	vector<wstring> mMsgs;

	CEnemyBattleAI* mAI = nullptr;

private:
	static wstring Utf8ToWString(const string& s);
	static int ClampInt(int v, int mn, int mx);

	const FMoveData* FindMove(MoveID id) const;

	int GetPriority(MoveID id) const;
	int GetSpeed(const FPokemonInstance& p)const;

	int  FindMoveSlot(const FPokemonInstance& p, MoveID id) const;
	bool HasPP(const FPokemonInstance& p, int slot) const;
	void ConsumePP(FPokemonInstance& p, int slot);

	// 명중/상태 체크
	bool CheckCanAct(FPokemonInstance& atk, FBattlerState& atkState);
	bool CheckHit(const FPokemonInstance& atk, const FBattlerState& atkState,
		const FPokemonInstance& def, const FBattlerState& defState,
		const FMoveData& mv) const;

	int  CalcDamageSimple(const FPokemonInstance& atk, const FPokemonInstance& def, const FMoveData& mv) const;

	// 기술 1회 적용(턴 순서대로 2번 호출)
	void ApplyMoveOnce(FPokemonInstance& atk, FBattlerState& atkState,
		FPokemonInstance& def, FBattlerState& defState,
		MoveID moveId, int moveSlot);

	// 상태이상 확률 적용
	void TryApplyAilmentsByMove(FPokemonInstance& def, FBattlerState& defState, const FMoveData& mv);

	// 턴 종료 도트딜(독/화상 등)
	void ApplyEndTurnEffects(FPokemonInstance& p, FBattlerState& st);

	// 랜덤
	int  Rand100() const; // 0~99


};

