#include "Pokemon.h"
#include "../../Component/SpriteComponent.h"
#include "PokemonManager.h"
#include "../../Asset/Animation/Animation2DManager.h"
#include "../../Animation/Animation2D.h"

CPokemon::CPokemon()
{
}

CPokemon::CPokemon(const CPokemon& Obj)
	: CSceneObject(Obj)
{
}

CPokemon::CPokemon(CPokemon&& Obj)
	: CSceneObject(Obj)
{
}

CPokemon::~CPokemon()
{
}

bool CPokemon::Init()
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	

	return true;
}

void CPokemon::Update(float DeltaTime)
{

}

bool CPokemon::Init(int ID)
{
	if (!CSceneObject::Init())
	{
		return false;
	}

	const auto& IDMap = CPokemonManager::GetInst()->GetIDMap();
	const auto& PosMap = CPokemonManager::GetInst()->GetPosMap();

	auto itInfo = IDMap.find(ID);
	auto itPos = PosMap.find(ID);

	
	if (itInfo == IDMap.end() || itPos == PosMap.end())
	{
		return false;
	}

	mInfo.SpeciesID = ID;
	mInfo.Info = itInfo->second;
	mInfo.ImageInfo = itPos->second;

	
	mSprite = CreateComponent<CSpriteComponent>("PokemonSprite");

	return true;
}

void CPokemon::SetPokemonName(const string& Name)
{
	mName = Name;
	mInfo.Info.Name = Name;
}
