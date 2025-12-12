#pragma once

#include "Gameinfo.h"

namespace ECCWResult
{
	enum Type
	{
		CW = -1,	//-1
		Line,		// 0
		CCW			// 1
	};
}


class CCollision
{
public:
	//호출
	static bool CollisionAABB2DToAABB2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderAABB2D* Dest);

	static bool CollisionSphere2DToSphere2D(FVector3D& HitPoint,
		class CColliderSphere2D* Src,
		class CColliderSphere2D* Dest);

	//AABB2D to Sphere2D

	static bool CollisionAABB2DToSphere2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderSphere2D* Dest);

	//OBB2D to OBB2D
	static bool CollisionOBB2DToOBB2D(FVector3D& HitPoint,
		class CColliderOBB2D* Src,
		class CColliderOBB2D* Dest);

	// AABB2D to OBB2D
	static bool CollisionAABB2DToOBB2D(FVector3D& HitPoint,
		class CColliderAABB2D* Src,
		class CColliderOBB2D* Dest);

	// Sphere2D to OBB2D
	static bool CollisionSphere2DToOBB2D(FVector3D& HitPoint,
		class CColliderSphere2D* Src,
		class CColliderOBB2D* Dest);

	// Line2D to Line2D
	static bool CollisionLine2DToLine2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderLine2D* Dest);

	// Line2D to AABB2D
	static bool CollisionLine2DToAABB2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderAABB2D* Dest);

	// Line2D to OBB2D
	static bool CollisionLine2DToOBB2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderOBB2D* Dest);

	// Line2D to Sphere2D
	static bool CollisionLine2DToSphere2D(FVector3D& HitPoint,
		class CColliderLine2D* Src,
		class CColliderSphere2D* Dest);

	//연산
	static bool CollisionAABB2DToAABB2D(FVector3D& HitPoint, const FAABB2D& Src, const FAABB2D& Dest);

	static bool CollisionSphere2DToSphere2D(FVector3D& HitPoint,
		const FVector3D& SrcCenter, float SrcRadius,
		const FVector3D& DestCenter, float DestRadius);

	static bool CollisionAABB2DToSphere2D(FVector3D& HitPoint,
		const FAABB2D& Src,
		const FVector3D& DestCenter, float DestRadius);

	static bool CollisionOBB2DToOBB2D(FVector3D& HitPoint,
		const FOBB2D& Src,
		const FOBB2D& Dest );

	static bool CollisionAABB2DToOBB2D(FVector3D& HitPoint,
		const FAABB2D& Src,
		const FOBB2D& Dest);

	static bool CollisionSphere2DToOBB2D(FVector3D& HitPoint,
		const FVector3D& SrcCenter, float SrcRadius,
		const FOBB2D& Dest);


	//라인
	static bool CollisionLine2DToLine2D(FVector3D& HitPoint, 
		const FLine2D& Src, 
		const FLine2D& Dest);

	static bool CollisionLine2DToAABB2D(FVector3D& HitPoint, 
		const FLine2D& Src, 
		const FAABB2D& Dest);

	static bool CollisionLine2DToOBB2D(FVector3D& HitPoint, 
		const FLine2D& Src, 
		const FOBB2D& Dest);

	static bool CollisionLine2DToSphere2D(FVector3D& HitPoint, 
		const FLine2D& Src, 
		const FVector3D& Center, float Radius);

	//점 충돌
	static bool CollisionPointToAABB2D(const FVector2D& Point, const FAABB2D& Info);
	static bool CollisionPointToOBB2D(const FVector2D& Point, const FOBB2D& Info);
	static bool CollisionPointToSphere2D(const FVector2D& Point, const FVector2D& Center, float Radius);
	static bool CollisionPointToLine2D(const FVector2D& Point, const FLine2D& Info);


private:
	// 1. 두 중심의 축 2. 분리축 3. Src 크기의 반  4. Dest축 5. Dest 크기 
	static bool ComputeAxisProjection(const FVector2D& CenterLine,
		const FVector2D& SeparationAxis, float AxisHalfSize, 
		const FVector2D* DestAxis, const FVector2D& DestHalfSize);

	//OBB 정보를 가지고 충돌용 AABB 사각형을 만들어주는 함수 
	static FAABB2D CreateAABB2D(const FOBB2D& Info);
	static FAABB2D CreateAABB2D(const FVector2D& Center, float Radius);
	static FOBB2D CreateOBB2D(const FAABB2D& Info);

	//라인 검사용 CCW
	// 점 3개를 넣어주고 점 3개에 대해 CCW 알고리즘을 실행시켜준다.
	// 시계방향 CW -1 직선 Line 0 반시계방향 CCW 1
	static ECCWResult::Type CCW2D(const FVector2D& P1, const FVector2D& P2, const FVector2D& P3);
	static bool PointOnLine2D(const FVector2D& LineStart,
		const FVector2D& LineEnd,
		const FVector2D& Point);
};

