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
	class CAnimation2D* mAnimation = nullptr;

protected:
	//능력치 
	FPokemonInstance mInfo;

	//사용할 스킬 리스트 
	vector<int> mSkill;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	bool Init(int ID);

public:
	void SetPokemonName(const string& Name);

public:
	PokemonID GetSpeciesID() const
	{
		return mInfo.SpeciesID;
	}
	int GetLevel() const
	{
		return mInfo.Level;
	}
	int GetCurrentHP() const
	{
		return mInfo.CurrentHP;
	}
	const FPokemonInstance& GetInstance() const
	{
		return mInfo;
	}
	const FPokemonSpritePos& GetImageInfo() const
	{
		return mInfo.ImageInfo;
	}


	void SetLevel(int Level)
	{
		mInfo.Level = Level;
	}
	void SetCurrentState(const FBaseStats& State, int CurrentHP)
	{
		mInfo.CurrentState = State;
		mInfo.CurrentHP = CurrentHP;
	}

	int GetExp() const 
	{ 
		return mInfo.Exp;
	}
	int GetCurrentExp() const
	{
		return mInfo.CurrentExp;
	}

	void SetExp(int Exp)
	{
		mInfo.Exp = Exp;
	}
	void SetCurrentExp(int Exp)
	{
		mInfo.CurrentExp = Exp; 
	}

};

