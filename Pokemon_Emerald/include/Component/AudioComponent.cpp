#include "AudioComponent.h"

#include "../Asset/Sound/Sound.h"
#include "../Asset/Sound/SoundManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Asset/AssetManager.h"


CAudioComponent::CAudioComponent()
{
}

CAudioComponent::CAudioComponent(const CAudioComponent& Com)
	: CSceneComponent(Com)
{
}

CAudioComponent::CAudioComponent(CAudioComponent&& Com)
	: CSceneComponent(Com)
{
}

CAudioComponent::~CAudioComponent()
{
	if (mSound)
	{
		mSound->Stop();
	}
}

void CAudioComponent::SetSound(const string& Name)
{
	if (mScene)
	{
		mSound = mScene->GetAssetManager()->FindSound(Name);
	}
	else
	{
		mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
	}
	

	if (mSound)
	{
		mSound->Play();
	}
}

void CAudioComponent::SetSound(const string& Name, const string& GroupName, bool Loop, const char* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadSound(Name, GroupName, Loop, FileName))
		{
			return;
		}
		mSound = mScene->GetAssetManager()->FindSound(Name);

	}
	else
	{
		if (!CAssetManager::GetInst()->GetSoundManager()->LoadSound(Name, GroupName, Loop, FileName))
		{
			return;
		}
		mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
	}

	if (mSound)
	{
		mSound->Play();
	}
}

void CAudioComponent::LoadSoundFullPath(const string& Name, const string& GroupName, bool Loop,
	const char* FullPath)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadSoundFullPath(Name, GroupName, Loop, FullPath))
		{
			return;
		}
		mSound = mScene->GetAssetManager()->FindSound(Name);

	}
	else
	{
		if (!CAssetManager::GetInst()->GetSoundManager()->LoadSoundFullPath(Name, GroupName, Loop, FullPath))
		{
			return;
		}
		mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
	}

	if (mSound)
	{
		mSound->Play();
	}
}

bool CAudioComponent::Init()
{
	return CSceneComponent::Init();
}

bool CAudioComponent::Init(const char* FileName)
{
	return CSceneComponent::Init(FileName);
}

void CAudioComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

CAudioComponent* CAudioComponent::Clone()
{
	return new CAudioComponent(*this);
}
