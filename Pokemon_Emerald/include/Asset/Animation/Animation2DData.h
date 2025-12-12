#pragma once
#include "../Asset.h"

class CAnimation2DData :
    public CAsset
{
public:
    CAnimation2DData();
    virtual ~CAnimation2DData();

private:
    //텍스쳐 
    CSharedPtr<class CTexture> mTexture;
    //텍스쳐 정보를 담고있는 프레임 정보들 
    vector<FAnimationFrame> mFrameList;
    //이미지 형식 
    EAnimationTextureType mType = EAnimationTextureType::SpriteSheet;

public:
    EAnimationTextureType GetAnimationTextureType() const
    {
        return mType;
    }

    class CTexture* GetTexture()
    {
        return mTexture;
    }

    const FAnimationFrame& GetFrame(int Index) const
    {
        return mFrameList[Index];
    }

    int GetFrameCount() const
    {
        return static_cast<int>(mFrameList.size());
    }

    ////
public:
    void SetAnimationTextureType(EAnimationTextureType Type)
    {
        mType = Type;
    }

    //텍스쳐 불러오기 
    //텍스쳐로 바로 넣어주기
    void SetTexture(class CTexture* Texture);
    //텍스쳐 이름으로 찾아와서 넣어주기
    void SetTexture(const string& TextureName);
    //이름이랑 파일경로 넣어주기
    void SetTexture(const string& Name, const TCHAR* FileName);
    //이름, 배열
    void SetTexture(const string& Name, vector<const TCHAR*> FileName);
    //이름 파일경로 확장자 갯수
    void SetTexture(const string& Name, const TCHAR* FileName, const TCHAR* Ext, int Count);

    // 풀경로
    void SetTextureFullPath(const string& Name, const TCHAR* FullPath);
    // 풀경로 배열 
    void SetTextureFullPath(const string& Name, const vector<const TCHAR*>& FullPath);

    ////
    //FAnimationFrame 를 만들어주는 함수를 만들어주자!
    void AddFrame(const FVector2D& Start, const FVector2D& Size);
    void AddFrame(float StartX, float StartY, float SizeX, float SizeY);
    void AddFrameCount(int Count, const FVector2D& Start, const FVector2D& Size);
    void AddFrameCount(int Count, float StartX, float StartY, float SizeX, float SizeY);
};

