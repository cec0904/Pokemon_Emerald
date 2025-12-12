#include "Font.h"

CFont::CFont()
{
}

CFont::~CFont()
{
}

bool CFont::LoadFont(IDWriteFactory5* Factory, IDWriteFontCollection* Collection, const TCHAR* FontName, int Weight, float FontSize, const TCHAR* LocalName,
	int Stretch)
{
	mFactory = Factory;

	if (FAILED(mFactory->CreateTextFormat(FontName, Collection, (DWRITE_FONT_WEIGHT)Weight, DWRITE_FONT_STYLE_NORMAL,
		(DWRITE_FONT_STRETCH)Stretch, FontSize, LocalName, &mFormat)))
	{
		return false;
	}

	return true;
}

IDWriteTextLayout* CFont::CreateLayout(const TCHAR* Text, int Length, float Width, float Height)
{
	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(mFactory->CreateTextLayout(Text, Length, mFormat, Width, Height, &Layout)))
	{
		return nullptr;
	}

	return Layout;
}
