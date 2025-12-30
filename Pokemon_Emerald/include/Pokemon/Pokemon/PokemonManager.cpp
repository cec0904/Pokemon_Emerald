#include "PokemonManager.h"
#include "../../PokemonData.h"
#include "../../Share/Log.h"

CPokemonManager::CPokemonManager()
{
}

CPokemonManager::~CPokemonManager()
{
}

bool CPokemonManager::Init()
{
	LoadSpecies();
	LoadSkill();
	BuildSkillSetTable();
	return true;
}
static inline void RemoveUTF8BOM(string& s)
{
	// UTF-8 BOM: EF BB BF
	if (s.size() >= 3 &&
		(unsigned char)s[0] == 0xEF &&
		(unsigned char)s[1] == 0xBB &&
		(unsigned char)s[2] == 0xBF)
	{
		s.erase(0, 3);
	}
}

static inline string Trim(string s)
{
	// 앞 공백 제거
	size_t first = s.find_first_not_of(" \t");
	if (first == string::npos)
		return "";

	// 뒤 공백 제거
	size_t last = s.find_last_not_of(" \t");
	s = s.substr(first, last - first + 1);

	// Windows CR 제거
	if (!s.empty() && s.back() == '\r')
		s.pop_back();

	return s;
}

static inline int ToIntSafe(const string& s)
{
	string t = Trim(s);
	if (t.empty())
		return 0;

	// 숫자 앞에 BOM이 붙는 케이스 방지
	RemoveUTF8BOM(t);

	try
	{
		return stoi(t);
	}
	catch (...)
	{
		return 0;
	}
}
void CPokemonManager::LoadSpecies()
{
	string filePath = string(gRootPathMultibyte)
		+ "Asset/Texture/Pokemon/Data/PokemonSpecies.csv";


	ifstream file(filePath);

	if (!file.is_open())
	{
		CLog::PrintLog("PokemonSpecies.csv open failed: %s\n");
		return;
	}

	string line;
	getline(file, line);


	
	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		stringstream ss(line);
		string row[17];

		for (int i = 0; i < 17; i++)
		{
			getline(ss, row[i], ',');

			if (!row[i].empty() && row[i].back() == '\r')
			{
				row[i].pop_back();
			}

		}

		int id = stoi(row[0]);
		PokemonID pid = (PokemonID)id;

		// 1. 도감 기본 정보
		FPokemonDefaultInfo info;
		info.id = pid;
		info.Name = row[1];
		info.Type1 = GetTypeFromString(row[2]);
		info.Type2 = row[3].empty()
			? EPokemonType::None
			: GetTypeFromString(row[3]);

		PokemonIDMap.emplace(pid, info);


		// 2. 종족치
		FBaseStats stats;
		stats.HP = stoi(row[4]);
		stats.Atk = stoi(row[5]);
		stats.Def = stoi(row[6]);
		stats.SpAtk = stoi(row[7]);
		stats.SpDef = stoi(row[8]);
		stats.Spd = stoi(row[9]);
		stats.CatchRate = stoi(row[10]);
		

		PokemonDefaultStateMap.emplace(pid, stats);

		// 3. 스프라이트 좌표
		FPokemonSpritePos pos;
		pos.Front1.x = (float)stoi(row[11]);
		pos.Front1.y = (float)stoi(row[12]);
		pos.Front2.x = (float)stoi(row[13]);
		pos.Front2.y = (float)stoi(row[14]);
		pos.Back1.x = (float)stoi(row[15]);
		pos.Back1.y = (float)stoi(row[16]);

		PokemonSpritePosInfoMap.emplace(pid, pos);
	}

	file.close();
	CLog::PrintLog("PokemonSpecies.csv loaded\n");
}

