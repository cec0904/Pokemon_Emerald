#pragma once
#include "SceneComponent.h"
#include "../Animation/Animation2D.h"

class CSpriteComponent :
    public CSceneComponent
{
	//씬이랑 오브젝트 모두 사용할거같다.
	friend class CScene;
	friend class CSceneObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& Com);
	CSpriteComponent(CSpriteComponent&& Com);
	virtual ~CSpriteComponent();

protected:
	CSharedPtr<class CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	//출력할 이미지 한장.
	CSharedPtr<class CTexture> mTexture;
	FVector4D mTint = FVector4D::White;
	int mTextureIndex = 0;

	bool mUseTime = false;


	//애니메이션 출력용도 
	class CAnimation2D* mAnimation = nullptr;

	//반전
	bool mIsFlip = false;

	class CSpriteCBuffer* mSpriteCBuffer;

	//애니메이션 객체 생성해주기
public:
	template<typename T>
	T* CreateAnimation2D()
	{
		mAnimation = new T;

		mAnimation->mOwner = this;
		mAnimation->mScene = mScene;

		if (!mAnimation->Init())
		{
			SAFE_DELETE(mAnimation);
			return nullptr;
		}

		return static_cast<T*>(mAnimation);
	}

	template<typename T>
	T* GetAnimationInstance()
	{
		return dynamic_cast<T*>(mAnimation);
	}

	bool GetIsFlip()
	{
		return mIsFlip;
	}


public:
	void SetUseTime(bool Use)
	{
		mUseTime = Use;
	}

	//쉐이더 셋팅 
	void SetShader(const string& Name);
	void SetShader(class CShader* Shader);
	//텍스쳐 셋팅
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	void SetTexture(const string& Name,
		int TextureIndex = 0);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTexture(const string& Name,
		const TCHAR* FileName,
		int TextureIndex = 0);
	//셋팅 바로 해주기 
	void SetTexture(class CTexture* Texture,
		int TextureIndex = 0);

	void SetTextureIndex(int Index);

	// 색상
	void SetTint(float r, float g, float b);
	//오파시티
	void SetOpacity(float Opacity);

	//반전
	void SetFlip(bool Flip);
	
	const CAnimation2DData* GetCurrentAnimationData();

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
	virtual CSpriteComponent* Clone();
};

