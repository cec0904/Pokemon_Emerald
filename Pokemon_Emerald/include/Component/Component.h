#pragma once
#include "../Share/Object.h"

class CComponent abstract :
	public CObject
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CComponent();
	CComponent(const CComponent& Obj);
	CComponent(CComponent&& Obj);
	virtual ~CComponent();

protected:
	class CScene* mScene = nullptr;
	class CSceneObject* mOwnerObject = nullptr;
	string		mName;

public:
	class CScene* GetScene()	const
	{
		return mScene;
	}

	class CSceneObject* GetOwner()	const
	{
		return mOwnerObject;
	}

	const char* GetName()	const
	{
		// c_str() 함수는 string이 가지고 있는 문자열 포인터를 반환한다.
		return mName.c_str();
	}

	void SetName(const string& Name)
	{
		mName = Name;
	}

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CComponent* Clone();
	virtual void EraseOwner();

	virtual void EndFrame();

};

