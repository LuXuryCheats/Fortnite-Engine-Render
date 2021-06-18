#pragma once
#include "stdafx.h"
#include "spoof_call.h"


#define E(str) skCrypt(str)

#define M_PI	3.14159265358979323846264338327950288419716939937510


namespace framework {


	inline POINT cursor;
	inline POINT cursor_corrected;
	inline bool should_drag = false;
	inline bool should_move = false;
	inline int x = 140, y = 140;
	inline int w = 400, h = 300;
	inline int current_tab = 0;
	inline bool show_menu = true;
	inline auto RobotoFont = UObject::FindObject<UFont>(skCrypt("Font Roboto.Roboto"));

	inline void K2_DrawFilledRect(UCanvas* canvas, const FVector2D& initial_pos, float w, float h, const FLinearColor& color);
	inline void groupbox(UCanvas* canvas, int x, int y, int w, int h, const FString string, bool show_label);
	inline void tab(UCanvas* canvas, int x, int y, int w, int h, const FString string, int& tab, int count, bool show_outline);
	inline void tabinactive(UCanvas* canvas, int x, int y, int w, int h, const FString string, int& tab, int count, bool show_outline);
	inline bool checkbox(UCanvas* canvas, int x, int y, int position, const FString string, bool& value);
	inline bool Text(UCanvas* canvas, int x, int y, int position, const FString string, bool& value);
	inline bool TextSelected(UCanvas* canvas, int x, int y, int position, const FString string, bool& value);
	inline bool TextActive(UCanvas* canvas, int x, int y, int position, const FString string, bool& value);
	inline void slider(UCanvas* canvas, int x, int y, int position, const FString string, float& value, float min_value, float max_value);
	inline void movement(int& x, int& y, int w, int h);
	inline void render_menu(UCanvas* canvas);
	inline void DrawCircle(UCanvas* canvas, int x, int y, int radius, int numsides, FLinearColor color);
	inline void Slider2(UCanvas* canvas, int x, int y, FString name, float& value);

}


inline void framework::K2_DrawFilledRect(UCanvas* canvas, const FVector2D& initial_pos, float w, float h, const FLinearColor& color)
{
	for (float i = 0.f; i < h; i += 1.f)
		canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.f, color);
}

inline void K2_DrawRect(UCanvas* canvas, const FVector2D& initial_pos, float w, float h, const FLinearColor& color, float thickness = 1.f)
{
	canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y }, FVector2D{ initial_pos.X + w, initial_pos.Y }, thickness, color);
	canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y }, FVector2D{ initial_pos.X, initial_pos.Y + h }, thickness, color);
	canvas->K2_DrawLine(FVector2D{ initial_pos.X + w, initial_pos.Y }, FVector2D{ initial_pos.X + w, initial_pos.Y + h }, thickness, color);
	canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + h }, FVector2D{ initial_pos.X + w, initial_pos.Y + h }, thickness, color);
}

inline void framework::groupbox(UCanvas* canvas, int x, int y, int w, int h, const FString string, bool show_label)
{
	GetCursorPos(&cursor);

	//K2_DrawFilledRect(canvas, FVector2D(x, y), w, h, FLinearColor(25.0f, 25.0f, 25.0f, 1.0f));
	//K2_DrawRect(canvas, FVector2D(x, y), w, h, FLinearColor(45.0f, 45.0f, 45.0f, 1.0f));

	if (show_label)
		canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 2, y - 12), FVector2D(1, 1), FLinearColor(255.0f, 255.0f, 255.0f, 1.0f), 1.0f,
			FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.0f, 255.0f, 255.0f, 1.0f));
}
inline void framework::tabinactive(UCanvas* canvas, int x, int y, int w, int h, const FString string, int& tab, int count, bool show_outline)
{
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		tab = count;

	if (show_outline)
		//K2_DrawRect(canvas, FVector2D(x + 26, y + 25), w, 1, tab == count ? FLinearColor(255.f, 0.f, 0.f, 1.0f) : FLinearColor(0.f, 0.f, 0.f, 1.0f));
		canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 1, y + 1 + h / 2 - 8), FVector2D(1, 1), show_outline ? FLinearColor(0.f, 0.f, 0.f, 1.0f) : tab == count ? FLinearColor(0.f, 0.f, 0.f, 1.0f) : FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
			FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(85.f, 85.f, 85.f, 1.0f));

	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x, y + h / 2 - 8), FVector2D(1, 1), show_outline ? FLinearColor(80.f, 80.f, 80, 1.0f) : tab == count ? FLinearColor(140.0f, 1.0f, 250.0f, 1.0f) : FLinearColor(255.0f, 255.0f, 255.0f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(85.f, 85.f, 85.f, 1.0f));
}
inline void framework::tab(UCanvas* canvas, int x, int y, int w, int h, const FString string, int& tab, int count, bool show_outline)
{
	GetCursorPos(&cursor);

	if ((cursor.x + 30 > x) && (cursor.x + 30 < x + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		tab = count;

	if (show_outline)
		//K2_DrawRect(canvas, FVector2D(x + 26, y + 25), w, 1, tab == count ? FLinearColor(255.f, 0.f, 0.f, 1.0f) : FLinearColor(0.f, 0.f, 0.f, 1.0f));
		canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 1, y + 1 + h / 2 - 8), FVector2D(1, 1), show_outline ? FLinearColor(0.f, 0.f, 0.f, 1.0f) : tab == count ? FLinearColor(0.f, 0.f, 0.f, 1.0f) : FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
			FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(85.f, 85.f, 85.f, 1.0f));

	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x, y + h / 2 - 8), FVector2D(1, 1), show_outline ? FLinearColor(255.f, 255.f, 255.f, 1.0f) : tab == count ? FLinearColor(255.f, 255.f, 255.f, 1.0f) : FLinearColor(255.0f, 255.0f, 255.0f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.f, 255.f, 255.f, 1.0f));
}
inline void framework::DrawCircle(UCanvas* canvas, int x, int y, int radius, int numsides, FLinearColor color)
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
		canvas->K2_DrawLine(FVector2D({ V[Count].X, V[Count].Y }), FVector2D({ X2, Y2 }), 5.0f, color);
	}
}


