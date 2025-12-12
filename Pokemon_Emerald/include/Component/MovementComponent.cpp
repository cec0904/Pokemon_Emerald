#include "MovementComponent.h"
#include "SceneComponent.h"
#include "RotationComponent.h"

CMovementComponent::CMovementComponent()
{
}

CMovementComponent::CMovementComponent(const CMovementComponent& Com)
	: CComponent(Com)
{
}

CMovementComponent::CMovementComponent(CMovementComponent&& Com)
	: CComponent(Com)
{
}

CMovementComponent::~CMovementComponent()
{
}

void CMovementComponent::SetUpdateComponent(CSceneComponent* Target)
{
	mUpdateComponent = Target;
}

bool CMovementComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}

	return true;
}
bool CMovementComponent::Init(const char* FileName)
{
	if (!CComponent::Init(FileName))
	{
		return false;
	}

	return true;
}
void CMovementComponent::PreUpdate(float DeltaTime)
{
	CComponent::PreUpdate(DeltaTime);

	if (mUpdateComponent)
	{
		if (!mUpdateComponent->IsActive())
		{
			mUpdateComponent = nullptr;
		}
		else if (mUpdateComponent->IsEnable())
		{
			// 여기서 작업 
		}
	}

}
void CMovementComponent::Update(float DeltaTime)
{
	CComponent::Update(DeltaTime);

	if (mUpdateComponent)
	{
		if (!mUpdateComponent->IsActive())
		{
			mUpdateComponent = nullptr;
		}
		else if (mUpdateComponent->IsEnable())
		{
			if (mMoveAxis != EAxis::None)
			{
				mVelocity = mUpdateComponent->GetAxis(mMoveAxis);
			}

			//이동 벡터 정규화
			mVelocity.Normalize();

			if (mVelocity.Length() > 0.f)
			{
				// 한 프레임당 이동할 거리 
				mMoveStep = mVelocity* mSpeed* DeltaTime;
				mUpdateComponent->AddWorldPos(mMoveStep);
			}
		}
	}
}
void CMovementComponent::PostUpdate(float DeltaTime)
{
	CComponent::PostUpdate(DeltaTime);

	if (mUpdateComponent)
	{
		if (!mUpdateComponent->IsActive())
		{
			mUpdateComponent = nullptr;
		}
		else if (mUpdateComponent->IsEnable())
		{
			// 여기서 작업 
		}
	}
}
void CMovementComponent::PostRender()
{
	CComponent::PostRender();

	if (mUpdateComponent)
	{
		if (!mUpdateComponent->IsActive())
		{
			mUpdateComponent = nullptr;
		}
		else if (mUpdateComponent->IsEnable())
		{
			// 여기서 작업 
		}
	}


}
CMovementComponent* CMovementComponent::Clone()
{
	return new CMovementComponent(*this);
}

void CMovementComponent::EndFrame()
{
	if (mVelocityInit)
		mVelocity = FVector3D::Zero;

	mMoveStep = FVector3D::Zero;
}
