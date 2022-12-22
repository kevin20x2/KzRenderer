#pragma once


struct FColor
{
	FColor(float InR , float InG , float InB) : 
		r(InR),g(InG),b(InB)
	{ }
	float r, g, b;
	FColor operator+ (const FColor& rhs)
	{
		return FColor(r + rhs.r, g + rhs.g, b + rhs.b);
	}
	FColor  operator *(float v)
	{
		return FColor(r * v, g * v, b * v);
	}
};

inline FColor operator*(float v,const FColor & lhs)
{
	return FColor(lhs.r * v, lhs.g * v, lhs.b * v);
}

struct FVector2
{
	FVector2(float InX, float InY) :
		x(InX), y(InY) {}
	float x, y;
};