inline bool framework::checkbox(UCanvas* canvas, int x, int y, int position, const FString string, bool& value)
{
	GetCursorPos(&cursor);
	int w = 15, h = 15;
	if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	else if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	K2_DrawFilledRect(canvas, FVector2D(position - 31, y + 1), w + 2, h + 2, value ? FLinearColor(0.f, 0.f, 0.f, 1.0f) : FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(position - 30, y + 2), w, h, value ? FLinearColor(46.f, 0.f, 156.f, 1.0f) : FLinearColor(18.f, 18.f, 18.f, 1.0f));

	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 37, y + 2), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, value ? FLinearColor(255.0f, 255.0f, 255.0f, 1.0f) : FLinearColor(70.f, 70.f, 70.f, 1.0f));
	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 36, y + 1), FVector2D(1, 1), value ? FLinearColor(255.0f, 255.f, 255.0f, 1.0f) : FLinearColor(80.f, 80.f, 80.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, value ? FLinearColor(255.0f, 255.0f, 255.0f, 1.0f) : FLinearColor(70.f, 70.f, 70.f, 1.0f));

	return value;
}


inline bool framework::Text(UCanvas* canvas, int x, int y, int position, const FString string, bool& value)
{
	GetCursorPos(&cursor);
	
	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 33, y + 3), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.f, 255.f, 255.f, 1.0f));

	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 32, y + 2), FVector2D(1, 1), FLinearColor(90.f, 90.f, 90.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(90.f, 90.f, 90.f, 1.0f));

	return value;
}


