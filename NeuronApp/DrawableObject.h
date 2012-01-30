#pragma once
class Vec4;
class Vec2;

class DrawableObject
{
public:
	virtual void Draw() = 0;
	virtual Vec4& GetColour() = 0;
	virtual Vec2& GetPosition() = 0;
	virtual Vec2& GetSize() = 0;
	virtual ~DrawableObject(void) {}
};

