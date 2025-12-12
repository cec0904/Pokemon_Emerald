#include "ProgressBar.h"
#include"../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/AssetManager.h"
#include "../../Asset/Texture/TextureManager.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Shader/TransformCBuffer.h"
#include "../../Shader/UICBuffer.h"
#include "../../Shader/Shader.h"
#include "../../Asset/Mesh/Mesh.h"
#include "../../Scene/CameraManager.h"
#include "../../Device.h"

CProgressBar::CProgressBar()
{
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetTexture(EProgressBarImageType Type, const string& Name)
{
	CTexture* Texture = nullptr;

	if (mScene)
	{
		Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Texture = Texture;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Texture = Texture;;
		break;
	}
}

void CProgressBar::SetTexture(EProgressBarImageType Type, const string& Name, const TCHAR* FileName)
{
	CTexture* Texture = nullptr;

	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;
		}
		Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}
		Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Texture = Texture;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Texture = Texture;;
		break;
	}
}

void CProgressBar::SetTexture(EProgressBarImageType Type, class CTexture* Texture)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Texture = Texture;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Texture = Texture;;
		break;
	}
}

void CProgressBar::SetTint(EProgressBarImageType Type, float r, float g, float b)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Tint.x = r;
		mBackBrush.Tint.y = g;
		mBackBrush.Tint.z = b;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Tint.x = r;
		mFillBrush.Tint.y = g;
		mFillBrush.Tint.z = b;
		break;
	}
}

void CProgressBar::SetOpacity(EProgressBarImageType Type, float Opacity)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Tint.w = Opacity;
		break;
	case EProgressBarImageType::Fill:
		mBackBrush.Tint.w = Opacity;
		break;
	}
}

void CProgressBar::SetBrushAnimation(EProgressBarImageType Type, bool Animation)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.AnimationEnable = Animation;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.AnimationEnable = Animation;
		break;
	}
}

void CProgressBar::AddBurshFrame(EProgressBarImageType Type, const FVector2D& Start, const FVector2D& Size)
{
	FAnimationFrame Frame;
	Frame.Start = Start;
	Frame.Size = Size;

	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Frames.emplace_back(Frame);
		//프레임당 재생시간 
		mBackBrush.FrameTime = mBackBrush.PlayTime / mBackBrush.Frames.size();

		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Frames.emplace_back(Frame);
		//프레임당 재생시간 = 총 플레이 시간 / 프레임 갯수
		mFillBrush.FrameTime = mFillBrush.PlayTime / mFillBrush.Frames.size();
		break;
	}
}

void CProgressBar::AddBurshFrame(EProgressBarImageType Type, float StartX, float StartY, float SizeX, float SizeY)
{
	AddBurshFrame(Type, FVector2D(StartX, StartY), FVector2D(SizeX, SizeY));
}

void CProgressBar::SetCurrentFrame(EProgressBarImageType Type, int Frame)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.Frame = Frame;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.Frame = Frame;
		break;
	}
}

void CProgressBar::SetAnimationPlayTime(EProgressBarImageType Type, float PlayTime)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.PlayTime = PlayTime;
		//프레임당 재생시간 
		mBackBrush.FrameTime = mBackBrush.PlayTime / mBackBrush.Frames.size();

		break;
	case EProgressBarImageType::Fill:
		mFillBrush.PlayTime = PlayTime;
		//프레임당 재생시간 = 총 플레이 시간 / 프레임 갯수
		mFillBrush.FrameTime = mFillBrush.PlayTime / mFillBrush.Frames.size();
		break;
	}
}

void CProgressBar::SetAnimationPlayRate(EProgressBarImageType Type, float PlayRate)
{
	switch (Type)
	{
	case EProgressBarImageType::Back:
		mBackBrush.PlayRate = PlayRate;
		break;
	case EProgressBarImageType::Fill:
		mFillBrush.PlayRate = PlayRate;
		break;
	}
}

void CProgressBar::SetPercent(float Percent)
{
	mPercent = Percent;
}

void CProgressBar::SetBarDir(EProgressBarDir Dir)
{
	mBarDir = Dir;
}

bool CProgressBar::Init()
{
	return CWidget::Init();
}

void CProgressBar::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	UpdateBrush(mBackBrush, DeltaTime);
	UpdateBrush(mFillBrush, DeltaTime);

}

void CProgressBar::Render()
{
	CWidget::Render();

	RenderBrush(mBackBrush, mSize);

	FVector2D Size = mSize;

	switch (mBarDir)
	{
	case EProgressBarDir::RightToLeft:
		Size.x *= mPercent;
		break;
	case EProgressBarDir::TopToBottom:
		Size.y *= mPercent;
		break;
	}

	RenderBrush(mFillBrush, Size);
}

void CProgressBar::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);

	FVector2D RenderPos = mRenderPos;

	RenderPos.x += Pos.x;
	RenderPos.y += Pos.y;

	FResolution RS = CDevice::GetInst()->GetResolution();

	RenderPos.x -= mPivot.x * mSize.x;
	RenderPos.y -= mPivot.y * mSize.y;

	RenderBrush(mBackBrush, RenderPos, mSize);

	FVector2D Size = mSize;

	switch (mBarDir)
	{
	case EProgressBarDir::RightToLeft:
		Size.x *= mPercent;
		break;
	case EProgressBarDir::TopToBottom:
		Size.y *= mPercent;
		break;
	}

	RenderBrush(mFillBrush, RenderPos, Size);
}