inline bool framework::TextSelected(UCanvas* canvas, int x, int y, int position, const FString string, bool& value)
{
	GetCursorPos(&cursor);
	//canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y), FVector2D(3.0f, 3.0f), 5.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));

	int w = 15, h = 15;

	if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	else if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 32, y + 2), FVector2D(1, 1), FLinearColor(255.0f, 255.f, 0.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.0f, 255.0f, 0.f, 1.0f));

	return value;
}
inline bool framework::TextActive(UCanvas* canvas, int x, int y, int position, const FString string, bool& value)
{
	GetCursorPos(&cursor);
	//canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y), FVector2D(3.0f, 3.0f), 5.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));

	int w = 15, h = 15;

	if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	else if ((cursor.x > position - 30) && (cursor.x < position - 30 + w) && (cursor.y > y) && (cursor.y < y + h) && (Basic::LocalController->WasInputKeyJustPressed(KeyName::Left) & 1))
		value = !value;
	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 32, y + 2), FVector2D(1, 1), FLinearColor(0.f, 255.f, 0.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(0.f, 255.0f, 0.f, 1.0f));

	return value;
}
inline void framework::slider(UCanvas* canvas, int x, int y, int position, const FString string, float& value, float min_value, float max_value)
{
	GetCursorPos(&cursor);
	//canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y), FVector2D(3.0f, 3.0f), 5.0f, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	//int ix = x + 140;
	int ix = x + 60;
	int yi = y + 4;

	if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (Basic::LocalController->IsInputKeyDown(KeyName::Left)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	//K2_DrawFilledRect(canvas, FVector2D(ix, yi - 3), position, 11.5, FLinearColor(6.f, 6.f, 6.f, 1.0f));
	//K2_DrawFilledRect(canvas, FVector2D(ix - 3, yi), position, 11.5, FLinearColor(6.f, 6.f, 6.f, 1.0f));
	//K2_DrawFilledRect(canvas, FVector2D(ix + 3, yi), position, 11.5, FLinearColor(6.f, 6.f, 6.f, 1.0f));
	//K2_DrawFilledRect(canvas, FVector2D(ix - 3, yi - 3), position, 11.5, FLinearColor(6.f, 6.f, 6.f, 1.0f));
	//K2_DrawFilledRect(canvas, FVector2D(ix + 3, yi - 3), position, 11.5, FLinearColor(6.f, 6.f, 6.f, 1.0f));

	K2_DrawFilledRect(canvas, FVector2D(ix, yi - 2), position, 11, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix - 2, yi), position, 11, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix + 2, yi), position, 11, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix - 2, yi - 2), position, 11, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix + 2, yi - 2), position, 11, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix, yi - 1), position, 10, FLinearColor(46.f, 0.f, 156.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix - 1, yi), position, 10, FLinearColor(46.f, 0.f, 156.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix + 1, yi), position, 10, FLinearColor(46.f, 0.f, 156.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix - 1, yi - 1), position, 10, FLinearColor(46.f, 0.f, 156.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix + 1, yi - 1), position, 10, FLinearColor(46.f, 0.f, 156.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix, yi), position, 9, FLinearColor(3.f, 3.f, 3.f, 1.0f));
	K2_DrawFilledRect(canvas, FVector2D(ix, yi), value * (float(position) / float(max_value)), 9, FLinearColor(46.f, 0.f, 156.f, 1.0f));


	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 58, y - 13), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.0f, 0.0f, 0.0f, 1.0f));
	canvas->K2_DrawText(RobotoFont, FString(string), FVector2D(x + 57, y - 14), FVector2D(1, 1), FLinearColor(255.0f, 255.f, 255.0f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.0f, 0.0f, 0.0f, 1.0f));
}

inline void framework::movement(int& x, int& y, int w, int h)
{
	GetCursorPos(&cursor);

	if ((Basic::LocalController->WasInputKeyJustPressed(KeyName::Left)) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h))
	{
		should_drag = true;

		if (!should_move)
		{
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}
	else
	{
		should_drag = false;
		should_move = false;
	}

	if (should_drag)
	{
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}
}

inline auto begin_frame = [&](UCanvas* canvas, int x, int y, int w, int h, FLinearColor bg, FLinearColor header_text, FLinearColor header_line, FString name)
{
	GetCursorPos(&framework::cursor);
	//canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y), FVector2D(3.0f, 3.0f), 5.0f, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	framework::K2_DrawFilledRect(canvas, FVector2D(x, y), w, h, FLinearColor(69.0f, 69.0f, 69.0f, 1.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(x, y), w, 20, FLinearColor(69.0f, 69.0f, 69.0f, 1.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(x, y + 20), w, 1, FLinearColor(69.0f, 69.0f, 69.0f, 1.0f));
	canvas->K2_DrawText(framework::RobotoFont, FString(name), FVector2D(x + 5, y + 4), FVector2D(1, 1), FLinearColor(255.0f, 255.0f, 255.0f, 1.0f), 1.0f,
		FLinearColor(0, 0, 0, 0), FVector2D(), false, false, false, FLinearColor(255.0f, 255.0f, 255.0f, 1.0f));
};



