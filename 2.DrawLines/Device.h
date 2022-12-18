#pragma once

#include "BaseTypes.h"

class Device
{
public:
	Device(void* InBuf, int InWidth , int InHeight);

	void SetPixel(int x, int y, const FColor & Color );

	void DrawLine(const FVector2& a, const FVector2& b, const FColor& Color);
protected:

	int Width, Height;
	void * FrameBuf;

	int** FrameBufRows; 

};