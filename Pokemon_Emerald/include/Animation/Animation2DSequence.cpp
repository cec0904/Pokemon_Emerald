#include "Animation2DSequence.h"

#include "../Asset/Animation/Animation2DData.h"

CAnimation2DSequence::CAnimation2DSequence()
{

}

CAnimation2DSequence::CAnimation2DSequence(const CAnimation2DSequence& Anim)
{
	// 얕은복사를 한다.
	*this = Anim;

	mNotifyList.clear();
}

CAnimation2DSequence::~CAnimation2DSequence()
{
	size_t	Size = mNotifyList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mNotifyList[i]);
	}
}

const string& CAnimation2DSequence::GetName() const
{
	return mAsset->GetName();
}


CAnimation2DSequence* CAnimation2DSequence::Clone()
{
	return new CAnimation2DSequence(*this);
}

void CAnimation2DSequence::SetAsset(class CAnimation2DData* Asset)
{
	mAsset = Asset;

	// 1프레임이 변경되어야 하는 시간 설정
	// mPlayTime이 1초이고 프레임이 10프레임이면
	// 1프레임이 변경되어야 하는 시간은 0.1초가 된다.
	mFrameTime = mPlayTime / mAsset->GetFrameCount();

}

void CAnimation2DSequence::SetPlayTime(float PlayTime)
{
	mPlayTime = PlayTime;

	if (mAsset)
	{
		mFrameTime = mPlayTime / mAsset->GetFrameCount();
	}

}

void CAnimation2DSequence::CallNotify()
{
	size_t	Size = mNotifyList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (mNotifyList[i]->Frame == mFrame)
		{
			if (mNotifyList[i]->Function)
				mNotifyList[i]->Function();
		}
	}
}

void CAnimation2DSequence::Update(float DeltaTime)
{
	mTime += DeltaTime * mPlayRate;

	if (mTime >= mFrameTime)
	{
		mTime -= mFrameTime;

		if (mReverse)
		{
			--mFrame;

			if (mFrame < 0)
			{
				if (mEndFunction && mEndFunctionEnable)
				{
					mEndFunctionEnable = false;
					mEndFunction();
				}

				if (mLoop)
				{
					mEndFunctionEnable = true;
					mFrame = mAsset->GetFrameCount() - 1;
				}

				else
				{
					mFrame = 0;
				}

			}
		}

		else
		{
			++mFrame;

			if (mFrame == mAsset->GetFrameCount())
			{
				if (mEndFunction && mEndFunctionEnable)
				{
					mEndFunctionEnable = false;
					mEndFunction();
				}

				if (mLoop)
				{
					mEndFunctionEnable = true;
					mFrame = 0;
				}

				else
				{
					mFrame = mAsset->GetFrameCount() - 1;
				}

			}
		}

		CallNotify();
	}
}
