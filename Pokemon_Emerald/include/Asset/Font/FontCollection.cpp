#include "FontCollection.h"

CFontCollection::CFontCollection()
{
}

CFontCollection::~CFontCollection()
{
	SAFE_RELEASE(mCollection);
}

bool CFontCollection::LoadCollection(IDWriteFactory5* Factory, const TCHAR* FileName)
{
	TCHAR FullPath[MAX_PATH] = {};
	
	lstrcpy(FullPath, gRootPath);
	lstrcat(FullPath, TEXT("Asset\\"));
	lstrcat(FullPath, FileName);

	//폰트 파일
	IDWriteFontFile* FontFile = nullptr;

	if (FAILED(Factory->CreateFontFileReference(FullPath, nullptr, &FontFile)))
	{
		return false;
	}

	IDWriteFontSetBuilder1* FontBuilder = nullptr;

	if (FAILED(Factory->CreateFontSetBuilder(&FontBuilder)))
	{
		return false;
	}

	FontBuilder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;

	if (FAILED(FontBuilder->CreateFontSet(&FontSet)))
	{
		return false;
	}

	if (FAILED(Factory->CreateFontCollectionFromFontSet(FontSet, &mCollection)))
	{
		return false;
	}

	SAFE_RELEASE(FontSet);
	SAFE_RELEASE(FontBuilder);
	SAFE_RELEASE(FontFile);

	//Font FaceName 가져오기
	UINT32 Count = mCollection->GetFontFamilyCount();

	IDWriteFontFamily* Family = nullptr;

	if (FAILED(mCollection->GetFontFamily(0, &Family)))
	{
		return false;
	}

	IDWriteLocalizedStrings* LocalizeName = nullptr;

	if (FAILED(Family->GetFamilyNames(&LocalizeName)))
	{
		return false;
	}

	if (FAILED(LocalizeName->GetString(0, mFontFaceName, 128)))
	{
		return false;
	}

	SAFE_RELEASE(LocalizeName);
	SAFE_RELEASE(Family);

	return true;
}
