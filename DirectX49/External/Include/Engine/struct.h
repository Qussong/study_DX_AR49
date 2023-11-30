#pragma once

struct Vec2
{
public:
	float x;
	float y;

public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(int _x, int _y)
		: x(static_cast<float>(_x))
		, y(static_cast<float>(_y))
	{}

	Vec2(POINT _pt)
		: x(static_cast<float>(_pt.x))
		, y(static_cast<float>(_pt.y))
	{}
} ;