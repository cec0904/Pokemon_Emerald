#pragma once

#include "../../Gameinfo.h"

class CTextureManager
{
	friend class CAssetManager;

private:
	CTextureManager();
	~CTextureManager();

private:
	unordered_map<string, CSharedPtr<class CTexture>> mTextureMap;

public:
	bool Init();

public:
	//파일명 
	bool LoadTexture(const string& Name, const TCHAR* FileName);
	bool LoadTextureFullPath(const string& Name, const TCHAR* FullPath);

	//텍스쳐 여러장 불러오기
	bool LoadTexture(const string& Name, const vector<const TCHAR*>& FileName);
	bool LoadTextureFullPath(const string& Name, const vector<const TCHAR*>& FullPath);
	//규칙적인 파일이름으로 여러장 불러온다.
	bool LoadTexture(const string& Name, const TCHAR* FileName, const TCHAR* Ext, int Count);


	class CTexture* FindTexture(const string& Name);
	void ReleaseTexture(class CAsset* Texture);

};