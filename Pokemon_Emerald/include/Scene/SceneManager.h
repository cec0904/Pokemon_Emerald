#pragma once

#include"../Gameinfo.h"
#include "Scene.h"
#include <vector>

class CScene;

class CSceneManager
{
	DECLARE_SINGLETON(CSceneManager);

private:
	class CScene* mCurrentScene = nullptr;
	class CScene* mLoadScene = nullptr;

	vector<CScene*> mSceneStack;
	bool mDeleteCurrentOnLoad = true;
	bool mCallResumeOnLoad = false;

public:
	bool Init();
	void Input(float DeltaTime);
	void Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render();

	void RenderUI();
	void EndFrame();

public:
	//씬 만들기 
	template <typename T>
	T* CreateScene()
	{
		T* Scene = new T;

		if (!Scene->Init())
		{
			SAFE_DELETE(Scene);
			return nullptr;
		}

		return Scene;
	}

	//씬 만들기 
	template <typename T>
	T* CreateLoadScene()
	{
		T* Scene = new T;

		if (!Scene->Init())
		{
			SAFE_DELETE(Scene);
			return nullptr;
		}

		mDeleteCurrentOnLoad = true;
		mCallResumeOnLoad = false;

		mLoadScene = Scene;

		return Scene;
	}



public:
	template<typename T>
	void PushScene();
	void PopScene();

private:
	template<typename T>
	CScene* CreateSceneInternal();


};

template<typename T>
inline void CSceneManager::PushScene()
{
	if (mCurrentScene)
	{
		mCurrentScene->OnPause();
		mSceneStack.push_back(mCurrentScene);
	}

	CScene* NewScene = CreateScene<T>();
	if (!NewScene)
	{
		return;
	}

	mDeleteCurrentOnLoad = false;
	mCallResumeOnLoad = false;

	mLoadScene = NewScene;
}

template<typename T>
inline CScene* CSceneManager::CreateSceneInternal()
{
	CScene* NewScene = new T;

	if (!NewScene->Init())
	{
		delete NewScene;
		return nullptr;
	}

	return NewScene;
}
