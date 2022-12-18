#pragma once


struct FColor
{
	FColor(float InR , float InG , float InB) : 
		r(InR),g(InG),b(InB)
	{ }
	float r, g, b;
};


struct FVector2
{
	FVector2(float InX, float InY) :
		x(InX), y(InY) {}
	float x, y;
};