#pragma once
#include "SceneObject.h"
class CEditorPlayer :
    public CSceneObject
{
	friend class CScene;

protected:
	CEditorPlayer();
	CEditorPlayer(const CEditorPlayer& Obj);
	CEditorPlayer(CEditorPlayer&& Obj);
	virtual ~CEditorPlayer();

protected:
	CSharedPtr<class CCameraComponent>		mCamera;
	CSharedPtr<class CMovementComponent>	mMovement;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);

};

