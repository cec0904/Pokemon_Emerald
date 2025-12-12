#pragma once
#include "../Asset.h"

class CFont :
    public CAsset
{
    friend class CFontManager;

private:
    CFont();
    ~CFont();

private:
    // 폰트 관련 객체를 생성할때 진입점
    // 디바이스 같은 친구다. 
    IDWriteFactory5* mFactory = nullptr;
    // 폰트 속서을 지정한다. 
    IDWriteTextFormat* mFormat = nullptr;

public:
    // 폰트 불러오기
    // Factory , 폰트 이름, 폰트 굵기, 폰트 크기, 폰트 지역(KO)
    bool LoadFont(
        IDWriteFactory5* Factory,
        IDWriteFontCollection* Collection,
        const TCHAR* FontName,
        int  Weight, float FontSize,
        const TCHAR* LocalName,
        int Stretch = DWRITE_FONT_STRETCH_NORMAL);

    //폰트 레이아웃
    IDWriteTextLayout* CreateLayout(const TCHAR* Text, int Length, float Width, float Height);

};

