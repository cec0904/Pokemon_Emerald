#pragma once
#include "../Asset.h"

//실제 폰트 데이터를 불러오는 클래스

class CFontCollection :
    public CAsset
{
    friend class CFontManager;

private:
    CFontCollection();
    ~CFontCollection();

private:
    //폰트 컬랙션 객체 
    IDWriteFontCollection1* mCollection = nullptr;
    //Fonst Face 이름
    TCHAR mFontFaceName[128] = {};

public:
    IDWriteFontCollection1* GetCollection() const
    {
        return mCollection;
    }

    const TCHAR* GetFontFaceName() const
    {
        return mFontFaceName;
    }

public:
    bool LoadCollection(IDWriteFactory5* Factory, const TCHAR* FileName);
};

