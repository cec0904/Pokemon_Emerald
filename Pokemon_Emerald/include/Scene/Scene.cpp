#include "Scene.h"

#include "Input.h"
#include "CameraManager.h"
#include "SceneCollision.h"
#include "SceneAssetManager.h"
#include "SceneUIManager.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();

	for (;iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();

	}

	mObjList.clear();

	SAFE_DELETE(mAssetManager);
	SAFE_DELETE(mInput);
	SAFE_DELETE(mCameraManager);
	SAFE_DELETE(mCollision);
	SAFE_DELETE(mUIManager)
}

bool CScene::Init()
{
	mInput = new CInput;
	mInput->mScene = this;
	if (!mInput->Init())
	{
		SAFE_DELETE(mInput);
		return false;
	}

	mCameraManager = new CCameraManager;
	if (!mCameraManager->Init())
	{
		SAFE_DELETE(mCameraManager);
		return false;
	}

	mCollision = new CSceneCollision;
	mCollision->mScene = this;
	if (!mCollision->Init())
	{
		SAFE_DELETE(mCollision);
		return false;
	}

	mAssetManager = new CSceneAssetManager;
	mAssetManager->mScene = this;
	if (!mAssetManager->Init())
	{
		SAFE_DELETE(mAssetManager);
		return false;
	}

	mUIManager = new CSceneUIManager;
	mUIManager->mScene = this;
	if (!mUIManager->Init())
	{
		SAFE_DELETE(mUIManager);
		return false;
	}

	return true;
}

bool CScene::Init(const char* FileName)
{
	mInput = new CInput;
	if (!mInput->Init())
	{
		SAFE_DELETE(mInput);
		return false;
	}

	mCameraManager = new CCameraManager;
	if (!mCameraManager->Init())
	{
		SAFE_DELETE(mCameraManager);
		return false;
	}

	mCollision = new CSceneCollision;

	mCollision->mScene = this;

	if (!mCollision->Init())
	{
		SAFE_DELETE(mCollision);
		return false;
	}

	mAssetManager = new CSceneAssetManager;
	mAssetManager->mScene = this;
	if (!mAssetManager->Init())
	{
		SAFE_DELETE(mAssetManager);
		return false;
	}

	mUIManager = new CSceneUIManager;
	mUIManager->mScene = this;
	if (!mUIManager->Init())
	{
		SAFE_DELETE(mUIManager);
		return false;
	}

	return true;
}

void CScene::Input(float DeltaTime)
{
	mInput->Update(DeltaTime);
}

void CScene::PreUpdate(float DeltaTime)
{
	list<CSharedPtr<CSceneObject>>::iterator iter;
	list<CSharedPtr<CSceneObject>>::iterator iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PreUpdate(DeltaTime);
		++iter;
	}
}

void CScene::Update(float DeltaTime)
{
	list<CSharedPtr<CSceneObject>>::iterator iter;
	list<CSharedPtr<CSceneObject>>::iterator iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}


	mCameraManager->Update(DeltaTime);

	//UI 매니져 업데이트
	mUIManager->Update(DeltaTime);

}

void CScene::PostUpdate(float DeltaTime)
{
	list<CSharedPtr<CSceneObject>>::iterator iter;
	list<CSharedPtr<CSceneObject>>::iterator iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CScene::Collision(float DeltaTime)
{
	// SceneCollsion이 해당 씬의 충돌을 모두 관리하게 한다!!
	// 콜리전도 조만간 수정 할것이다 .
	mCollision->Update(DeltaTime, mUIManager, mInput);

	//UI 충돌 호출순서
	//Scene -> SceneCollision -> SceneUIManager -> UserWidget -> Common UI들 충돌검사를 한다. 

}

void CScene::Render()
{
#ifdef _DEBUG
	if (mDebugQuadTree)
	{
		mCollision->Render();
	}
#endif // _DEBUG
	mCollision->ReturnNodePool();

}

void CScene::RenderUI()
{
	mUIManager->Render();
}

void CScene::EndFrame()
{
	list<CSharedPtr<CSceneObject>>::iterator iter;
	list<CSharedPtr<CSceneObject>>::iterator iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EndFrame();
	}
}
