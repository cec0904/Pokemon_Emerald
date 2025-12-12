#include "Animation2DData.h"

#include "../AssetManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"


CAnimation2DData::CAnimation2DData()
{
}

CAnimation2DData::~CAnimation2DData()
{
}

void CAnimation2DData::SetTexture(class CTexture* Texture)
{
	mTexture = Texture;
}

void CAnimation2DData::SetTexture(const string& TextureName)
{
	if (mScene)
	{
		mTexture = mScene->GetAssetManager()->FindTexture(TextureName);
	}
	else
	{
		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(TextureName);
	}
}

void CAnimation2DData::SetTexture(const string& Name, const TCHAR* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

}

void CAnimation2DData::SetTexture(const string& Name, vector<const TCHAR*> FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CAnimation2DData::SetTexture(const string& Name, const TCHAR* FileName, const TCHAR* Ext, int Count)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName, Ext, Count))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName, Ext, Count))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CAnimation2DData::SetTextureFullPath(const string& Name, const TCHAR* FullPath)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTextureFullPath(Name, FullPath))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTextureFullPath(Name, FullPath))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CAnimation2DData::SetTextureFullPath(const string& Name, const vector<const TCHAR*>& FullPath)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTextureFullPath(Name, FullPath))
		{
			return;
		}
		mTexture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTextureFullPath(Name, FullPath))
		{
			return;
		}

		mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CAnimation2DData::AddFrame(const FVector2D& Start, const FVector2D& Size)
{
	FAnimationFrame Frame;
	Frame.Start = Start;
	Frame.Size = Size;

	mFrameList.emplace_back(Frame);
}

void CAnimation2DData::AddFrame(float StartX, float StartY, float SizeX, float SizeY)
{
	FAnimationFrame Frame;
	Frame.Start = FVector2D(StartX, StartY);
	Frame.Size = FVector2D(SizeX, SizeY);

	mFrameList.emplace_back(Frame);
}

void CAnimation2DData::AddFrameCount(int Count, const FVector2D& Start, const FVector2D& Size)
{
	mFrameList.clear();

	//사용할 만큼 미리 메모리 공간을 할당해둔다. 
	mFrameList.reserve(Count);

	for (int i = 0; i < Count; ++i)
	{
		FAnimationFrame Frame;
		Frame.Start = Start;
		Frame.Size = Size;

		mFrameList.emplace_back(Frame);
	}

}

void CAnimation2DData::AddFrameCount(int Count, float StartX, float StartY, float SizeX, float SizeY)
{
	mFrameList.clear();

	//사용할 만큼 미리 메모리 공간을 할당해둔다. 
	mFrameList.reserve(Count);

	for (int i = 0; i < Count; ++i)
	{
		FAnimationFrame Frame;
		Frame.Start = FVector2D(StartX, StartY);
		Frame.Size = FVector2D(SizeX, SizeY);

		mFrameList.emplace_back(Frame);
	}
}
