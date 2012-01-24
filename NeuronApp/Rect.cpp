#include "Rect.h"
#include "glut.h"

Rect::Rect(Vec2 pPosition, Vec2 pExtents, Vec4 pColour)
{
	centre = pPosition;
	extents = pExtents;
	colour = pColour;
}

Rect::~Rect(void)
{
}

void Rect::Draw()
{
	glBegin(GL_QUADS);
	glColor4fv(colour.Ref());
	glVertex2f(centre[0] + extents[0], centre[1] + extents[1]);
	glVertex2f(centre[0] + extents[0], centre[1] - extents[1]);
	glVertex2f(centre[0] - extents[0], centre[1] - extents[1]);
	glVertex2f(centre[0] - extents[0], centre[1] + extents[1]);
	glEnd();
}

void Rect::DrawBorder()
{
	glBegin(GL_LINE_STRIP);
	glColor4fv(colour.Ref());
	glVertex2f(centre[0] + extents[0], centre[1] + extents[1]);
	glVertex2f(centre[0] + extents[0], centre[1] - extents[1]);
	glVertex2f(centre[0] - extents[0], centre[1] - extents[1]);
	glVertex2f(centre[0] - extents[0], centre[1] + extents[1]);
	glVertex2f(centre[0] + extents[0], centre[1] + extents[1]);
	glEnd();
}
