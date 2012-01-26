#pragma once
class Vec4;

class DrawableObject
{
public:
	virtual void Draw() = 0;
	virtual Vec4& GetColour() = 0;
};

