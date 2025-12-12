#pragma once
#include "Scene.h"

class CSceneMain :
    public CScene
{
	friend class CSceneManager;

protected:
	CSceneMain();
	virtual ~CSceneMain();



public:
	virtual bool Init();

};

