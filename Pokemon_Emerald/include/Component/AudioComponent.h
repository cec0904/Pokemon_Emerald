#pragma once
#include "SceneComponent.h"

class CAudioComponent :
    public CSceneComponent
{
	//씬이랑 오브젝트 모두 사용할거같다.
	friend class CScene;
	friend class CSceneObject;

protected:
	CAudioComponent();
	CAudioComponent(const CAudioComponent& Com);
	CAudioComponent(CAudioComponent&& Com);
	virtual ~CAudioComponent();

protected:
	CSharedPtr<class CSound> mSound;

public:
	void SetSound(const string& Name);
	void SetSound(const string& Name, const string& GroupName, bool Loop, const char* FileName);
	void LoadSoundFullPath(const string& Name, const string& GroupName, bool Loop, const char* FullPath);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void Update(float DeltaTime);
	virtual CAudioComponent* Clone();
};

