#pragma once

#include "../Gameinfo.h"

//레이어
struct FRenderLayer
{
	list <CSharedPtr<class CSceneComponent>> RenderList;
	int ZOrder = 0;
};


class CRenderManager
{
	DECLARE_SINGLETON(CRenderManager);

private:
	class CRenderStateManager* mStateManager = nullptr;

	//Render 할 SceneComponent를 모아둘것이다.
	//list <CSharedPtr<class CSceneComponent>> mRenderList;
	// map , unorderd_map
	// Z오더값, 그려질 목록 
	map<int, FRenderLayer*>			mLayerList;
	//어떤 레이어가 있고, 그 해당 레이어의 Z오더값 
	unordered_map<string, int>	mLayerNameList;	


	//정렬 타입
	ERenderSortType mRenderSortType = ERenderSortType::Y;

	//기본 샘플러
	ID3D11SamplerState* mSampler = nullptr;

	//알파블랜드
	class CRenderState* mAlphaBlend = nullptr;
	//레스터라이즈스테이트
	class CRenderState* mRasterizer = nullptr;
	//뎁스 검사 X 뎁스스텐실스테이트
	class CRenderState* mDepthDisable = nullptr;


public:
	//RenderList 등록함수
	void AddRenderList(class CSceneComponent* Component);
	void ClearRenderList();

	//레이어 생성
	bool CreateRenderLayer(const string& Name, int ZOrder);
	//레이어 탐색
	FRenderLayer* FindLayer(const string& Name);


public:
	class CRenderStateManager* GetStateManager()
	{
		return mStateManager;
	}



public:
	bool Init();
	void Render();


private:
	static bool SortY(const CSharedPtr<class CSceneComponent>& Src, const CSharedPtr<class CSceneComponent>& Dest);

	static bool SortAlpha(const CSharedPtr<class CSceneComponent>& Src, const CSharedPtr<class CSceneComponent>& Dest);

};

