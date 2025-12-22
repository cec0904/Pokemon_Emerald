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
	LoadFile();
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
void CPokemonManager::LoadFile()
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
		string row[16];

		for (int i = 0; i < 16; i++)
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

		PokemonDefaultStateMap.emplace(pid, stats);

		// 3. 스프라이트 좌표
		FPokemonSpritePos pos;
		pos.Front1.x = (float)stoi(row[10]);
		pos.Front1.y = (float)stoi(row[11]);
		pos.Front2.x = (float)stoi(row[12]);
		pos.Front2.y = (float)stoi(row[13]);
		pos.Back1.x = (float)stoi(row[14]);
		pos.Back1.y = (float)stoi(row[15]);

		PokemonSpritePosInfoMap.emplace(pid, pos);
	}

	file.close();

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



