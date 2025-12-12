#pragma once
#include "SceneComponent.h"
class CTileMapRenderComponent :
    public CSceneComponent
{
	//씬이랑 오브젝트 모두 사용할거같다.
	friend class CScene;
	friend class CSceneObject;

protected:
	CTileMapRenderComponent();
	CTileMapRenderComponent(const CTileMapRenderComponent& Com);
	CTileMapRenderComponent(CTileMapRenderComponent&& Com);
	virtual ~CTileMapRenderComponent();

protected:
	CSharedPtr<class CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	//출력할 이미지 한장.
	// 배경용 이미지 
	CSharedPtr<class CTexture> mBackTexture;
	// 타일에 사용할 스프라이트 이미지 
	CSharedPtr<class CTexture> mTileTexture;
	// 타일정보를 들고있는 타일 컴포넌트 
	class CTileMapComponent* mTileMap = nullptr;

public:
	CTexture* GetBackTexture() const
	{
		return mBackTexture;
	}

	CTexture* GetTileTexture() const
	{
		return mTileTexture;
	}


public:
	void SetTileMapComponent(CTileMapComponent* TileMap);

	//쉐이더 셋팅 
	void SetShader(const string& Name);
	void SetShader(class CShader* Shader);

	//텍스쳐 셋팅
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	//Back
	void SetBackTexture(const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetBackTexture(const string& Name,
		const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetBackTexture(class CTexture* Texture);

	//Tile
	void SetTileTexture(const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTileTexture(const string& Name,
		const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetTileTexture(class CTexture* Texture);

	//시점 함수 
public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CTileMapRenderComponent* Clone();
};

