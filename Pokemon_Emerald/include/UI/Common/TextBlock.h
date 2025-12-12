#pragma once
#include "../Widget.h"
#include "../../Asset/Font/Font.h"

//가로정렬
enum class ETextAlignH
{
	Left,
	Center,
	Right
};

//세로정렬
enum class ETextAlignV
{
	Top,
	Middle,
	Bottom
};


class CTextBlock :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CTextBlock();
	virtual ~CTextBlock();

protected:
	//출력할 문자열
	wstring mText;
	//사용할 폰트
	CSharedPtr<CFont> mFont;

	//레이아웃
	IDWriteTextLayout* mLayout = nullptr;
	//2D용 렌더타겟
	ID2D1RenderTarget* mTarget = nullptr;

	//레이아웃 정보
	float mFontSize = 20.f;
	ETextAlignH mAlignH = ETextAlignH::Left;
	ETextAlignV mAlignV = ETextAlignV::Middle;

	//그리기용도
	ID2D1SolidColorBrush* mTextColor = nullptr;
	//투명도 사용 여부
	bool mTransparency = false;
	// 투명도
	float mOpacity = 1.f;

public:
	const TCHAR* GetText() const
	{
		return mText.c_str();
	}

	int GetTextCount() const
	{
		return static_cast<int>(mText.length());
	}

public:
	virtual void SetSize(const FVector2D& Size)
	{
		mSize = Size;

		CreateTextLayout();
	}

	virtual void SetSize(float X, float Y)
	{
		mSize.x = X;
		mSize.y = Y;

		CreateTextLayout();
	}

	//레이아웃 
	void SetFontSize(float FontSize)
	{
		mFontSize = FontSize;
		CreateTextLayout();
	}

	void SetAlignH(ETextAlignH Align);
	void SetAlignV(ETextAlignV Align);



public:
	//Font
	void SetFont(const string& FontName);

	//Text 관련 함수들 
	void SetText(const TCHAR* Text);
	void SetTextFromInt(int Number);
	void SetTextFromFloat(float Number);
	void AddText(const TCHAR* Text);
	void AddTextFromInt(int Number);
	void AddTextFromFloat(float Number);
	void PopBack();
	void ClearText();

	//그리기용도
	void EnableTransparency(bool Transparency);
	void SetOpacity(float Opacity);
	void SetTextColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	void SetTextColor(const FVector4D& Color);


private:
	//텍스트에 변화가 있을때마다 호출해준다.
	// 텍스트 정보를 갱신
	void CreateTextLayout();

	//시점
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	// 텍스트블록도 충돌 할거면 만들수도 있다. 
	//virtual bool CollisionMouse(float DeltaTime, const FVector2D& MousePos);
};