inline void framework::render_menu(UCanvas* canvas)
{
	if (!show_menu)
		return;

	framework::K2_DrawFilledRect(canvas, FVector2D(198, 198), 304, 358, FLinearColor(0.f, 0.f, 0.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(199, 199), 302, 356, FLinearColor(6.f, 6.f, 6.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(200, 200), 300, 354, FLinearColor(3.f, 3.f, 3.f, 255.0f));

	canvas->K2_DrawLine(FVector2D{ 200, 230 }, FVector2D{ 500, 230 }, 3.f, FLinearColor(0.f, 0.f, 0.f, 1.0f));
	canvas->K2_DrawLine(FVector2D{ 200, 230 }, FVector2D{ 500, 230 }, 1.f, FLinearColor(125.f, 0.f, 255.f, 1.0f));

	Text(canvas, 175, 204, 0, FString(E(L"velocity-aim.io")), options::Aim::memory);

	framework::K2_DrawFilledRect(canvas, FVector2D(208, 238), 284, 274, FLinearColor(0.f, 0.f, 0.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(209, 239), 282, 272, FLinearColor(6.f, 6.f, 6.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(210, 240), 280, 270, FLinearColor(2.f, 2.f, 2.f, 255.0f));


	framework::K2_DrawFilledRect(canvas, FVector2D(276, 515), 144, 34, FLinearColor(0.f, 0.f, 0.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(277, 516), 142, 32, FLinearColor(6.f, 6.f, 6.f, 255.0f));
	framework::K2_DrawFilledRect(canvas, FVector2D(278, 517), 140, 30, FLinearColor(2.f, 2.f, 2.f, 255.0f));

	if (current_tab == 0)
	{
		tab(canvas, 298, 517, x + 300, 30, FString(E(L"aim")), current_tab, 0, true);
	}
	else
	{
		tabinactive(canvas, 298, 517, x + 300, 30, FString(E(L"aim")), current_tab, 0, true);
	}

	if (current_tab == 1)
	{
		tab(canvas, 334, 517, x + 300, 30, FString(E(L"esp")), current_tab, 1, true);
	}
	else
	{
		tabinactive(canvas, 334, 517, x + 300, 30, FString(E(L"esp")), current_tab, 1, true);

	}

	if (current_tab == 2)
	{
		tab(canvas, 368, 517, x + 300, 30, FString(E(L"misc")), current_tab, 2, true);
	}
	else
	{
		tabinactive(canvas, 368, 517, x + 300, 30, FString(E(L"misc")), current_tab, 2, true);
	}

	if (current_tab == 0)
	{
		checkbox(canvas, 205, 250, x + 110, FString(E(L"enable aim")), options::Aim::memory);
		slider(canvas, x + 20, y + 148, 150, FString(E(L"aimbot fov")), options::Aim::fov_range, 0.0f, 800.0f);
		slider(canvas, x + 20, y + 188, 150, FString(E(L"aimbot smooth")), options::Aim::smooth, 0.0f, 50.f);
		checkbox(canvas, 205, 355, x + 110, FString(E(L"hitscan")), options::Aim::RandomAimBone);
		checkbox(canvas, 205, 385, x + 110, FString(E(L"visible check")), options::Aim::visible_check);

	}

	if (current_tab == 1)
	{
		checkbox(canvas, 205, 250, x + 110, FString(E(L"box")), options::Visuals::BoxESP);
		checkbox(canvas, 205, 280, x + 110, FString(E(L"corner-box")), options::Visuals::CornerBox);
		checkbox(canvas, 205, 310, x + 110, FString(E(L"player-name")), options::Visuals::NameESP);
		checkbox(canvas, 205, 340, x + 110, FString(E(L"player-distance")), options::Visuals::Distance);
		slider(canvas, x + 20, y + 236, 150, FString(E(L"box thickness")), options::Visuals::BoxThickness, 0.0f, 8.0f);
		checkbox(canvas, 205, 400, x + 110, FString(E(L"outline text")), options::Visuals::OutlineText);
		checkbox(canvas, 205, 430, x + 110, FString(E(L"outline box")), options::Visuals::OutlineBox);
		checkbox(canvas, 205, 460, x + 110, FString(E(L"skeleton")), options::Visuals::skeleton);
		checkbox(canvas, 205, 490, x + 110, FString(E(L"head dot")), options::Visuals::headdot);

	}
	

	if (current_tab == 2)
	{
		checkbox(canvas, 205, 250, x + 110, FString(E(L"show fov")), options::Aim::drawfov);
		checkbox(canvas, 205, 280, x + 110, FString(E(L"custom crosshair")), options::Misc::CustomCrosshair);
		checkbox(canvas, 205, 310, x + 110, FString(E(L"instant equip (may crash)")), options::Misc::InstantEquip);
		checkbox(canvas, 205, 340, x + 110, FString(E(L"fov slider")), options::Misc::FovChanger);

		if (options::Misc::FovChanger)
		{
			slider(canvas, x + 20, y + 236, 150, FString(E(L"field of view")), options::Misc::FOV, 80.0f, 140.0f);
		}
	}


	canvas->K2_DrawBox(FVector2D(framework::cursor.x + 1, framework::cursor.y + 1), FVector2D(3.0f, 3.0f), 5.0f, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y - 5), FVector2D(3.0f, 3.0f), 1.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));
	canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y + 5), FVector2D(3.0f, 3.0f), 1.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));
	canvas->K2_DrawBox(FVector2D(framework::cursor.x - 5, framework::cursor.y), FVector2D(3.0f, 3.0f), 1.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));
	canvas->K2_DrawBox(FVector2D(framework::cursor.x + 5, framework::cursor.y), FVector2D(3.0f, 3.0f), 1.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));
	canvas->K2_DrawBox(FVector2D(framework::cursor.x, framework::cursor.y), FVector2D(3.0f, 3.0f), 1.0f, FLinearColor(255.f, 255.f, 255.f, 1.0f));


}