void CPokemonManager::LoadSkill()
{
	string filePath = string(gRootPathMultibyte)
		+ "Asset/Texture/Pokemon/Data/PokemonSkill.csv";

	ifstream file(filePath);

	if (!file.is_open())
	{
		CLog::PrintLog("PokemonSkill.csv open failed\n");
		return;
	}

	string line;
	getline(file, line); // 헤더 스킵

	MoveDataMap.clear();

	while (getline(file, line))
	{
		if (line.empty()) continue;

		stringstream ss(line);
		string row[16];

		for (int i = 0; i < 16; ++i)
		{
			// CSV면 ',', TSV(메모장/엑셀탭)면 '\t'
			getline(ss, row[i], ',');

			if (!row[i].empty() && row[i].back() == '\r')
				row[i].pop_back();
		}

		// ID가 비어있으면 건너뛰기
		if (Trim(row[0]).empty()) continue;

		int id = ToIntSafe(row[0]);
		MoveID mid = (MoveID)id;

		FMoveData data;
		data.ID = mid;
		data.Name = Trim(row[1]);
		data.Type = GetTypeFromString(Trim(row[2]));

		// Category 판정
		string cat = Trim(row[3]);
		if (cat == "Physical")      data.Category = EMoveCategory::Physical;
		else if (cat == "Special")  data.Category = EMoveCategory::Special;
		else if (cat == "Status")   data.Category = EMoveCategory::Status;
		else                        data.Category = EMoveCategory::Special;

		// 스탯 매핑
		data.PP = ToIntSafe(row[4]);
		data.Power = ToIntSafe(row[5]);
		data.Accuracy = ToIntSafe(row[6]);
		data.Priority = ToIntSafe(row[7]);

		// 상태 이상 확률
		data.ConfusionChancePercent = ToIntSafe(row[9]);
		data.PoisonChancePercent = ToIntSafe(row[10]);
		data.BurnChancePercent = ToIntSafe(row[11]);
		data.ParalysisChancePercent = ToIntSafe(row[12]);
		data.SleepChancePercent = ToIntSafe(row[13]);
		data.FreezeChancePercent = ToIntSafe(row[14]);

		MoveDataMap.emplace(mid, data);
	}

	file.close();
	CLog::PrintLog("PokemonSkill.csv loaded\n");
}
void CPokemonManager::BuildSkillSetTable()
{
	PokemonSkillSet.clear();

	PokemonSkillSet[1] = { {1,1}, {1,2} };					 // 나무지기
	PokemonSkillSet[2] = { {1,1}, {1,2}, {3,3} };			 // 나무돌이
	PokemonSkillSet[3] = { {1,1}, {1,2}, {3,3}, {6,4} };		 // 나무킹
	PokemonSkillSet[4] = { {1,5}, {1,6} };					 // 아차모
	PokemonSkillSet[5] = { {1,5}, {1,6}, {3,7} };			 // 영치코
	PokemonSkillSet[6] = { {1,5}, {1,6}, {3,7}, {6,8} };		 // 번치코
	PokemonSkillSet[7] = { {1,9}, {1,10} };					 // 물짱이
	PokemonSkillSet[8] = { {1,9}, {1,10}, {3,11} };			 // 늪짱이
	PokemonSkillSet[9] = { {1,9}, {1,10}, {3,11}, {6,12} };	 // 대짱이


	PokemonSkillSet[10] = { {1,13},{2,14}};			 // 테일로
	PokemonSkillSet[11] = { {1,13},{2,14},{3,15}};// 스왈로
	PokemonSkillSet[12] = { {1,16}, {1, 17}, {3, 18} };// 꼴깍몬
	PokemonSkillSet[13] = { {1,16}, {1, 17}, {3, 18}, {4, 19} };// 꿀꺽몬



	PokemonSkillSet[14] = { {1,19}, {1,20}, {1,21}, {1,22} };//독케일
	PokemonSkillSet[15] = { {1,23}, {1,24}, {1,25}, {1,19} };//네오비트
	PokemonSkillSet[16] = { {1,26}, {1,27}, {1,28}, {1,29} };//아이스크


	PokemonSkillSet[17] = { {1,30}, {1,31}, {1,32}, {1,33} };//씨카이저
	PokemonSkillSet[18] = { {1,34}, {1,35}, {1,36}, {1,17} };//밀로틱
	PokemonSkillSet[19] = { {1,37}, {1,38}, {1,39}, {1,33} };//고래왕
	PokemonSkillSet[20] = { {1,40}, {1,41}, {1,42} };//썬더볼트
	PokemonSkillSet[21] = { {1,43}, {1,44}, {1,18},};//미라몽


	PokemonSkillSet[22] = { {1,45}, {1,46}, {1,51}, {1,58} };//보만다
	PokemonSkillSet[23] = { {1,17}, {1,46}, {1,24}, {1,47} };//릴리요
	PokemonSkillSet[24] = { {1,44}, {1,18}, {1,48}, {1,4} };//점토도리
	PokemonSkillSet[25] = { {1,50}, {1,48}, {1,24}, {1,33} };//아말도
	PokemonSkillSet[26] = { {1,12}, {1,42}, {1,51}, {1,52} };//보스로라
	PokemonSkillSet[27] = { {1,53}, {1,54}, {1,45}, {1,12} };//메타그로스


	PokemonSkillSet[28] = { {1,55}, {1,56}, {1,57}, {1,45} };//레쿠자
}

