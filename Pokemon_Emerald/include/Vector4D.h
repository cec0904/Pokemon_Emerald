#pragma once

#include <math.h>
#include "EngineMath.h"

struct FVector4D
{
	float	x = 0.f;
	float	y = 0.f;
	float	z = 0.f;
	float	w = 0.f;

#pragma region Construction

	FVector4D()
	{
	}

	FVector4D(float _x, float _y, float _z, float _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{
	}

	FVector4D(const FVector4D& v) :
		x(v.x),
		y(v.y),
		z(v.z),
		w(v.w)
	{
	}

	FVector4D(FVector4D&& v) :
		x(v.x),
		y(v.y),
		z(v.z),
		w(v.w)
	{
	}

#pragma endregion Construction

#pragma region Equal

	const FVector4D& operator = (const FVector4D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const FVector4D& operator = (float Value)
	{
		x = Value;
		y = Value;
		z = Value;
		w = Value;
		return *this;
	}

	const FVector4D& operator = (double Value)
	{
		x = (float)Value;
		y = (float)Value;
		z = (float)Value;
		w = (float)Value;
		return *this;
	}

	const FVector4D& operator = (int Value)
	{
		x = (float)Value;
		y = (float)Value;
		z = (float)Value;
		w = (float)Value;
		return *this;
	}

#pragma endregion Equal

#pragma region Plus
	// +
	FVector4D operator + (const FVector4D& v)	const
	{
		FVector4D	result;
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;
		result.w = w + v.w;
		return result;
	}

	FVector4D operator + (float Value)	const
	{
		FVector4D	result;
		result.x = x + Value;
		result.y = y + Value;
		result.z = z + Value;
		result.w = w + Value;
		return result;
	}

	FVector4D operator + (double Value)	const
	{
		FVector4D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		result.z = z + (float)Value;
		result.w = w + (float)Value;
		return result;
	}

	FVector4D operator + (int Value)	const
	{
		FVector4D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		result.z = z + (float)Value;
		result.w = w + (float)Value;
		return result;
	}

	// +=
	const FVector4D& operator += (const FVector4D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	const FVector4D& operator += (float Value)
	{
		x += Value;
		y += Value;
		z += Value;
		w += Value;
		return *this;
	}

	const FVector4D& operator += (double Value)
	{
		x += (float)Value;
		y += (float)Value;
		z += (float)Value;
		w += (float)Value;
		return *this;
	}

	const FVector4D& operator += (int Value)
	{
		x += (float)Value;
		y += (float)Value;
		z += (float)Value;
		w += (float)Value;
		return *this;
	}

	// ++
	const FVector4D& operator ++ ()
	{
		x += 1.f;
		y += 1.f;
		z += 1.f;
		w += 1.f;
		return *this;
	}

	const FVector4D& operator ++ (int)
	{
		x += 1.f;
		y += 1.f;
		z += 1.f;
		w += 1.f;
		return *this;
	}

#pragma endregion Plus

#pragma region Minus
	// -
	FVector4D operator - (const FVector4D& v)	const
	{
		FVector4D	result;
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
		result.w = w - v.w;
		return result;
	}

	FVector4D operator - (float Value)	const
	{
		FVector4D	result;
		result.x = x - Value;
		result.y = y - Value;
		result.z = z - Value;
		result.w = w - Value;
		return result;
	}

	FVector4D operator - (double Value)	const
	{
		FVector4D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		result.z = z - (float)Value;
		result.w = w - (float)Value;
		return result;
	}

	FVector4D operator - (int Value)	const
	{
		FVector4D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		result.z = z - (float)Value;
		result.w = w - (float)Value;
		return result;
	}

	// -=
	const FVector4D& operator -= (const FVector4D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	const FVector4D& operator -= (float Value)
	{
		x -= Value;
		y -= Value;
		z -= Value;
		w -= Value;
		return *this;
	}

	const FVector4D& operator -= (double Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		z -= (float)Value;
		w -= (float)Value;
		return *this;
	}

	const FVector4D& operator -= (int Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		z -= (float)Value;
		w -= (float)Value;
		return *this;
	}

	// --
	const FVector4D& operator -- ()
	{
		x -= 1.f;
		y -= 1.f;
		z -= 1.f;
		w -= 1.f;
		return *this;
	}

	const FVector4D& operator -- (int)
	{
		x -= 1.f;
		y -= 1.f;
		z -= 1.f;
		w -= 1.f;
		return *this;
	}

#pragma endregion Minus

#pragma region Multiply

	// *
	FVector4D operator * (const FVector4D& v)	const
	{
		FVector4D	result;
		result.x = x * v.x;
		result.y = y * v.y;
		result.z = z * v.z;
		result.w = w * v.w;
		return result;
	}

	FVector4D operator * (float Value)	const
	{
		FVector4D	result;
		result.x = x * Value;
		result.y = y * Value;
		result.z = z * Value;
		result.w = w * Value;
		return result;
	}

	FVector4D operator * (double Value)	const
	{
		FVector4D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		result.z = z * (float)Value;
		result.w = w * (float)Value;
		return result;
	}

	FVector4D operator * (int Value)	const
	{
		FVector4D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		result.z = z * (float)Value;
		result.w = w * (float)Value;
		return result;
	}

	// *=
	const FVector4D& operator *= (const FVector4D& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	const FVector4D& operator *= (float Value)
	{
		x *= Value;
		y *= Value;
		z *= Value;
		w *= Value;
		return *this;
	}

	const FVector4D& operator *= (double Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		z *= (float)Value;
		w *= (float)Value;
		return *this;
	}

	const FVector4D& operator *= (int Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		z *= (float)Value;
		w *= (float)Value;
		return *this;
	}

#pragma endregion Multiply

#pragma region Divide

	// /
	FVector4D operator / (const FVector4D& v)	const
	{
		FVector4D	result;
		result.x = x / v.x;
		result.y = y / v.y;
		result.z = z / v.z;
		result.w = w / v.w;
		return result;
	}

	FVector4D operator / (float Value)	const
	{
		FVector4D	result;
		result.x = x / Value;
		result.y = y / Value;
		result.z = z / Value;
		result.w = w / Value;
		return result;
	}

	FVector4D operator / (double Value)	const
	{
		FVector4D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		result.z = z / (float)Value;
		result.w = w / (float)Value;
		return result;
	}

	FVector4D operator / (int Value)	const
	{
		FVector4D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		result.z = z / (float)Value;
		result.w = w / (float)Value;
		return result;
	}

	// /=
	const FVector4D& operator /= (const FVector4D& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	const FVector4D& operator /= (float Value)
	{
		x /= Value;
		y /= Value;
		z /= Value;
		w /= Value;
		return *this;
	}

	const FVector4D& operator /= (double Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		z /= (float)Value;
		w /= (float)Value;
		return *this;
	}

	const FVector4D& operator /= (int Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		z /= (float)Value;
		w /= (float)Value;
		return *this;
	}

#pragma endregion Divide

	float& operator [] (int Index)
	{
		assert(0 <= Index && Index <= 3);

		switch (Index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}

		return w;
	}


#pragma region Function

#pragma endregion Function

	static FVector4D	Black;
	static FVector4D	White;
	static FVector4D	Red;
	static FVector4D	Blue;
	static FVector4D	Green;
};
