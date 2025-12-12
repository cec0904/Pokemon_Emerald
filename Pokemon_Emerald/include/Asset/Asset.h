#pragma once
#include "../Share/Object.h"

class CAsset abstract : public CObject
{
public:
	CAsset();
	virtual ~CAsset();

protected:
	//에셋도 구분이 필요하다
	// 메쉬 또는 텍스쳐 그외에도 추가될거니까
	// 스테이지(Scene) 별로 사용할 에셋을 관리해보자!
	class CScene* mScene = nullptr;
	//관리할 에셋 이름 
	string mName;
	//에셋 타입 
	EAssetType mAssetType;

public:
	const string& GetName() const
	{
		return mName;
	}

	EAssetType GetAssetType() const
	{
		return mAssetType;
	}

	void SetName(const string& Name)
	{
		mName = Name;
	}

};

