#pragma once

#include"../Gameinfo.h"

class CSceneManager
{
	DECLARE_SINGLETON(CSceneManager);

private:
	class CScene* mCurrentScene = nullptr;
	class CScene* mLoadScene = nullptr;

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

		mLoadScene = Scene;

		return Scene;
	}

};

