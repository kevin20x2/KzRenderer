#pragma once

#include "BaseTypes.h"

class Device
{
public:
	Device(void* InBuf, int InWidth , int InHeight);

	void SetPixel(int x, int y, const FColor & Color );

	void DrawLine(const FVector2& a, const FVector2& b, const FColor& Color);

	void DrawTriangle(const FVector2& a, const FVector2& b, const FVector2& c,
		const FColor& ca, const FColor& cb, const FColor& cc);
	
protected:

	int Width, Height;
	void * FrameBuf;

	int** FrameBufRows; 

};