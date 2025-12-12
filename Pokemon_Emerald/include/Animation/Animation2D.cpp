#include "Animation2D.h"

#include "../Asset/AssetManager.h"
#include "../Asset/Animation/Animation2DData.h"
#include "../Asset/Animation/Animation2DManager.h"
#include "../Asset/Texture/Texture.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"

CAnimation2DCBuffer* CAnimation2D::mAnimCBuffer = nullptr;

CAnimation2D::CAnimation2D()
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& Anim)
{
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = mSequenceMap.begin();
	auto	iterEnd = mSequenceMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

}

void CAnimation2D::CreateCBuffer()
{
	mAnimCBuffer = new CAnimation2DCBuffer;

	mAnimCBuffer->Init();
}

void CAnimation2D::DestroyCBuffer()
{
	SAFE_DELETE(mAnimCBuffer);
}

void CAnimation2D::DisableAnimation()
{
	mAnimCBuffer->SetAnimation2DEnable(false);
	mAnimCBuffer->UpdateBuffer();
}

void CAnimation2D::SetAnimFlip(bool Flip)
{
	mAnimCBuffer->SetAnimation2DFlip(Flip);
	mAnimCBuffer->UpdateBuffer();
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Update(float DeltaTime)
{
	mCurrentSequence->Update(DeltaTime);
}

CAnimation2DSequence* CAnimation2D::FindSequence(const string& Name)
{
	auto iter = mSequenceMap.find(Name);

	if (iter == mSequenceMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CAnimation2D::AddSequence(const string& Name, float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	//이미 있다. 그래서 해줄 필요가 없다! 
	if (Sequence)
	{
		return false;
	}

	//먼저 Animation2DData 있는가?
	CAnimation2DData* Data = nullptr;

	if (mScene)
	{
		Data = mScene->GetAssetManager()->FindAnimation(Name);
	}
	else
	{
		Data = CAssetManager::GetInst()->GetAnimationManager()->FindAnimation(Name);
	}

	if (!Data)
	{
		return false;
	}

	//데이터를 토대로 애니메이션 시퀀스를 만들어준다.
	Sequence = new CAnimation2DSequence;

	Sequence->mOwner = this;

	Sequence->SetAsset(Data);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	if (!mCurrentSequence)
	{
		mCurrentSequence = Sequence;
		//기존 Spirte컴포넌트의 Texture를 애니메이션의 Texture로 변경해준다. 
		mOwner->SetTexture(mCurrentSequence->GetAnimationAsset()->GetTexture());
	}

	mSequenceMap.insert(make_pair(Name, Sequence));

	return true;
}

bool CAnimation2D::AddSequence(class CAnimation2DData* Asset, float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Asset->GetName());

	//이미 있다. 그래서 해줄 필요가 없다! 
	if (Sequence)
	{
		return false;
	}

	//데이터를 토대로 애니메이션 시퀀스를 만들어준다.
	Sequence = new CAnimation2DSequence;

	Sequence->mOwner = this;

	Sequence->SetAsset(Asset);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	if (!mCurrentSequence)
	{
		mCurrentSequence = Sequence;
		//기존 Spirte컴포넌트의 Texture를 애니메이션의 Texture로 변경해준다. 
		mOwner->SetTexture(mCurrentSequence->GetAnimationAsset()->GetTexture());
	}

	mSequenceMap.insert(make_pair(Asset->GetName(), Sequence));

	return true;
}

void CAnimation2D::ResetFrame()
{
	mCurrentSequence->mFrame = 0;
}


void CAnimation2D::SetPlayTime(const string& Name, float PlayTime)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetPlayTime(PlayTime);
}

void CAnimation2D::SetPlayRate(const string& Name, float PlayRate)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetPlayRate(PlayRate);
}

void CAnimation2D::SetLoop(const string& Name, bool Loop)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetLoop(Loop);
}

void CAnimation2D::SetReverse(const string& Name, bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetReverse(Reverse);
}

void CAnimation2D::ChangeAnimation(const string& Name)
{
	if (Name.empty())
	{
		return;
	}
	//else if (!mCurrentSequence)
	//{
	//	return;
	//}
	else if (mCurrentSequence->GetName() == Name)
	{
		return;
	}

	//기존 시퀀스 초기화
	mCurrentSequence->mFrame = 0;
	mCurrentSequence->mTime = 0.f;
	mCurrentSequence->mEndFunctionEnable = true;

	//새로운 시퀀스 
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	mCurrentSequence = Sequence;
	mCurrentSequence->mFrame = 0;
	mCurrentSequence->mTime = 0;

	mOwner->SetTexture(mCurrentSequence->GetAnimationAsset()->GetTexture());
}

CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	// UV값들 계산해서 상수버퍼로 넒겨줄것이다.
	//Frame 정보를 가지고 UV 좌표를 구할것이다!!
	float LTX = 0.f, LTY = 0.f, RBX = 1.f, RBY = 1.f;

	const FAnimationFrame& Frame = mCurrentSequence->mAsset->GetFrame(mCurrentSequence->mFrame);
	EAnimationTextureType TexType = mCurrentSequence->mAsset->GetAnimationTextureType();
	CTexture* Texture = mCurrentSequence->mAsset->GetTexture();

	switch (TexType)
	{
	case EAnimationTextureType::SpriteSheet:
		LTX = Frame.Start.x / Texture->GetTexture()->Width;
		LTY = Frame.Start.y / Texture->GetTexture()->Height;
		RBX = LTX + Frame.Size.x / Texture->GetTexture()->Width;
		RBY = LTY + Frame.Size.y / Texture->GetTexture()->Height;
		mOwner->SetTextureIndex(0);
		break;
	case EAnimationTextureType::Frame:
		mOwner->SetTextureIndex(mCurrentSequence->mFrame);
		break;
	}

	mAnimCBuffer->SetAnimation2DEnable(true);
	mAnimCBuffer->SetUV(LTX, LTY, RBX, RBY);

	mAnimCBuffer->UpdateBuffer();
}
