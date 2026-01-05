#include "SceneManager.h"
#include "Scene.h"
#include "SceneMain.h"
#include "SceneStart.h"

#include "../Share/Log.h"

CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	for (CScene* s : mSceneStack)
	{
		SAFE_DELETE(s);
	}
	mSceneStack.clear();

	SAFE_DELETE(mCurrentScene);
}

bool CSceneManager::Init()
{
	mCurrentScene = CreateScene<CSceneStart>();

	if (mCurrentScene == nullptr)
	{
		return false;
	}

	return true;

}

void CSceneManager::Input(float DeltaTime)
{
	mCurrentScene->Input(DeltaTime);
}


void CSceneManager::Update(float DeltaTime)
{
	mCurrentScene->PreUpdate(DeltaTime);

	mCurrentScene->Update(DeltaTime);

	mCurrentScene->PostUpdate(DeltaTime);


	//만약 변경할 Scene 이 있으면 변경해준다.
	if (mLoadScene)
	{
		if (mDeleteCurrentOnLoad)
		{
			SAFE_DELETE(mCurrentScene);
		}

		mCurrentScene = mLoadScene;
		mLoadScene = nullptr;

		if (mCallResumeOnLoad && mCurrentScene)
		{
			mCurrentScene->OnResume();
		}

		// 플래그 원복
		mDeleteCurrentOnLoad = true;
		mCallResumeOnLoad = false;

		return;
	}

	return;
}

void CSceneManager::Collision(float DeltaTime)
{
	mCurrentScene->Collision(DeltaTime);
}
void CSceneManager::Render() 
{
	mCurrentScene->Render();
}

void CSceneManager::RenderUI()
{
	mCurrentScene->RenderUI();
}

void CSceneManager::EndFrame()
{
	mCurrentScene->EndFrame();
}

void CSceneManager::PopScene()
{
	if (mSceneStack.empty())
		return;

	CScene* Prev = mSceneStack.back();
	mSceneStack.pop_back();

	mDeleteCurrentOnLoad = true;
	mCallResumeOnLoad = true;

	mLoadScene = Prev;
}
