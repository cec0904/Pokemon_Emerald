#include "TextureManager.h"


#include "Texture.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{

}

bool CTextureManager::Init()
{

	return true;
}

//파일 이름 중복이 없어야 한다.
bool CTextureManager::LoadTexture(const string& Name, const TCHAR* FileName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
	{
		return true;
	}

	Texture = new CTexture;

	if (!Texture->LoadTexture(FileName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	mTextureMap.insert(make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
	{
		return true;
	}

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(FullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	mTextureMap.insert(make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const string& Name, const vector<const TCHAR*>& FileName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
	{
		return true;
	}

	Texture = new CTexture;

	if (!Texture->LoadTexture(FileName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	mTextureMap.insert(make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const string& Name, const vector<const TCHAR*>& FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
	{
		return true;
	}

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(FullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	mTextureMap.insert(make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const string& Name, const TCHAR* FileName, const TCHAR* Ext, int Count)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
	{
		return true;
	}

	Texture = new CTexture;

	if (!Texture->LoadTexture(FileName, Ext, Count))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	mTextureMap.insert(make_pair(Name, Texture));

	return true;
}

class CTexture* CTextureManager::FindTexture(const string& Name)
{
	auto iter = mTextureMap.find(Name);

	if (iter == mTextureMap.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CTextureManager::ReleaseTexture(class CAsset* Texture)
{
	auto iter = mTextureMap.find(Texture->GetName());

	if (iter != mTextureMap.end())
	{
		// 다른데서 사용하고 있을때 해당 에셋을 지우면 안되니까!
		if (iter->second->GetRefCount() == 1)
		{
			mTextureMap.erase(iter);
		}
	}
}
