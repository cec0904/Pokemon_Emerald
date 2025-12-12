#pragma once

/*
	공용체
	메모리를 공유하는 문법이다.
*/
//union Test
//{
//    int num;
//    float fnum;
//};

/*
	공간 변환을 처리할때는 행렬을 사용합니다.
	4X4 행렬을 사용합니다.

	다이렉트 X에서는 그리고 16바이트 연산을 해야한다. 그이유는
	SIMD 명령어 때문이다.
	SIMD 명령어는 한번에 16바이트 단위로 연산을 처리한다.
	그래서 Vector4D 처럼 4개의 float를 가지는 벡터를 사용한다.

	SIMD 란? 하나의 명령어로 여러 데이터를 병렬로 처리하는 기술이다.

	이를 DX에서 제공해주고 있다.  DXMath 라는 라이브러리에서 제공해준다.
*/

#include "Vector4D.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include <string>


//__declspec(align(16))  해줘야 DX 지원하는거 사용가능 안해주면 행렬연산 할때 크레쉬나온다. 
__declspec(align(16)) union FMatrix
{
	//XM_ALIGNED_STRUCT(16) XMMATRIX
	// __declspec(align(x)) 메모리 정렬을 지정하는 키워드입니다.  
	DirectX::XMMATRIX m;	//사용하기 까다롭기 때문에 랩핑을 해줄 공용체를 만든거다. 

	// 이름을 안붙인 이유는
	// 공용체이기 때문에 안붙여도 된다. 
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	FVector4D v[4];

	FMatrix()
	{
		//XMMatrixIdentity == 단위 행렬
		m = DirectX::XMMatrixIdentity();
	};
	FMatrix(const FMatrix& _m)
	{
		m = _m.m;
	}
	FMatrix(FMatrix&& _m)
	{
		m = _m.m;
	}
	FMatrix(const DirectX::XMMATRIX& _m)
	{
		m = _m;
	}
	FMatrix(const FVector4D _v[4])
	{
		memcpy(v, _v, sizeof(FVector4D) * 4);
	}

	//연사자
	//접근
	FVector4D& operator [] (int Index)
	{
		assert(0 <= Index && Index < 4);

		return v[Index];
	}

	//대입
	const FMatrix& operator = (const FMatrix& _m)
	{
		m = _m.m;
		return *this;
	}

	const FMatrix& operator = (const DirectX::XMMATRIX& _m)
	{
		m = _m;
		return *this;
	}

	const FMatrix& operator = (const FVector4D _v[4])
	{
		memcpy(v, _v, sizeof(FVector4D) * 4);
		return *this;
	}

	//곱셈
	// XMMATRIX 에서 행렬 곱을 해줄것이다. 
	FMatrix operator * (const FMatrix& _m)	const
	{
		return FMatrix(m * _m.m);
	}

	FMatrix operator * (const DirectX::XMMATRIX& _m)	const
	{
		return FMatrix(m * _m);
	}

	// Function
	// 단위 행렬로 만들어주는 기능
	void Identity()
	{
		m = DirectX::XMMatrixIdentity();
	}

	// 전치 행렬로 만들어주는 기능
	void Transpose()
	{
		m = DirectX::XMMatrixTranspose(m);
	}

	// 역행렬로 만들어주는 기능
	void Inverse()
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(m);	// 행렬식 구해주기 
		m = DirectX::XMMatrixInverse(&det, m);
	}

	//월드 행렬을 만들때 사용하는 것들 
	//월드 행렬 설명 
	//크기 행렬 
	void Scaling(const FVector3D& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	void Scaling(float x, float y, float z)
	{
		m = DirectX::XMMatrixScaling(x, y, z);
	}

	void Scaling(const FVector2D& _v)
	{
		m = DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	void Scaling(float x, float y)
	{
		m = DirectX::XMMatrixScaling(x, y, 1.f);
	}

	//회전행렬 
	// 라디안 : 호도법 : 180도 = 파이
	// 디그리 : 각도
	// PI = 180도를 의미한다.
	//	디그리 = 라디안 * 180 / PI
	//	라디안 = 각도 * PI / 180


	void Rotation(const FVector3D& _v)
	{
		float x = DirectX::XMConvertToRadians(_v.x);
		float y = DirectX::XMConvertToRadians(_v.y);
		float z = DirectX::XMConvertToRadians(_v.z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		// roll : x축 회전
		// yaw : y축 회전
		// pitch : z축 회전
		DirectX::XMVECTOR Quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		m = DirectX::XMMatrixRotationQuaternion(Quat);

		//다 따로 구해서 곱하기도 가능하다. 
		//DirectX::XMMatrixRotationX
	}

	void Rotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		m = DirectX::XMMatrixRotationQuaternion(Quat);
	}

	void RotationX(float _x)
	{
		float x = DirectX::XMConvertToRadians(_x);

		m = DirectX::XMMatrixRotationX(x);
	}

	void RotationY(float _y)
	{
		float y = DirectX::XMConvertToRadians(_y);

		m = DirectX::XMMatrixRotationY(y);
	}

	void RotationZ(float _z)
	{
		float z = DirectX::XMConvertToRadians(_z);

		m = DirectX::XMMatrixRotationZ(z);
	}

	//축 회전
	void RotationAxis(const FVector3D& Axis, float Angle)
	{
		float angle = DirectX::XMConvertToRadians(Angle);

		DirectX::XMVECTOR	_Axis = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		m = DirectX::XMMatrixRotationAxis(_Axis, Angle);
	}

	// 이동 행렬
	void Translation(const FVector3D& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	void Translation(float x, float y, float z)
	{
		m = DirectX::XMMatrixTranslation(x, y, z);
	}

	void Translation(const FVector2D& _v)
	{
		m = DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	void Translation(float x, float y)
	{
		m = DirectX::XMMatrixTranslation(x, y, 0.f);
	}

	//전역함수 
	static FMatrix StaticIdentity()
	{
		return DirectX::XMMatrixIdentity();
	}
	//전치
	static FMatrix StaticTranspose(const FMatrix& _m)
	{
		return DirectX::XMMatrixTranspose(_m.m);
	}
	//역
	static FMatrix StaticInverse(const FMatrix& _m)
	{
		DirectX::XMVECTOR	det = DirectX::XMMatrixDeterminant(_m.m);
		return DirectX::XMMatrixInverse(&det, _m.m);
	}

	//전역 월드 
	static FMatrix StaticScaling(const FVector3D& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticScaling(float x, float y, float z)
	{
		return DirectX::XMMatrixScaling(x, y, z);
	}

	static FMatrix StaticScaling(const FVector2D& _v)
	{
		return DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
	}

	static FMatrix StaticScaling(float x, float y)
	{
		return DirectX::XMMatrixScaling(x, y, 1.f);
	}

	static FMatrix StaticRotation(const FVector3D& _v)
	{
		float x = DirectX::XMConvertToRadians(_v.x);
		float y = DirectX::XMConvertToRadians(_v.y);
		float z = DirectX::XMConvertToRadians(_v.z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		return DirectX::XMMatrixRotationQuaternion(Quat);

		//DirectX::XMMatrixRotationX
	}

	static FMatrix StaticRotation(float _x, float _y, float _z)
	{
		float x = DirectX::XMConvertToRadians(_x);
		float y = DirectX::XMConvertToRadians(_y);
		float z = DirectX::XMConvertToRadians(_z);

		// x, y, z 회전값을 이용하여 사원수를 구한다.
		DirectX::XMVECTOR Quat =
			DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);

		// 위에서 구해준 사원수를 이용하여 회전행렬을
		// 만들어준다.
		return DirectX::XMMatrixRotationQuaternion(Quat);
	}

	static FMatrix StaticRotationX(float _x)
	{
		float x = DirectX::XMConvertToRadians(_x);

		return DirectX::XMMatrixRotationX(x);
	}

	static FMatrix StaticRotationY(float _y)
	{
		float y = DirectX::XMConvertToRadians(_y);

		return DirectX::XMMatrixRotationY(y);
	}

	static FMatrix StaticRotationZ(float _z)
	{
		float z = DirectX::XMConvertToRadians(_z);

		return DirectX::XMMatrixRotationZ(z);
	}

	static FMatrix StaticRotationAxis(const FVector3D& Axis, float _Angle)
	{
		float Angle = DirectX::XMConvertToRadians(_Angle);

		DirectX::XMVECTOR	_Axis =
			DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&Axis);

		return DirectX::XMMatrixRotationAxis(_Axis, Angle);
	}

	static FMatrix StaticTranslation(const FVector3D& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
	}

	static FMatrix StaticTranslation(float x, float y, float z)
	{
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	static FMatrix StaticTranslation(const FVector2D& _v)
	{
		return DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
	}

	static FMatrix StaticTranslation(float x, float y)
	{
		return DirectX::XMMatrixTranslation(x, y, 0.f);
	}


};


/*
4x4 * 4x4

1x4       4x4
1 2 3 4 * 1 2 3 4 = 1 + 10 + 27 + 12 = 50  = 50, 30, 40, 78
		  5 6 7 8   2 + 12 + 0 + 16 = 30
		  9 0 1 2   3 + 14 + 3 + 20 = 40
		  3 4 5 6   16 + 32 + 6 + 24 = 78

1 2 3 4	  1 2 3 4   50, 30, 40, 78
5 6 7 8 * 5 6 7 8 = 122,
9 0 1 2	  9 0 1 2
3 4 5 6	  3 4 5 6

1 0 0 0	  1 2 3 4   1 2 3 4
0 1 0 0 * 5 6 7 8 = 5 6 7 8
0 0 1 0	  9 0 1 2
0 0 0 1	  3 4 5 6

1 2 3 4   1 0 0 0   1 2 3 4
5 6 7 8 * 0 1 0 0 =
9 0 1 2	  0 0 1 0
3 4 5 6	  0 0 0 1

A 행렬에 A행렬의 역행렬을 곱하면 항등(단위)행렬이 나온다.

전치행렬 : 행과 열을 바꿔준다.
1 2 3 4
5 6 7 8
9 0 1 2
3 4 5 6

1 5 9 3
2 6 0 4
3 7 1 5
4 8 2 6

월드행렬 구성요소
크기, 자전, 위치(이동), 공전, 부모
월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬
크자이공부


//크기행렬
x 0 0 0
0 y 0 0
0 0 z 0
0 0 0 1

1, 1, 0, 1 * 100 0   0   0 = 100, 100, 0
			 0   100 0   0
			 0   0   100 0
			 0   0   0   1

-1, 1, 0, 1 * 100 0   0   0 = -100, 100, 0
			  0   100 0   0
			  0   0   100 0
			  0   0   0   1


//회전행렬
//오일러회전
cos -sin
sin cos

X축 회전
1 0   0    0
0 cos -sin 0
0 sin cos  0
0 0   0    1

Y축 회전
cos  0 sin 0
0    1 0   0
-sin 0 cos 0
0    0 0   1

Z축 회전
cos -sin 0 0
sin cos  0 0
0   0    1 0
0   0    0 1

최종 회전행렬 = X축회전행렬 * Y축회전행렬 * Z축회전행렬

100, 100, 0, 1 * 1 0   0    0 = 100, 0
				 0 cos -sin 0
				 0 sin cos  0
				 0 0   0    1


이동행렬
1 0 0 0
0 1 0 0
0 0 1 0
x y z 1

1, 1, 0, 1 * 1   0   0   0 = 201, 201, 0
			 0   1   0   0
			 0   0   1   0
			 200 200 0   1


PI = 180도를 의미한다.
라디안 * 180 / PI
각도 * PI / 180



*/