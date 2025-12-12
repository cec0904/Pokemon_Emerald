#include "Collision.h"

#include "Component/ColliderAABB2D.h"
#include "Component/ColliderLine2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderSphere2D.h"


bool CCollision::CollisionAABB2DToAABB2D(FVector3D& HitPoint, CColliderAABB2D* Src, CColliderAABB2D* Dest)
{
    if (CollisionAABB2DToAABB2D(HitPoint, Src->GetBox(), Dest->GetBox()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionSphere2DToSphere2D(FVector3D& HitPoint, CColliderSphere2D* Src, CColliderSphere2D* Dest)
{
	if (CollisionSphere2DToSphere2D(HitPoint,
		Src->GetWorldPosition(), Src->GetRadius(),
		Dest->GetWorldPosition(), Dest->GetRadius()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionAABB2DToSphere2D(FVector3D& HitPoint, CColliderAABB2D* Src, CColliderSphere2D* Dest)
{
	if (CollisionAABB2DToSphere2D(HitPoint, Src->GetBox(), Dest->GetWorldPosition(), Dest->GetRadius()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(FVector3D& HitPoint, class CColliderOBB2D* Src, class CColliderOBB2D* Dest)
{
	if (CollisionOBB2DToOBB2D(HitPoint, Src->GetBox(), Dest->GetBox()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionAABB2DToOBB2D(FVector3D& HitPoint, class CColliderAABB2D* Src, class CColliderOBB2D* Dest)
{
	if (CollisionAABB2DToOBB2D(HitPoint, Src->GetBox(), Dest->GetBox()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionSphere2DToOBB2D(FVector3D& HitPoint, class CColliderSphere2D* Src, class CColliderOBB2D* Dest)
{
	if (CollisionSphere2DToOBB2D(HitPoint,
		Src->GetWorldPosition(), Src->GetRadius(),
		Dest->GetBox()))
	{
		return true;
	}

	return false;\
}

bool CCollision::CollisionLine2DToLine2D(FVector3D& HitPoint, class CColliderLine2D* Src, class CColliderLine2D* Dest)
{
	if (CollisionLine2DToLine2D(HitPoint, Src->GetLine(), Dest->GetLine()))
	{
		return true;
	}

	return false;

}

bool CCollision::CollisionLine2DToAABB2D(FVector3D& HitPoint, class CColliderLine2D* Src, class CColliderAABB2D* Dest)
{
	if (CollisionLine2DToAABB2D(HitPoint, Src->GetLine(), Dest->GetBox()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionLine2DToOBB2D(FVector3D& HitPoint, class CColliderLine2D* Src, class CColliderOBB2D* Dest)
{
	if (CollisionLine2DToOBB2D(HitPoint, Src->GetLine(), Dest->GetBox()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionLine2DToSphere2D(FVector3D& HitPoint, class CColliderLine2D* Src,
	class CColliderSphere2D* Dest)
{
	if (CollisionLine2DToSphere2D(HitPoint, Src->GetLine(), Dest->GetWorldPosition(), Dest->GetRadius()))
	{
		return true;
	}

	return false;
}

bool CCollision::CollisionAABB2DToAABB2D(FVector3D& HitPoint, const FAABB2D& Src, const FAABB2D& Dest)
{
    //AABB 충돌 
	//좌표
	if (Src.Min.x > Dest.Max.x)
	{
		// 충돌체 왼쪽이 노드의 오른쪽 보다 크다.
		return false;
	}
	else if (Dest.Min.x > Src.Max.x)
	{
		// 충돌체 오른쪽이 노드의 왼쪽보다 작다 
		return false;
	}
	else if (Src.Min.y > Dest.Max.y)
	{
		// 충돌체의 아래가 노드의 위보다 크다.
		return false;
	}
	else if (Dest.Min.y > Src.Max.y)
	{
		// 충돌체의 위가 노드의 아래보다 작다.
		return false;
	}

	//충돌했다!
	//충돌 좌표 
	FVector3D Min, Max;
			// 비교 ?	참 : 거짓
	// 겹친부분의 최소 좌표
	Min.x = Src.Min.x > Dest.Min.x ? Src.Min.x : Dest.Min.x;
	Min.y = Src.Min.y > Dest.Min.y ? Src.Min.y : Dest.Min.y;

	// 겹친 부분의 최대 좌표 
	Max.x = Src.Max.x < Dest.Max.x ? Src.Max.x : Dest.Max.x;
	Max.y = Src.Max.y < Dest.Max.y ? Src.Max.y : Dest.Max.y;

	HitPoint.x = (Min.x + Max.x) * 0.5f;
	HitPoint.y = (Min.y + Max.y) * 0.5f;


	return true;
}

bool CCollision::CollisionSphere2DToSphere2D(FVector3D& HitPoint, const FVector3D& SrcCenter, float SrcRadius, const FVector3D& DestCenter, float DestRadius)
{
	//Src 와 Dest의 원충돌이 일어날것이다. 
	// 두 원의 중심간의 거리를 구한다. 
	float Dist = SrcCenter.Distance(DestCenter);

	//원충돌 했니? 
	// 두원점의 거리가 두 원의 반지름의 합보다 작니?
	if (Dist <= SrcRadius + DestRadius)
	{
		// 충돌 

		//충돌 지점의 길이의 절반을 구한다
		float Gap =  SrcRadius + DestRadius - Dist;
		Gap *= 0.5;

		//두 중심을 잇는 방향 벡터를 구한다. 
		FVector3D Dir = DestCenter - SrcCenter;
		Dir.Normalize();	// Src 에서 Dest 가는 방향을 구했다. 

		HitPoint =  SrcCenter + Dir * (SrcRadius - Gap);	// 충돌 사이 좌표 

		return true;
	}

	// 충돌안함!
	return false;
}

bool CCollision::CollisionAABB2DToSphere2D(FVector3D& HitPoint, const FAABB2D& Src, const FVector3D& DestCenter, float DestRadius)
{
	//1. 원의 중심이 사각형의 안에 있는지 검사해야한다.
	// 원의 중점의 x값이 AABB 사각형의 좌, 우 사이에 있거나 
	// 또는 y값이 AABB 사각형의 위, 아래 사이에 있는지 먼저 검사한다. 
	if ((Src.Min.x <= DestCenter.x && DestCenter.x <= Src.Max.x) ||
		(Src.Min.y <= DestCenter.y && DestCenter.y <= Src.Max.y))
	{
		// 사각형의 크기에서 반지름 만큼 더한 사각형 안에 있는지 검사한다.
		// 사각형을 원의 반지름 만큼 확장시켜준다.
		FAABB2D SrcExtent;
		SrcExtent.Min = Src.Min - DestRadius;
		SrcExtent.Max = Src.Max + DestRadius;

		//확장된 사각형 안에 원의 중점이 들어있는지 확인한다
		if (DestCenter.x < SrcExtent.Min.x)
		{
			return false;
		}
		else if (SrcExtent.Max.x < DestCenter.x)
		{
			return false;
		}
		else if (DestCenter.y < SrcExtent.Min.y)
		{
			return false;
		}
		else if (SrcExtent.Max.y < DestCenter.y)
		{
			return false;
		}

		//충돌된거다
		//Dest 원을 내접원으로 하는 사각형 구했다. 
		FAABB2D SphereAABB;
		SphereAABB.Min.x = DestCenter.x - DestRadius;
		SphereAABB.Min.y = DestCenter.y - DestRadius;
		SphereAABB.Max.x = DestCenter.x + DestRadius;
		SphereAABB.Max.y = DestCenter.y + DestRadius;

		FVector3D Min, Max;
		// 비교 ?	참 : 거짓
		// 겹친부분의 최소 좌표
		Min.x = Src.Min.x > SphereAABB.Min.x ? Src.Min.x : SphereAABB.Min.x;
		Min.y = Src.Min.y > SphereAABB.Min.y ? Src.Min.y : SphereAABB.Min.y;

		// 겹친 부분의 최대 좌표 
		Max.x = Src.Max.x < SphereAABB.Max.x ? Src.Max.x : SphereAABB.Max.x;
		Max.y = Src.Max.y < SphereAABB.Max.y ? Src.Max.y : SphereAABB.Max.y;

		HitPoint.x = (Min.x + Max.x) * 0.5f;
		HitPoint.y = (Min.y + Max.y) * 0.5f;


		return true;
	}

	//3. 대각선 영역을 검사한다.
	FVector3D Vertex;
	//가장 가까운 꼭지점을 구한다
	//왼쪽 대각선 영역에 있는가?
	if (DestCenter.x < Src.Min.x)
	{
		Vertex.x = Src.Min.x;
	}
	else
	{
		Vertex.x = Src.Max.x;
	}

	//대각선에 아래쪽인지 위쪽인지
	if (DestCenter.y < Src.Min.y)
	{
		Vertex.y = Src.Min.y;
	}
	else
	{
		Vertex.y = Src.Max.y;
	}

	// 해당 꼭지점이랑 원의 중심이랑 거리가 반지름보다 작은지 확인해야한다. 
	float Dist = Vertex.Distance(DestCenter);

	if (Dist <= DestRadius)
	{
		//모서리할때는 충돌지점을 모서리로 하겠다. 
		HitPoint = Vertex;
		return true;
	}
	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(FVector3D& HitPoint, const FOBB2D& Src, const FOBB2D& Dest)
{
	// 두 충돌체의 중심의 거리 벡터를 구한다.
	FVector2D CenterLine = Src.Center - Dest.Center;

	//축검사를 실시한다. 
	// Src X축
	if (!ComputeAxisProjection(CenterLine, Src.Axis[EAxis::X], Src.HalfSize.x,
		Dest.Axis, Dest.HalfSize))
	{
		return false;
	}

	// Src Y축
	if (!ComputeAxisProjection(CenterLine, Src.Axis[EAxis::Y], Src.HalfSize.y,
		Dest.Axis, Dest.HalfSize))
	{
		return false;
	}

	// Dest X축
	if (!ComputeAxisProjection(CenterLine, Dest.Axis[EAxis::X], Dest.HalfSize.x,
		Src.Axis, Src.HalfSize))
	{
		return false;
	}

	// Dest Y축
	if (!ComputeAxisProjection(CenterLine, Dest.Axis[EAxis::Y], Dest.HalfSize.y,
		Src.Axis, Src.HalfSize))
	{
		return false;
	}

	//충돌 성공이다!!

	//충돌 지점은 AABBB 정보로 만들어서 두 교지합의 중점을 충돌지점으로 사용할 것이다.
	FAABB2D Hit1, Hit2;


	Hit1 = CreateAABB2D(Src);
	Hit2 = CreateAABB2D(Dest);

	FVector3D Min, Max;
	// 비교 ?	참 : 거짓
	// 겹친부분의 최소 좌표
	Min.x = Hit1.Min.x > Hit2.Min.x ? Hit1.Min.x : Hit2.Min.x;
	Min.y = Hit1.Min.y > Hit2.Min.y ? Hit1.Min.y : Hit2.Min.y;

	// 겹친 부분의 최대 좌표 
	Max.x = Hit1.Max.x < Hit2.Max.x ? Hit1.Max.x : Hit2.Max.x;
	Max.y = Hit1.Max.y < Hit2.Max.y ? Hit1.Max.y : Hit2.Max.y;

	HitPoint.x = (Min.x + Max.x) * 0.5f;
	HitPoint.y = (Min.y + Max.y) * 0.5f;

	return true;
}

// AABB to OBB 
bool CCollision::CollisionAABB2DToOBB2D(FVector3D& HitPoint, const FAABB2D& Src, const FOBB2D& Dest)
{
	FOBB2D SrcOBB = CreateOBB2D(Src);

	return CollisionOBB2DToOBB2D(HitPoint, SrcOBB, Dest);

}

bool CCollision::CollisionSphere2DToOBB2D(FVector3D& HitPoint, const FVector3D& SrcCenter, float SrcRadius,
	const FOBB2D& Dest)
{
	FVector2D ConvertCenter;
	ConvertCenter.x = SrcCenter.x;
	ConvertCenter.y = SrcCenter.y;

	FVector2D CenterLine = ConvertCenter - Dest.Center;

	FVector2D Axis = CenterLine;
	Axis.Normalize();	//정사영 받을 축을 구했다.

	// 충앙 연결 라인을 축으로 검사한다.
	if (!ComputeAxisProjection(CenterLine, Axis, SrcRadius, Dest.Axis, Dest.HalfSize))
	{
		//충돌 실패 
		return false;
	}

	//Dest의 각 축을 분리축으로 사용한다.
	//Dest의 x축
	 float CenterProjectionDist = abs(CenterLine.Dot(Dest.Axis[EAxis::X]));

	if (CenterProjectionDist > SrcRadius + Dest.HalfSize.x)
	{
		return false;
	}

	//Dest의 x축
	CenterProjectionDist = abs(CenterLine.Dot(Dest.Axis[EAxis::Y]));

	if (CenterProjectionDist > SrcRadius + Dest.HalfSize.y)
	{
		return false;
	}

	FAABB2D Hit1, Hit2;

	Hit1 = CreateAABB2D(ConvertCenter, SrcRadius);
	Hit2 = CreateAABB2D(Dest);

	FVector3D Min, Max;
	// 비교 ?	참 : 거짓
	// 겹친부분의 최소 좌표
	Min.x = Hit1.Min.x > Hit2.Min.x ? Hit1.Min.x : Hit2.Min.x;
	Min.y = Hit1.Min.y > Hit2.Min.y ? Hit1.Min.y : Hit2.Min.y;

	// 겹친 부분의 최대 좌표 
	Max.x = Hit1.Max.x < Hit2.Max.x ? Hit1.Max.x : Hit2.Max.x;
	Max.y = Hit1.Max.y < Hit2.Max.y ? Hit1.Max.y : Hit2.Max.y;

	HitPoint.x = (Min.x + Max.x) * 0.5f;
	HitPoint.y = (Min.y + Max.y) * 0.5f;

	return true;
}

bool CCollision::CollisionLine2DToLine2D(FVector3D& HitPoint, const FLine2D& Src, const FLine2D& Dest)
{
	//A : Src.Start
	//B : Src.End
	//C : Dest.Start
	//D : Dest.End
	FVector2D A = Src.Start;
	FVector2D B = Src.End;
	FVector2D C = Dest.Start;
	FVector2D D = Dest.End;

	//A B C
	int ccw1 = CCW2D(A, B, C);
	//A B D
	int ccw2 = CCW2D(A, B, D);
	//C D A
	int ccw3 = CCW2D(C, D, A);
	//C D B
	int ccw4 = CCW2D(C, D, B);

	if (ccw1 * ccw2 < 0 && ccw3 * ccw4 <0)
	{
		// 두 직선은 충돌했다.

		//1. 방향 벡터를 계산해준다.
		float V1x = B.x - A.x;
		float V1y = B.y - A.y;

		float V2x = D.x - C.x;
		float V2y = D.y - C.y;

		// 크래머의 공식
		// 2. 두 시작점 A와 C사이의 벡터 R (R = C - A)
		float Rx = C.x - A.x;
		float Ry = C.y - A.y;

		// 3. 행렬식 (D, Determinant) 계산
		// 행렬식이 0이 아니면
		float Determinant = V1x * V2y - V1y * V2x;

		// U를 대체할
		float numerator = Rx * V2y - Ry * V2x;
		float t = numerator / Determinant;

		// 최종 교차점
		HitPoint.x = A.x + V1x * t;
		HitPoint.y = A.y + V1y * t;

		return true;
	}

	//직선 검사 
	// 직선 A B 에 점 C가 있니?
	if (ccw1 == 0 && PointOnLine2D(A, B, C))
	{
		HitPoint.x = C.x;
		HitPoint.y = C.y;
		return true;
	}

	//A B D
	if (ccw2 == 0 && PointOnLine2D(A, B, D))
	{
		HitPoint.x = D.x;
		HitPoint.y = D.y;
		return true;
	}

	//C D A
	if (ccw3 == 0 && PointOnLine2D(C,D,A ))
	{
		HitPoint.x = A.x;
		HitPoint.y = A.y;
		return true;
	}

	//C D B
	if (ccw4 == 0 && PointOnLine2D(C, D, B))
	{
		HitPoint.x = B.x;
		HitPoint.y = B.y;
		return true;
	}

	//충돌X
	return false;
}

bool CCollision::CollisionLine2DToAABB2D(FVector3D& HitPoint, const FLine2D& Src, const FAABB2D& Dest)
{
	// 선을 구성하는 두 점 이 사각형안에 들어가니?
	if (CollisionPointToAABB2D(Src.Start, Dest))
	{
		HitPoint.x = Src.Start.x;
		HitPoint.y = Src.Start.y;
		return true;
	}

	if (CollisionPointToAABB2D(Src.End, Dest))
	{
		HitPoint.x = Src.End.x;
		HitPoint.y = Src.End.y;
		return true;
	}

	//사각형을 구성하는 4개의 변과 선이 충돌하니?
	FLine2D AABBLine[4];

	// 좌하 -> 좌상
	AABBLine[0].Start = Dest.Min;
	AABBLine[0].End.x = Dest.Min.x;
	AABBLine[0].End.y = Dest.Max.y;

	// 좌상 -> 우상
	AABBLine[1].Start.x = Dest.Min.x;
	AABBLine[1].Start.y = Dest.Max.y;
	AABBLine[1].End = Dest.Max;

	// 우상 -> 우하
	AABBLine[2].Start = Dest.Max;
	AABBLine[2].End.x = Dest.Max.x;
	AABBLine[2].End.y = Dest.Min.y;

	// 우하 -> 좌하 
	AABBLine[3].Start.x = Dest.Max.x;
	AABBLine[3].Start.y = Dest.Min.y;
	AABBLine[3].End = Dest.Min;

	bool result = false;
	float resultDist = -1.f;

	FVector2D HitResult;

	for (int i = 0; i< 4; ++i)
	{
		if (CollisionLine2DToLine2D(HitPoint,Src, AABBLine[i]))
		{
			result = true;

			FVector2D v;
			v.x = HitPoint.x;
			v.y = HitPoint.y;

			float Dist = Src.Start.Distance(v);

			if (resultDist == -1.f)
			{
				HitResult = v;
				resultDist = Dist;
			}
			else if (Dist < resultDist)
			{
				HitResult = v;
				resultDist = Dist;
			}
		}
	}

	if (result)
	{
		HitPoint.x = HitResult.x;
		HitPoint.y = HitResult.y;
	}

	return result;
}

bool CCollision::CollisionLine2DToOBB2D(FVector3D& HitPoint, const FLine2D& Src, const FOBB2D& Dest)
{
	// 선을 구성하는 두 점 이 사각형안에 들어가니?
	if (CollisionPointToOBB2D(Src.Start, Dest))
	{
		HitPoint.x = Src.Start.x;
		HitPoint.y = Src.Start.y;
		return true;
	}

	if (CollisionPointToOBB2D(Src.End, Dest))
	{
		HitPoint.x = Src.End.x;
		HitPoint.y = Src.End.y;
		return true;
	}

	//사각형을 구성하는 점을 만들어준다.
	FVector2D Pos[4];

	//좌상 
	Pos[0] = Dest.Center - Dest.Axis[EAxis::X] * Dest.HalfSize.x + 
		Dest.Axis[EAxis::Y] * Dest.HalfSize.y;
	// 좌하 
	Pos[1] = Dest.Center - Dest.Axis[EAxis::X] * Dest.HalfSize.x - 
		Dest.Axis[EAxis::Y] * Dest.HalfSize.y;
	// 우상 
	Pos[2] = Dest.Center + Dest.Axis[EAxis::X] * Dest.HalfSize.x + 
		Dest.Axis[EAxis::Y] * Dest.HalfSize.y;
	// 우하
	Pos[3] = Dest.Center + Dest.Axis[EAxis::X] * Dest.HalfSize.x - 
		Dest.Axis[EAxis::Y] * Dest.HalfSize.y;


	//사각형을 구성하는 4개의 변과 선이 충돌하니?
	FLine2D OBBLine[4];

	// 좌상 : 0 좌하 1 우상 2 우하 3
	// 좌상 -> 우상
	OBBLine[0].Start = Pos[0];
	OBBLine[0].End = Pos[2];

	// 우상 -> 우하 
	OBBLine[1].Start = Pos[2];
	OBBLine[1].End = Pos[3];

	// 우하 -> 좌하 
	OBBLine[2].Start = Pos[3];
	OBBLine[2].End = Pos[1];

	// 좌하 -> 좌상
	OBBLine[3].Start = Pos[1];
	OBBLine[3].End = Pos[0];
	

	bool result = false;
	float resultDist = -1.f;

	FVector2D HitResult;

	for (int i = 0; i < 4; ++i)
	{
		if (CollisionLine2DToLine2D(HitPoint, Src, OBBLine[i]))
		{
			result = true;

			FVector2D v;
			v.x = HitPoint.x;
			v.y = HitPoint.y;

			float Dist = Src.Start.Distance(v);

			if (resultDist == -1.f)
			{
				HitResult = v;
				resultDist = Dist;
			}
			else if (Dist < resultDist)
			{
				HitResult = v;
				resultDist = Dist;
			}
		}
	}

	if (result)
	{
		HitPoint.x = HitResult.x;
		HitPoint.y = HitResult.y;
	}

	return result;
}

bool CCollision::CollisionLine2DToSphere2D(FVector3D& HitPoint, const FLine2D& Src, const FVector3D& Center,
	float Radius)
{
	// 선의 방향 D
	FVector2D Dir = Src.End - Src.Start;


	// 선의 길이 t
	float LineLength = Dir.Length();

	Dir.Normalize();


	//원의 방정식
	FVector2D Center2D;
	Center2D.x = Center.x;
	Center2D.y = Center.y;
	// 직선의 시작지점 - 원의 중심 : M = S - C
	FVector2D M = Src.Start - Center2D;

	// at^2 + bt + c;
	float b = 2.f * M.Dot(Dir);
	float c = M.Dot(M) - Radius * Radius;

	//판별식
	float Det = b * b - 4.f * c;
	// 양수 : 근이 2개		0 : 근이 하나		음수 : 허수
	if (Det < 0.f)
	{
		return false;
	}

	// 근의 공식을 이용해서
	// 충돌 지점을 구해야 한다. 
	//근의 공식 =  (-B +- 루트(B^2 - 4AC)) / 2A
	// 우리는 A가 1이니까
	// (-B +- 루트(B^2 - 4C)) / 2

	//근의 공식 + -값을 각각 구해준다.
	float t1, t2;
	//sqrt 루트 
	float det = sqrtf(Det);


	t1 = (-b + det) / 2.f;
	t2 = (-b - det) / 2.f;

	if (t1 < 0.f && t2 < 0.f)
	{
		return false;
	}

	bool result = false;

	if (t1  > 0.f && t1 < LineLength || 
		t2 > 0.f && t2 < LineLength)
	{
		result = true;
	}

	//시작 지점에서 누가 더 가까운가
	if(result)
	{
						// 1 < 2  
		float HitDist = t1 < t2 ? t1 : t2;

		//충돌하고나서 거리가 음수가 아닌가?
		if (HitDist < 0.f)
		{
			// -0.1  > -0.9 
			HitDist = t1 > t2 ? t1 : t2;
		}

		HitPoint.x = Src.Start.x + Dir.x * HitDist;
		HitPoint.y = Src.Start.y + Dir.y * HitDist;
	}

	return result;
}

bool CCollision::CollisionPointToAABB2D(const FVector2D& Point, const FAABB2D& Info)
{
	if (Point.x < Info.Min.x)
	{
		return false;
	}
	else if (Info.Max.x <  Point.x)
	{
		return false;
	}
	else if (Point.y < Info.Min.y)
	{
		return false;
	}
	else if (Info.Max.y < Point.y)
	{
		return false;
	}

	return true;
}

bool CCollision::CollisionPointToOBB2D(const FVector2D& Point, const FOBB2D& Info)
{
	//점과 사각형의 중심
	FVector2D CenterLine = Point - Info.Center;

	//사각형의 X축 
	float Dist =  abs(CenterLine.Dot(Info.Axis[EAxis::X]));

	if (Dist > Info.HalfSize.x)
	{
		return false;
	}

	// 사각형의 Y축 
	Dist = abs(CenterLine.Dot(Info.Axis[EAxis::Y]));

	if (Dist > Info.HalfSize.y)
	{
		return false;
	}

	return true;
}

bool CCollision::CollisionPointToSphere2D(const FVector2D& Point, const FVector2D& Center, float Radius)
{
	// 원의 중심과 점의 거리를 구하고 
	float Dist = Center.Distance(Point);

	// 그 거리가 반지름 보다 작으면 충돌
	return Dist <= Radius;
}

bool CCollision::CollisionPointToLine2D(const FVector2D& Point, const FLine2D& Info)
{
	//직선의 벡터
	// AB방향벡터를 구하고싶다. -> 
	FVector2D LineDir = Info.End - Info.Start;
	LineDir.Normalize();	//길이를 1로 만들어준다.

	//시작점에서 점으로가는 방향벡터를 만들어준다.
	FVector2D LineDir1 = Point - Info.Start;
	LineDir1.Normalize();

	float Angle = LineDir.Dot(LineDir1);

	if (Angle < 1.f)
	{
		return false;
	}

	//같은 직선상에 있으면 거리에 따라서 충돌 여부를 확인해 줘야한다.
	float Dist = Info.Start.Distance(Info.End);
	float Dist1 = Info.Start.Distance(Point);

	return Dist >= Dist1;
}

//축 검사
//정사영 
bool CCollision::ComputeAxisProjection(const FVector2D& CenterLine, const FVector2D& SeparationAxis, float AxisHalfSize,
	const FVector2D* DestAxis, const FVector2D& DestHalfSize)
{
	// 중심을 연결하는 벡터를 분리축에 투영하여 구간의 길이를 구했다.
	// 길이값이므로 부호가 필요 없어서 절대값으로 사용한다. 
	//abs : 절대값 
	float CenterProjectionDist = abs(CenterLine.Dot(SeparationAxis));

	//
	float DestProjectionDist = 
		abs(SeparationAxis.Dot(DestAxis[EAxis::X]))* DestHalfSize.x + 
		abs(SeparationAxis.Dot(DestAxis[EAxis::Y]))* DestHalfSize.y;

	// 두 점의 거리 > 각 축에 투영된 두 도형의 반길이의 합
	// 충돌 실패
	if (CenterProjectionDist > DestProjectionDist + AxisHalfSize)
	{
		return false;
	}

	return true;
}

FAABB2D CCollision::CreateAABB2D(const FOBB2D& Info)
{
	FAABB2D result;

	FVector2D Pos[4];
	//좌상
	Pos[0] = Info.Center - Info.Axis[0] * Info.HalfSize.x + Info.Axis[1] * Info.HalfSize.y;
	//좌하
	Pos[1] = Info.Center - Info.Axis[0] * Info.HalfSize.x - Info.Axis[1] * Info.HalfSize.y;
	//우상
	Pos[2] = Info.Center + Info.Axis[0] * Info.HalfSize.x + Info.Axis[1] * Info.HalfSize.y;
	//우하
	Pos[3] = Info.Center + Info.Axis[0] * Info.HalfSize.x - Info.Axis[1] * Info.HalfSize.y;

	//4개의 꼭지점에서 가장 작은 X,Y 값을 Min, 가장 큰 X, Y값을 Max로 넣으면 된다. 

	//초기화 
	result.Min.x = Pos[0].x;
	result.Min.y = Pos[0].y;

	result.Max.x = Pos[0].x;
	result.Max.y = Pos[0].y;

	for (int i = 0; i < 4; ++i)
	{
		result.Min.x = result.Min.x > Pos[i].x ? Pos[i].x : result.Min.x;
		result.Min.y = result.Min.y > Pos[i].y ? Pos[i].y : result.Min.y;

		result.Max.x = result.Max.x < Pos[i].x ? Pos[i].x : result.Max.x;
		result.Max.y = result.Max.y < Pos[i].y ? Pos[i].y : result.Max.y;
	}

	return result;
}

FAABB2D CCollision::CreateAABB2D(const FVector2D& Center, float Radius)
{
	FAABB2D result;

	result.Min = Center - Radius;
	result.Max = Center + Radius;

	return result;
}

FOBB2D CCollision::CreateOBB2D(const FAABB2D& Info)
{
	FOBB2D result;

	result.Center = (Info.Min + Info.Max) * 0.5;

	result.Axis[EAxis::X].x = 1.f;
	result.Axis[EAxis::X].y = 0.f;

	result.Axis[EAxis::Y].x = 0.f;
	result.Axis[EAxis::Y].y = 1.f;

	result.HalfSize = (Info.Max - Info.Min) * 0.5;

	return result;
}

ECCWResult::Type CCollision::CCW2D(const FVector2D& P1, const FVector2D& P2, const FVector2D& P3)
{
	//CCW(Counter ClockWise) 알고리즘
	// 점 3개가 이루는 방향을 계산하는 알고리즘
	//CCW(A, B, C) = (Bx - Ax) * (Cy - Ay) - (By - Ay) * (Cx - Ax)
	//CCW(P1, P2, P3) = (P2x - P1x) * (P3y - P1y) - (P2y - P1y) * (P3x - P1x)

	//CCW 는 외적이다 
	float Cross = (P2.x - P1.x) * (P3.y - P1.y) - (P2.y - P1.y) * (P3.x - P1.x);

	//음수
	if (Cross < 0)
	{
		return ECCWResult::CW;
	}
	//양수
	else if (Cross > 0)
	{
		return ECCWResult::CCW;
	}
	//직선
	return ECCWResult::Line;
}

bool CCollision::PointOnLine2D(const FVector2D& LineStart, const FVector2D& LineEnd, const FVector2D& Point)
{
	float MinX, MinY, MaxX, MaxY;

	if (LineStart.x < LineEnd.x)
	{
		MinX = LineStart.x;
		MaxX = LineEnd.x;
	}
	else
	{
		MinX = LineEnd.x;
		MaxX = LineStart.x;
	}

	if (LineStart.y < LineEnd.y)
	{
		MinY = LineStart.y;
		MaxY = LineEnd.y;
	}
	else
	{
		MinY = LineEnd.y;
		MaxY = LineStart.y;
	}

	if (Point.x < MinX)
	{
		return false;
	}
	else if (MaxX < Point.x)
	{
		return false;
	}
	else if (Point.y < MinY)
	{
		return false;
	}
	else if (MaxY < Point.y)
	{
		return false;
	}

	return true;
}
