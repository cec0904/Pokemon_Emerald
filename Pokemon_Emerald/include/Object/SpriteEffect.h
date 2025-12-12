#pragma once
#include "SceneObject.h"

class CSpriteEffect :
    public CSceneObject
{
	friend class CScene;

protected:
	CSharedPtr<class CSpriteComponent> mRoot;
	CSharedPtr<class CAudioComponent> mAudio;

	class CAnimation2D* mAnimation = nullptr;

	bool mLoop = false;

protected:
	CSpriteEffect();
	CSpriteEffect(const CSpriteEffect& Obj);
	CSpriteEffect(CSpriteEffect&& Obj);
	virtual ~CSpriteEffect();


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void AnimationEnd();

public:
	void SetAnimation(const string& Name, float PlayTime = 1.f, float PlayRate = 1.f, bool Loop = false, bool Reverse = false);
};

