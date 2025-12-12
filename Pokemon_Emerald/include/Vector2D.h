#pragma once

#include <math.h>
#include "EngineMath.h"


/*
	2차원 방향 벡터 구조체를 만들 것이다. 
	벡터는 방향과 힘이있는 것이다. 좌표값은 없다. 
	(1 , 1)	x축으로 1만큼, y축으로 1만큼 이동할것이다.
	(-1 , 10)	x축으로 -1만큼, y축으로 10만큼 이동할것이다.
*/

struct FVector2D
{
	float x = 0.f;
	float y = 0.f;

#pragma region Construction
	FVector2D()
	{
	}

	FVector2D(float _x, float _y)
		: x(_x), y(_y)
	{
	}

	FVector2D(const FVector2D& v)
		: x(v.x), y(v.y)
	{
	}

	FVector2D(FVector2D&& v)
		: x(v.x), y(v.y)
	{
	}
#pragma endregion

#pragma region Equal

	const FVector2D& operator = (const FVector2D& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	const FVector2D& operator = (float Value)
	{
		x = Value;
		y = Value;
		return *this;
	}

	const FVector2D& operator = (double Value)
	{
		x = (float)Value;
		y = (float)Value;
		return *this;
	}

	const FVector2D& operator = (int Value)
	{
		x = (float)Value;
		y = (float)Value;
		return *this;
	}

#pragma endregion Equal
#pragma region Plus
	// +
	FVector2D operator + (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	FVector2D operator + (float Value)	const
	{
		FVector2D	result;
		result.x = x + Value;
		result.y = y + Value;
		return result;
	}

	FVector2D operator + (double Value)	const
	{
		FVector2D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		return result;
	}

	FVector2D operator + (int Value)	const
	{
		FVector2D	result;
		result.x = x + (float)Value;
		result.y = y + (float)Value;
		return result;
	}

	// +=
	const FVector2D& operator += (const FVector2D& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	const FVector2D& operator += (float Value)
	{
		x += Value;
		y += Value;
		return *this;
	}

	const FVector2D& operator += (double Value)
	{
		x += (float)Value;
		y += (float)Value;
		return *this;
	}

	const FVector2D& operator += (int Value)
	{
		x += (float)Value;
		y += (float)Value;
		return *this;
	}

	// ++
	const FVector2D& operator ++ ()
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

	const FVector2D& operator ++ (int)
	{
		x += 1.f;
		y += 1.f;
		return *this;
	}

#pragma endregion Plus

#pragma region Minus
	// -
	FVector2D operator - (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	FVector2D operator - (float Value)	const
	{
		FVector2D	result;
		result.x = x - Value;
		result.y = y - Value;
		return result;
	}

	FVector2D operator - (double Value)	const
	{
		FVector2D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		return result;
	}

	FVector2D operator - (int Value)	const
	{
		FVector2D	result;
		result.x = x - (float)Value;
		result.y = y - (float)Value;
		return result;
	}

	// -=
	const FVector2D& operator -= (const FVector2D& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	const FVector2D& operator -= (float Value)
	{
		x -= Value;
		y -= Value;
		return *this;
	}

	const FVector2D& operator -= (double Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		return *this;
	}

	const FVector2D& operator -= (int Value)
	{
		x -= (float)Value;
		y -= (float)Value;
		return *this;
	}

	// --
	const FVector2D& operator -- ()
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

	const FVector2D& operator -- (int)
	{
		x -= 1.f;
		y -= 1.f;
		return *this;
	}

#pragma endregion Minus

#pragma region Multiply

	// *
	FVector2D operator * (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	FVector2D operator * (float Value)	const
	{
		FVector2D	result;
		result.x = x * Value;
		result.y = y * Value;
		return result;
	}

	FVector2D operator * (double Value)	const
	{
		FVector2D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		return result;
	}

	FVector2D operator * (int Value)	const
	{
		FVector2D	result;
		result.x = x * (float)Value;
		result.y = y * (float)Value;
		return result;
	}

	// *=
	const FVector2D& operator *= (const FVector2D& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	const FVector2D& operator *= (float Value)
	{
		x *= Value;
		y *= Value;
		return *this;
	}

	const FVector2D& operator *= (double Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		return *this;
	}

	const FVector2D& operator *= (int Value)
	{
		x *= (float)Value;
		y *= (float)Value;
		return *this;
	}

#pragma endregion Multiply

#pragma region Divide

	// /
	FVector2D operator / (const FVector2D& v)	const
	{
		FVector2D	result;
		result.x = x / v.x;
		result.y = y / v.y;
		return result;
	}

	FVector2D operator / (float Value)	const
	{
		FVector2D	result;
		result.x = x / Value;
		result.y = y / Value;
		return result;
	}

	FVector2D operator / (double Value)	const
	{
		FVector2D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		return result;
	}

	FVector2D operator / (int Value)	const
	{
		FVector2D	result;
		result.x = x / (float)Value;
		result.y = y / (float)Value;
		return result;
	}

	// /=
	const FVector2D& operator /= (const FVector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	const FVector2D& operator /= (float Value)
	{
		x /= Value;
		y /= Value;
		return *this;
	}

	const FVector2D& operator /= (double Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		return *this;
	}

	const FVector2D& operator /= (int Value)
	{
		x /= (float)Value;
		y /= (float)Value;
		return *this;
	}

#pragma endregion Divide

#pragma region Function
	float Length() const
	{
		//sqrtf : 루트값을 구해주는 기능.
		return sqrtf(x * x + y * y);
	}

	//정규화 : 벡터의 크기를 1로 만들어주는 기능.
	void Normalize()
	{
		float Size = Length();

		if (0.f == Size)
		{
			return;
		}

		x /= Size;
		y /= Size;
	}

	static FVector2D Normalize(const FVector2D& v)
	{
		FVector2D	result;

		float	Size = v.Length();

		if (Size == 0.f)
			return result;

		result.x = v.x / Size;
		result.y = v.y / Size;
		return result;
	}

	float Dot(const FVector2D& v) const
	{
		return x * v.x + y * v.y;
	}

	float Distance(const FVector2D& v) const
	{
		FVector2D	diff = *this - v;

		return diff.Length();
	}

#pragma endregion

};