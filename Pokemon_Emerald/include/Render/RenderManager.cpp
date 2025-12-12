#include "RenderManager.h"

#include "RenderState.h"
#include "RenderStateManager.h"
#include "../Component/SceneComponent.h"
#include "../Device.h"
#include "../Scene/SceneManager.h"

CRenderManager::CRenderManager()
{
	
}

CRenderManager::~CRenderManager()
{
	auto iter = mLayerList.begin();
	auto iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_RELEASE(mSampler);
	SAFE_DELETE(mStateManager);
}

void CRenderManager::AddRenderList(class CSceneComponent* Component)
{
	//mRenderList.emplace_back(Component);

	FRenderLayer* Layer =  FindLayer(Component->GetRenderLayerName());

	if (!Layer)
	{
		return;
	}

	//등록
	Layer->RenderList.emplace_back(Component);

}

void CRenderManager::ClearRenderList()
{
	auto iter = mLayerList.begin();
	auto iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->RenderList.clear();
	}
}

bool CRenderManager::CreateRenderLayer(const string& Name, int ZOrder)
{
	FRenderLayer* Layer = FindLayer(Name);

	if (Layer)
	{
		return true;
	}

	Layer = new FRenderLayer;

	Layer->ZOrder = ZOrder;

	mLayerList.insert(make_pair(ZOrder, Layer));
	mLayerNameList[Name] = ZOrder;

	return true;
}

FRenderLayer* CRenderManager::FindLayer(const string& Name)
{
	auto iter = mLayerNameList.find(Name);

	if (iter == mLayerNameList.end())
	{
		return nullptr;
	}

	int ZOrder = iter->second;

	auto iter1 = mLayerList.find(ZOrder);

	return iter1->second;
}

bool CRenderManager::Init()
{
	//사용할 레이어 등록
	CreateRenderLayer("BackGround", INT_MIN);
	CreateRenderLayer("Object", 0);


	mStateManager = new CRenderStateManager;

	if (!mStateManager->Init())
	{
		return false;
	}

	D3D11_SAMPLER_DESC Desc = {};

	Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	/*
	WRAP	: UV 0 ~ 1을 벗어나면 다시 0~ 1 사이로 반복한다.
	MIRROR	: UV 0 ~ 1을 벗어나면 거울에 비친것처럼 반전되어 반복한다.
	CLAMP	:  UV 0 ~ 1을 벗어나면 가장자리 픽셀 색상으로 고정된다.
	*/
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//LOD 사용시 필요하다.
	//우리는 사용안할것이기에 필요없다. 
	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	// 샘플링 비교 함수
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &mSampler)))
	{
		return false;
	}

	mAlphaBlend = mStateManager->FindState("AlphaBlend");

	mRasterizer = mStateManager->FindState("Rasterizer");

	mDepthDisable = mStateManager->FindState("DepthDisable");

	return true;
}

void CRenderManager::Render()
{
	//기본 샘플러 셋팅 
	CDevice::GetInst()->GetContext()->PSSetSamplers(0, 1, &mSampler);

	//알파블랜드 
	mAlphaBlend->SetState();
	//레스터라이저 셋팅 
	mRasterizer->SetState();

	mDepthDisable->SetState();

	auto iter = mLayerList.begin();
	auto iterEnd = mLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		FRenderLayer* Layer = iter->second;

		//정렬해주기
		switch (mRenderSortType)
		{
		case ERenderSortType::None:
			break;
		case ERenderSortType::Y:
			if (Layer->RenderList.size() > 1)
			{
				Layer->RenderList.sort(CRenderManager::SortY);
			}
			break;
		case ERenderSortType::Alpha:
			if (Layer->RenderList.size() > 1)
			{
				Layer->RenderList.sort(CRenderManager::SortAlpha);
			}
			break;
		}


		//그리기 
		auto iter1 = Layer->RenderList.begin();
		auto iterEnd1 = Layer->RenderList.end();

		for (; iter1 != iterEnd1;)
		{
			//Active
			if (!(*iter1)->IsActive())
			{
				iter1 = Layer->RenderList.erase(iter1);
				iterEnd1 = Layer->RenderList.end();
				continue;
			}
			else if (!(*iter1)->IsEnable())
			{
				++iter1;
				continue;
			}

			// 그려줘 
			(*iter1)->PreRender();

			(*iter1)->Render();

			(*iter1)->PostRender();

			++iter1;
		}
	}

	// UI는 여기서 그려줄 것이다!
	CSceneManager::GetInst()->RenderUI();

	mDepthDisable->ResetState();



	//알파 블랜드 종료 
	mAlphaBlend->ResetState();
	//레스터라이저 종료
	mRasterizer->ResetState();

}

bool CRenderManager::SortY(const CSharedPtr<class CSceneComponent>& Src, const CSharedPtr<class CSceneComponent>& Dest)
{
	float SrcY = Src->GetWorldPosition().y - Src->GetPivot().y * Src->GetWorldScale().y;

	float DestY = Dest->GetWorldPosition().y - Dest->GetPivot().y * Dest->GetWorldScale().y;

	return SrcY < DestY;
}

bool CRenderManager::SortAlpha(const CSharedPtr<class CSceneComponent>& Src,
	const CSharedPtr<class CSceneComponent>& Dest)
{
	return false;
}