void CPokemonManager::Evolve(FPokemonInstance& inst, PokemonID nextID) const
{
	// 1) 유효성 체크
	auto itInfo = PokemonIDMap.find(nextID);
	auto itStat = PokemonDefaultStateMap.find(nextID);
	auto itPos = PokemonSpritePosInfoMap.find(nextID);

	if (itInfo == PokemonIDMap.end() || itStat == PokemonDefaultStateMap.end() || itPos == PokemonSpritePosInfoMap.end())
		return;

	// 2) 종 교체 + 기본정보/이미지 갱신
	inst.SpeciesID = nextID;
	inst.Info = itInfo->second;
	inst.ImageInfo = itPos->second;

	// 3) 스탯 재계산
	int level = inst.Level;
	const FBaseStats& base = itStat->second;

	inst.CurrentState.HP = ((base.HP * 2) * level / 100) + 10 + level;
	inst.CurrentState.Atk = ((base.Atk * 2) * level / 100) + 5;
	inst.CurrentState.Def = ((base.Def * 2) * level / 100) + 5;
	inst.CurrentState.SpAtk = ((base.SpAtk * 2) * level / 100) + 5;
	inst.CurrentState.SpDef = ((base.SpDef * 2) * level / 100) + 5;
	inst.CurrentState.Spd = ((base.Spd * 2) * level / 100) + 5;

	// HP는 풀로 회복
	inst.CurrentHP = inst.CurrentState.HP;

	// 4) 스킬 재구성(종 바뀌었으니 nextID 테이블 기준)
	BuildPokemonMoves(inst);
}

void CPokemonManager::LevelChange(FPokemonInstance& inst) const
{
	if (inst.SpeciesID == 1 && inst.Level >= 3)
	{
		Evolve(inst, 2);
	}

	if (inst.SpeciesID == 2 && inst.Level >= 6)
	{
		Evolve(inst, 3);
	}

	if (inst.SpeciesID == 4 && inst.Level >= 3)
	{
		Evolve(inst, 5);
	}

	if (inst.SpeciesID == 5 && inst.Level >= 6)
	{
		Evolve(inst, 6);
	}

	if (inst.SpeciesID == 7 && inst.Level >= 3)
	{
		Evolve(inst, 8);
	}

	if (inst.SpeciesID == 8 && inst.Level >= 6)
	{
		Evolve(inst, 9);
	}

	if (inst.SpeciesID == 10 && inst.Level >= 3)
	{
		Evolve(inst, 11);
	}

	if (inst.SpeciesID == 12 && inst.Level >= 3)
	{
		Evolve(inst, 13);
	}
}



EPokemonType CPokemonManager::GetTypeFromString(const string& _typeStr)
{
	if (_typeStr == "Normal") return EPokemonType::Normal;
	if (_typeStr == "Fight") return EPokemonType::Fight;
	if (_typeStr == "Flying") return EPokemonType::Flying;
	if (_typeStr == "Poison") return EPokemonType::Poison;
	if (_typeStr == "Ground") return EPokemonType::Ground;
	if (_typeStr == "Rock") return EPokemonType::Rock;
	if (_typeStr == "Bug") return EPokemonType::Bug;
	if (_typeStr == "Ghost") return EPokemonType::Ghost;
	if (_typeStr == "Steel") return EPokemonType::Steel;
	if (_typeStr == "Fire") return EPokemonType::Fire;
	if (_typeStr == "Water") return EPokemonType::Water;
	if (_typeStr == "Grass") return EPokemonType::Grass;
	if (_typeStr == "Electr") return EPokemonType::Electr;
	if (_typeStr == "Psychc") return EPokemonType::Psychc;
	if (_typeStr == "Ice") return EPokemonType::Ice;
	if (_typeStr == "Dragon") return EPokemonType::Dragon;
	if (_typeStr == "Dark") return EPokemonType::Dark;

	return EPokemonType::None;
}


void CPokemonManager::BuildPokemonMoves(FPokemonInstance& inst) const
{
	inst.Moves.clear();
	inst.CurrentPP.clear();

	const auto& skillSet = GetSkillSet(inst.SpeciesID);

	for (size_t i = 0; i < skillSet.size(); ++i)
	{
		int requiredLevel = skillSet[i].first;
		MoveID moveID = skillSet[i].second;

		if (requiredLevel > inst.Level)
			continue;

		// 중복 방지
		if (std::find(inst.Moves.begin(), inst.Moves.end(), moveID) != inst.Moves.end())
			continue;

		inst.Moves.push_back(moveID);

		int pp = 0;
		auto it = MoveDataMap.find(moveID);
		if (it != MoveDataMap.end())
			pp = it->second.PP;

		inst.CurrentPP.push_back(pp);
	}
}


