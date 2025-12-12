
#include "Vector3D.h"
#include "Matrix.h"

FVector3D FVector3D::Zero = { 0.f, 0.f, 0.f };
FVector3D FVector3D::One = { 1.f, 1.f, 1.f };
FVector3D FVector3D::Axis[EAxis::End] =
{
	{1.f, 0.f, 0.f},
	{0.f, 1.f, 0.f},
	{0.f, 0.f, 1.f}
};


#pragma region Construction

FVector3D::FVector3D()
{
}

FVector3D::FVector3D(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

FVector3D::FVector3D(const FVector3D& v) :
	x(v.x),
	y(v.y),
	z(v.z)
{
}

FVector3D::FVector3D(FVector3D&& v) :
	x(v.x),
	y(v.y),
	z(v.z)
{
}

FVector3D::FVector3D(const DirectX::XMVECTOR& v)
{
	// XMVECTOR를 XMFLOAT3로 만들어준다.
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

#pragma endregion Construction

#pragma region Equal

const FVector3D& FVector3D::operator = (const FVector3D& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

const FVector3D& FVector3D::operator = (const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
	return *this;
}

const FVector3D& FVector3D::operator = (float Value)
{
	x = Value;
	y = Value;
	z = Value;
	return *this;
}

const FVector3D& FVector3D::operator = (double Value)
{
	x = (float)Value;
	y = (float)Value;
	z = (float)Value;
	return *this;
}

const FVector3D& FVector3D::operator = (int Value)
{
	x = (float)Value;
	y = (float)Value;
	z = (float)Value;
	return *this;
}

#pragma endregion Equal

#pragma region Plus
// +
FVector3D FVector3D::operator + (const FVector3D& v)	const
{
	FVector3D	result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	return result;
}

FVector3D FVector3D::operator + (float Value)	const
{
	FVector3D	result;
	result.x = x + Value;
	result.y = y + Value;
	result.z = z + Value;
	return result;
}

FVector3D FVector3D::operator + (double Value)	const
{
	FVector3D	result;
	result.x = x + (float)Value;
	result.y = y + (float)Value;
	result.z = z + (float)Value;
	return result;
}

FVector3D FVector3D::operator + (int Value)	const
{
	FVector3D	result;
	result.x = x + (float)Value;
	result.y = y + (float)Value;
	result.z = z + (float)Value;
	return result;
}

// +=
const FVector3D& FVector3D::operator += (const FVector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

const FVector3D& FVector3D::operator += (float Value)
{
	x += Value;
	y += Value;
	z += Value;
	return *this;
}

const FVector3D& FVector3D::operator += (double Value)
{
	x += (float)Value;
	y += (float)Value;
	z += (float)Value;
	return *this;
}

const FVector3D& FVector3D::operator += (int Value)
{
	x += (float)Value;
	y += (float)Value;
	z += (float)Value;
	return *this;
}

// ++
const FVector3D& FVector3D::operator ++ ()
{
	x += 1.f;
	y += 1.f;
	z += 1.f;
	return *this;
}

const FVector3D& FVector3D::operator ++ (int)
{
	x += 1.f;
	y += 1.f;
	z += 1.f;
	return *this;
}

#pragma endregion Plus

#pragma region Minus
// -
FVector3D FVector3D::operator - (const FVector3D& v)	const
{
	FVector3D	result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	return result;
}

FVector3D FVector3D::operator - (float Value)	const
{
	FVector3D	result;
	result.x = x - Value;
	result.y = y - Value;
	result.z = z - Value;
	return result;
}

FVector3D FVector3D::operator - (double Value)	const
{
	FVector3D	result;
	result.x = x - (float)Value;
	result.y = y - (float)Value;
	result.z = z - (float)Value;
	return result;
}

FVector3D FVector3D::operator - (int Value)	const
{
	FVector3D	result;
	result.x = x - (float)Value;
	result.y = y - (float)Value;
	result.z = z - (float)Value;
	return result;
}

// -=
const FVector3D& FVector3D::operator -= (const FVector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

const FVector3D& FVector3D::operator -= (float Value)
{
	x -= Value;
	y -= Value;
	z -= Value;
	return *this;
}

const FVector3D& FVector3D::operator -= (double Value)
{
	x -= (float)Value;
	y -= (float)Value;
	z -= (float)Value;
	return *this;
}

const FVector3D& FVector3D::operator -= (int Value)
{
	x -= (float)Value;
	y -= (float)Value;
	z -= (float)Value;
	return *this;
}

// --
const FVector3D& FVector3D::operator -- ()
{
	x -= 1.f;
	y -= 1.f;
	z -= 1.f;
	return *this;
}

const FVector3D& FVector3D::operator -- (int)
{
	x -= 1.f;
	y -= 1.f;
	z -= 1.f;
	return *this;
}

#pragma endregion Minus

#pragma region Multiply

// *
FVector3D FVector3D::operator * (const FVector3D& v)	const
{
	FVector3D	result;
	result.x = x * v.x;
	result.y = y * v.y;
	result.z = z * v.z;
	return result;
}

FVector3D FVector3D::operator * (float Value)	const
{
	FVector3D	result;
	result.x = x * Value;
	result.y = y * Value;
	result.z = z * Value;
	return result;
}

FVector3D FVector3D::operator * (double Value)	const
{
	FVector3D	result;
	result.x = x * (float)Value;
	result.y = y * (float)Value;
	result.z = z * (float)Value;
	return result;
}

FVector3D FVector3D::operator * (int Value)	const
{
	FVector3D	result;
	result.x = x * (float)Value;
	result.y = y * (float)Value;
	result.z = z * (float)Value;
	return result;
}

// *=
const FVector3D& FVector3D::operator *= (const FVector3D& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

const FVector3D& FVector3D::operator *= (float Value)
{
	x *= Value;
	y *= Value;
	z *= Value;
	return *this;
}

const FVector3D& FVector3D::operator *= (double Value)
{
	x *= (float)Value;
	y *= (float)Value;
	z *= (float)Value;
	return *this;
}

const FVector3D& FVector3D::operator *= (int Value)
{
	x *= (float)Value;
	y *= (float)Value;
	z *= (float)Value;
	return *this;
}

#pragma endregion Multiply

#pragma region Divide

// /
FVector3D FVector3D::operator / (const FVector3D& v)	const
{
	FVector3D	result;
	result.x = x / v.x;
	result.y = y / v.y;
	result.z = z / v.z;
	return result;
}

FVector3D FVector3D::operator / (float Value)	const
{
	FVector3D	result;
	result.x = x / Value;
	result.y = y / Value;
	result.z = z / Value;
	return result;
}

FVector3D FVector3D::operator / (double Value)	const
{
	FVector3D	result;
	result.x = x / (float)Value;
	result.y = y / (float)Value;
	result.z = z / (float)Value;
	return result;
}

FVector3D FVector3D::operator / (int Value)	const
{
	FVector3D	result;
	result.x = x / (float)Value;
	result.y = y / (float)Value;
	result.z = z / (float)Value;
	return result;
}

// /=
const FVector3D& FVector3D::operator /= (const FVector3D& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

const FVector3D& FVector3D::operator /= (float Value)
{
	x /= Value;
	y /= Value;
	z /= Value;
	return *this;
}

const FVector3D& FVector3D::operator /= (double Value)
{
	x /= (float)Value;
	y /= (float)Value;
	z /= (float)Value;
	return *this;
}

const FVector3D& FVector3D::operator /= (int Value)
{
	x /= (float)Value;
	y /= (float)Value;
	z /= (float)Value;
	return *this;
}

#pragma endregion Divide

#pragma region Function

float FVector3D::Length()	const
{
	// sqrtf : 루트값 구해주는 기능.
	return sqrtf(x * x + y * y + z * z);
}

void FVector3D::Normalize()
{
	float	Size = Length();

	if (Size == 0.f)
		return;

	x /= Size;
	y /= Size;
	z /= Size;
}


FVector3D FVector3D::Normalize(const FVector3D& v)
{
	FVector3D	result;
	float	Size = v.Length();

	if (Size == 0.f)
		return result;

	result.x = v.x / Size;
	result.y = v.y / Size;
	result.z = v.z / Size;
	return result;
}

float FVector3D::Dot(const FVector3D& v)	const
{
	return x * v.x + y * v.y + z * v.z;
}

FVector3D FVector3D::Cross(const FVector3D& v)	const
{
	FVector3D	result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;
	return result;
}

float FVector3D::Distance(const FVector3D& v)	const
{
	FVector3D v1 = *this - v;

	return v1.Length();
}

DirectX::XMVECTOR FVector3D::Convert()	const
{
	return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

FVector3D FVector3D::TransformNormal(FMatrix& mat)	const
{
	// XMVector3TransformNormal : w를 0으로 하여 4x4 행렬과 곱하고 그 결과를
	// 반환해준다.
	DirectX::XMVECTOR result = DirectX::XMVector3TransformNormal(Convert(),
		mat.m);

	return result;
}

FVector3D FVector3D::TransformCoord(FMatrix& mat)	const
{
	// XMVector3TransformCoord : w를 1로 하여 4x4 행렬과 곱하고 그 결과를
	// 반환해준다.
	DirectX::XMVECTOR result = DirectX::XMVector3TransformCoord(Convert(),
		mat.m);

	return result;
}

FVector3D FVector3D::GetRotation(const FVector3D& Rot)	const
{
	FMatrix	matRot;

	matRot.Rotation(Rot);

	return TransformNormal(matRot);
}

float FVector3D::GetAngle(const FVector3D& v) const
{
	FVector3D v1 = *this;
	FVector3D v2 = v;

	v1.Normalize();
	v2.Normalize();
	
	float Angle = v1.Dot(v2);
	
	Angle = DirectX::XMConvertToDegrees(acosf(Angle));
	return Angle;
}

float FVector3D::GetViewTargetAngle(const FVector3D& v, EAxis::Type AxisType) const
{
	FVector3D v1 = Axis[AxisType];	// 비교할 축 벡터 
	FVector3D v2 = v - *this;	// 상대를 바라보는 방향벡터 

	v1.Normalize();
	v2.Normalize();

	float Angle = v1.Dot(v2);

	Angle = DirectX::XMConvertToDegrees(acosf(Angle));

	if (v.x > x)
	{
		Angle = 360.f - Angle;
	}
	return Angle;
}

float FVector3D::GetAngle(const FVector3D& v1, const FVector3D& v2)
{
	FVector3D v3 = v1;
	FVector3D v4 = v2;

	v3.Normalize();
	v4.Normalize();

	float Angle = v3.Dot(v4);

	Angle = DirectX::XMConvertToDegrees(acosf(Angle));
	return Angle;
}

#pragma endregion Function