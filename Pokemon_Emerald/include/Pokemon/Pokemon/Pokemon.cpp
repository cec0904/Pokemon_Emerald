#include "Pokemon.h"
#include "../../Component/SpriteComponent.h"

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
	//
	return true;
}

void CPokemon::Update(float DeltaTime)
{

}

bool CPokemon::Init(int ID)
{
	mSkill.resize(4);

	return true;
}

void CPokemon::SetPokemonName(const std::string& Name)
{
	mName = Name;
	mInfo.Info.Name = Name;
}
