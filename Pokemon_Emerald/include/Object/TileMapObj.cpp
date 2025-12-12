#include "TileMapObj.h"

#include "../Scene/Input.h"
#include "../Scene/Scene.h"

CTileMapObj::CTileMapObj()
{
}

CTileMapObj::CTileMapObj(const CTileMapObj& Obj)
{
}

CTileMapObj::CTileMapObj(CTileMapObj&& Obj)
{
}

CTileMapObj::~CTileMapObj()
{
}

void CTileMapObj::AddTileType()
{
	mEditTileType = static_cast<ETileType>((int)mEditTileType + 1);

	if (mEditTileType ==ETileType::End)
	{
		mEditTileType = ETileType::Nomal;
	}
}

void CTileMapObj::AddTileFrame()
{
	mEditorTileFrame = (mEditorTileFrame + 1) % mTileMap->GetTileFrameCount();
}

bool CTileMapObj::Init()
{
	CSceneObject::Init();

	mTileMapRender = CreateComponent<CTileMapRenderComponent>();

	mTileMap = CreateComponent<CTileMapComponent>();

	// 아웃라인 그리기 -> 쉐이더랑 메쉬 설정 까지 
	mTileMap->SetTileOutLineRender(true);

	mTileMapRender->SetBackTexture("TileMapBack", TEXT("Texture/MapBackGround.png"));

	//타일 스프라이트 텍스쳐 설정 
	mTileMapRender->SetTileTexture("Tile", TEXT("Texture/Floors.png"));

	mTileMap->AddTileFrame(0.f, 0.f, 64.f, 64.f);
	mTileMap->AddTileFrame(0.f, 64.f, 64.f, 64.f);
	mTileMap->AddTileFrame(0.f, 128.f, 64.f, 64.f);
	mTileMap->AddTileFrame(0.f, 192.f, 64.f, 64.f);
	mTileMap->AddTileFrame(0.f, 256.f, 64.f, 64.f);

	SetRootComponent(mTileMapRender);

	mTileMap->CreateTile(100, 100, FVector2D(64.f, 64.f));

	return true;
}

void CTileMapObj::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	const FVector2D& MousePos = mScene->GetInput()->GetMouseWorldPos();

	if (mEditorMode == EEditorMode::TileType)
	{
		if (mScene->GetInput()->GetMouseHold(EMouseButtonType::LButton))
		{
			mTileMap->ChangeTileType(mEditTileType, MousePos);
		}
		//마우스가 올려져 있을때
		else if (!mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton))
		{
			int Index = mTileMap->GetTileIndex(MousePos);

			//이전 인덱스와 지금 인덱스가 다를때만 변경
			if ( Index != mOnMousePrevIndex)
			{
				ETileType PrevType = mTileMap->ChangeTileType(ETileType::MouseOver, MousePos);

				if (PrevType != ETileType::None)
				{
					if (mOnMousePrevIndex != -1)
					{
						mTileMap->ChangeTileType(mOnMousePrevTileType, mOnMousePrevIndex);
					}

					mOnMousePrevIndex = mTileMap->GetTileIndex(MousePos);
					mOnMousePrevTileType = PrevType;
				}
				else
				{
					mTileMap->ChangeTileType(mOnMousePrevTileType, mOnMousePrevIndex);
					mOnMousePrevIndex = -1;
					mOnMousePrevTileType = ETileType::None;
				}
			}
		}
	}
	else if (mEditorMode == EEditorMode::TileImage)
	{
		if (mScene->GetInput()->GetMouseHold(EMouseButtonType::LButton))
		{
			mTileMap->ChangeTileFrame(mEditorTileFrame, MousePos);
		}
	}
}

void CTileMapObj::Save(const TCHAR* FullPath)
{
	mTileMap->Save(FullPath);
}

void CTileMapObj::Load(const TCHAR* FullPath)
{
	mTileMap->Load(FullPath);
}

void CTileMapObj::Load(const char* FileName)
{
	mTileMap->Load(FileName);
}
