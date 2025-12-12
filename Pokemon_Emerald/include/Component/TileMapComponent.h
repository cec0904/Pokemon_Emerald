#pragma once
#include "Component.h"
#include "Tile.h"

class CTileMapComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CTileMapComponent();
	CTileMapComponent(const CTileMapComponent& Com);
	CTileMapComponent(CTileMapComponent&& Com);
	virtual ~CTileMapComponent();

protected:
	vector<CTile*> mTileList;
	int mCountX = 0;
	int mCountY = 0;
	FVector2D mTileSize;
	//타일맵 전체크기
	FVector2D mMapSize;


	//타일그리기
	CSharedPtr<class CMesh> mTileMesh;
	CSharedPtr<class CShader> mTileShader;
	//스프라이트의 프레임 정보를 저장한다. 
	vector<FAnimationFrame> mTileFrameList;
	//텍스쳐 사이즈
	FVector2D mTileTextureSize;
	//타일 그리기용 상수버퍼 
	class  CTileMapCBuffer* mTileMapCBuffer = nullptr;



	//외곽선 그리기
	bool mTileOutlineRender = false;
	//메쉬
	CSharedPtr<class CMesh> mOutlineMesh;
	//쉐이더 
	CSharedPtr<class CShader> mOutlineShader;
	//위치용 상수버퍼 
	class CTransformCBuffer* mLineTransformCBuffer = nullptr;
	//충돌체 상수버퍼 
	class CColliderCBuffer* mCBuffer = nullptr;
	

	//그리기 범위
	//화면이 그려지고있는 범위를 구할것이다. 
	int mViewStartX;
	int mViewStartY;
	int mViewEndX;
	int mViewEndY;


public:
	int GetTileFrameCount() const
	{
		return static_cast<int>(mTileFrameList.size());
	}

	const FVector2D& GetTileSize() const
	{
		return mTileSize;
	}

	int GetTileCountX() const
	{
		return mCountX;
	}

	int GetTileCountY() const
	{
		return mCountY;
	}

public:
	void SetTileTextureSize(const FVector2D& Size)
	{
		mTileTextureSize = Size;
	}

	void SetTileTextureSize(unsigned int Width, unsigned int Height)
	{
		mTileTextureSize.x = static_cast<float>(Width);
		mTileTextureSize.y = static_cast<float>(Height);
	}



public:
	void CreateTile(int CountX, int CountY, const FVector2D& TileSize, int TileTextureFrame = -1);

public:
	//외곽선 그리기
	void SetTileOutLineRender(bool Render);
	void RenderTileOutLine();

public:
	//타일그리기
	void RenderTile();

	//타일 텍스쳐 설정
	void SetTileTexture(const string& Name);
	//파일 이름으로 텍스쳐 매니져에서 로드 후 추가하기 
	void SetTileTexture(const string& Name, const TCHAR* FileName);
	//셋팅 바로 해주기 
	void SetTileTexture(class CTexture* Texture);

	//텍스쳐 프레임도 지정 
	void AddTileFrame(const FVector2D& Start, const FVector2D& Size);
	void AddTileFrame(float StartX, float StartY, float SizeX, float SizeY);




public:
	//타일 인덱스 구하기
	int GetTileIndexX(const FVector3D& Pos) const;
	int GetTileIndexX(const FVector2D& Pos) const;
	int GetTileIndexX(float x) const;

	int GetTileIndexY(const FVector3D& Pos) const;
	int GetTileIndexY(const FVector2D& Pos) const;
	int GetTileIndexY(float y) const;

	int GetTileIndex(const FVector3D& Pos) const;
	int GetTileIndex(const FVector2D& Pos) const;
	int GetTileIndex(float x, float y) const;

public:
	//타일 타입 변경해주기
	ETileType ChangeTileType(ETileType Type, const FVector3D& Pos);
	ETileType ChangeTileType(ETileType Type, const FVector2D& Pos);
	ETileType ChangeTileType(ETileType Type, float x, float y);
	ETileType ChangeTileType(ETileType Type, int Index);

public:
	//타일 이미지 변경하기
	void ChangeTileFrame(int Frame, const FVector3D& Pos);
	void ChangeTileFrame(int Frame, const FVector2D& Pos);
	void ChangeTileFrame(int Frame, float x, float y);
	void ChangeTileFrame(int Frame, int Index);


	//시점 함수 
public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PostRender();
	virtual CTileMapComponent* Clone();
	virtual void EndFrame();

public:
	//세이브 로드
	void Save(const TCHAR* FullPath);
	void Save(const char* FileName);
	void Load(const TCHAR* FullPath);
	void Load(const char* FileName);

};

