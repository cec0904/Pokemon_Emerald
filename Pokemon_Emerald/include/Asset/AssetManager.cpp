#include "AssetManager.h"

#include "Asset.h"
#include "Mesh/MeshManager.h"
#include "Texture/TextureManager.h"
#include "Material/MaterialManager.h"
#include "Animation/Animation2DManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"

CAssetManager::CAssetManager()
{

}

CAssetManager::~CAssetManager()
{
	SAFE_DELETE(mFontManager);
	SAFE_DELETE(mAnimation2DManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mTextureManager);
	SAFE_DELETE(mSoundManager);
}

bool CAssetManager::Init()
{
	//이런게 있구나 하고 보면 되는거 어디 저장하고나 메모해두고 사용할때마다 찾아서 쓰면된다.
	//gRootPath 경로를 초기화 해준다.
	GetModuleFileName(0, gRootPath, MAX_PATH);

	//D:\KDT4\KDT4FrameWork\KDT4Framework\KDT4Framework\Bin\KDT4Framework_Debug.exe
	// 실행파일부분을 제거하고 폴더 경로만 사용하고싶다. 
	// D:\KDT4\KDT4FrameWork\KDT4Framework\KDT4Framework\Bin//
	// '/' 비슷하다 '\\'

	int Length = lstrlen(gRootPath);

	for (int i = Length - 1; i >= 0; --i)
	{
		if (gRootPath[i] == '/' || gRootPath[i] == '\\' )
		{
			// abc\def
			// 0123456
			memset(&gRootPath[i + 1], 0, sizeof(TCHAR) * (Length - (i + 1)));
			break;
		}
	}

	//TCHar -> char
	Length = WideCharToMultiByte(CP_ACP, 0, gRootPath, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, gRootPath, -1, gRootPathMultibyte, Length, nullptr, nullptr);

	//머티리얼
	mMaterialManager = new CMaterialManager;

	if (!mMaterialManager->Init())
	{
		SAFE_DELETE(mMaterialManager);
		return false;
	}

	//메쉬를 관리 
	mMeshManager = new CMeshManager;

	if (!mMeshManager->Init())
	{
		SAFE_DELETE(mMeshManager);
		return false;
	}

	//이미지 텍스쳐

	mTextureManager = new CTextureManager;

	if (!mTextureManager->Init())
	{
		SAFE_DELETE(mTextureManager);
		return false;
	}

	// 애니메이션

	mAnimation2DManager = new CAnimation2DManager;

	if (!mAnimation2DManager->Init())
	{
		SAFE_DELETE(mAnimation2DManager);
		return false;
	}

	//사운드

	mSoundManager = new CSoundManager;

	if (!mSoundManager->Init())
	{
		SAFE_DELETE(mSoundManager);
		return false;
	}

	//폰트
	mFontManager = new CFontManager;

	if (!mFontManager->Init())
	{
		SAFE_DELETE(mFontManager);
		return false;
	}


	return true;
}

void CAssetManager::ReleaseAsset(class CAsset* Asset)
{
	switch (Asset->GetAssetType())
	{
	case EAssetType::Mesh:
		mMeshManager->ReleaseMesh(Asset);
		break;
	case EAssetType::Texture:
		mTextureManager->ReleaseTexture(Asset);
		break;
	case EAssetType::Material:
		mMaterialManager->ReleaseMaterial(Asset);
		break;
	case EAssetType::Animation2D:
		mAnimation2DManager->ReleaseAnimation(Asset);
		break;
	case EAssetType::Sound:
		mSoundManager->ReleaseSound(Asset);
		break;
	}
}
