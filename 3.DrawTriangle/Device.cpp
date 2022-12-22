#include "Device.h"
#include "math.h"
#include <algorithm>



Device::Device(void* InBuf, int InWidth, int InHeight) :
	FrameBuf(InBuf),Width(InWidth),Height(InHeight)
{

	int BufSize = Width * Height * sizeof(int);
	FrameBufRows = new int* [Height];

	for (int i = 0; i < Height; ++i)
	{
		FrameBufRows[i] = &((int*)(FrameBuf))[i * Width];
	}

}


static constexpr int ROffset = 16;
static constexpr int GOffset = 8;
static constexpr int BOffset = 0;
static constexpr int AOffset = 24;

static int ColorToInt(const FColor& Color)
{
	int r = (Color.r * 255);
	int g = (Color.g * 255);
	int b = (Color.b * 255);

	return (r << ROffset) | (g << GOffset) | (b << BOffset) | (0xff << AOffset);
}

static void lineDDA (int xa, int ya, int xb, int yb,Device * pDevice, const FColor & Color )
{
#define ROUND(x) ((int)(x + 0.5f))

    int dx = xb - xa, dy = yb - ya, steps, k;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs (dx) > abs (dy)) steps = abs (dx) ;
    else steps = abs(dy);
    xIncrement = dx /(float) steps;
    yIncrement = dy / (float) steps;
	pDevice->SetPixel( ROUND(x),ROUND(y),Color );
    for (k=0; k<steps; k++) {
    x += xIncrement;
    y += yIncrement;
    pDevice->SetPixel (ROUND(x), ROUND(y),Color );
    }
#undef ROUND
}

static void lineBresenham(int xa, int ya, int xb, int yb, Device* pDevice, const FColor& Color)
{
	int dx = abs(xa - xb), dy = abs(ya - yb);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy ;
	int twoDyDx = 2 * (dy - dx);
	int x, y, xEnd;

	if (xa > xb)
	{
		x = xb;
		y = yb;
		xEnd = xa;
	}
	else
	{
		x = xa;
		y = ya;
		xEnd = xb;
	}

	pDevice->SetPixel(x,y,Color);

	while (x < xEnd)
	{
		x++;
		if (p < 0)
		{
			p += twoDy;
		}
		else
		{
			y++;
			p += twoDyDx;
		}

		pDevice->SetPixel(x, y, Color);
	}


}

void Device::SetPixel(int x, int y, const FColor& Color)
{
	if (x < 0 || x >= Width ||
		y < 0 || y > Height)
	{
		return;
	}
	FrameBufRows[y][x] = ColorToInt(Color);
}

void Device::DrawLine(const FVector2& a, const FVector2& b, const FColor& Color)
{

	int xa = a.x * Width;
	int ya = a.y * Height;
	int xb = b.x * Width;
	int yb = b.y * Height;

	//lineDDA(xa,ya,xb,yb,this,Color);
	lineBresenham(xa,ya,xb,yb,this,Color);

}

void Device::DrawTriangle(const FVector2& a, const FVector2& b, const FVector2& c, const FColor& ca, const FColor& cb, const FColor& cc)
{

	int ax = (int)(a.x * Width);
	int ay = (int)(a.y * Height);
	int bx = (int)(b.x * Width);
	int by = (int)(b.y * Height);
	int cx = (int)(c.x * Width);
	int cy = (int)(c.y * Height);
	int xMax = std::max( std::max(ax,bx), cx);
	int xMin = std::min(std::min(ax, bx), cx);
	int yMax = std::max(std::max(ay, by), cy);
	int yMin = std::min(std::min(ay, by), cy);

	float fBeta = (cy - ay) * bx + (ax - cx) * by + cx * ay - cy * ax;
	float fGamma = (ay - by) * cx + (bx - ax) * cy + ax * by - ay * bx;

	for (int x = xMin; x <= xMax; ++x)
	{
		for (int y = yMin; y <= yMax; ++y)
		{
			float beta = ((cy - ay) * x + (ax - cx) * y + cx * ay - cy * ax) / fBeta;
			float gamma = ((ay - by) * x + (bx - ax) * y + ax * by - ay * bx) / fGamma;
			float alpha = (1 - beta - gamma);
			if (alpha >= 0 && alpha <= 1
				&& beta >= 0 && beta <= 1 &&
				gamma >= 0 && gamma <= 1)
			{
				FColor c = alpha * ca + beta * cb + gamma * cc;
				SetPixel(x,y,c);
			}

		}

	}

}

