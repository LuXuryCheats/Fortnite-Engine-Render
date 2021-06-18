#pragma once

#include "stdafx.h"

#define M_PI	3.14159265358979323846264338327950288419716939937510

namespace hkrender {

	inline SDK::UCanvas* RenderCanvas;

	inline float minX = FLT_MAX;
	inline float maxX = -FLT_MAX;
	inline float minY = FLT_MAX;
	inline float maxY = -FLT_MAX;

	inline auto topLeft = FVector2D(minX - 3.0f, minY - 3.0f);
	inline auto bottomRight = FVector2D(maxX + 3.0f, maxY + 3.0f);

	inline auto centerTop = FVector2D((topLeft.X + bottomRight.X) / 2.0f + +10 + 1.65, topLeft.Y);

	inline auto theAdditionY = (bottomRight.Y - topLeft.Y) / 3;
	inline auto theAdditionX = (bottomRight.X - topLeft.X) / 3;

	inline auto topRight = FVector2D(bottomRight.X, topLeft.Y);

	inline auto bottomLeft = FVector2D(topLeft.X, bottomRight.Y);

	inline void DrawCorner(SDK::FVector2D x, SDK::FVector2D y, SDK::FVector2D x2, SDK::FVector2D y2, float OutlineBoxSize, float MainBoxSize);
	inline void DrawBox(float MainBoxSize);
	inline void DrawSkeleton();
	inline void DrawCircle(UCanvas* canvas, int x, int y, int radius, int numsides, FLinearColor color);

}

void hkrender::DrawCircle(UCanvas* canvas, int x, int y, int radius, int numsides, FLinearColor color)
{
	float Step = M_PI * 2.0 / numsides;
	int Count = 0;
	FVector2D V[128];
	for (float a = 0; a < M_PI * 2.0; a += Step)
	{
		float X1 = radius * cosf(a) + x;
		float Y1 = radius * sinf(a) + y;
		float X2 = radius * cosf(a + Step) + x;
		float Y2 = radius * sinf(a + Step) + y;
		V[Count].X = X1;
		V[Count].Y = Y1;
		V[Count + 1].X = X2;
		V[Count + 1].Y = Y2;
		canvas->K2_DrawLine(FVector2D({ V[Count].X, V[Count].Y }), FVector2D({ X2, Y2 }), 1.0f, color);
	}
}

