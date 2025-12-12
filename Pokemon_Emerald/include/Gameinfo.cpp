#include "Gameinfo.h"

int Clamp(int Value, int Min, int Max)
{
	// int값이 몇이들어오든
	// 0 ~ 100
	// Volume > 100 -> 100 | 그 사이값은 그대로 사용 | Volume < 0 -> 0
	// Clamp

	if (Value < Min)
	{
		Value = Min;
	}
	else if ( Value > Max)
	{
		Value = Max;
	}

	return Value;
}

float Clamp(float Value, float Min, float Max)
{
	if (Value < Min)
	{
		Value = Min;
	}
	else if (Value > Max)
	{
		Value = Max;
	}

	return Value;
}
