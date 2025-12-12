#pragma once
#include "SceneObject.h"
class CBackObject :
    public CSceneObject
{
	friend class CScene;

protected:
	CBackObject();
	CBackObject(const CBackObject& Obj);
	CBackObject(CBackObject&& Obj);
	virtual ~CBackObject();

protected:
	//CSharedPtr<class CStaticMeshComponent> mRoot;
	CSharedPtr<class CSpriteComponent> mRoot;

public:
	virtual bool Init();

};

