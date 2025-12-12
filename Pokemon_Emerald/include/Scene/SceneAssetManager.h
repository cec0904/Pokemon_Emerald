#pragma once
#include <unordered_map>

#include "../Gameinfo.h"

class CSceneAssetManager
{
	friend class CScene;

private:
	CSceneAssetManager();
	~CSceneAssetManager();

private:
	class CScene* mScene = nullptr;

private:
	// 관리할 에셋들
	unordered_map<string, CSharedPtr<class CAsset>> mAssetMap;

public:
	bool Init();

#pragma region Mesh

	bool CreateMesh(const string& Name, void* VertexData, int Size, int Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitiv, void* IndexData = nullptr, int IndexSize = 0, int IndexCount = 0, DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	class CMesh* FindMesh(const string& Name);

#pragma endregion Mesh

#pragma region Texture
	bool LoadTexture(const string& Name, const TCHAR* FileName);
	bool LoadTextureFullPath(const string& Name, const TCHAR* FullPath);

	//텍스쳐 여러장 불러오기
	bool LoadTexture(const string& Name, const vector<const TCHAR*>& FileName);
	bool LoadTextureFullPath(const string& Name, const vector<const TCHAR*>& FullPath);
	//규칙적인 파일이름으로 여러장 불러온다.
	bool LoadTexture(const string& Name, const TCHAR* FileName, const TCHAR* Ext, int Count);


	class CTexture* FindTexture(const string& Name);

#pragma endregion  Texture

#pragma region Material

	bool CreateMaterial(const string& Name);

	class CMaterial* FindMaterial(const string& Name);

#pragma endregion Material

#pragma region Animation

    bool CreateAnimation(const string& Name);
    class CAnimation2DData* FindAnimation(const string& Name);

    //애니메이션에 각각 설정 해주기
    bool SetAnimationTextureType(const string& Name, EAnimationTextureType Type);

    //텍스쳐 불러오기 
    //텍스쳐로 바로 넣어주기
    bool SetAnimationTexture(const string& Name, class CTexture* Texture);

    //텍스쳐 이름으로 찾아와서 넣어주기
    bool SetAnimationTexture(const string& Name, const string& TextureName);

    //이름이랑 파일경로 넣어주기
    bool SetAnimationTexture(const string& Name,
        const string& TextureName, const TCHAR* FileName);

    //이름, 배열
    bool SetAnimationTexture(const string& Name,
        const string& TextureName,
        vector<const TCHAR*> FileName);

    //이름 파일경로 확장자 갯수
    bool SetAnimationTexture(const string& Name,
        const string& TextureName,
        const TCHAR* FileName, const TCHAR* Ext, int Count);

    // 풀경로
    bool SetAnimationTextureFullPath(const string& Name,
        const string& TextureName, const TCHAR* FullPath);
    // 풀경로 배열 
    bool SetAnimationTextureFullPath(const string& Name,
        const string& TextureName, const vector<const TCHAR*>& FullPath);

    ////
    //FAnimationFrame 를 만들어주는 함수를 만들어주자!
    bool AddAnimationFrame(const string& Name, const FVector2D& Start, const FVector2D& Size);
    bool AddAnimationFrame(const string& Name, float StartX, float StartY, float SizeX, float SizeY);
    bool AddAnimationFrameCount(const string& Name, int Count, const FVector2D& Start, const FVector2D& Size);
    bool AddAnimationFrameCount(const string& Name, int Count, float StartX, float StartY, float SizeX, float SizeY);

#pragma endregion Animation

#pragma region Sound

    bool LoadSound(const string& Name, const string& GroupName, bool Loop, const char* FileName);
    bool LoadSoundFullPath(const string& Name, const string& GroupName, bool Loop, const char* FullPath);

    class CSound* FindSound(const string& Name);
#pragma endregion Sound

#pragma  region Font

    // Font
    bool LoadFont(const string& Name,
        const TCHAR* FontName,
        int Weight, float FontSize,
        const TCHAR* LocalName,
        int Stretch = DWRITE_FONT_STRETCH_NORMAL);
    class CFont* FindFont(const string& Name);

    // Font Collection
    bool LoadFontCollection(const string& Name, const TCHAR* FileName);
    class CFontCollection* FindFontCollection(const string& Name);

    const TCHAR* GetFontFaceName(const string& CollectionName);

    // Font Color
    bool CreateFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    bool CreateFontColor(const FVector4D& Color);

    ID2D1SolidColorBrush* FindFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    ID2D1SolidColorBrush* FindFontColor(const FVector4D& Color);



#pragma  endregion Font

};