void CProgressBar::UpdateBrush(FUIBrush& Brush, float DeltaTime)
{
	if (Brush.AnimationEnable)
	{
		Brush.Time += DeltaTime * Brush.PlayRate;

		if (Brush.Time >= Brush.FrameTime)
		{
			Brush.Time -= Brush.FrameTime;

			++Brush.Frame;

			if (Brush.Frame == Brush.Frames.size())
			{
				Brush.Frame = 0;
			}
		}
	}
}

void CProgressBar::RenderBrush(FUIBrush& Brush, const FVector2D& Size)
{
	// 크기 자전 이동 공전 부모 
	FMatrix matScale, matRot, matTranslate, matWorld;

	matScale.Scaling(Size);
	matRot.RotationZ(mRotation);
	matTranslate.Translation(mPos);

	//크자이공부 !!!!!
	matWorld = matScale * matRot * matTranslate;

	mTransformCBuffer->SetWorldMatrix(matWorld);
	mTransformCBuffer->SetProjMatrix(mUIProj);
	mTransformCBuffer->SetPivot(mPivot);

	mTransformCBuffer->UpdateBuffer();

	//여기 까지가 Transform 셋팅 
	///////////////////////////////////////////////////////////////////
	mUICBuffer->SetTint(Brush.Tint);

	//만약 텍스쳐가 있다면 
	if (Brush.Texture)
	{
		mUICBuffer->SetTextureEnable(true);
		Brush.Texture->SetShader(0, EShaderBufferType::Pixel, 0);
	}
	else
	{
		mUICBuffer->SetTextureEnable(false);
	}

	//만약 애니메이션이 있다면
	if (Brush.AnimationEnable)
	{
		mUICBuffer->SetAnimationEnable(true);
		int Frame = Brush.Frame;

		FAnimationFrame FrameInfo = Brush.Frames[Frame];

		mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
			FrameInfo.Start.x + FrameInfo.Size.x,
			FrameInfo.Start.y + FrameInfo.Size.y);


		float LTX = 0.f, LTY = 0.f, RBX = 1.f, RBY = 1.f;
		FVector2D FrameStart = Brush.Frames[Brush.Frame].Start;
		FVector2D FrameSize = Brush.Frames[Brush.Frame].Size;


		LTX = FrameStart.x / Brush.Texture->GetTexture()->Width;
		LTY = FrameStart.y / Brush.Texture->GetTexture()->Height;
		RBX = LTX + FrameSize.x / Brush.Texture->GetTexture()->Width;
		RBY = LTY + FrameSize.y / Brush.Texture->GetTexture()->Height;
		mUICBuffer->SetUV(LTX, LTY, RBX, RBY);

		mUICBuffer->UpdateBuffer();
	}
	else
	{
		mUICBuffer->SetAnimationEnable(false);
	}

	mUICBuffer->UpdateBuffer();

	//////////////////////////////////////////////////////////

	mShader->SetShader();

	mMesh->Render();
}

void CProgressBar::RenderBrush(FUIBrush& Brush, const FVector2D& Pos, const FVector2D& Size)
{
	// 크기 자전 이동 공전 부모 
	FMatrix matScale, matRot, matTranslate, matWorld;

	matScale.Scaling(Size);
	matRot.RotationZ(mRotation);
	matTranslate.Translation(Pos);

	//크자이공부 !!!!!
	matWorld = matScale * matRot * matTranslate;

	FMatrix matView= mScene->GetCameraManager()->GetViewMatrix();
	FMatrix matProj= mScene->GetCameraManager()->GetProjMatrix();


	mTransformCBuffer->SetWorldMatrix(matWorld);
	mTransformCBuffer->SetViewMatrix(matView);
	mTransformCBuffer->SetProjMatrix(matProj);
	mTransformCBuffer->SetPivot(mPivot);

	mTransformCBuffer->UpdateBuffer();

	//여기 까지가 Transform 셋팅 
	///////////////////////////////////////////////////////////////////
	mUICBuffer->SetTint(Brush.Tint);

	//만약 텍스쳐가 있다면 
	if (Brush.Texture)
	{
		mUICBuffer->SetTextureEnable(true);
		Brush.Texture->SetShader(0, EShaderBufferType::Pixel, 0);
	}
	else
	{
		mUICBuffer->SetTextureEnable(false);
	}

	//만약 애니메이션이 있다면
	if (Brush.AnimationEnable)
	{
		mUICBuffer->SetAnimationEnable(true);
		int Frame = Brush.Frame;

		FAnimationFrame FrameInfo = Brush.Frames[Frame];

		mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
			FrameInfo.Start.x + FrameInfo.Size.x,
			FrameInfo.Start.y + FrameInfo.Size.y);


		float LTX = 0.f, LTY = 0.f, RBX = 1.f, RBY = 1.f;
		FVector2D FrameStart = Brush.Frames[Brush.Frame].Start;
		FVector2D FrameSize = Brush.Frames[Brush.Frame].Size;


		LTX = FrameStart.x / Brush.Texture->GetTexture()->Width;
		LTY = FrameStart.y / Brush.Texture->GetTexture()->Height;
		RBX = LTX + FrameSize.x / Brush.Texture->GetTexture()->Width;
		RBY = LTY + FrameSize.y / Brush.Texture->GetTexture()->Height;
		mUICBuffer->SetUV(LTX, LTY, RBX, RBY);

		mUICBuffer->UpdateBuffer();
	}
	else
	{
		mUICBuffer->SetAnimationEnable(false);
	}

	mUICBuffer->UpdateBuffer();

	//////////////////////////////////////////////////////////

	mShader->SetShader();

	mMesh->Render();

}
