#pragma once
#include "drawableobject.h"
#include <svl\SVL.h>

class Rect : public DrawableObject
{
public:
	Rect(Vec2 pPosition, Vec2 pExtents, Vec4 pColour);
	~Rect(void);
	void Draw();
	void DrawBorder();
	void SetScissor();
	inline bool Intersects(Vec2 point)
	{
		return (point[0] > centre[0] - extents[0] && point[0] < centre[0] + extents[0] && \
			point[1] > centre[1] - extents[1] && point[1] < centre[1] + extents[1]);
	}
	inline Vec2 GetCentre()
	{
		return centre;
	}
	inline Vec2 GetExtents()
	{
		return extents;
	}
	inline Vec4& GetColour()
	{
		return colour;
	}
	inline Vec2& GetPosition()
	{
		return centre;
	}
	inline Vec2& GetSize()
	{
		return extents;
	}
private:
	Vec2 centre;
	Vec2 extents;
	Vec4 colour;
};

