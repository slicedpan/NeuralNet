#include "Rect.h"
#include "glut.h"

Rect::Rect(Vec2 pPosition, Vec2 pExtents, Vec4 pColour)
{
	centre = pPosition;
	extents = pExtents / 2.0f;
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

void Rect::SetScissor()
{
	int x = (int)(centre[0] - extents[0]);
	int y = (int)(centre[1] - extents[1]);
	int width = (int)(extents[0] * 2.0f);
	int height = (int)(extents[1] * 2.0f);
	glScissor(x, y, width, height);
}
