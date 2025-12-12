#include "FontManager.h"

#include "../../Device.h"

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	mFontMap.clear();
	mFontCollectionMap.clear();

	auto iter = mFontColorMap.begin();
	auto iterEnd = mFontColorMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	SAFE_RELEASE(mWirteFactory);

}


bool CFontManager::Init()
{
	//Factory aksemfrl
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mWirteFactory), (IUnknown**)&mWirteFactory)))
	{
		return false;
	}

	//폰트 불러오기
	//LoadFontCollection("Default", TEXT("Font\\WantedSans-Regular.otf"));
	LoadFontCollection("Default", TEXT("Font\\Griu.ttf"));

	//폰터 정보를 넣어준다.

	// 한국어 ko  영어 미국꺼 en-us
	LoadFont("Default", GetFontFaceName("Default"), 600, 20.f, TEXT("ko-kr"));

	CreateFontColor(0, 0, 0, 255);				// 검정
	CreateFontColor(255, 255, 255, 255);		// 흰색
	CreateFontColor(255, 0, 0, 255);			// 빨
	CreateFontColor(0, 255, 0, 255);			// 초
	CreateFontColor(0, 0, 255, 255);			// 파
	CreateFontColor(255, 0, 255, 255);			// 마젠타
	CreateFontColor(255, 255, 0, 255);			// 노랑
	CreateFontColor(0, 255, 255, 255);			// 시안

	return true;
}


bool CFontManager::LoadFont(const string& Name, const TCHAR* FontName, int Weight, float FontSize,
	const TCHAR* LocalName, int Stretch)
{
	CFont* Font = FindFont(Name);
	if (Font)
	{
		return true;
	}

	Font = new CFont;

	Font->mName = Name;

	auto iter =  mFontCollectionMap.find("Default");
	if (!Font->LoadFont(mWirteFactory, iter->second.Get()->GetCollection(), FontName, Weight, FontSize, LocalName, Stretch))
	{
		SAFE_DELETE(Font);
		return false;
	}
	// Name을 키값으로 Font를 Value
	mFontMap.insert(make_pair(Name, Font));

	return true;
}

CFont* CFontManager::FindFont(const string& Name)
{
	auto iter = mFontMap.find(Name);

	if (iter == mFontMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CFontManager::LoadFontCollection(const string& Name, const TCHAR* FileName)
{
	CFontCollection* FontCollection = FindFontCollection(Name);

	if (FontCollection)
	{
		return true;
	}

	FontCollection = new CFontCollection;

	FontCollection->mName = Name;

	if (!FontCollection->LoadCollection(mWirteFactory, FileName))
	{
		SAFE_DELETE(FontCollection);
		return false;
	}

	mFontCollectionMap.insert(make_pair(Name, FontCollection));

	return true;
}

CFontCollection* CFontManager::FindFontCollection(const string& Name)
{
	auto iter = mFontCollectionMap.find(Name);

	if (iter == mFontCollectionMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

const TCHAR* CFontManager::GetFontFaceName(const string& CollectionName)
{
	CFontCollection* FontCollection = FindFontCollection(CollectionName);

	if (!FontCollection)
	{
		return nullptr;
	}

	return FontCollection->GetFontFaceName();
}

bool CFontManager::CreateFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(r, g, b, a);

	if (Brush)
	{
		return true;
	}

	//넣어줄때는 A -> R -> G ->B 순서로 넣어줘야 한다. 
	unsigned int Color = a;
	// 2진수 0010  << 2;
	Color = Color << 8;

	Color = Color | r;
	// Color		1	1	0	0
	// R			0	0	0	1
	// |			1	1	0	1
	// &			0	0	0	0
	Color = Color << 8;

	Color = Color | g;
	Color = Color << 8;

	Color = Color | b;
	// Color 값을 키값으로 사용할 것이다. 

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &Brush)))
	{
		return false;
	}

	mFontColorMap.insert(make_pair(Color, Brush));
	return true;
}

bool CFontManager::CreateFontColor(const FVector4D& Color)
{
	ID2D1SolidColorBrush* Brush = FindFontColor(Color);

	if (Brush)
	{
		return true;
	}

	unsigned char r, g, b, a;
	r = (unsigned char)(Color.x * 255);
	g = (unsigned char)(Color.y * 255);
	b = (unsigned char)(Color.z * 255);
	a = (unsigned char)(Color.w * 255);

	unsigned int Key = a;
	// 2진수 0010  << 2;
	Key = Key << 8;

	Key = Key | r;
	Key = Key << 8;

	Key = Key | g;
	Key = Key << 8;

	Key = Key | b;

	if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(D2D1::ColorF(Color.x, Color.y, Color.z, Color.w), &Brush)))
	{
		return false;
	}

	mFontColorMap.insert(make_pair(Key, Brush));
	return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	unsigned int Key = a;
	Key = Key << 8;

	Key = Key | r;
	Key = Key << 8;

	Key = Key | g;
	Key = Key << 8;

	Key = Key | b;


	auto iter = mFontColorMap.find(Key);

	if (iter == mFontColorMap.end())
	{
		return nullptr;
	}

	return iter->second;

}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const FVector4D& Color)
{
	unsigned char r, g, b, a;
	r = (unsigned char)(Color.x * 255);
	g = (unsigned char)(Color.y * 255);
	b = (unsigned char)(Color.z * 255);
	a = (unsigned char)(Color.w * 255);

	unsigned int Key = a;
	// 2진수 0010  << 2;
	Key = Key << 8;

	Key = Key | r;
	Key = Key << 8;

	Key = Key | g;
	Key = Key << 8;

	Key = Key | b;

	auto iter = mFontColorMap.find(Key);

	if (iter == mFontColorMap.end())
	{
		return nullptr;
	}

	return iter->second;
}


