#pragma once

#include "../../Gameinfo.h"

class CMaterialManager
{
	friend class CAssetManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	unordered_map<string, CSharedPtr<class CMaterial>> mMaterialMap;

public:
	bool Init();

	bool CreateMaterial(const string& Name);
	class CMaterial* FindMaterial(const string& Name);
	void ReleaseMaterial(class CAsset* Material);


};

