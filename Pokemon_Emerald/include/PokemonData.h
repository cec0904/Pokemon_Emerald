#pragma once
#include "Gameinfo.h"

#include <string>
#include <array>
#include <vector>
#include <cstdint>

using PokemonID = int;
using MoveID = int;

#define IMAGE_SIZE 64

// 타입
enum class EPokemonType : uint8_t
{
	None = 0,
	Normal,
	Fight,
	Flying,
	Poison,
	Ground,
	Rock,
	Bug,
	Ghost,
	Steel,
	Fire,
	Water,
	Grass,
	Electr,
	Psychc,
	Ice,
	Dragon,
	Dark
};

// 기술 분류(물리/특수/변화)
enum class EMoveCategory : uint8_t
{
	Physical = 0,
	Special,
	Status
};

// 능력치
struct FBaseStats
{
	int HP = 1;
	int Atk = 1;
	int Def = 1;
	int SpAtk = 1;
	int SpDef = 1;
	int Spd = 1;
};

// 이미지 키 값
struct FPokemonSpriteKey
{
	std::string FrontKey;	// 앞모습
	std::string BackKey;	// 뒷모습
};

// 기술 애니메이션 키 값
struct FMoveAnimKey
{
	std::string CasterAnimKey; // 기술 쓰는 쪽
	std::string TargetAnimKey; // 맞는 쪽
};

// 기술 데이터
struct FMoveData
{
	MoveID ID = 0;
	std::string Name;

	EPokemonType Type = EPokemonType::None;
	EMoveCategory Category = EMoveCategory::Special;

	int Power = 0;     // 변화기면 0 가능
	int Accuracy = 0;
	int PP = 0;

	// 상태이상 확률
	int ConfusionChancePercent = 0; // 예: 물의파동 20%
	int PoisonChancePercent = 0;
	int BurnChancePercent = 0;
	int ParalysisChancePercent = 0;
	int SleepChancePercent = 0;
	int FreezeChancePercent = 0;

	FMoveAnimKey Anim;

	int Priority = 0;
};

// 상태이상 종류
enum class FPokemonAilment
{
	None = 0,
	Confusion,
	Poison,
	Burn,
	Paralysis,
	Sleep,
	Freeze
};

// 명중률, 회피율 랭크
struct FBattleStatStage
{
	int AccuracyStage = 0; // -6 ~ +6
	int EvasionStage = 0; // -6 ~ +6
};


// 포켓몬 도감 데이터
struct FPokemonSpeciesData
{
	PokemonID ID = 0;
	std::string Name;

	EPokemonType Type1 = EPokemonType::None;
	EPokemonType Type2 = EPokemonType::None;

	FBaseStats Base;

	int CatchRate = 0;
	int ExpYield = 0;

	FPokemonSpriteKey Sprite;

};



//포켓몬 기본 정보
struct FPokemonDefaultInfo
{
	int id = 0;
	std::string Name;
	EPokemonType Type1;
	EPokemonType Type2;
};

//포켓몬 스프라이트위치
struct FPokemonSpritePos
{
	//전면 이미지1
	FVector2D Front1;
	//전면 이미지1
	FVector2D Front2;
	//후면 이미지1
	FVector2D Back1;
};

// 런타임 포켓몬 개체
struct FPokemonInstance
{
	PokemonID SpeciesID = 0;
	FPokemonDefaultInfo Info;
	int Level = 1;

	FBaseStats CurrentState;
	int CurrentHP = 1;
	int Exp = 0;
	int CurrentExp = 0;

	//그려질 이미지 정보 
	FPokemonSpritePos ImageInfo;


	// 고정 4슬롯: 0이면 빈 슬롯
	//std::array<MoveID, 4> MoveSlots{ 0,0,0,0 };
};