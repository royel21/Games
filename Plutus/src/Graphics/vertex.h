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
		//Empty Constructor
		ColorRGBA8() : r(255), g(255), b(255), a(255) {}

		ColorRGBA8(GLubyte m_r, GLubyte m_g, GLubyte m_b, GLubyte m_a)
		{
			r = m_r;
			g = m_g;
			b = m_b;
			a = m_a;
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct Vertex
	{
		Vertex() : position(), uv() {}
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
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
	};
} // namespace Plutus

#endif