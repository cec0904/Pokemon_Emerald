#include "Button.h"

#include "../../Asset/AssetManager.h"
#include "../../Asset/Mesh/Mesh.h"
#include "../../Asset/Texture/Texture.h"
#include "../../Asset/Texture/TextureManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "../../Asset/Sound/Sound.h"
#include "../../Asset/Sound/SoundManager.h"
#include "../../Scene/Input.h"
#include "../../Shader/Shader.h"
#include "../../Shader/TransformCBuffer.h"
#include "../../Shader/UICBuffer.h"

#include "../../Share/Log.h"


CButton::CButton()
{
}

CButton::~CButton()
{
}

// 해당 스테이트에 Texture 찾아와서 넣어준다. 
void CButton::SetTexture(EButtonState::Type State, const string& Name)
{
	if (mScene)
	{
		mBrush[State].Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		mBrush[State].Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CButton::SetTexture(EButtonState::Type State, const string& Name, const TCHAR* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			return;

		}

		mBrush[State].Texture = mScene->GetAssetManager()->FindTexture(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			return;
		}

		mBrush[State].Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}
}

void CButton::SetTexture(EButtonState::Type State, class CTexture* Texture)
{
	mBrush[State].Texture = Texture;
}

void CButton::SetTint(EButtonState::Type State, float r, float g, float b, float a)
{
	mBrush[State].Tint.x = r;
	mBrush[State].Tint.y = g;
	mBrush[State].Tint.z = b;
	mBrush[State].Tint.w = a;
}

void CButton::AddBurshFrame(EButtonState::Type State, const FVector2D& Start, const FVector2D& Size)
{
	FAnimationFrame Frame;
	Frame.Start = Start;
	Frame.Size = Size;

	mBrush[State].Frames.emplace_back(Frame);

	mBrush[State].FrameTime = mBrush[State].PlayTime / mBrush[State].Frames.size();
}

void CButton::AddBurshFrame(EButtonState::Type State, float StartX, float StartY, float SizeX, float SizeY)
{
	FAnimationFrame Frame;
	Frame.Start.x = StartX;
	Frame.Start.y = StartY;
	Frame.Size.x = SizeX;
	Frame.Size.y = SizeY;

	mBrush[State].Frames.emplace_back(Frame);

	mBrush[State].FrameTime = mBrush[State].PlayTime / mBrush[State].Frames.size();
}

void CButton::SetBrushAnimation(EButtonState::Type State, bool Animation)
{
	mBrush[State].AnimationEnable = Animation;
}

void CButton::SetCurrentFrame(EButtonState::Type State, int Frame)
{
	mBrush[State].Frame = Frame;
}

void CButton::SetAnimationPlayTime(EButtonState::Type State, float PlayTime)
{
	mBrush[State].PlayTime = PlayTime;
}

void CButton::SetAnimationPlayRate(EButtonState::Type State, float PlayRate)
{
	mBrush[State].PlayRate = PlayRate;
}

void CButton::SetSound(EButtonState::Type State, const string& Name)
{
	if (mScene)
	{
		mSound[State] = mScene->GetAssetManager()->FindSound(Name);
	}
	else
	{
		mSound[State] = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
	}
}

void CButton::SetSound(EButtonState::Type State, const string& Name, const char* FileName)
{
	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadSound(Name, "UI", false, FileName))
		{
			return;

		}

		mSound[State] = mScene->GetAssetManager()->FindSound(Name);
	}
	else
	{
		if (!CAssetManager::GetInst()->GetSoundManager()->LoadSound(Name, "UI", false, FileName))
		{
			return;
		}

		mSound[State] = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
	}
}

bool CButton::Init()
{
	CWidget::Init();

	SetEventCallBack(EButtonEventState::Click, this, &CButton::ButtonClick);

	return true;
}

void CButton::Update(float DeltaTime)
{
	CWidget::Update(DeltaTime);

	if (mState != EButtonState::Disable)
	{
		if (mMouseOn)
		{
			//왼쪽 마우스 클릭 
			if (mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton))
			{
				mState = EButtonState::Click;
				//클릭
			}
			else if (mState == EButtonState::Click &&
				mScene->GetInput()->GetMouseUp(EMouseButtonType::LButton))
			{
				// 호버
				if (mSound[EButtonEventState::Click])
				{
					mSound[EButtonEventState::Click]->Play();
				}

				if (mEventCallBack[EButtonEventState::Click])
				{
					mEventCallBack[EButtonEventState::Click]();
				}

				mState = EButtonState::Hovered;
			}
			else if (mScene->GetInput()->GetMouseHold(EMouseButtonType::LButton))
			{
				mState = EButtonState::Click;
			}
		}
	}
}

void CButton::Render()
{
	CWidget::Render();

	// 크기 자전 이동 공전 부모 
	FMatrix matScale, matRot, matTranslate, matWorld;

	matScale.Scaling(mSize);
	matRot.RotationZ(mRotation);
	matTranslate.Translation(mRenderPos);

	//크자이공부 !!!!!
	matWorld = matScale * matRot * matTranslate;
	
	mTransformCBuffer->SetWorldMatrix(matWorld);
	mTransformCBuffer->SetProjMatrix(mUIProj);
	mTransformCBuffer->SetPivot(mPivot);

	mTransformCBuffer->UpdateBuffer();	

	//여기 까지가 Transform 셋팅 
	///////////////////////////////////////////////////////////////////
	mUICBuffer->SetTint(mBrush[mState].Tint);
	mUICBuffer->SetWidgetColor(mColor);

	//만약 텍스쳐가 있다면 
	if (mBrush[mState].Texture)
	{
		mUICBuffer->SetTextureEnable(true);
		mBrush[mState].Texture->SetShader(0, EShaderBufferType::Pixel, 0);
	}
	else
	{
		mUICBuffer->SetTextureEnable(false);
	}

	//만약 애니메이션이 있다면
	if (mBrush[mState].AnimationEnable)
	{
		mUICBuffer->SetAnimationEnable(true);
		int Frame = mBrush[mState].Frame;

		FAnimationFrame FrameInfo = mBrush[mState].Frames[Frame];

		mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
			FrameInfo.Start.x + FrameInfo.Size.x,
			FrameInfo.Start.y + FrameInfo.Size.y);
	}
	else
	{
		mUICBuffer->SetAnimationEnable(false);
	}

	mUICBuffer->UpdateBuffer();

	//////////////////////////////////////////////////////////

	mShader->SetShader();

	mMesh->Render();

	//
	if (mChild)
	{
		mChild->Render();
	}
	


}

void CButton::Render(const FVector3D& Pos)
{
	CWidget::Render(Pos);





}

void CButton::MouseHovered()
{
	if (mState == EButtonState::Normal)
	{

		if(mSound[EButtonEventState::Hovered])
		{
			mSound[EButtonEventState::Hovered]->Play();
		}

		if (mEventCallBack[EButtonEventState::Hovered])
		{
			mEventCallBack[EButtonEventState::Hovered]();
		}

		mState = EButtonState::Hovered;
		CLog::PrintLog("Button Hovered");
	}
}

void CButton::MouseUnHovered()
{
	if (mState !=EButtonState::Disable )
	{
		CLog::PrintLog("Button UnHovered");
		mState = EButtonState::Normal;
	}
}

void CButton::ButtonClick()
{
	CLog::PrintLog("Button Click");
}
