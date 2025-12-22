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
	LoadFile();
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
		string row[20];
		string item;
		int i = 0;
		while (getline(ss, item, ',') && i< 20)
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



