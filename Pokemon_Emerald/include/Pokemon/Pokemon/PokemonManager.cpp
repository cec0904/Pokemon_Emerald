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

}

void CPokemonManager::InitTypeTable()
{
}

EPokemonType CPokemonManager::GetTypeFromString(const string& _typeStr)
{
	return EPokemonType();
}



