#pragma once
#include "../../PokemonData.h"
#include "../../Object/SceneObject.h"

class CPokemon :
    public CSceneObject
{
	friend class CScene;

protected:
	CPokemon();
	CPokemon(const CPokemon& Obj);
	CPokemon(CPokemon&& Obj);
	virtual ~CPokemon();

protected:
	CSharedPtr<class CSpriteComponent>		mSprite;

protected:
	//능력치 
	FPokemonInstance mInfo;

	//사용할 스킬 리스트 
	std::vector<int> mSkill;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	bool Init(int ID);

public:
	void SetPokemonName(const std::string& Name);

};

