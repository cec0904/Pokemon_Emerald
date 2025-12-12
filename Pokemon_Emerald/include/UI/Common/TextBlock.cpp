#include "TextBlock.h"

#include "../../Device.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Font/FontManager.h"
#include "../../Scene/CameraManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"

CTextBlock::CTextBlock()
{
}

CTextBlock::~CTextBlock()
{
	SAFE_RELEASE(mLayout);
}

void CTextBlock::SetAlignH(ETextAlignH Align)
{
	mAlignH = Align;

	if (!mLayout)
	{
		return;
	}

	switch (mAlignH)
	{
	case ETextAlignH::Left:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case ETextAlignH::Center:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case ETextAlignH::Right:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}
}

void CTextBlock::SetAlignV(ETextAlignV Align)
{
	mAlignV = Align;

	if (!mLayout)
	{
		return;
	}

	//세로정렬
	switch (mAlignV)
	{
	case ETextAlignV::Top:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case ETextAlignV::Middle:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case ETextAlignV::Bottom:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}

}

void CTextBlock::SetFont(const string& FontName)
{
	if (mScene)
	{
		mFont = mScene->GetAssetManager()->FindFont(FontName);
	}
	else
	{
		mFont = CAssetManager::GetInst()->GetFontManager()->FindFont(FontName);
	}

	CreateTextLayout();
}

void CTextBlock::SetText(const TCHAR* Text)
{
	mText = Text;
	CreateTextLayout();
}

void CTextBlock::SetTextFromInt(int Number)
{
	mText = to_wstring(Number);
	CreateTextLayout();
}

void CTextBlock::SetTextFromFloat(float Number)
{
	mText = to_wstring(Number);
	CreateTextLayout();
}

void CTextBlock::AddText(const TCHAR* Text)
{
	mText += Text;
	CreateTextLayout();
}

void CTextBlock::AddTextFromInt(int Number)
{
	mText += to_wstring(Number);
	CreateTextLayout();
}

void CTextBlock::AddTextFromFloat(float Number)
{
	mText += to_wstring(Number);
	CreateTextLayout();
}

void CTextBlock::PopBack()
{
	mText.pop_back();
	CreateTextLayout();
}

void CTextBlock::ClearText()
{
	mText.clear();
	CreateTextLayout();
}

void CTextBlock::EnableTransparency(bool Transparency)
{
	mTransparency = Transparency;
}

void CTextBlock::SetOpacity(float Opacity)
{
	mOpacity = Opacity;
}

void CTextBlock::SetTextColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	mTextColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(r, g, b, a);
}

void CTextBlock::SetTextColor(const FVector4D& Color)
{
	mTextColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(Color);
}


void CTextBlock::CreateTextLayout()
{
	SAFE_RELEASE(mLayout);

	mLayout = mFont->CreateLayout(mText.c_str(), (int)mText.length(), mSize.x, mSize.y);

	if (!mLayout)
	{
		return;
	}

	DWRITE_TEXT_RANGE Range = {};
	Range.startPosition = 0;
	Range.length = (UINT32)mText.length();

	mLayout->SetFontSize(mFontSize, Range);

	//정렬
	//가로 정렬
	switch (mAlignH)
	{
	case ETextAlignH::Left:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case ETextAlignH::Center:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case ETextAlignH::Right:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	//세로정렬
	switch (mAlignV)
	{
	case ETextAlignV::Top:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case ETextAlignV::Middle:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case ETextAlignV::Bottom:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}
}

bool CTextBlock::Init()
{
	CWidget::Init();

	mTarget = CDevice::GetInst()->Get2DTarget();

	SetFont("Default");
	SetTextColor(FVector4D::White);


	CreateTextLayout();

	return true;
}

void CTextBlock::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);
}

void CTextBlock::Render()
{
	CWidget::Render();

	mTarget->BeginDraw();

	FResolution RS = CDevice::GetInst()->GetResolution();

	D2D1_POINT_2F Point;
	Point.x = mRenderPos.x;
	Point.y = RS.Height - mPos.y - mSize.y;

	if (mTransparency)
	{
		mTextColor->SetOpacity(mOpacity);
	}
	else
	{
		mTextColor->SetOpacity(1.f);
	}

	mTarget->DrawTextLayout(Point, mLayout, mTextColor, D2D1_DRAW_TEXT_OPTIONS_NONE);
	
	mTarget->EndDraw();
}

void CTextBlock::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);

	mTarget->BeginDraw();

	FVector2D RenderPos = mRenderPos;

	RenderPos.x += Pos.x;
	RenderPos.y += Pos.y;

	FResolution RS = CDevice::GetInst()->GetResolution();

	const FVector3D& CameraPos = mScene->GetCameraManager()->GetCameraWorldPos();

	RenderPos.x -= CameraPos.x;
	RenderPos.y -= CameraPos.y;

	RenderPos.x -= mPivot.x * mSize.x;
	RenderPos.y -= mPivot.y * mSize.y;

	RenderPos.x += (RS.Width * 0.5f);
	RenderPos.y += (RS.Height * 0.5f);


	D2D1_POINT_2F Point;
	Point.x = RenderPos.x;
	Point.y = RS.Height - RenderPos.y - mSize.y;

	if (mTransparency)
	{
		mTextColor->SetOpacity(mOpacity);
	}
	else
	{
		mTextColor->SetOpacity(1.f);
	}

	mTarget->DrawTextLayout(Point, mLayout, mTextColor, D2D1_DRAW_TEXT_OPTIONS_NONE);

	mTarget->EndDraw();

}
