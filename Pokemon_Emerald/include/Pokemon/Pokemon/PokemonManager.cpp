#include "PokemonManager.h"
#include "../../PokemonData.h"




CPokemonManager::CPokemonManager()
{
}

CPokemonManager::~CPokemonManager()
{
}




bool CPokemonManager::Init()
{
	return true;
}

void CPokemonManager::LoadFile()
{
	ifstream file("PokemonSpecies.csv");

	if (!file.is_open())
	{
		return;
	}

	string line;
	getline(file, line);
	
	while (getline(file, line))
	{
		stringstream ss(line);
		string row[10];
		string item;
		int i = 0;
		while (getline(ss, item, ',') && i< 10)
		{
			row[i++] = item;
		}

		int id = stoi(row[0]);

		// 기본 정보 저장
		FPokemonDefaultInfo info;
		info.Name = row[1];
		info.Type1 = GetTypeFromString(row[2]);
		info.Type2 = GetTypeFromString(row[3]);
		PokemonIDMap[id] = info;

		// 종족값 저장
		FBaseStats stats;
		stats.HP = stoi(row[4]);
		stats.Atk = stoi(row[5]);
		stats.Def = stoi(row[6]);
		stats.SpAtk = stoi(row[7]);
		stats.SpDef = stoi(row[8]);
		stats.Spd = stoi(row[9]);
		PokemonDefaultStateMap[id] = stats;

		// 스프라이트 좌표
		FPokemonSpritePos Pos;
		Pos.Front1 = FVector2D(stof(row[11]), stof(row[12]));
		Pos.Front2 = FVector2D(stof(row[13]), stof(row[14]));
		Pos.Back1 = FVector2D(stof(row[15]), stof(row[16]));
		PokemonSpritePosInfoMap[id] = Pos;
	}

}


EPokemonType CPokemonManager::GetTypeFromString(const string& _typeStr)
{
	return EPokemonType();
}



