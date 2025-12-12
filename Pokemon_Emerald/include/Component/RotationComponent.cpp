#include "RotationComponent.h"
#include "SceneComponent.h"

CRotationComponent::CRotationComponent()
{
}

CRotationComponent::CRotationComponent(const CRotationComponent& Com)
	: CComponent(Com)
{
}

CRotationComponent::CRotationComponent(CRotationComponent&& Com)
	: CComponent(Com)
{
}

CRotationComponent::~CRotationComponent()
{
}

void CRotationComponent::SetUpdateComponent(class CSceneComponent* Target)
{
	mUpdateComponent = Target;
}

bool CRotationComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}

	return true;
}
bool CRotationComponent::Init(const char* FileName)
{
	if (!CComponent::Init(FileName))
	{
		return false;
	}

	return true;
}
void CRotationComponent::PreUpdate(float DeltaTime)
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
void CRotationComponent::Update(float DeltaTime)
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
			//회전은 정규화 안할거다 
			//회전에 Velocity는 회전 속도가 될것이다!
			if (mVelocity.Length() > 0.f)
			{
				mRotationStep = mVelocity * DeltaTime;
			}

			mUpdateComponent->AddWorldRotation(mRotationStep);
		}
	}
}
void CRotationComponent::PostUpdate(float DeltaTime)
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
void CRotationComponent::PostRender()
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

CRotationComponent* CRotationComponent::Clone()
{
	return new CRotationComponent(*this);
}

void CRotationComponent::EndFrame()
{
	CComponent::EndFrame();

	if (mVelocityInit)
	{
		mVelocity = FVector3D::Zero;
	}

	mRotationStep = FVector3D::Zero;

}

