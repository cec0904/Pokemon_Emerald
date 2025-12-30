#pragma once
#include "Scene.h"
#include "../Object/PlayerObject.h"

class CSceneMain :
    public CScene
{
	friend class CSceneManager;

protected:
	CSceneMain();
	virtual ~CSceneMain();

private:
	CPlayerObject* mPlayer = nullptr;

public:
	CPlayerObject* GetPlayer() const
	{
		return mPlayer;
	}

public:
	virtual bool Init();

};

