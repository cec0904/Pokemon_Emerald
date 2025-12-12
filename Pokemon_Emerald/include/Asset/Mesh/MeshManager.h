#pragma once

#include "../../Gameinfo.h"

class CMeshManager
{
	friend class CAssetManager;

private:
	CMeshManager();
	~CMeshManager();

private:
	unordered_map<string, CSharedPtr<class CMesh>> mMeshMap;

public:
	bool Init();

	bool CreateMesh(const string& Name, void* VertexData, int Size, int Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitiv, void* IndexData = nullptr, int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	class CMesh* FindMesh(const string& Name);

	void ReleaseMesh(class CAsset* Mesh);
};

