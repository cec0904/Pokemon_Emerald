#include "Image.h"

#include "../../Asset/AssetManager.h"
#include "../../Asset/Mesh/Mesh.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Asset/Texture/TextureManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Scene/Input.h"
#include "../../Shader/Shader.h"
#include "../../Shader/TransformCBuffer.h"
#include "../../Shader/UICBuffer.h"

#include "../../Share/Log.h"

CImage::CImage()
{
}

CImage::~CImage()
{
}

void CImage::SetTexture(const string& Name)
{
	if (mScene)
	{
		mBrush.Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		mBrush.Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CImage::SetTexture(const string& Name, const TCHAR* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;

		}

		mBrush.Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mBrush.Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CImage::SetTexture(class CTexture* Texture)
{
	mBrush.Texture = Texture;
}

void CImage::SetTint(float r, float g, float b, float a)
{
	mBrush.Tint.x = r;
	mBrush.Tint.y = g;
	mBrush.Tint.z = b;
	mBrush.Tint.w = a;
}

void CImage::SetBrushAnimation(bool Animation)
{
	mBrush.AnimationEnable = Animation;
}

void CImage::AddBurshFrame(const FVector2D& Start, const FVector2D& Size)
{
	FAnimationFrame Frame;
	Frame.Start = Start;
	Frame.Size = Size;

	mBrush.Frames.emplace_back(Frame);

	mBrush.FrameTime = mBrush.PlayTime / mBrush.Frames.size();
}

void CImage::AddBurshFrame(float StartX, float StartY, float SizeX, float SizeY)
{
	FAnimationFrame Frame;
	Frame.Start.x = StartX;
	Frame.Start.y = StartY;
	Frame.Size.x = SizeX;
	Frame.Size.y = SizeY;

	mBrush.Frames.emplace_back(Frame);

	mBrush.FrameTime = mBrush.PlayTime / mBrush.Frames.size();
}

void CImage::SetCurrentFrame(int Frame)
{
	mBrush.Frame = Frame;
}

void CImage::SetAnimationPlayTime(float PlayTime)
{
	mBrush.PlayTime = PlayTime;
}

void CImage::SetAnimationPlayRate(float PlayRate)
{
	mBrush.PlayRate = PlayRate;
}

bool CImage::Init()
{
	if (!CWidget::Init())
	{
		return false;
	}

	return true;
}

void CImage::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (mBrush.AnimationEnable)
	{
		mBrush.Time += DeltaTime * mBrush.PlayRate;

		if (mBrush.Time >= mBrush.FrameTime)
		{
			mBrush.Time -= mBrush.FrameTime;

			++mBrush.Frame;

			if (mBrush.Frame == mBrush.Frames.size())
			{
				mBrush.Frame = 0;
			}
		}
	}

	//if (mBrush.AnimationEnable)
		//CLog::PrintLog(to_string(mBrush.Frame));
}

void CImage::Render()
{
	CWidget::Render();

	// 크기 자전 이동 공전 부모 
	FMatrix matScale, matRot, matTranslate, matWorld;

	matScale.Scaling(mSize);
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
	mUICBuffer->SetTint(mBrush.Tint);

	//만약 텍스쳐가 있다면 
	if (mBrush.Texture)
	{
		mUICBuffer->SetTextureEnable(true);
		mBrush.Texture->SetShader(0, EShaderBufferType::Pixel, 0);
	}
	else
	{
		mUICBuffer->SetTextureEnable(false);
	}

	//만약 애니메이션이 있다면
	if (mBrush.AnimationEnable)
	{
		mUICBuffer->SetAnimationEnable(true);
		int Frame = mBrush.Frame;

		FAnimationFrame FrameInfo = mBrush.Frames[Frame];

		mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
			FrameInfo.Start.x + FrameInfo.Size.x,
			FrameInfo.Start.y + FrameInfo.Size.y);


		float LTX = 0.f, LTY = 0.f, RBX = 1.f, RBY = 1.f;
		FVector2D FrameStart = mBrush.Frames[mBrush.Frame].Start;
		FVector2D FrameSize = mBrush.Frames[mBrush.Frame].Size;


		LTX = FrameStart.x / mBrush.Texture->GetTexture()->Width;
		LTY = FrameStart.y / mBrush.Texture->GetTexture()->Height;
		RBX = LTX + FrameSize.x / mBrush.Texture->GetTexture()->Width;
		RBY = LTY + FrameSize.y / mBrush.Texture->GetTexture()->Height;
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

void CImage::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);
}
