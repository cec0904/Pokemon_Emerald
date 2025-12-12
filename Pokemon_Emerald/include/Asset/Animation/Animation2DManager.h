#pragma once

#include "../../Gameinfo.h"

class CAnimation2DManager
{
	friend class CAssetManager;

private:
	CAnimation2DManager();
	~CAnimation2DManager();

private:
	unordered_map<string, CSharedPtr<class CAnimation2DData>> mAnimationMap;

public:
	bool Init();

	bool CreateAnimation(const string& Name);
	class CAnimation2DData* FindAnimation(const string& Name);
	void ReleaseAnimation(class CAsset* Animation);

	//애니메이션에 각각 설정 해주기
    bool SetAnimationTextureType(const string& Name, EAnimationTextureType Type);

    //텍스쳐 불러오기 
    //텍스쳐로 바로 넣어주기
    bool SetTexture(const string& Name, class CTexture* Texture);

    //텍스쳐 이름으로 찾아와서 넣어주기
    bool SetTexture(const string& Name, const string& TextureName);

    //이름이랑 파일경로 넣어주기
    bool SetTexture(const string& Name,
        const string& TextureName, const TCHAR* FileName);

    //이름, 배열
    bool SetTexture(const string& Name,
        const string& TextureName,
        vector<const TCHAR*> FileName);

    //이름 파일경로 확장자 갯수
    bool SetTexture(const string& Name,
        const string& TextureName, 
        const TCHAR* FileName, const TCHAR* Ext, int Count);

    // 풀경로
    bool SetTextureFullPath(const string& Name,
        const string& TextureName, const TCHAR* FullPath);
    // 풀경로 배열 
    bool SetTextureFullPath(const string& Name,
        const string& TextureName, const vector<const TCHAR*>& FullPath);

    ////
    //FAnimationFrame 를 만들어주는 함수를 만들어주자!
    bool AddFrame(const string& Name, const FVector2D& Start, const FVector2D& Size);
    bool AddFrame(const string& Name, float StartX, float StartY, float SizeX, float SizeY);
    bool AddFrameCount(const string& Name, int Count, const FVector2D& Start, const FVector2D& Size);
    bool AddFrameCount(const string& Name, int Count, float StartX, float StartY, float SizeX, float SizeY);

};

