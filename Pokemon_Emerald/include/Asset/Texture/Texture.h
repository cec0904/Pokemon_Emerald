#pragma once
#include "../Asset.h"

//외부 라이브러리 헤더 
#include "DirectXTex.h"

//텍스쳐를 포현하기 위한 구조체
struct FTextureInfo
{
    // 픽셀 정보를 담기 위한 이미지 정보
    DirectX::ScratchImage* Image = nullptr;
    // 쉐이더 리소스 뷰
    // 쉐이더에 들어갈 데이터의 포맷을 만들어준다.
    ID3D11ShaderResourceView* SRV = nullptr;
    //이미지 크기
    unsigned int Width = 0;
    unsigned int Height = 0;
    //파일 경로 이름
    TCHAR FileName[MAX_PATH] = {};

    ~FTextureInfo()
    {
        SAFE_DELETE(Image);
        SAFE_RELEASE(SRV);
    }
};

class CTexture :
    public CAsset
{
    friend class CTextureManager;

private:
    CTexture();
    ~CTexture();

private:
    vector<FTextureInfo*> mTextureList;

public:
    const FTextureInfo* GetTexture(int Index = 0)
    {
        return mTextureList[Index];
    }

    int GetTextureCount() const
    {
        return (int)mTextureList.size();
    }

public:
    //파일명 
    bool LoadTexture(const TCHAR* FileName);
    bool LoadTextureFullPath(const TCHAR* FullPath);

    //텍스쳐 여러장 불러오기
    bool LoadTexture(const vector<const TCHAR*>& FileName);
    bool LoadTextureFullPath(const vector<const TCHAR*>& FullPath);
    //규칙적인 파일이름으로 여러장 불러온다.
    bool LoadTexture(const TCHAR* FileName, const TCHAR* Ext, int Count);



public:
    void SetShader(int Register, int ShaderBufferType, int TextureIndex);
    void ResetShader(int Register, int ShaderBufferType);


protected:
    bool CreateResourceView(int Index = 0);


};

