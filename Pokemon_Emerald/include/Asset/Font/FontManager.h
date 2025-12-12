#pragma once

#include "Font.h"
#include "FontCollection.h"


class CFontManager
{
	friend class CAssetManager;

private:
	CFontManager();
	~CFontManager();

public:
	bool Init();

private:
	// 디바이스 같은 친구다. 
	IDWriteFactory5* mWirteFactory = nullptr;

	//폰트 목록 
	unordered_map<string, CSharedPtr<CFont>> mFontMap;
	//폰트 콜렉션
	unordered_map<string, CSharedPtr<CFontCollection>> mFontCollectionMap;
	// 폰트 브러쉬
	//ID2D1SolidColorBrush;	// ARGB 형식으로 폰트 색상을 들고있는다.
	unordered_map<unsigned int, ID2D1SolidColorBrush*> mFontColorMap;

public:
	// Font
	bool LoadFont(const string& Name,
		const TCHAR* FontName,
		int Weight, float FontSize,
		const TCHAR* LocalName,
		int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	CFont* FindFont(const string& Name);

	// Font Collection
	bool LoadFontCollection(const string& Name, const TCHAR* FileName);
	CFontCollection* FindFontCollection(const string& Name);

	const TCHAR* GetFontFaceName(const string& CollectionName);

	// Font Color
	bool CreateFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	bool CreateFontColor(const FVector4D& Color);

	ID2D1SolidColorBrush* FindFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	ID2D1SolidColorBrush* FindFontColor(const FVector4D& Color);





};

