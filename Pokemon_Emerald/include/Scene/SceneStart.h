#pragma once
#include "Scene.h"
class CSceneStart :
    public CScene
{
	friend class CSceneManager;

protected:
	CSceneStart();
	virtual ~CSceneStart();



public:
	virtual bool Init();

};

