#ifndef _VERTEX_H
#define _VERTEX_H

#include <glm/glm.hpp>
#include "GLheaders.h"

namespace Plutus
{
	struct Position
	{
		Position() : x(0), y(0) {}
		Position(float _x, float _y) : x(_x), y(_y) {}
		float x;
		float y;
	};

	struct UV
	{
		UV() : u(0), v(0) {}
		UV(float _u, float _v) : u(_u), v(_v) {}
		float u;
		float v;
	};

	struct ColorRGBA8
	{
		GLuint rgba;
		//Empty Constructor
		ColorRGBA8() : rgba(0xffffffff) {}

		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			rgba = a << 24 | b << 16 | g << 8 | r;
		}
		GLuint get() { return rgba; };
		void setColor(GLuint c) { rgba = c; }
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) { rgba = a << 24 | b << 16 | g << 8 | r; };
	};

	struct Vertex
	{
		Vertex() : position(), uv() {}
		Vertex(float x, float y, float uvX, float uvY, ColorRGBA8 c) : position(x, y), uv(uvX, uvY), color(c) {}
		Position position;
		UV uv;
		ColorRGBA8 color;

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.setColor(r, g, b, a);
		}
	};
} // namespace Plutus

#endif