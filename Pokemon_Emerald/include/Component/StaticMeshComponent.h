#pragma once
#include "MeshComponent.h"

class CStaticMeshComponent :
	public CMeshComponent
{
	//씬이랑 오브젝트 모두 사용할거같다.
	friend class CScene;
	friend class CSceneObject;

protected:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& Com);
	CStaticMeshComponent(CStaticMeshComponent&& Com);
	virtual ~CStaticMeshComponent();

protected:
	CSharedPtr<class CShader> mShader;
	CSharedPtr<class CStaticMesh> mMesh;

public:
	void SetShader(const string& Name);
	void SetShader(class CShader* Shader);
	void SetMesh(const string& Name);
	void SetMesh(class CMesh* Mesh);
	//중간에 머티리얼을 변경할수있게 함수를 제공한다.
	void SetMaterial(int SlotIndex, const string Name);
	void SetMaterial(int SlotIndex, class CMaterial* Material);
	//텍스쳐 가져오기
	// Texture 클래스를 바로 넣어서 셋팅 해준다. 
	void AddTexture(int SlotIndex, class CTexture* Texture,
		int Register,
		int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);
	//Name으로 찾아와서 텍스쳐를 넣어주는 바업
	void AddTexture(int SlotIndex, const string& Name,
		int Register,
		int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void AddTexture(int SlotIndex, const string& Name,
		const TCHAR* FileName,
		int Register,
		int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);

	//BaseColor
	void SetBaseColor(int SlotIndex, float r, float g, float b, float a);
	//Opacity
	void SetOpacity(int SlotIndex, float Opacity);



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
	virtual CStaticMeshComponent* Clone();
};

