#include "MeshManager.h"
#include "MeshManager.h"

#include "StaticMesh.h"

CMeshManager::CMeshManager()
{

}

CMeshManager::~CMeshManager()
{

}

bool CMeshManager::Init()
{
	//육면체 
	// 8개의 점을 만든다. 
	FVertexColor BoxVtx[8] =
	{
		FVertexColor(-0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f),	//0
		FVertexColor(0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f),	//1
		FVertexColor(-0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f),	//2
		FVertexColor(0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f),	//3

		FVertexColor(-0.5f, 0.5f, 0.5f, 1.f, 0.f, 1.f, 1.f),	//4
		FVertexColor(0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.f, 1.f),		//5
		FVertexColor(-0.5f, -0.5f, 0.5f, 1.f, 1.f, 1.f, 1.f),	//6
		FVertexColor(0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f),	//7
	};

	//점들을 연결해줄 선들을 만들어 줘야한다. 

	//화면에는 앞면만 나오는데 
	// 앞면에 기준은 인덱스가 시계방향으로 그려줘야한다. 
	// 인덱스를 반시계 반향으로 그리면
	// 뒤면이 보이게 된다. 
	// 뒤면이 보이면 화면에 안그려지게 백스페이스 컬링이 된다. 

	unsigned short BoxIdx[36] =
	{
		0, 1, 3,  0, 3, 2,
		1, 5, 7,  1, 7, 3,
		5, 4, 6,  5, 6, 7,
		4, 0, 2,  4 ,2, 6,
		4, 5, 1,  4, 1, 0,
		2, 3, 7,  2, 7, 6
	};

	if (!CreateMesh("Box", BoxVtx, sizeof(FVertexColor), 8, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, BoxIdx, sizeof(unsigned short), 36, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}

	//사각형 
	FVertexColor CenterRect[4] =
	{
		FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
		FVertexColor(0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
		FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f),
		FVertexColor(0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f, 1.f)
	};

	unsigned short RectIdx[6] = { 0 , 1, 3, 0, 3, 2 };

	if (!CreateMesh("CenterRect", CenterRect, sizeof(FVertexColor), 4, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIdx, sizeof(unsigned short), 6, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}

	// 사각형 와이어 프레임 
	FVector3D FrameCenterRect[4] =
	{
		FVector3D(-0.5f, 0.5f, 0.f),
		FVector3D(0.5f, 0.5f, 0.f),
		FVector3D(-0.5f, -0.5f, 0.f),
		FVector3D(0.5f, -0.5f, 0.f)
	};

	unsigned short FrameRectIdx[5] = { 0, 1, 3, 2, 0 };

	if (!CreateMesh("FrameCenterRect", FrameCenterRect, sizeof(FVector3D), 4, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, FrameRectIdx, sizeof(unsigned short), 5, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}

	//LB 프레임
	FVector3D FrameLBRect[4] =
	{
		FVector3D(0.f, 1.f, 0.f),
		FVector3D(1.f, 1.f, 0.f),
		FVector3D(0.f, 0.f, 0.f),
		FVector3D(1.f, 0.f, 0.f)
	};

	if (!CreateMesh("FrameLBRect", FrameLBRect, sizeof(FVector3D), 4, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, FrameRectIdx, sizeof(unsigned short), 5, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}



	// 원 와이어 
	//정점 36개짜리 도형 => 원이라고 생각한다. 
	FVector3D Sphere2DPoint[37];

	for (int i = 0; i < 37; ++i)
	{
		float Angle = DirectX::XMConvertToRadians(i * 10.f);

		//지름이 1인 도형을 만들고 있기 때문에 
		//반지름을 0.5로 만들어 준것이다. 
		Sphere2DPoint[i].x = cosf(Angle) * 0.5f;
		Sphere2DPoint[i].y = sinf(Angle) * 0.5f;
	}

	if (!CreateMesh("FrameSphere2D", Sphere2DPoint, sizeof(FVector3D), 37, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP))
	{
		return false;
	}

	//Line 직선
	//Y방향 
	FVector3D LineUp[2] =
	{
		FVector3D(0.f, 0.f , 0.f),
		FVector3D(0.f, 1.f, 0.f)
	};

	if (!CreateMesh("LineUp2D", LineUp, sizeof(FVector3D), 2, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP))
	{
		return false;
	}

	//X축방향 
	FVector3D LineRight[2] =
	{
		FVector3D(0.f, 0.f , 0.f),
		FVector3D(1.f, 0.f, 0.f)
	};

	if (!CreateMesh("LineRight2D", LineRight, sizeof(FVector3D), 2, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP))
	{
		return false;
	}


	//텍스쳐를 그려주는 사각형 메시
	FVertexTexture CenterTexRect[4] =
	{
		//좌상
		FVertexTexture(-0.5f, 0.5f, 0.f, 0.f, 0.f),
		//우상
		FVertexTexture(0.5f, 0.5f, 0.f, 1.f, 0.f),
		//좌하
		FVertexTexture(-0.5f, -0.5f, 0.f, 0.f, 1.f),
		//우하 
		FVertexTexture(0.5f, -0.5f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh("CenterTexRect", CenterTexRect, sizeof(FVertexTexture), 4, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIdx, sizeof(unsigned short), 6, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}

	//SpriteMesh
	FVertexTexture SpriteRect[4] =
	{
		//좌상
		FVertexTexture(0.f, 1.f, 0.f, 0.f, 0.f),
		//우상
		FVertexTexture(1.f, 1.f, 0.f, 1.f, 0.f),
		//좌하
		FVertexTexture(0.f, 0.f, 0.f, 0.f, 1.f),
		//우하 
		FVertexTexture(1.f, 0.f, 0.f, 1.f, 1.f)
	};

	if (!CreateMesh("SpriteRect", SpriteRect, sizeof(FVertexTexture), 4, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, RectIdx, sizeof(unsigned short), 6, DXGI_FORMAT_R16_UINT))
	{
		return false;
	}



	return true;
}

bool CMeshManager::CreateMesh(const string& Name, void* VertexData, int Size, int Count, D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitiv, void* IndexData, int IndexSize, int IndexCount, DXGI_FORMAT Fmt, D3D11_USAGE IndexUsage)
{
	//메쉬 찾아오기 
	//기존에 같은이름을 등록된 mesh가 있는지 검사한다. 
	CMesh* Mesh = FindMesh(Name);

	if (Mesh)
	{
		return true;
	}

	//기존에 사용한 mesh가 없으면 새로 추가한다. 
	
	Mesh = new CStaticMesh;

	if (!Mesh->CreateMesh(VertexData, Size, Count, VertexUsage, Primitiv, IndexData, IndexSize, IndexCount, Fmt, IndexUsage))
	{
		SAFE_DELETE(Mesh);
		return false;
	}

	mMeshMap.insert(make_pair(Name, CSharedPtr<CMesh>(Mesh)));

	return true;
}

CMesh* CMeshManager::FindMesh(const string& Name)
{
	unordered_map<string, CSharedPtr<CMesh>>::iterator iter = mMeshMap.find(Name);

	if (iter == mMeshMap.end())
	{
		return nullptr;
	}

	return (CMesh*)iter->second.Get();
}

void CMeshManager::ReleaseMesh(class CAsset* Mesh)
{
	auto iter = mMeshMap.find(Mesh->GetName());

	if (iter != mMeshMap.end())
	{
		// 다른데서 사용하고 있을때 해당 에셋을 지우면 안되니까!
		if (iter->second->GetRefCount() == 1)
		{
			mMeshMap.erase(iter);
		}
	}
}
