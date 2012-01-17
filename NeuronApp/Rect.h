#pragma once
#include "drawableobject.h"
#include "svl/svl.h"

class Rect : public DrawableObject
{
public:
	Rect(Vec2 pPosition, Vec2 pExtents, Vec4 pColour);
	~Rect(void);
	void Draw();
private:
	Vec2 centre;
	Vec2 extents;
	Vec4 colour;
};

