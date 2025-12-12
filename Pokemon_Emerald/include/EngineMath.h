#pragma once

// DX 수학기능을 제공하고요.
// SIMD를 이용한 16바이트 연산을 제공한다. 
#include <DirectXMath.h>

//DX의 벡터 자료형이다. 
//DirectX::XMVECTOR a; // __m128 16바이트 처리를 한다고 명시적으로 달아둔것이다. 
// 프로젝트 셋팅에 따라서 크기가 달라져요.
// 64비트면 우리가 사용할 16바이트 
// 32비트면 16바이트 정렬이 기본이 아니다 
// 16바이트 연산할때는 사용하면 편하기 때문에 

#include <DirectXPackedVector.h>

namespace EAxis
{
	enum Type
	{
		None = -1,
		X,
		Y,
		Z,
		End
	};
}