#include "SceneManager.h"

#include "SceneMain.h"
#include "SceneStart.h"

CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
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
		//기존 씬 제거
		SAFE_DELETE(mCurrentScene);

		mCurrentScene = mLoadScene;

		mLoadScene = nullptr;
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
