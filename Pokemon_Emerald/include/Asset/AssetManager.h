#pragma once

#include "../Gameinfo.h"

class CAssetManager
{
	DECLARE_SINGLETON(CAssetManager)
private:
	class CMeshManager* mMeshManager = nullptr;
	class CTextureManager* mTextureManager = nullptr;
	class CMaterialManager* mMaterialManager = nullptr;
	class CAnimation2DManager* mAnimation2DManager = nullptr;
	class CSoundManager* mSoundManager = nullptr;
	class CFontManager* mFontManager = nullptr;

public:
	class CFontManager* GetFontManager() const
	{
		return mFontManager;
	}

	class CMeshManager* GetMeshManager() const
	{
		return mMeshManager;
	}

	class CTextureManager* GetTextureManager() const
	{
		return mTextureManager;
	}

	class CMaterialManager* GetMaterialManager() const
	{
		return mMaterialManager;
	}

	class CAnimation2DManager* GetAnimationManager() const
	{
		return mAnimation2DManager;
	}

	class CSoundManager* GetSoundManager() const
	{
		return mSoundManager;
	}

public:
	bool Init();
	void ReleaseAsset(class CAsset* Asset);
};

