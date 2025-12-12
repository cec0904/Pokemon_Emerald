#pragma once
#include "../Component/SceneComponent.h"
#include "../Share/Object.h"

class CSceneObject : public CObject
{
	friend class CScene;

protected:
	CSceneObject();
	CSceneObject(const CSceneObject& Obj);
	CSceneObject(CSceneObject&& Obj);
	virtual ~CSceneObject();

protected:
	//어떤 씬이 나를 들고있는지 
	class CScene* mScene = nullptr;
	string mName;
	//내가어떤 컴포넌트를 가지고 있을것인가?
	CSharedPtr<class CSceneComponent> mRootComponent;
	// 20251029 NonSceneComponent
	vector<CSharedPtr<class CComponent>> mNonComponentList;

	vector<CSharedPtr<class CComponent>> mSceneComponentList;

	float mLifeTime = 0.f;
	CSharedPtr<class CObjectSpawnPoint> mSpawnPoint;


	//무적상태 여부 
	bool mDamageEnable = true;

public:
	void SetLifeTime(float Time)
	{
		mLifeTime = Time;
	}

	class CSceneComponent* GetRootComponent() const
	{
		return mRootComponent;
	}

	class CScene* GetScene() const
	{
		return mScene;
	}

	const char* GetName() const
	{
		return  mName.c_str();
	}

	//set 
	void SetName(const string& Name)
	{
		mName = Name;
	}

	void SetRootComponent(class CSceneComponent* Root);

	//스폰 포인트 
	void SetSpawnPoint(class CObjectSpawnPoint* Point);

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
	virtual CSceneObject* Clone();
	virtual void Destroy() override;

	virtual void EndFrame();

public:
						// 대미지, 대미지를 준 대상 
	virtual float Damage(float Attack,CSceneObject* Obj);

public:
	//컴포넌트 생성해주는 친구 
	template<typename T>
	T* CreateComponent(const string LayerName = "")
	{
		T* Component = new T;

		Component->mScene = mScene;
		Component->mOwnerObject = this;

		class CSceneComponent* Com = dynamic_cast<CSceneComponent*>(Component);

		if (!LayerName.empty() && Com)
		{
			Com->mRenderLayerName = LayerName;
		}

		if (!Component->Init())
		{
			SAFE_DELETE(Component);
			return nullptr;
		}

		if (!Com)
		{
			mNonComponentList.emplace_back(Component);
		}
		else
		{
			mSceneComponentList.emplace_back(Component);
		}

		return Component;
	}

	//컴포넌트 찾아오기 
	template<typename T>
	T* FindNonSceneComponent()
	{
		auto iter = mNonComponentList.begin();
		auto iterEnd = mNonComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Com = dynamic_cast<T*>((*iter).Get());

			if (Com)
			{
				return Com;
			}
		}

		return nullptr;
	}

	template<typename T>
	T* FindSceneComponent()
	{
		auto iter = mSceneComponentList.begin();
		auto iterEnd = mSceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Com = dynamic_cast<T*>((*iter).Get());

			if (Com)
			{
				return Com;
			}
		}

		return nullptr;
	}


public:
#pragma region Transform
	//Getter
	const FMatrix& GetScaleMatrix() const;
	const FMatrix& GetRotationMatrix() const;
	const FMatrix& GetTranslateMatrix() const;
	const FMatrix& GetWorldMatrix() const;

	const FVector3D& GetAxis(EAxis::Type Axis) const;
	const FVector3D& GetRelativeScale() const;
	const FVector3D& GetRelativeRotation() const;
	const FVector3D& GetRelativePosition() const;
	const FVector3D& GetWorldScale() const;
	const FVector3D& GetWorldRotation() const;
	const FVector3D& GetWorldPosition() const;

	//setter
	void SetRelativeScale(const FVector3D& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeScale(const FVector2D& Scale);
	void SetRelativeScale(float x, float y);

	void SetRelativeRotation(const FVector3D& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotation(const FVector2D& Rot);
	void SetRelativeRotation(float x, float y);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativeRotationAxis(float Angle, const FVector3D& Axis);

	void SetRelativePos(const FVector3D& Pos);
	void SetRelativePos(float x, float y, float z);
	void SetRelativePos(const FVector2D& Pos);
	void SetRelativePos(float x, float y);

	//월드
	void SetWorldScale(const FVector3D& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScale(const FVector2D& Scale);
	void SetWorldScale(float x, float y);

	void SetWorldRotation(const FVector3D& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotation(const FVector2D& Rot);
	void SetWorldRotation(float x, float y);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldRotationAxis(float Angle, const FVector3D& Axis);

	void SetWorldPos(const FVector3D& Pos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPos(const FVector2D& Pos);
	void SetWorldPos(float x, float y);

	//Add
	void AddRelativeScale(const FVector3D& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeScale(const FVector2D& Scale);
	void AddRelativeScale(float x, float y);

	void AddRelativeRotation(const FVector3D& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotation(const FVector2D& Rot);
	void AddRelativeRotation(float x, float y);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

	void AddRelativePos(const FVector3D& Pos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativePos(const FVector2D& Pos);
	void AddRelativePos(float x, float y);

	void AddWorldScale(const FVector3D& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldScale(const FVector2D& Scale);
	void AddWorldScale(float x, float y);

	void AddWorldRotation(const FVector3D& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotation(const FVector2D& Rot);
	void AddWorldRotation(float x, float y);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);

	void AddWorldPos(const FVector3D& Pos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldPos(const FVector2D& Pos);
	void AddWorldPos(float x, float y);

#pragma endregion

};

