#pragma once
#include "Scene.h"
#include "../Object/PlayerObject.h"
#include "../Pokemon/Npc/NpcProfessor.h"

class CSceneMain :
    public CScene
{
	friend class CSceneManager;

protected:
	CSceneMain();
	virtual ~CSceneMain();

private:
	CPlayerObject* mPlayer = nullptr;
	CNpcProfessor* mProfessor = nullptr;

public:
	CPlayerObject* GetPlayer() const
	{
		return mPlayer;
	}

public:
	virtual bool Init();

};

